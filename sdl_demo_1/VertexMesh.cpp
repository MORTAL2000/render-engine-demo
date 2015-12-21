#include "VertexMesh.h"
#include "Shader.h"
#include <iostream>

namespace Bagnall
{
	// PUBLIC
	VertexMesh::VertexMesh(const Material& mat, int vOffset, int vCount, bool tStrip)
	{
		material = mat;
		globalVertexOffset = vOffset;
		vertexCount = vCount;
		triangleStrip = tStrip;
	}


	void VertexMesh::AddVertexMesh(const char *name, const Material& mat, int vOffset, int vCount, bool tStrip)
	{
		auto it = vertexMeshMap.find(name);
		if (it == vertexMeshMap.end())
			vertexMeshMap.emplace(name, VertexMesh(mat, vOffset, vCount, tStrip));
		else
			std::cerr << "vertex mesh already exists with name: " << name << std::endl;
	}

	VertexMesh VertexMesh::GetVertexMeshByName(const char *name)
	{
		auto it = vertexMeshMap.find(name);
		if (it != vertexMeshMap.end())
			return vertexMeshMap[name];
		else
		{
			std::cerr << "vertex mesh doesn't exist with name: " << name << std::endl;
			return VertexMesh(Material::None(), 0, 0, 0);
		}
	}

	void VertexMesh::Draw() const
	{
		Shader::SetMaterial(material);

		// hacky way of doing triangle strip instead of triangles when the bool is set
		// because GL_TRIANGLES is 4 and GL_TRIANGLE_STRIP is 5
		glDrawArrays(GL_TRIANGLES + triangleStrip, globalVertexOffset, vertexCount);
	}

	// PRIVATE

	std::unordered_map<const char*, VertexMesh> VertexMesh::vertexMeshMap;
}
