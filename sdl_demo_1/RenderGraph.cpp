#include "RenderGraph.h"
#include "DrawableObject.h"
#include "Shader.h"
#include "Texture.h"
#include "Shadow.h"
#include "Game.h"

namespace Bagnall
{
	// RENDERGRAPH PUBLIC

	RenderGraph::RenderGraph()
	{
		shadowsEnabled = false;
	}

	RenderNode* RenderGraph::AddDrawableObject(DrawableObject *o)
	{
		auto cubeMap = o->GetCubeMap();
		auto texture = o->GetTexture();
		auto material = o->GetMaterial();
		auto textureBumpmap = Texture::GetBumpMapByTexture(texture);
		auto cubeBumpmap = Texture::GetBumpMapByCubeMap(cubeMap);

		// non-emissive
		if (!o->GetEmissive())
		{
			MaterialNode *materialNode;

			// cube map
			if (cubeMap != 0)
			{
				// with bump map
				if (cubeBumpmap != 0 && o->GetBumpMapEnabled())
				{
					if (cubeMapBumpNodeMap.find(cubeMap) == cubeMapBumpNodeMap.end())
						cubeMapBumpNodeMap.emplace(cubeMap, new CubeMapBumpNode(cubeMap, cubeBumpmap));

					// without material
					if (material == Material::None())
					{
						auto *cubeMapBumpNode = cubeMapBumpNodeMap[cubeMap];

						cubeMapBumpNode->objects.push_back(o);
						return cubeMapBumpNode;
					}
					// with material
					else
					{
						auto _materialNodeMap = &cubeMapBumpNodeMap[cubeMap]->materialNodeMap;

						if (_materialNodeMap->find(material) == _materialNodeMap->end())
							_materialNodeMap->emplace(material, new MaterialNode(material));

						materialNode = (*_materialNodeMap)[material];

						materialNode->objects.push_back(o);
						return materialNode;
					}
				}
				// without bump map
				else
				{
					if (cubeMapNodeMap.find(cubeMap) == cubeMapNodeMap.end())
						cubeMapNodeMap.emplace(cubeMap, new CubeMapNode(cubeMap));

					// without material
					if (material == Material::None())
					{
						auto *cubeMapNode = cubeMapNodeMap[cubeMap];

						cubeMapNode->objects.push_back(o);
						return cubeMapNode;
					}
					// with material
					else
					{
						auto _materialNodeMap = &cubeMapNodeMap[cubeMap]->materialNodeMap;

						if (_materialNodeMap->find(material) == _materialNodeMap->end())
							_materialNodeMap->emplace(material, new MaterialNode(material));

						materialNode = (*_materialNodeMap)[material];

						materialNode->objects.push_back(o);
						return materialNode;
					}
				}
			}
			// no texture, only material
			else if (texture == 0)
			{
				if (materialNodeMap.find(material) == materialNodeMap.end())
					materialNodeMap.emplace(material, new MaterialNode(material));

				materialNode = materialNodeMap[material];
			}
			// no material, texture only
			else if (material == Material::None())
			{
				// with bump map
				if (textureBumpmap != 0 && o->GetBumpMapEnabled())
				{
					if (textureBumpNodeMap.find(texture) == textureBumpNodeMap.end())
						textureBumpNodeMap.emplace(texture, new TextureBumpNode(texture, textureBumpmap));

					auto *textureBumpNode = textureBumpNodeMap[texture];

					textureBumpNode->objects.push_back(o);
					return textureBumpNode;
				}
				// without bump map
				else
				{
					if (textureNodeMap.find(texture) == textureNodeMap.end())
						textureNodeMap.emplace(texture, new TextureNode(texture));

					auto *textureNode = textureNodeMap[texture];

					textureNode->objects.push_back(o);
					return textureNode;
				}
			}
			// texture and material
			else
			{
				std::unordered_map<Material, MaterialNode*> *_materialNodeMap;

				// with bump map
				if (textureBumpmap != 0 && o->GetBumpMapEnabled())
				{
					if (textureBumpNodeMap.find(texture) == textureBumpNodeMap.end())
						textureBumpNodeMap.emplace(texture, new TextureBumpNode(texture, textureBumpmap));

					_materialNodeMap = &textureBumpNodeMap[texture]->materialNodeMap;
				}
				// without bump map
				else
				{
					if (textureNodeMap.find(texture) == textureNodeMap.end())
						textureNodeMap.emplace(texture, new TextureNode(texture));

					_materialNodeMap = &textureNodeMap[texture]->materialNodeMap;
				}

				if (_materialNodeMap->find(material) == _materialNodeMap->end())
					_materialNodeMap->emplace(material, new MaterialNode(material));

				materialNode = (*_materialNodeMap)[material];
			}

			materialNode->objects.push_back(o);
			return materialNode;
		}
		// emissive
		else
		{
			std::unordered_map<vec4, EmissiveNode*> *_emissiveNodeMap;
			EmissiveNode *emissiveNode;

			// cubemap
			if (cubeMap != 0)
			{
				if (cubeMapEmissiveNodeMap.find(cubeMap) == cubeMapEmissiveNodeMap.end())
					cubeMapEmissiveNodeMap.emplace(cubeMap, new CubeMapEmissiveNode(cubeMap));

				auto cubeMapEmissiveNode = cubeMapEmissiveNodeMap[cubeMap];
				cubeMapEmissiveNode->objects.push_back(o);
				return cubeMapEmissiveNode;
			}
			// texture
			else if (texture != 0)
			{
				if (textureEmissiveNodeMap.find(texture) == textureEmissiveNodeMap.end())
					textureEmissiveNodeMap.emplace(texture, new TextureEmissiveNode(texture));

				auto textureEmissiveNode = textureEmissiveNodeMap[texture];
				textureEmissiveNode->objects.push_back(o);
				return textureEmissiveNode;
			}
			// no texture
			else
			{
				_emissiveNodeMap = &emissiveNodeMap;
			}

			if (_emissiveNodeMap->find(o->GetEmissionColor()) == _emissiveNodeMap->end())
				_emissiveNodeMap->emplace(o->GetEmissionColor(), new EmissiveNode(o->GetEmissionColor()));

			emissiveNode = (*_emissiveNodeMap)[o->GetEmissionColor()];

			emissiveNode->objects.push_back(o);
			return emissiveNode;
		}
	}

