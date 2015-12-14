#ifndef RENDERGRAPH_H
#define RENDERGRAPH_H

#include "Bagnall.h"
#include "Material.h"
#include <vector>
#include <unordered_map>

namespace Bagnall
{
	class DrawableObject;

	struct RenderNode
	{
		std::vector<DrawableObject*> objects;

		virtual void Render() const = 0;
	};

	struct EmissiveNode : public RenderNode
	{
		vec4 emissionColor;

		EmissiveNode(const vec4& emissionCol) { emissionColor = emissionCol; }

		void Render() const;
	};

	struct MaterialNode : public RenderNode
	{
		Material material;

		MaterialNode(const Material& mat) { material = mat; }

		void Render() const;
	};

	struct TextureNode
	{
		GLuint texture;
		std::unordered_map<Material, MaterialNode*> materialNodeMap;

		TextureNode(GLuint tex) { texture = tex; }

		void Render() const;
	};

	class RenderGraph
	{
	public:
		std::unordered_map<GLuint, TextureNode*> textureNodeMap;
		std::unordered_map<vec4, EmissiveNode*> emissiveNodeMap;

		RenderNode* AddDrawableObject(DrawableObject *o);

		void Render() const;
	};
}

#endif
