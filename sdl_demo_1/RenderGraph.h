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

		virtual void Render() const;
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

	struct TextureBumpNode : public RenderNode
	{
		GLuint texture;
		GLuint bumpmap;
		std::unordered_map<Material, MaterialNode*> materialNodeMap;

		TextureBumpNode(GLuint tex, GLuint bump) { texture = tex; bumpmap = bump; }

		void Render() const;
	};

	struct TextureNode : public RenderNode
	{
		GLuint texture;
		std::unordered_map<Material, MaterialNode*> materialNodeMap;

		TextureNode(GLuint tex) { texture = tex; }

		void Render() const;
	};

	struct TextureEmissiveNode
	{
		GLuint texture;
		std::unordered_map<vec4, EmissiveNode*> emissiveNodeMap;

		TextureEmissiveNode(GLuint tex) { texture = tex; }

		void Render() const;
	};

	class RenderGraph
	{
	public:
		RenderGraph();

		// root node for textured objects with bump maps and normal lighting
		std::unordered_map<GLuint, TextureBumpNode*> textureBumpNodeMap;

		// root node for textured objects without bump maps and normal lighting
		std::unordered_map<GLuint, TextureNode*> textureNodeMap;

		// root node for textured emissive objects
		std::unordered_map<GLuint, TextureEmissiveNode*> textureEmissiveNodeMap;

		// root node for non-textured objects with normal lighting
		std::unordered_map<Material, MaterialNode*> materialNodeMap;

		// root node for emissive objects
		std::unordered_map<vec4, EmissiveNode*> emissiveNodeMap;

		RenderNode* AddDrawableObject(DrawableObject *o);

		void Render() const;

		void SetShadowsEnabled(bool s);

		bool GetShadowsEnabled() const;

	private:
		bool shadowsEnabled;
	};
}

#endif