	void RenderGraph::Render() const
	{
		// render emissive objects
		Shader::SetProgram("emissive_color");
		for (auto it = emissiveNodeMap.begin(); it != emissiveNodeMap.end(); ++it)
			(*it).second->Render();

		// render emissive objects with cubemap
		Shader::SetProgram("emissive_cubemap");
		for (auto it = cubeMapEmissiveNodeMap.begin(); it != cubeMapEmissiveNodeMap.end(); ++it)
			(*it).second->Render();

		// render emissive objects with texture
		Shader::SetProgram("emissive_texture");
		for (auto it = textureEmissiveNodeMap.begin(); it != textureEmissiveNodeMap.end(); ++it)
			(*it).second->Render();

		// render objects with texture, bump map, material
		Shader::SetProgram("texture_bump");
		for (auto it = textureBumpNodeMap.begin(); it != textureBumpNodeMap.end(); ++it)
			(*it).second->Render();

		// render objects with texture, material
		Shader::SetProgram("texture");
		for (auto it = textureNodeMap.begin(); it != textureNodeMap.end(); ++it)
			(*it).second->Render();

		// render objects with only material
		Shader::SetProgram("material");
		for (auto it = materialNodeMap.begin(); it != materialNodeMap.end(); ++it)
			(*it).second->Render();

		// render objects with cubemap
		Shader::SetProgram("cubemap");
		for (auto it = cubeMapNodeMap.begin(); it != cubeMapNodeMap.end(); ++it)
			(*it).second->Render();

		// render objects with cubemap and bumpmap
		Shader::SetProgram("cubemap_bump");
		for (auto it = cubeMapBumpNodeMap.begin(); it != cubeMapBumpNodeMap.end(); ++it)
			(*it).second->Render();
	}

