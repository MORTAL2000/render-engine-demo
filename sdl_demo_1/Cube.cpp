/*************************************************************************************

Program:			CS 4250 Final Project

Author:				Alexander Bagnall
Email:				ab667712@ohio.edu

Description:		Implementation file for the Cube class.

Date:				December 9, 2015

*************************************************************************************/

#include "Cube.h"
#include "Geometry.h"
#include "Util.h"
#include "Shader.h"
#include "VertexMesh.h"
#include "Material.h"

namespace Bagnall
{
	// PUBLIC

	void Cube::Init()
	{
		createPrototypeVertices();
	}

	Cube::Cube(Object *par) : DrawableObject(par)
	{
		auto mesh = VertexMesh::GetVertexMeshPrototypeByName("cube");
		mesh.SetOwner(this);
		vertexMeshes.push_back(mesh);
	}

	//void Cube::Draw() const
	//{
	//	DrawableObject::Draw();
	//	glDrawArrays(GL_TRIANGLES, globalVertexOffset, vertexCount);
	//}

	bool Cube::ContainsPoint(const vec4& p) const
	{
		vec4 tp = Util::RotateX(-theta.x) * Util::RotateY(-theta.y) * Util::RotateZ(-theta.z)
			* translate(-vec3(position.x, position.y, position.z))
			* p;

		return (tp.x >= -scale.x / 2.0 && tp.x <= scale.x / 2.0
			&& tp.y >= -scale.y / 2.0 && tp.y <= scale.y / 2.0
			&& tp.z >= -scale.z / 2.0 && tp.z <= scale.z / 2.0);
	}

	// PRIVATE

	int Cube::globalVertexOffset, Cube::vertexCount;

	void Cube::createPrototypeVertices()
	{
		std::vector<vec4> cube = Geometry::CreateCube(1.0);
		std::vector<vec4> normals = Geometry::CreateCubeNormals();
		std::vector<vec4> tangents = Geometry::CreateCubeTangents();
		std::vector<vec4> binormals = Geometry::CreateCubeBiNormals();

		vertexCount = cube.size();
		globalVertexOffset = Shader::Vertices.size();
		Shader::Vertices.insert(Shader::Vertices.end(), cube.begin(), cube.end());
		Shader::Normals.insert(Shader::Normals.end(), normals.begin(), normals.end());
		Shader::Tangents.insert(Shader::Tangents.end(), tangents.begin(), tangents.end());
		Shader::Binormals.insert(Shader::Binormals.end(), binormals.begin(), binormals.end());

		//std::vector<vec2> texCoords = Util::Vec4toVec2(cube);
		std::vector<vec2> texCoords = Geometry::CreateCubeTextureCoordinates();
		Shader::TextureCoordinates.insert(Shader::TextureCoordinates.end(), texCoords.begin(), texCoords.end());

		VertexMesh::AddVertexMeshPrototype("cube", Material::Plastic(vec4(0.0, 0.0, 0.0, 1.0)), globalVertexOffset, vertexCount, false);
	}
}
