#ifndef RENDERGRAPH_H
#define RENDERGRAPH_H

#include "RenderNode.h"
#include "Material.h"
#include <GL/glew.h>
#include <vector>
#include <unordered_map>

namespace Bagnall
{
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
		RenderNode* AddVertexMesh(VertexMesh *o);

		// render everything
		void Render() const;

		// shadowsEnabled getter/setter
		void SetShadowMode(int s);
		int GetShadowMode() const;

		void SetShadowZRange(float zNear, float zFar);

	private:
		int shadowMode;
	};
}

#endif
