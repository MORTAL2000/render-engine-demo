#include "Sphere.h"
#include "Geometry.h"
#include "Util.h"
#include "Shader.h"

namespace Bagnall
{
	// PUBLIC

	void Sphere::Init()
	{
		createPrototypeVertices();
	}

	void Sphere::Draw() const
	{
		DrawableObject::Draw();
		glDrawArrays(GL_TRIANGLE_STRIP, globalVertexOffset, vertexCount);
	}

	// PRIVATE

	int Sphere::globalVertexOffset, Sphere::vertexCount;

	void Sphere::createPrototypeVertices()
	{
		std::vector<vec4> sphere = Geometry::CreateSphere(15.0f);
		std::vector<vec4> normals = Geometry::CreateSphereNormals(sphere);

		// garbage values for these
		std::vector<vec4> tangents(sphere);
		std::vector<vec4> binormals(sphere);
		std::vector<vec2> texCoords = Util::Vec4toVec2(sphere);

		vertexCount = sphere.size();
		globalVertexOffset = Shader::Vertices.size();
		Shader::Vertices.insert(Shader::Vertices.end(), sphere.begin(), sphere.end());
		Shader::Normals.insert(Shader::Normals.end(), normals.begin(), normals.end());
		Shader::Tangents.insert(Shader::Tangents.end(), tangents.begin(), tangents.end());
		Shader::Binormals.insert(Shader::Binormals.end(), binormals.begin(), binormals.end());

		Shader::TextureCoordinates.insert(Shader::TextureCoordinates.end(), texCoords.begin(), texCoords.end());
	}
}
