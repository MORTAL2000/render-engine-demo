#include "Shader.h"
#include "InitShader.h"
#include <iostream>

namespace Bagnall
{
	// PUBLIC

	std::vector<vec4> Shader::Vertices;
	std::vector<vec4> Shader::Normals;
	std::vector<vec4> Shader::Tangents;
	std::vector<vec4> Shader::Binormals;
	std::vector<vec2> Shader::TextureCoordinates;

	GLuint Shader::MaterialAmbientLoc;
	GLuint Shader::MaterialDiffuseLoc;
	GLuint Shader::MaterialSpecularLoc;
	GLuint Shader::MaterialShininessLoc;
	GLuint Shader::LightSourceLoc;
	GLuint Shader::CameraPositionLoc;
	GLuint Shader::ModelLoc;
	GLuint Shader::InverseModelLoc;
	GLuint Shader::CameraLoc;
	GLuint Shader::ProjectionLoc;
	GLuint Shader::EmissiveLoc;
	GLuint Shader::EmissionColorLoc;
	GLuint Shader::AlphaOverrideLoc;
	GLuint Shader::UseTextureLoc;
	GLuint Shader::TexLoc;
	GLuint Shader::UseBumpMapLoc;
	GLuint Shader::BumpTexLoc;
	GLuint Shader::UseCubeMapLoc;
	GLuint Shader::CubeMapLoc;

