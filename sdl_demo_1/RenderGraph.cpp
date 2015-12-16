#include "RenderGraph.h"
#include "DrawableObject.h"
#include "Shader.h"
#include "Texture.h"

namespace Bagnall
{
	// RENDERGRAPH PUBLIC

	RenderNode* RenderGraph::AddDrawableObject(DrawableObject *o)
	{
		// non-emissive
		if (!o->GetEmissive())
		{
			auto texture = o->GetTexture();

			// no texture, only material
			if (texture == 0)
			{
				if (materialNodeMap.find(o->GetMaterial()) == materialNodeMap.end())
					materialNodeMap.emplace(o->GetMaterial(), new MaterialNode(o->GetMaterial()));

				auto materialNode = materialNodeMap[o->GetMaterial()];

				materialNode->objects.push_back(o);

				return materialNode;
			}
			// texture and material
			else
			{
				std::unordered_map<Material, MaterialNode*> *_materialNodeMap;

				auto bumpmap = Texture::GetBumpMapByTexture(texture);
				// with bump map
				if (bumpmap != 0)
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

				auto materialNode = (*_materialNodeMap)[o->GetMaterial()];

				materialNode->objects.push_back(o);

				return materialNode;
			}
		}
		// emissive
		else
		{
			if (emissiveNodeMap.find(o->GetEmissionColor()) == emissiveNodeMap.end())
				emissiveNodeMap.emplace(o->GetEmissionColor(), new EmissiveNode(o->GetEmissionColor()));

			auto emissiveNode = emissiveNodeMap[o->GetEmissionColor()];

			emissiveNode->objects.push_back(o);

			return emissiveNode;
		}
	}

	void RenderGraph::Render() const
	{
		// render objects with texture, bump map, material
		Shader::SetUseTexture(true);
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

		// render emissive objects
		Shader::SetEmissive(true);
		for (auto it = emissiveNodeMap.begin(); it != emissiveNodeMap.end(); ++it)
		{
			(*it).second->Render();
		}
		Shader::SetEmissive(false);
	}

	// TEXTUREBUMPNODE PUBLIC

	void TextureBumpNode::Render() const
	{
		//// bind texture
		//glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_2D, texture);

		//// bind bump map
		//glActiveTexture(GL_TEXTURE1);
		//glBindTexture(GL_TEXTURE_2D, bumpmap);

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

		// render material nodes
		for (auto it = materialNodeMap.begin(); it != materialNodeMap.end(); ++it)
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
		for (auto it = objects.begin(); it != objects.end(); ++it)
		{
			(*it)->Draw();
		}
	}

	// EMISSIVENODE PUBLIC

	void EmissiveNode::Render() const
	{
		// send color to GPU
		Shader::SetEmissionColor(emissionColor);

		// render objects
		for (auto it = objects.begin(); it != objects.end(); ++it)
		{
			(*it)->Draw();
		}
	}
}
