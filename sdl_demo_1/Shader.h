#ifndef SHADER_H
#define SHADER_H

#include "Bagnall.h"
#include <vector>

namespace Bagnall
{
	class Shader
	{
	public:
		static std::vector<vec4> Vertices;
		static std::vector<vec4> Normals;
		static std::vector<vec4> Tangents;
		static std::vector<vec4> Binormals;
		static std::vector<vec2> TextureCoordinates;

		static GLuint MaterialAmbientLoc;
		static GLuint MaterialDiffuseLoc;
		static GLuint MaterialSpecularLoc;
		static GLuint MaterialShininessLoc;
		static GLuint LightSourceLoc;
		static GLuint CameraPositionLoc;
		static GLuint ModelLoc;
		static GLuint InverseModelLoc;
		static GLuint CameraLoc;
		static GLuint ProjectionLoc;
		static GLuint EmissiveLoc;
		static GLuint EmissionColorLoc;
		static GLuint AlphaOverrideLoc;
		static GLuint UseTextureLoc;
		static GLuint TexLoc;
		static GLuint UseBumpMapLoc;
		static GLuint BumpTexLoc;
		static GLuint UseCubeMapLoc;
		static GLuint CubeMapLoc;

		static void Init();

		static void SetMaterialAmbient(const vec4& materialAmbient);
		static void SetMaterialDiffuse(const vec4& materialDiffuse);
		static void SetMaterialSpecular(const vec4& materialSpecular);
		static void SetMaterialShininess(float materialShininess);
		static void SetLightSource(const mat4& lightSource);
		static void SetCameraPosition(const vec4& cameraPosition);
		static void SetModel(const mat4& model);
		static void SetInverseModel(const mat4& model);
		static void SetCamera(const mat4& camera);
		static void SetProjection(const mat4& projection);
		static void SetEmissive(bool emissive);
		static void SetEmissionColor(const vec4& emissionColor);
		static void SetAlphaOverride(const vec4& alphaOverride);
		static void SetUseTexture(bool useTexture);
		static void SetTex(int tex);
		static void SetUseBumpMap(bool useBumpMap);
		static void SetBumpTex(int bumpTex);
		static void SetUseCubeMap(bool useCubeMap);
		static void SetCubeMap(int cubeMap);

	private:
		static GLuint buffer;
		static GLuint vao;
	};
}

#endif