	void RenderGraph::SetShadowsEnabled(bool s)
	{
		shadowsEnabled = s;

		Shader::SetProgram("material");
		Shader::SetUseShadowCubeMap(s);
		if (shadowsEnabled)
			Shadow::BindToGPU();

		Shader::SetProgram("texture");
		Shader::SetUseShadowCubeMap(s);
		if (shadowsEnabled)
			Shadow::BindToGPU();

		Shader::SetProgram("texture_bump");
		Shader::SetUseShadowCubeMap(s);
		if (shadowsEnabled)
			Shadow::BindToGPU();

		Shader::SetProgram("cubemap");
		Shader::SetUseShadowCubeMap(s);
		if (shadowsEnabled)
			Shadow::BindToGPU();
	}

	bool RenderGraph::GetShadowsEnabled() const
	{
		return shadowsEnabled;
	}

	// CUBEMAPNODE PUBLIC

	void CubeMapNode::Render() const
	{
		// bind cube map
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMap);

		// render objects without materials
		Shader::SetTextureBlend(false);
		RenderNode::Render();
		Shader::SetTextureBlend(true);

		// render material nodes
		for (auto it = materialNodeMap.begin(); it != materialNodeMap.end(); ++it)
		{
			(*it).second->Render();
		}
	}

	// CUBEMAPBUMPNODE PUBLIC

	void CubeMapBumpNode::Render() const
	{
		// bind texture
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMap);

		// bind bump map
		glActiveTexture(GL_TEXTURE4);
		glBindTexture(GL_TEXTURE_CUBE_MAP, bumpMap);

		// render objects without materials
		Shader::SetTextureBlend(false);
		RenderNode::Render();
		Shader::SetTextureBlend(true);

		// render material nodes
		for (auto it = materialNodeMap.begin(); it != materialNodeMap.end(); ++it)
		{
			(*it).second->Render();
		}
	}

	// CUBEMAPEMISSIVENODE PUBLIC

	void CubeMapEmissiveNode::Render() const
	{
		// bind cube map
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMap);

		// render objects
		RenderNode::Render();
	}

	// TEXTUREBUMPNODE PUBLIC

	void TextureBumpNode::Render() const
	{
		// bind texture
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);

		// bind bump map
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, bumpmap);

		// render objects without materials
		Shader::SetTextureBlend(false);
		RenderNode::Render();
		Shader::SetTextureBlend(true);

		// render material nodes
		for (auto it = materialNodeMap.begin(); it != materialNodeMap.end(); ++it)
		{
			(*it).second->Render();
		}
	}

	// TEXTURENODE PUBLIC

	void TextureNode::Render() const
	{
		// bind texture
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);

		// render objects without materials
		Shader::SetTextureBlend(false);
		RenderNode::Render();
		Shader::SetTextureBlend(true);

		// render material nodes
		for (auto it = materialNodeMap.begin(); it != materialNodeMap.end(); ++it)
		{
			(*it).second->Render();
		}
	}

	// TEXTUREEMISSIVENODE PUBLIC

	void TextureEmissiveNode::Render() const
	{
		// bind texture
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);

		// render objects
		RenderNode::Render();
	}

	// MATERIALNODE PUBLIC

	void MaterialNode::Render() const
	{
		// send material properties to GPU
		Shader::SetMaterialAmbient(material.ambient);
		Shader::SetMaterialDiffuse(material.diffuse);
		Shader::SetMaterialSpecular(material.specular);
		Shader::SetMaterialShininess(material.shininess);

		// render objects
		RenderNode::Render();
	}

	// EMISSIVENODE PUBLIC

	void EmissiveNode::Render() const
	{
		// send color to GPU
		Shader::SetEmissionColor(emissionColor);

		// render objects
		RenderNode::Render();
	}

	// RENDERNODE PUBLIC

	void RenderNode::Render() const
	{
		// render objects
		for (auto it = objects.begin(); it != objects.end(); ++it)
		{
			auto o = *it;
			if (o->GetRenderEnabled())
				o->Draw();
		}
	}
}
