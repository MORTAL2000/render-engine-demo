#include "Skybox.h"
#include "Geometry.h"
#include "Util.h"
#include "Shader.h"

namespace Bagnall
{
	// PUBLIC

	void Skybox::Init()
	{
		createPrototypeVertices();
	}

	void Skybox::Draw() const
	{
		DrawableObject::Draw();
		glDrawArrays(GL_TRIANGLES, globalVertexOffset, vertexCount);
	}

	// PRIVATE

	int Skybox::globalVertexOffset, Skybox::vertexCount;

	void Skybox::createPrototypeVertices()
	{
		std::vector<vec4> cube = Geometry::CreateCube(1.0);
		std::vector<vec4> normals = Geometry::CreateCubeNormals();
		// negate the normals
		Util::Apply(normals, [](const vec4& v) -> vec4
		{
			return -v;
		});
		std::vector<vec4> tangents = Geometry::CreateCubeTangents();
		std::vector<vec4> binormals = Geometry::CreateCubeBiNormals();

		vertexCount = cube.size();
		globalVertexOffset = Shader::Vertices.size();
		Shader::Vertices.insert(Shader::Vertices.end(), cube.begin(), cube.end());
		Shader::Normals.insert(Shader::Normals.end(), normals.begin(), normals.end());
		Shader::Tangents.insert(Shader::Tangents.end(), tangents.begin(), tangents.end());
		Shader::Binormals.insert(Shader::Binormals.end(), binormals.begin(), binormals.end());

		std::vector<vec2> texCoords = Geometry::CreateSkyboxTextureCoordinates();
		Shader::TextureCoordinates.insert(Shader::TextureCoordinates.end(), texCoords.begin(), texCoords.end());
	}
}
