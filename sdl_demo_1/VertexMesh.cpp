#include "VertexMesh.h"
#include <iostream>

namespace Bagnall
{
	// PUBLIC

	void VertexMesh::AddVertexMesh(const char *name, int vOffset, int vCount, bool tStrip)
	{
		auto it = vertexMeshMap.find(name);
		if (it == vertexMeshMap.end())
			vertexMeshMap.emplace(name, new VertexMesh(vOffset, vCount, tStrip));
		else
			std::cerr << "vertex mesh already exists with name: " << name << std::endl;
	}

	VertexMesh* VertexMesh::GetVertexMeshByName(const char *name)
	{
		auto it = vertexMeshMap.find(name);
		if (it != vertexMeshMap.end())
			return vertexMeshMap[name];
		else
		{
			std::cerr << "vertex mesh doesn't exist with name: " << name << std::endl;
			return nullptr;
		}
	}

	void VertexMesh::Draw() const
	{
		// hacky way of doing triangle strip instead of triangles when the bool is set
		// because GL_TRIANGLES is 4 and GL_TRIANGLE_STRIP is 5
		glDrawArrays(GL_TRIANGLES + triangleStrip, globalVertexOffset, vertexCount);
	}

	// PRIVATE

	std::unordered_map<const char*, VertexMesh*> VertexMesh::vertexMeshMap;

	VertexMesh::VertexMesh(int vOffset, int vCount, bool tStrip)
	{
		globalVertexOffset = vOffset;
		vertexCount = vCount;
		triangleStrip = tStrip;
	}
}
