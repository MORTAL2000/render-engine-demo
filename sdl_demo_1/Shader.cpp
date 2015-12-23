#include "Shader.h"
#include "InitShader.h"
#include "Material.h"
#include <iostream>

namespace Bagnall
{
	// PUBLIC

	std::vector<vec4> Shader::Vertices;
	std::vector<vec4> Shader::Normals;
	std::vector<vec4> Shader::Tangents;
	std::vector<vec4> Shader::Binormals;
	std::vector<vec2> Shader::TextureCoordinates;

	void Shader::Init()
	{
		// set up vertex buffer
		if (Vertices.size())
		{
			glGenBuffers(1, &vertexBuffer);
			glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
			glBufferData(GL_ARRAY_BUFFER, sizeof(vec4)*Vertices.size() + sizeof(vec4)*Normals.size() + sizeof(vec4)*Tangents.size() + sizeof(vec4)*Binormals.size() + sizeof(vec4)*TextureCoordinates.size(), NULL, GL_STATIC_DRAW);
			glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vec4)*Vertices.size(), &Vertices[0]);
			glBufferSubData(GL_ARRAY_BUFFER, sizeof(vec4)*Vertices.size(), sizeof(vec4)*Normals.size(), &Normals[0]);
			glBufferSubData(GL_ARRAY_BUFFER, sizeof(vec4)*Vertices.size() + sizeof(vec4)*Normals.size(), sizeof(vec4)*Tangents.size(), &Tangents[0]);
			glBufferSubData(GL_ARRAY_BUFFER, sizeof(vec4)*Vertices.size() + sizeof(vec4)*Normals.size() + sizeof(vec4)*Tangents.size(), sizeof(vec4)*Binormals.size(), &Binormals[0]);
			glBufferSubData(GL_ARRAY_BUFFER, sizeof(vec4)*Vertices.size() + sizeof(vec4)*Normals.size() + sizeof(vec4)*Tangents.size() + sizeof(vec4)*Binormals.size(), sizeof(vec2)*TextureCoordinates.size(), &TextureCoordinates[0]);
		}

		// EMISSIVE COLOR PROGRAM
		initEmissiveColorProgram();

		// EMISSIVE TEXTURE PROGRAM
		initEmissiveTextureProgram();

		// EMISSIVE CUBEMAP PROGRAM
		initEmissiveCubeMapProgram();

		// DEPTH PROGRAM
		initDepthProgram();

		// MATERIAL PROGRAM
		initMaterialProgram();

		// TEXTURE PROGRAM
		initTextureProgram();

		// TEXTURE BUMP PROGRAM
		initTextureBumpProgram();

		// CUBEMAP PROGRAM
		initCubeMapProgram();

		// CUBEMAP BUMP PROGRAM
		initCubeMapBumpProgram();

		glUseProgram(nameToProgramMap["emissive_color"]);
	}

	void Shader::SetProgram(const char* programName)
	{
		GLuint program = nameToProgramMap[programName];
		if (program == 0)
			std::cerr << "unable to bind shader program: " << programName << std::endl;
		else
		{
			currentProgram = nameToProgramMap[programName];
			glUseProgram(currentProgram);
			glBindVertexArray(programToVaoMap[currentProgram]);

			SetCamera(camera);
			SetProjection(projection);

			GLuint loc = getUniformFromCurrentProgram("lightSource");
			if (loc != -1)
				glUniformMatrix4fv(loc, 1, GL_FALSE, value_ptr(lightSource));
			loc = getUniformFromCurrentProgram("cameraPosition");
			if (loc != -1)
				glUniform4fv(loc, 1, value_ptr(cameraPosition));
		}
	}

	void Shader::SetMaterialAmbient(const vec4& materialAmbient)
	{
		//GLuint loc = getUniformFromCurrentProgram("materialAmbient");
		//if (loc != -1)
		GLuint loc = programToUniformMap[currentProgram]["materialAmbient"];
			glUniform4fv(loc, 1, value_ptr(materialAmbient));
	}

	void Shader::SetMaterialDiffuse(const vec4& materialDiffuse)
	{
		//GLuint loc = getUniformFromCurrentProgram("materialDiffuse");
		//if (loc != -1)
		GLuint loc = programToUniformMap[currentProgram]["materialDiffuse"];
			glUniform4fv(loc, 1, value_ptr(materialDiffuse));
	}

	void Shader::SetMaterialSpecular(const vec4& materialSpecular)
	{
		//GLuint loc = getUniformFromCurrentProgram("materialSpecular");
		//if (loc != -1)
		GLuint loc = programToUniformMap[currentProgram]["materialSpecular"];
			glUniform4fv(loc, 1, value_ptr(materialSpecular));
	}

	void Shader::SetMaterialShininess(float materialShininess)
	{
		//GLuint loc = getUniformFromCurrentProgram("materialShininess");
		//if (loc != -1)
		GLuint loc = programToUniformMap[currentProgram]["materialShininess"];
			glUniform1f(loc, materialShininess);
	}

	void Shader::SetMaterial(const Material& material)
	{
		SetMaterialAmbient(material.ambient);
		SetMaterialDiffuse(material.diffuse);
		SetMaterialSpecular(material.specular);
		SetMaterialShininess(material.shininess);
	}

	void Shader::SetLightSource(const mat4& lSource)
	{
		lightSource = lSource;

		//GLuint loc = getUniformFromCurrentProgram("lightSource");
		//if (loc != -1)
		GLuint loc = programToUniformMap[currentProgram]["lightSource"];
			glUniformMatrix4fv(loc, 1, GL_FALSE, value_ptr(lSource));
	}

	void Shader::SetCameraPosition(const vec4& camPos)
	{
		cameraPosition = camPos;
		//GLuint loc = getUniformFromCurrentProgram("cameraPosition");
		//if (loc != -1)
		GLuint loc = programToUniformMap[currentProgram]["cameraPosition"];
			glUniform4fv(loc, 1, value_ptr(camPos));
	}

	void Shader::SetModel(const mat4& model)
	{
		//GLuint loc = getUniformFromCurrentProgram("model");
		//if (loc != -1)
		GLuint loc = programToUniformMap[currentProgram]["model"];
			glUniformMatrix4fv(loc, 1, GL_FALSE, value_ptr(model));
	}

	void Shader::SetCamera(const mat4& cam)
	{
		camera = cam;

		//GLuint loc = getUniformFromCurrentProgram("camera");
		//if (loc != -1)
		GLuint loc = programToUniformMap[currentProgram]["camera"];
			glUniformMatrix4fv(loc, 1, GL_FALSE, value_ptr(cam));
	}

	void Shader::SetProjection(const mat4& proj)
	{
		projection = proj;

		//GLuint loc = getUniformFromCurrentProgram("projection");
		//if (loc != -1)
		GLuint loc = programToUniformMap[currentProgram]["projection"];
			glUniformMatrix4fv(loc, 1, GL_FALSE, value_ptr(proj));
	}

	void Shader::SetEmissionColor(const vec4& emissionColor)
	{
		//GLuint loc = getUniformFromCurrentProgram("emissionColor");
		//if (loc != -1)
		GLuint loc = programToUniformMap[currentProgram]["emissionColor"];
			glUniform4fv(loc, 1, value_ptr(emissionColor));
	}

	void Shader::SetTextureBlend(bool textureBlend)
	{
		//GLuint loc = getUniformFromCurrentProgram("textureBlend");
		//if (loc != -1)
		GLuint loc = programToUniformMap[currentProgram]["textureBlend"];
			glUniform1i(loc, textureBlend);
	}

	void Shader::SetUseShadowCubeMap(bool useShadowCubeMap)
	{
		//GLuint loc = getUniformFromCurrentProgram("useShadowCubeMap");
		//if (loc != -1)
		GLuint loc = programToUniformMap[currentProgram]["useShadowCubeMap"];
			glUniform1i(loc, useShadowCubeMap);
	}

	void Shader::SetShadowZRange(const vec2& shadowZRange)
	{
		//GLuint loc = getUniformFromCurrentProgram("shadowZRange");
		//if (loc != -1)
		GLuint loc = programToUniformMap[currentProgram]["shadowZRange"];
			glUniform2fv(loc, 1, value_ptr(shadowZRange));
	}

	void Shader::SetReflectiveCubeMap(bool b)
	{
		GLuint loc = programToUniformMap[currentProgram]["reflectiveCubeMap"];
		glUniform1i(loc, b);
	}

	// PRIVATE

	GLuint Shader::vertexBuffer;
	GLuint Shader::currentProgram;

	mat4 Shader::camera;
	vec4 Shader::cameraPosition;
	mat4 Shader::projection;
	mat4 Shader::lightSource;

	std::unordered_map<const char*, GLuint> Shader::nameToProgramMap;
	std::unordered_map<GLuint, GLuint> Shader::programToVaoMap;
	std::unordered_map<GLuint, std::unordered_map<const char*, GLuint>> Shader::programToUniformMap;

	void Shader::initEmissiveColorProgram()
	{
		GLuint program = InitShader("shaders\\vertex\\vshader_emissive_color.glsl", "shaders\\fragment\\fshader_emissive_color.glsl");
		nameToProgramMap.emplace("emissive_color", program);

		// create a vertex array object
		GLuint vao;
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);
		programToVaoMap.emplace(program, vao);

		// set up vertex arrays
		GLuint vPositionLoc = glGetAttribLocation(program, "vPosition");
		glEnableVertexAttribArray(vPositionLoc);
		glVertexAttribPointer(vPositionLoc, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

		// get uniform locations
		std::unordered_map<const char*, GLuint> uniformMap;
		uniformMap.emplace("model", getUniform(program, "model"));
		uniformMap.emplace("camera", getUniform(program, "camera"));
		uniformMap.emplace("projection", getUniform(program, "projection"));
		uniformMap.emplace("emissionColor", getUniform(program, "emissionColor"));

		// copy uniform map to program uniform map
		programToUniformMap.emplace(program, uniformMap);
	}

	void Shader::initEmissiveTextureProgram()
	{
		GLuint program = InitShader("shaders\\vertex\\vshader_emissive_texture.glsl", "shaders\\fragment\\fshader_emissive_texture.glsl");
		nameToProgramMap.emplace("emissive_texture", program);

		// create a vertex array object
		GLuint vao;
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);
		programToVaoMap.emplace(program, vao);

		// set up vertex arrays
		GLuint vPositionLoc = glGetAttribLocation(program, "vPosition");
		glEnableVertexAttribArray(vPositionLoc);
		glVertexAttribPointer(vPositionLoc, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

		GLuint vTextureCoordinateLoc = glGetAttribLocation(program, "vTextureCoordinate");
		glEnableVertexAttribArray(vTextureCoordinateLoc);
		glVertexAttribPointer(vTextureCoordinateLoc, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(vec4)*Shader::Vertices.size() + sizeof(vec4)*Shader::Normals.size() + sizeof(vec4)*Shader::Tangents.size() + sizeof(vec4)*Shader::Binormals.size()));

		// get uniform locations
		std::unordered_map<const char*, GLuint> uniformMap;
		uniformMap.emplace("model", getUniform(program, "model"));
		uniformMap.emplace("camera", getUniform(program, "camera"));
		uniformMap.emplace("projection", getUniform(program, "projection"));
		uniformMap.emplace("tex", getUniform(program, "tex"));

		// copy uniform map to program uniform map
		programToUniformMap.emplace(program, uniformMap);

		// initialize uniforms
		glUseProgram(program);
		glUniform1i(uniformMap["tex"], 0);
	}

	void Shader::initEmissiveCubeMapProgram()
	{
		GLuint program = InitShader("shaders\\vertex\\vshader_emissive_cubemap.glsl", "shaders\\fragment\\fshader_emissive_cubemap.glsl");
		nameToProgramMap.emplace("emissive_cubemap", program);

		// create a vertex array object
		GLuint vao;
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);
		programToVaoMap.emplace(program, vao);

		// set up vertex arrays
		GLuint vPositionLoc = glGetAttribLocation(program, "vPosition");
		glEnableVertexAttribArray(vPositionLoc);
		glVertexAttribPointer(vPositionLoc, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

		GLuint vNormalLoc = glGetAttribLocation(program, "vNormal");
		glEnableVertexAttribArray(vNormalLoc);
		glVertexAttribPointer(vNormalLoc, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(vec4)*Shader::Vertices.size()));

		// get uniform locations
		std::unordered_map<const char*, GLuint> uniformMap;
		uniformMap.emplace("model", getUniform(program, "model"));
		uniformMap.emplace("camera", getUniform(program, "camera"));
		uniformMap.emplace("projection", getUniform(program, "projection"));
		uniformMap.emplace("cameraPosition", getUniform(program, "cameraPosition"));
		uniformMap.emplace("cubeMap", getUniform(program, "cubeMap"));
		uniformMap.emplace("reflectiveCubeMap", getUniform(program, "reflective"));

		// copy uniform map to program uniform map
		programToUniformMap.emplace(program, uniformMap);

		// initialize uniforms
		glUseProgram(program);
		glUniform1i(uniformMap["cubeMap"], 2);
		glUniform1i(uniformMap["reflectiveCubeMap"], 0);
	}

	void Shader::initDepthProgram()
	{
		//GLuint program = InitShader("shaders\\vertex\\vshader_depth.glsl", "shaders\\fragment\\fshader_depth.glsl");
		GLuint program = InitShader("shaders\\vertex\\vshader_depth.glsl");
		nameToProgramMap.emplace("depth", program);

		// create a vertex array object
		GLuint vao;
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);
		programToVaoMap.emplace(program, vao);

		// set up vertex arrays
		GLuint vPositionLoc = glGetAttribLocation(program, "vPosition");
		glEnableVertexAttribArray(vPositionLoc);
		glVertexAttribPointer(vPositionLoc, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

		// get uniform locations
		std::unordered_map<const char*, GLuint> uniformMap;
		uniformMap.emplace("model", getUniform(program, "model"));
		uniformMap.emplace("camera", getUniform(program, "camera"));
		uniformMap.emplace("projection", getUniform(program, "projection"));

		// copy uniform map to program uniform map
		programToUniformMap.emplace(program, uniformMap);
	}

	void Shader::initMaterialProgram()
	{
		GLuint program = InitShader("shaders\\vertex\\vshader_material.glsl", "shaders\\fragment\\fshader_material.glsl");
		nameToProgramMap.emplace("material", program);

		// create a vertex array object
		GLuint vao;
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);
		programToVaoMap.emplace(program, vao);

		// set up vertex arrays
		GLuint vPositionLoc = glGetAttribLocation(program, "vPosition");
		glEnableVertexAttribArray(vPositionLoc);
		glVertexAttribPointer(vPositionLoc, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

		GLuint vNormalLoc = glGetAttribLocation(program, "vNormal");
		glEnableVertexAttribArray(vNormalLoc);
		glVertexAttribPointer(vNormalLoc, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(vec4)*Shader::Vertices.size()));

		// get uniform locations
		std::unordered_map<const char*, GLuint> uniformMap;
		uniformMap.emplace("model", getUniform(program, "model"));
		uniformMap.emplace("camera", getUniform(program, "camera"));
		uniformMap.emplace("projection", getUniform(program, "projection"));
		uniformMap.emplace("lightSource", getUniform(program, "lightSource"));
		uniformMap.emplace("cameraPosition", getUniform(program, "cameraPosition"));
		uniformMap.emplace("shadowZRange", getUniform(program, "shadowZRange"));
		uniformMap.emplace("materialAmbient", getUniform(program, "materialAmbient"));
		uniformMap.emplace("materialDiffuse", getUniform(program, "materialDiffuse"));
		uniformMap.emplace("materialSpecular", getUniform(program, "materialSpecular"));
		uniformMap.emplace("materialShininess", getUniform(program, "materialShininess"));
		uniformMap.emplace("useShadowCubeMap", getUniform(program, "useShadowCubeMap"));
		uniformMap.emplace("shadowCubeMap", getUniform(program, "shadowCubeMap"));

		// copy uniform map to program uniform map
		programToUniformMap.emplace(program, uniformMap);

		// initialize uniforms
		glUseProgram(program);
		glUniform1i(uniformMap["useShadowCubeMap"], 0);
		glUniform1i(uniformMap["shadowCubeMap"], 3);
	}

	void Shader::initTextureProgram()
	{
		GLuint program = InitShader("shaders\\vertex\\vshader_texture.glsl", "shaders\\fragment\\fshader_texture.glsl");
		nameToProgramMap.emplace("texture", program);

		// create a vertex array object
		GLuint vao;
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);
		programToVaoMap.emplace(program, vao);

		// set up vertex arrays
		GLuint vPositionLoc = glGetAttribLocation(program, "vPosition");
		glEnableVertexAttribArray(vPositionLoc);
		glVertexAttribPointer(vPositionLoc, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

		GLuint vNormalLoc = glGetAttribLocation(program, "vNormal");
		glEnableVertexAttribArray(vNormalLoc);
		glVertexAttribPointer(vNormalLoc, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(vec4)*Shader::Vertices.size()));

		GLuint vTextureCoordinateLoc = glGetAttribLocation(program, "vTextureCoordinate");
		glEnableVertexAttribArray(vTextureCoordinateLoc);
		glVertexAttribPointer(vTextureCoordinateLoc, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(vec4)*Shader::Vertices.size() + sizeof(vec4)*Shader::Normals.size() + sizeof(vec4)*Shader::Tangents.size() + sizeof(vec4)*Shader::Binormals.size()));

		// get uniform locations
		std::unordered_map<const char*, GLuint> uniformMap;
		uniformMap.emplace("model", getUniform(program, "model"));
		uniformMap.emplace("camera", getUniform(program, "camera"));
		uniformMap.emplace("projection", getUniform(program, "projection"));
		uniformMap.emplace("lightSource", getUniform(program, "lightSource"));
		uniformMap.emplace("cameraPosition", getUniform(program, "cameraPosition"));
		uniformMap.emplace("shadowZRange", getUniform(program, "shadowZRange"));
		uniformMap.emplace("materialAmbient", getUniform(program, "materialAmbient"));
		uniformMap.emplace("materialDiffuse", getUniform(program, "materialDiffuse"));
		uniformMap.emplace("materialSpecular", getUniform(program, "materialSpecular"));
		uniformMap.emplace("materialShininess", getUniform(program, "materialShininess"));
		uniformMap.emplace("tex", getUniform(program, "tex"));
		uniformMap.emplace("textureBlend", getUniform(program, "textureBlend"));
		uniformMap.emplace("useShadowCubeMap", getUniform(program, "useShadowCubeMap"));
		uniformMap.emplace("shadowCubeMap", getUniform(program, "shadowCubeMap"));

		// copy uniform map to program uniform map
		programToUniformMap.emplace(program, uniformMap);

		// initialize uniforms
		glUseProgram(program);
		glUniform1i(uniformMap["tex"], 0);
		glUniform1i(uniformMap["textureBlend"], 1);
		glUniform1i(uniformMap["useShadowCubeMap"], 0);
		glUniform1i(uniformMap["shadowCubeMap"], 3);
	}

	void Shader::initTextureBumpProgram()
	{
		GLuint program = InitShader("shaders\\vertex\\vshader_texture_bump.glsl", "shaders\\fragment\\fshader_texture_bump.glsl");
		nameToProgramMap.emplace("texture_bump", program);

		// create a vertex array object
		GLuint vao;
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);
		programToVaoMap.emplace(program, vao);

		// set up vertex arrays
		GLuint vPositionLoc = glGetAttribLocation(program, "vPosition");
		glEnableVertexAttribArray(vPositionLoc);
		glVertexAttribPointer(vPositionLoc, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

		GLuint vNormalLoc = glGetAttribLocation(program, "vNormal");
		glEnableVertexAttribArray(vNormalLoc);
		glVertexAttribPointer(vNormalLoc, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(vec4)*Shader::Vertices.size()));

		GLuint vTangentLoc = glGetAttribLocation(program, "vTangent");
		glEnableVertexAttribArray(vTangentLoc);
		glVertexAttribPointer(vTangentLoc, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(vec4)*Shader::Vertices.size() + sizeof(vec4)*Shader::Normals.size()));

		GLuint vBinormalLoc = glGetAttribLocation(program, "vBinormal");
		glEnableVertexAttribArray(vBinormalLoc);
		glVertexAttribPointer(vBinormalLoc, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(vec4)*Shader::Vertices.size() + sizeof(vec4)*Shader::Normals.size() + sizeof(vec4)*Shader::Tangents.size()));

		GLuint vTextureCoordinateLoc = glGetAttribLocation(program, "vTextureCoordinate");
		glEnableVertexAttribArray(vTextureCoordinateLoc);
		glVertexAttribPointer(vTextureCoordinateLoc, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(vec4)*Shader::Vertices.size() + sizeof(vec4)*Shader::Normals.size() + sizeof(vec4)*Shader::Tangents.size() + sizeof(vec4)*Shader::Binormals.size()));

		// get uniform locations
		std::unordered_map<const char*, GLuint> uniformMap;
		uniformMap.emplace("model", getUniform(program, "model"));
		uniformMap.emplace("camera", getUniform(program, "camera"));
		uniformMap.emplace("projection", getUniform(program, "projection"));
		uniformMap.emplace("lightSource", getUniform(program, "lightSource"));
		uniformMap.emplace("cameraPosition", getUniform(program, "cameraPosition"));
		uniformMap.emplace("shadowZRange", getUniform(program, "shadowZRange"));
		uniformMap.emplace("materialAmbient", getUniform(program, "materialAmbient"));
		uniformMap.emplace("materialDiffuse", getUniform(program, "materialDiffuse"));
		uniformMap.emplace("materialSpecular", getUniform(program, "materialSpecular"));
		uniformMap.emplace("materialShininess", getUniform(program, "materialShininess"));
		uniformMap.emplace("tex", getUniform(program, "tex"));
		uniformMap.emplace("textureBlend", getUniform(program, "textureBlend"));
		uniformMap.emplace("bumpTex", getUniform(program, "bumpTex"));
		uniformMap.emplace("useShadowCubeMap", getUniform(program, "useShadowCubeMap"));
		uniformMap.emplace("shadowCubeMap", getUniform(program, "shadowCubeMap"));

		// copy uniform map to program uniform map
		programToUniformMap.emplace(program, uniformMap);

		// initialize uniforms
		glUseProgram(program);
		glUniform1i(uniformMap["tex"], 0);
		glUniform1i(uniformMap["textureBlend"], 1);
		glUniform1i(uniformMap["bumpTex"], 1);
		glUniform1i(uniformMap["useShadowCubeMap"], 0);
		glUniform1i(uniformMap["shadowCubeMap"], 3);
	}

	void Shader::initCubeMapProgram()
	{
		GLuint program = InitShader("shaders\\vertex\\vshader_cubemap.glsl", "shaders\\fragment\\fshader_cubemap.glsl");
		nameToProgramMap.emplace("cubemap", program);

		// create a vertex array object
		GLuint vao;
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);
		programToVaoMap.emplace(program, vao);

		// set up vertex arrays
		GLuint vPositionLoc = glGetAttribLocation(program, "vPosition");
		glEnableVertexAttribArray(vPositionLoc);
		glVertexAttribPointer(vPositionLoc, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

		GLuint vNormalLoc = glGetAttribLocation(program, "vNormal");
		glEnableVertexAttribArray(vNormalLoc);
		glVertexAttribPointer(vNormalLoc, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(vec4)*Shader::Vertices.size()));

		// get uniform locations
		std::unordered_map<const char*, GLuint> uniformMap;
		uniformMap.emplace("model", getUniform(program, "model"));
		uniformMap.emplace("camera", getUniform(program, "camera"));
		uniformMap.emplace("projection", getUniform(program, "projection"));
		uniformMap.emplace("lightSource", getUniform(program, "lightSource"));
		uniformMap.emplace("cameraPosition", getUniform(program, "cameraPosition"));
		uniformMap.emplace("shadowZRange", getUniform(program, "shadowZRange"));
		uniformMap.emplace("materialAmbient", getUniform(program, "materialAmbient"));
		uniformMap.emplace("materialDiffuse", getUniform(program, "materialDiffuse"));
		uniformMap.emplace("materialSpecular", getUniform(program, "materialSpecular"));
		uniformMap.emplace("materialShininess", getUniform(program, "materialShininess"));
		uniformMap.emplace("cubeMap", getUniform(program, "cubeMap"));
		uniformMap.emplace("textureBlend", getUniform(program, "textureBlend"));
		uniformMap.emplace("useShadowCubeMap", getUniform(program, "useShadowCubeMap"));
		uniformMap.emplace("shadowCubeMap", getUniform(program, "shadowCubeMap"));
		uniformMap.emplace("reflectiveCubeMap", getUniform(program, "reflective"));

		// copy uniform map to program uniform map
		programToUniformMap.emplace(program, uniformMap);

		// initialize uniforms
		glUseProgram(program);
		glUniform1i(uniformMap["cubeMap"], 2);
		glUniform1i(uniformMap["textureBlend"], 1);
		glUniform1i(uniformMap["bumpTex"], 1);
		glUniform1i(uniformMap["useShadowCubeMap"], 0);
		glUniform1i(uniformMap["shadowCubeMap"], 3);
		glUniform1i(uniformMap["reflectiveCubeMap"], 0);
	}

	void Shader::initCubeMapBumpProgram()
	{
		GLuint program = InitShader("shaders\\vertex\\vshader_cubemap_bump.glsl", "shaders\\fragment\\fshader_cubemap_bump.glsl");
		nameToProgramMap.emplace("cubemap_bump", program);

		// create a vertex array object
		GLuint vao;
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);
		programToVaoMap.emplace(program, vao);

		// set up vertex arrays
		GLuint vPositionLoc = glGetAttribLocation(program, "vPosition");
		glEnableVertexAttribArray(vPositionLoc);
		glVertexAttribPointer(vPositionLoc, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

		GLuint vNormalLoc = glGetAttribLocation(program, "vNormal");
		glEnableVertexAttribArray(vNormalLoc);
		glVertexAttribPointer(vNormalLoc, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(vec4)*Shader::Vertices.size()));

		GLuint vTangentLoc = glGetAttribLocation(program, "vTangent");
		glEnableVertexAttribArray(vTangentLoc);
		glVertexAttribPointer(vTangentLoc, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(vec4)*Shader::Vertices.size() + sizeof(vec4)*Shader::Normals.size()));

		GLuint vBinormalLoc = glGetAttribLocation(program, "vBinormal");
		glEnableVertexAttribArray(vBinormalLoc);
		glVertexAttribPointer(vBinormalLoc, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(vec4)*Shader::Vertices.size() + sizeof(vec4)*Shader::Normals.size() + sizeof(vec4)*Shader::Tangents.size()));

		// get uniform locations
		std::unordered_map<const char*, GLuint> uniformMap;
		uniformMap.emplace("model", getUniform(program, "model"));
		uniformMap.emplace("camera", getUniform(program, "camera"));
		uniformMap.emplace("projection", getUniform(program, "projection"));
		uniformMap.emplace("lightSource", getUniform(program, "lightSource"));
		uniformMap.emplace("cameraPosition", getUniform(program, "cameraPosition"));
		uniformMap.emplace("shadowZRange", getUniform(program, "shadowZRange"));
		uniformMap.emplace("materialAmbient", getUniform(program, "materialAmbient"));
		uniformMap.emplace("materialDiffuse", getUniform(program, "materialDiffuse"));
		uniformMap.emplace("materialSpecular", getUniform(program, "materialSpecular"));
		uniformMap.emplace("materialShininess", getUniform(program, "materialShininess"));
		uniformMap.emplace("cubeMap", getUniform(program, "cubeMap"));
		uniformMap.emplace("bumpCubeMap", getUniform(program, "bumpCubeMap"));
		uniformMap.emplace("textureBlend", getUniform(program, "textureBlend"));
		uniformMap.emplace("useShadowCubeMap", getUniform(program, "useShadowCubeMap"));
		uniformMap.emplace("shadowCubeMap", getUniform(program, "shadowCubeMap"));
		uniformMap.emplace("reflectiveCubeMap", getUniform(program, "reflective"));

		// copy uniform map to program uniform map
		programToUniformMap.emplace(program, uniformMap);

		// initialize uniforms
		glUseProgram(program);
		glUniform1i(uniformMap["cubeMap"], 2);
		glUniform1i(uniformMap["textureBlend"], 1);
		glUniform1i(uniformMap["bumpCubeMap"], 4);
		glUniform1i(uniformMap["useShadowCubeMap"], 0);
		glUniform1i(uniformMap["shadowCubeMap"], 3);
		glUniform1i(uniformMap["reflectiveCubeMap"], 0);
	}

	GLuint Shader::getUniform(GLuint program, const char* name)
	{
		GLuint loc = glGetUniformLocation(program, name);
		if (loc == -1)
			std::cerr << "unable to get " << name << " parameter from shader program\n";
		return loc;
	}

	int Shader::getUniformFromCurrentProgram(const char* uniformName)
	{
		auto it = programToUniformMap[currentProgram].find(uniformName);
		if (it != programToUniformMap[currentProgram].end())
			return programToUniformMap[currentProgram][uniformName];
		else
			return -1;
	}
}