	void Shader::Init()
	{
		// Create a vertex array object
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		// set up vertex buffer
		if (Vertices.size())
		{
			glGenBuffers(1, &buffer);
			glBindBuffer(GL_ARRAY_BUFFER, buffer);
			glBufferData(GL_ARRAY_BUFFER, sizeof(vec4)*Vertices.size() + sizeof(vec4)*Normals.size() + sizeof(vec4)*Tangents.size() + sizeof(vec4)*Binormals.size() + sizeof(vec4)*TextureCoordinates.size(), NULL, GL_STATIC_DRAW);
			glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vec4)*Vertices.size(), &Vertices[0]);
			glBufferSubData(GL_ARRAY_BUFFER, sizeof(vec4)*Vertices.size(), sizeof(vec4)*Normals.size(), &Normals[0]);
			glBufferSubData(GL_ARRAY_BUFFER, sizeof(vec4)*Vertices.size() + sizeof(vec4)*Normals.size(), sizeof(vec4)*Tangents.size(), &Tangents[0]);
			glBufferSubData(GL_ARRAY_BUFFER, sizeof(vec4)*Vertices.size() + sizeof(vec4)*Normals.size() + sizeof(vec4)*Tangents.size(), sizeof(vec4)*Binormals.size(), &Binormals[0]);
			glBufferSubData(GL_ARRAY_BUFFER, sizeof(vec4)*Vertices.size() + sizeof(vec4)*Normals.size() + sizeof(vec4)*Tangents.size() + sizeof(vec4)*Binormals.size(), sizeof(vec2)*TextureCoordinates.size(), &TextureCoordinates[0]);
		}

		// Load shaders and use the resulting shader program
		GLuint program = InitShader("shaders\\vshader.glsl", "shaders\\fshader.glsl");
		glUseProgram(program);

		// set up vertex arrays
		GLuint vPositionLoc = glGetAttribLocation(program, "vPosition");
		glEnableVertexAttribArray(vPositionLoc);
		glVertexAttribPointer(vPositionLoc, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

		GLuint vNormalLoc = glGetAttribLocation(program, "vNormal");
		glEnableVertexAttribArray(vNormalLoc);
		glVertexAttribPointer(vNormalLoc, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(vec4)*Vertices.size()));

		GLuint vTangentLoc = glGetAttribLocation(program, "vTangent");
		glEnableVertexAttribArray(vTangentLoc);
		glVertexAttribPointer(vTangentLoc, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(vec4)*Vertices.size() + sizeof(vec4)*Normals.size()));

		GLuint vBinormal = glGetAttribLocation(program, "vBinormal");
		glEnableVertexAttribArray(vBinormal);
		glVertexAttribPointer(vBinormal, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(vec4)*Vertices.size() + sizeof(vec4)*Normals.size() + sizeof(vec4)*Tangents.size()));

		GLuint vTextureCoordinateLoc = glGetAttribLocation(program, "vTextureCoordinate");
		glEnableVertexAttribArray(vTextureCoordinateLoc);
		glVertexAttribPointer(vTextureCoordinateLoc, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(vec4)*Vertices.size() + sizeof(vec4)*Normals.size() + sizeof(vec4)*Tangents.size() + sizeof(vec4)*Binormals.size()));

		// Initialize attributes
		ModelLoc = glGetUniformLocation(program, "model");
		if (ModelLoc == -1)
			std::cerr << "Unable to find model parameter" << std::endl;

		InverseModelLoc = glGetUniformLocation(program, "inverseModel");
		if (InverseModelLoc == -1)
			std::cerr << "Unable to find inverseModel parameter" << std::endl;

		CameraLoc = glGetUniformLocation(program, "camera");
		if (CameraLoc == -1)
			std::cerr << "Unable to find camera parameter" << std::endl;

		ProjectionLoc = glGetUniformLocation(program, "projection");
		if (ProjectionLoc == -1)
			std::cerr << "Unable to find projection parameter" << std::endl;

		CameraPositionLoc = glGetUniformLocation(program, "cameraPosition");
		if (CameraPositionLoc == -1)
			std::cerr << "Unable to find cameraPosition parameter" << std::endl;

		LightSourceLoc = glGetUniformLocation(program, "lightSource");
		if (LightSourceLoc == -1)
			std::cerr << "Unable to find lightSource parameter" << std::endl;

		MaterialAmbientLoc = glGetUniformLocation(program, "materialAmbient");
		if (MaterialAmbientLoc == -1)
			std::cerr << "Unable to find materialAmbient parameter" << std::endl;

		MaterialDiffuseLoc = glGetUniformLocation(program, "materialDiffuse");
		if (MaterialDiffuseLoc == -1)
			std::cerr << "Unable to find diffuseProductLoc parameter" << std::endl;

		MaterialSpecularLoc = glGetUniformLocation(program, "materialSpecular");
		if (MaterialSpecularLoc == -1)
			std::cerr << "Unable to find specularProductLoc parameter" << std::endl;

		MaterialShininessLoc = glGetUniformLocation(program, "materialShininess");
		if (MaterialShininessLoc == -1)
			std::cerr << "Unable to find materialShininess parameter" << std::endl;

		EmissiveLoc = glGetUniformLocation(program, "emissive");
		if (EmissiveLoc == -1)
			std::cerr << "Unable to find emissive parameter" << std::endl;

		EmissionColorLoc = glGetUniformLocation(program, "emissionColor");
		if (EmissionColorLoc == -1)
			std::cerr << "Unable to find emissionColor parameter" << std::endl;

		AlphaOverrideLoc = glGetUniformLocation(program, "alphaOverride");
		if (AlphaOverrideLoc == -1)
			std::cerr << "Unable to find alphaOverride parameter" << std::endl;

		UseTextureLoc = glGetUniformLocation(program, "useTexture");
		if (UseTextureLoc == -1)
			std::cerr << "Unable to find useTexture parameter" << std::endl;

		UseBumpMapLoc = glGetUniformLocation(program, "useBumpMap");
		if (UseBumpMapLoc == -1)
			std::cerr << "Unable to find useBumpMap parameter" << std::endl;
		
		UseCubeMapLoc = glGetUniformLocation(program, "useCubeMap");
		if (UseCubeMapLoc == -1)
			std::cerr << "Unable to find useCubeMap parameter" << std::endl;

		TexLoc = glGetUniformLocation(program, "Tex");
		if (TexLoc == -1)
			std::cerr << "Unable to find Tex parameter" << std::endl;

		BumpTexLoc = glGetUniformLocation(program, "BumpTex");
		if (BumpTexLoc == -1)
			std::cerr << "Unable to find BumpTex parameter" << std::endl;

		CubeMapLoc = glGetUniformLocation(program, "cubeMap");
		if (CubeMapLoc == -1)
			std::cerr << "Unable to find cubeMap parameter" << std::endl;
		

		glUniform1i(EmissiveLoc, 0);
		glUniform1i(UseTextureLoc, 0);
		glUniform1i(UseBumpMapLoc, 0);
		glUniform1i(UseCubeMapLoc, 0);
		glUniform1i(TexLoc, 0);
		glUniform1i(BumpTexLoc, 1);
		glUniform1i(CubeMapLoc, 2);
		glUniform1f(AlphaOverrideLoc, 0.0f);
	}

	void Shader::SetMaterialAmbient(const vec4& materialAmbient)
	{
		glUniform4fv(MaterialAmbientLoc, 1, value_ptr(materialAmbient));
	}

	void Shader::SetMaterialDiffuse(const vec4& materialDiffuse)
	{
		glUniform4fv(MaterialDiffuseLoc, 1, value_ptr(materialDiffuse));
	}

	void Shader::SetMaterialSpecular(const vec4& materialSpecular)
	{
		glUniform4fv(MaterialSpecularLoc, 1, value_ptr(materialSpecular));
	}

	void Shader::SetMaterialShininess(float materialShininess)
	{
		glUniform1f(MaterialShininessLoc, materialShininess);
	}

	void Shader::SetLightSource(const mat4& lightSource)
	{
		glUniformMatrix4fv(LightSourceLoc, 1, GL_FALSE, value_ptr(lightSource));
	}

	void Shader::SetCameraPosition(const vec4& cameraPosition)
	{
		glUniform4fv(CameraPositionLoc, 1, value_ptr(cameraPosition));
	}

	void Shader::SetModel(const mat4& model)
	{
		glUniformMatrix4fv(ModelLoc, 1, GL_FALSE, value_ptr(model));
	}

	void Shader::SetInverseModel(const mat4& inverseModel)
	{
		glUniformMatrix4fv(InverseModelLoc, 1, GL_FALSE, value_ptr(inverseModel));
	}

	void Shader::SetCamera(const mat4& camera)
	{
		glUniformMatrix4fv(CameraLoc, 1, GL_FALSE, value_ptr(camera));
	}

	void Shader::SetProjection(const mat4& projection)
	{
		glUniformMatrix4fv(ProjectionLoc, 1, GL_FALSE, value_ptr(projection));
	}

	void Shader::SetEmissive(bool emissive)
	{
		glUniform1i(EmissiveLoc, emissive);
	}

	void Shader::SetEmissionColor(const vec4& emissionColor)
	{
		glUniform4fv(EmissionColorLoc, 1, value_ptr(emissionColor));
	}

	void Shader::SetAlphaOverride(const vec4& alphaOverride)
	{
		glUniform4fv(AlphaOverrideLoc, 1, value_ptr(alphaOverride));
	}

	void Shader::SetUseTexture(bool useTexture)
	{
		glUniform1i(UseTextureLoc, useTexture);
	}

	void Shader::SetTex(int tex)
	{
		glUniform1i(TexLoc, tex);
	}

	void Shader::SetUseBumpMap(bool useBumpMap)
	{
		glUniform1i(UseBumpMapLoc, useBumpMap);
	}

	void Shader::SetBumpTex(int bumpTex)
	{
		glUniform1i(BumpTexLoc, bumpTex);
	}

	void Shader::SetUseCubeMap(bool useCubeMap)
	{
		glUniform1i(UseCubeMapLoc, useCubeMap);
	}

	void Shader::SetCubeMap(int cubeMap)
	{
		glUniform1i(CubeMapLoc, cubeMap);
	}

	// PRIVATE

	GLuint Shader::vao;
	GLuint Shader::buffer;
}
