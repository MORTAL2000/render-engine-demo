#ifndef VERTEXMESH_H
#define VERTEXMESH_H

//#include "Shader.h"
//#include <glm/vec2.hpp>
//#include <glm/vec4.hpp>
#include <GL\glew.h>
#include <unordered_map>

namespace Bagnall
{
	class VertexMesh
	{
	public:
		static void AddVertexMesh(const char *name, int vOffset, int vCount, bool tStrip);

		static VertexMesh* GetVertexMeshByName(const char *name);

		void Draw() const;

	private:
		static std::unordered_map<const char*, VertexMesh*> vertexMeshMap;

		int globalVertexOffset;
		int vertexCount;
		bool triangleStrip;

		VertexMesh(int vOffset, int vCount, bool tStrip);
	};
}

#endif
