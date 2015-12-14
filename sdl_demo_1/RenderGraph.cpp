#include "RenderGraph.h"
#include "DrawableObject.h"
#include "Shader.h"

namespace Bagnall
{
	// RENDERGRAPH PUBLIC

	RenderNode* RenderGraph::AddDrawableObject(DrawableObject *o)
	{
		// emissive
		if (!o->GetEmissive())
		{
			if (textureNodeMap.find(o->GetTexture()) == textureNodeMap.end())
				textureNodeMap.emplace(o->GetTexture(), new TextureNode(o->GetTexture()));

			auto materialNodeMap = &textureNodeMap[o->GetTexture()]->materialNodeMap;

			if (materialNodeMap->find(o->GetMaterial()) == materialNodeMap->end())
				materialNodeMap->emplace(o->GetMaterial(), new MaterialNode(o->GetMaterial()));

			auto materialNode = (*materialNodeMap)[o->GetMaterial()];

			materialNode->objects.push_back(o);

			return materialNode;
		}
		// texture and material
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
		for (auto it = textureNodeMap.begin(); it != textureNodeMap.end(); ++it)
		{
			(*it).second->Render();
		}

		for (auto it = emissiveNodeMap.begin(); it != emissiveNodeMap.end(); ++it)
		{
			(*it).second->Render();
		}
	}

	// TEXTURENODE PUBLIC

	void TextureNode::Render() const
	{
		// bind texture

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
