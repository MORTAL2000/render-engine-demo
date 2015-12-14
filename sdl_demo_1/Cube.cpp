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

namespace Bagnall
{
	// PUBLIC

	void Cube::Init()
	{
		createPrototypeVertices();
	}

	void Cube::Draw() const
	{
		DrawableObject::Draw();
		glDrawArrays(GL_TRIANGLES, globalVertexOffset, vertexCount);
	}

	/*void Cube::DrawRaw() const
	{
		Object::DrawRaw();
		glDrawArrays(GL_TRIANGLES, globalVertexOffset, vertexCount);
	}

	void Cube::DrawEmissive() const
	{
		Object::StartDrawEmissive();
		glDrawArrays(GL_TRIANGLES, globalVertexOffset, vertexCount);
		Object::EndDrawEmissive();
	}

	void Cube::DrawWithTexture(GLuint t) const
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, t);
		glUniform1i(Game::UseTextureLoc, 1);
		Draw();
		glUniform1i(Game::UseTextureLoc, 0);
	}*/

	//void Cube::DrawWithTextureAndBumpMap(GLuint t, GLuint bmap) const
	//{
	//	glActiveTexture(GL_TEXTURE0);
	//	glBindTexture(GL_TEXTURE_2D, t);
	//	glActiveTexture(GL_TEXTURE0 + 1);
	//	glBindTexture(GL_TEXTURE_2D, bmap);
	//	glUniform1i(Shader::UseTextureLoc, 1);
	//	//glUniform1i(Game::UseBumpMapLoc, 1);
	//	Draw();
	//	glUniform1i(Shader::UseTextureLoc, 0);
	//	glUniform1i(Shader::UseBumpMapLoc, 0);
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

		vertexCount = cube.size();
		globalVertexOffset = Shader::Vertices.size();
		Shader::Vertices.insert(Shader::Vertices.end(), cube.begin(), cube.end());
		Shader::Normals.insert(Shader::Normals.end(), normals.begin(), normals.end());

		//std::vector<vec2> texCoords = Util::Vec4toVec2(cube);
		std::vector<vec2> texCoords = Geometry::CreateCubeTextureCoordinates();
		Shader::TextureCoordinates.insert(Shader::TextureCoordinates.end(), texCoords.begin(), texCoords.end());
	}
}
