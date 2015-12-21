#ifndef VERTEXMESH_H
#define VERTEXMESH_H

//#include "Shader.h"
//#include <glm/vec2.hpp>
//#include <glm/vec4.hpp>
#include "Material.h"
#include <GL/glew.h>
#include <unordered_map>

namespace Bagnall
{
	class VertexMesh
	{
	public:
		// need public default constructor to use it as non-dynamic memory struct
		VertexMesh() {}

		VertexMesh(const Material& mat, int vOffset, int vCount, bool tStrip);

		static void AddVertexMesh(const char *name, const Material& mat, int vOffset, int vCount, bool tStrip);

		static VertexMesh GetVertexMeshByName(const char *name);

		void Draw() const;

	private:
		static std::unordered_map<const char*, VertexMesh> vertexMeshMap;

		Material material;
		int globalVertexOffset;
		int vertexCount;
		bool triangleStrip;
	};
}

#endif
