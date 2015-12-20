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

	struct TextureEmissiveNode : public RenderNode
	{
		GLuint texture;

		TextureEmissiveNode(GLuint tex) { texture = tex; }

		void Render() const;
	};

	struct CubeMapNode : public RenderNode
	{
		GLuint cubeMap;

		std::unordered_map<Material, MaterialNode*> materialNodeMap;

		CubeMapNode(GLuint cubeM) { cubeMap = cubeM; }

		void Render() const;
	};

	struct CubeMapBumpNode : public RenderNode
	{
		GLuint cubeMap;
		GLuint bumpMap;
		std::unordered_map<Material, MaterialNode*> materialNodeMap;

		CubeMapBumpNode(GLuint cubeM, GLuint bump) { cubeMap = cubeM; bumpMap = bump; }

		void Render() const;
	};

	struct CubeMapEmissiveNode : public RenderNode
	{
		GLuint cubeMap;

		CubeMapEmissiveNode(GLuint cubeM) { cubeMap = cubeM; }

		void Render() const;
	};

	class RenderGraph
	{
	public:
		// constructor
		RenderGraph();

		// root node for objects with cube map without bump maps and normal lighting
		std::unordered_map<GLuint, CubeMapNode*> cubeMapNodeMap;

		// root node for objects with cube map with bump maps and normal lighting
		std::unordered_map<GLuint, CubeMapBumpNode*> cubeMapBumpNodeMap;

		// root node for emissive objects with cube map
		std::unordered_map<GLuint, CubeMapEmissiveNode*> cubeMapEmissiveNodeMap;

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

		// add an object to the graph
		RenderNode* AddDrawableObject(DrawableObject *o);

		// render everything
		void Render() const;

		// shadowsEnabled getter/setter
		void SetShadowsEnabled(bool s);
		bool GetShadowsEnabled() const;

	private:
		bool shadowsEnabled;
	};
}

#endif
