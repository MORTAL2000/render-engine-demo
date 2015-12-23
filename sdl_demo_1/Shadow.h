#ifndef SHADOW_H
#define SHADOW_H

//#define SHADOW_MAP_SIZE 512
#define SHADOW_MAP_SIZE 1024
//#define SHADOW_MAP_SIZE 2048

#include <GL/glew.h>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <vector>

namespace Bagnall
{
	class DrawableObject;
	class VertexMesh;

	enum ShadowMode
	{
		SHADOW_MODE_NONE,
		SHADOW_MODE_UNI,
		SHADOW_MODE_OMNI
	};

	class Shadow
	{
	public:
		static void Init();

		static void RenderShadowOrthoMap(const glm::vec3& sourcePos, const glm::vec3& destPos);

		static void RenderShadowCubeMap(const glm::vec3& sourcePos, DrawableObject* source);

		static void BindToGPU(int shadowMode);

		static void AddToDepthRenderList(VertexMesh *o);

		static void RemoveFromDepthRenderList(VertexMesh *o);

		static void SetZRange(const glm::vec2& zr);

		static GLuint depthBuffer; // for shadow ortho map

	private:
		static GLuint frameBuffer;
		//static GLuint depthBuffer; // for shadow ortho map
		static GLuint depthCubeMap; // for shadow cube map
		static glm::vec2 zRange;

		static std::vector<VertexMesh*> depthRenderList;

		static void renderDepthRenderList();
	};
}

#endif
