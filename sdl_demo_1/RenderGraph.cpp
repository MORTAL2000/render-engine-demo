#include "RenderGraph.h"
#include "DrawableObject.h"
#include "Shader.h"
#include "Texture.h"
#include "Shadow.h"

namespace Bagnall
{
	// RENDERGRAPH PUBLIC

	RenderGraph::RenderGraph()
	{
		shadowsEnabled = false;
	}

	RenderNode* RenderGraph::AddDrawableObject(DrawableObject *o)
	{
		auto texture = o->GetTexture();
		auto material = o->GetMaterial();
		auto bumpmap = Texture::GetBumpMapByTexture(texture);

		// non-emissive
		if (!o->GetEmissive())
		{
			MaterialNode *materialNode;

			// no texture, only material
			if (texture == 0)
			{
				if (materialNodeMap.find(o->GetMaterial()) == materialNodeMap.end())
					materialNodeMap.emplace(o->GetMaterial(), new MaterialNode(o->GetMaterial()));

				materialNode = materialNodeMap[o->GetMaterial()];
			}
			// no material, texture only
			else if (material == Material::None())
			{
				// with bump map
				if (bumpmap != 0 && o->GetBumpMapEnabled())
				{
					if (textureBumpNodeMap.find(texture) == textureBumpNodeMap.end())
						textureBumpNodeMap.emplace(texture, new TextureBumpNode(texture, bumpmap));

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
				if (bumpmap != 0 && o->GetBumpMapEnabled())
				{
					if (textureBumpNodeMap.find(texture) == textureBumpNodeMap.end())
						textureBumpNodeMap.emplace(texture, new TextureBumpNode(texture, bumpmap));

					_materialNodeMap = &textureBumpNodeMap[texture]->materialNodeMap;
				}
				// without bump map
				else
				{
					if (textureNodeMap.find(texture) == textureNodeMap.end())
						textureNodeMap.emplace(texture, new TextureNode(texture));

					_materialNodeMap = &textureNodeMap[texture]->materialNodeMap;
				}

				if (_materialNodeMap->find(o->GetMaterial()) == _materialNodeMap->end())
					_materialNodeMap->emplace(o->GetMaterial(), new MaterialNode(o->GetMaterial()));

				materialNode = (*_materialNodeMap)[o->GetMaterial()];
			}

			materialNode->objects.push_back(o);
			return materialNode;
		}
		// emissive
		else
		{
			EmissiveNode *emissiveNode;

			// no texture
			if (texture == 0)
			{
				if (emissiveNodeMap.find(o->GetEmissionColor()) == emissiveNodeMap.end())
					emissiveNodeMap.emplace(o->GetEmissionColor(), new EmissiveNode(o->GetEmissionColor()));

				emissiveNode = emissiveNodeMap[o->GetEmissionColor()];
			}
			// with texture
			else
			{
				if (textureEmissiveNodeMap.find(texture) == textureEmissiveNodeMap.end())
					textureEmissiveNodeMap.emplace(texture, new TextureEmissiveNode(texture));

				auto _emissiveNodeMap = &textureEmissiveNodeMap[texture]->emissiveNodeMap;

				if (_emissiveNodeMap->find(o->GetEmissionColor()) == _emissiveNodeMap->end())
					_emissiveNodeMap->emplace(o->GetEmissionColor(), new EmissiveNode(o->GetEmissionColor()));

				emissiveNode = (*_emissiveNodeMap)[o->GetEmissionColor()];
			}

			emissiveNode->objects.push_back(o);
			return emissiveNode;
		}
	}

	void RenderGraph::Render() const
	{
		// render emissive objects
		Shader::SetEmissive(true);
		for (auto it = emissiveNodeMap.begin(); it != emissiveNodeMap.end(); ++it)
		{
			(*it).second->Render();
		}

		// render emissive objects with texture
		Shader::SetUseTexture(true);
		for (auto it = textureEmissiveNodeMap.begin(); it != textureEmissiveNodeMap.end(); ++it)
		{
			(*it).second->Render();
		}
		Shader::SetEmissive(false);

		// activate shadows if enabled
		if (shadowsEnabled)
		{
			Shadow::SendToGPU();
			Shader::SetUseShadowMap(true);
		}

		// render objects with texture, bump map, material
		Shader::SetUseBumpMap(true);
		for (auto it = textureBumpNodeMap.begin(); it != textureBumpNodeMap.end(); ++it)
		{
			(*it).second->Render();
		}
		Shader::SetUseBumpMap(false);

		// render objects with texture, material
		for (auto it = textureNodeMap.begin(); it != textureNodeMap.end(); ++it)
		{
			(*it).second->Render();
		}
		Shader::SetUseTexture(false);
		glBindTexture(GL_TEXTURE_2D, 0);

		// render objects with only material
		for (auto it = materialNodeMap.begin(); it != materialNodeMap.end(); ++it)
		{
			(*it).second->Render();
		}
		
		// deactivate shadows
		if (shadowsEnabled)
		{
			Shader::SetUseShadowMap(false);
		}
	}

	void RenderGraph::SetShadowsEnabled(bool s)
	{
		shadowsEnabled = s;
	}

	bool RenderGraph::GetShadowsEnabled() const
	{
		return shadowsEnabled;
	}

	// TEXTUREBUMPNODE PUBLIC

	void TextureBumpNode::Render() const
	{
		//// bind texture
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

		// render material nodes
		for (auto it = emissiveNodeMap.begin(); it != emissiveNodeMap.end(); ++it)
		{
			(*it).second->Render();
		}
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
