/*************************************************************************************

Program:			CS 4250 Final Project

Author:				Alexander Bagnall
Email:				ab667712@ohio.edu

Description:		Implementation file for the Rectangle class.

Date:				December 9, 2015

*************************************************************************************/

#include "Rectangle.h"
#include "Geometry.h"
#include "Util.h"
#include "Shader.h"
#include "VertexMesh.h"

namespace Bagnall
{
	// PUBLIC

	void Rectangle::Init()
	{
		createPrototypeVertices();
	}

	Rectangle::Rectangle(Object *par) : DrawableObject(par)
	{
		auto mesh = VertexMesh::GetVertexMeshPrototypeByName("rectangle");
		mesh.SetOwner(this);
		vertexMeshes.push_back(mesh);
	}

	void Rectangle::Draw() const
	{
		DrawableObject::Draw();
		glDrawArrays(GL_TRIANGLE_STRIP, globalVertexOffset, vertexCount);
	}

	// PRIVATE

	int Rectangle::globalVertexOffset;
	int Rectangle::vertexCount;

	void Rectangle::createPrototypeVertices()
	{
		std::vector<vec4> rect = Geometry::CreateRectangle(1.0);
		std::vector<vec4> normals({ vec4(0.0, 0.0, 1.0, 0.0), vec4(0.0, 0.0, 1.0, 0.0), vec4(0.0, 0.0, 1.0, 0.0), vec4(0.0, 0.0, 1.0, 0.0) });
		std::vector<vec4> tangents({ vec4(1.0, 0.0, 0.0, 0.0), vec4(1.0, 0.0, 0.0, 0.0), vec4(1.0, 0.0, 0.0, 0.0), vec4(1.0, 0.0, 0.0, 0.0) });
		std::vector<vec4> binormals({ vec4(0.0, 1.0, 0.0, 0.0), vec4(0.0, 1.0, 0.0, 0.0), vec4(0.0, 1.0, 0.0, 0.0), vec4(0.0, 1.0, 0.0, 0.0) });

		vertexCount = rect.size();
		globalVertexOffset = Shader::Vertices.size();
		Shader::Vertices.insert(Shader::Vertices.end(), rect.begin(), rect.end());
		Shader::Normals.insert(Shader::Normals.end(), normals.begin(), normals.end());
		Shader::Tangents.insert(Shader::Tangents.end(), tangents.begin(), tangents.end());
		Shader::Binormals.insert(Shader::Binormals.end(), binormals.begin(), binormals.end());

		std::vector<vec2> texCoords = Util::Vec4toVec2(Util::TransformVertices(rect, translate(vec3(0.5f, 0.5f, 0.0f)) * glm::scale(vec3(1.0, 1.0, 1.0))));
		Shader::TextureCoordinates.insert(Shader::TextureCoordinates.end(), texCoords.begin(), texCoords.end());

		VertexMesh::AddVertexMeshPrototype("rectangle", Material::Plastic(vec4(0.0, 0.0, 0.0, 1.0)), globalVertexOffset, vertexCount, true);
	}
}
