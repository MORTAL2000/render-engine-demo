#ifndef SHADER_H
#define SHADER_H

#include "Bagnall.h"
#include <vector>
#include <unordered_map>

namespace Bagnall
{
	struct Material;

	enum TextureSampler
	{
		TEXTURE_2D,
		TEXTURE_2D_BUMP,
		TEXTURE_CUBE,
		TEXTURE_CUBE_BUMP,
		TEXTURE_SHADOW_2D,
		TEXTURE_SHADOW_CUBE
	};

	class Shader
	{
	public:
		static std::vector<vec4> Vertices;
		static std::vector<vec4> Normals;
		static std::vector<vec4> Tangents;
		static std::vector<vec4> Binormals;
		static std::vector<vec2> TextureCoordinates;

		static std::vector<uint> VertexIndices;

		static void Init();

		static void SetProgram(const char* programName);

		static void SetMaterialAmbient(const vec4& materialAmbient);
		static void SetMaterialDiffuse(const vec4& materialDiffuse);
		static void SetMaterialSpecular(const vec4& materialSpecular);
		static void SetMaterialShininess(float materialShininess);
		static void SetMaterial(const Material& material);
		static void SetLightSource(const mat4& lightSource);
		static void SetCameraPosition(const vec4& cameraPosition);
		static void SetModel(const mat4& model);
		static void SetCamera(const mat4& camera);
		static void SetProjection(const mat4& projection);
		static void SetLightProjection(const mat4& lightProj);
		static void SetEmissionColor(const vec4& emissionColor);
		static void SetTextureBlend(bool textureBlendb);
		static void SetShadowMode(int shadowMode);
		static void SetShadowZRange(const vec2& shadowZRange);
		static void SetReflectiveCubeMap(bool b);

	private:
		static GLuint vertexBuffer;
		static GLuint elementBuffer;
		static GLuint currentProgram;

		static mat4 camera;
		static vec4 cameraPosition;
		static mat4 projection;
		static mat4 lightSource;
		static mat4 lightProjection;

		static std::unordered_map<const char*, GLuint> nameToProgramMap;
		static std::unordered_map<GLuint, GLuint> programToVaoMap;
		static std::unordered_map<GLuint, std::unordered_map<const char*, GLuint>> programToUniformMap;

		static void initEmissiveColorProgram();
		static void initEmissiveTextureProgram();
		static void initEmissiveCubeMapProgram();
		static void initDepthProgram();
		static void initMaterialProgram();
		static void initTextureProgram();
		static void initTextureBumpProgram();
		static void initCubeMapProgram();
		static void initCubeMapBumpProgram();

		static GLuint getUniform(GLuint program, const char* name);

		static int getUniformFromCurrentProgram(const char* uniformName);
	};
}

#endif
