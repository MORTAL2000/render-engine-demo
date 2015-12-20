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

	class Shadow
	{
	public:
		static void Init();

		static void RenderShadowMap(const glm::vec3& sourcePos, DrawableObject* source);

		static void BindToGPU();

		static void AddToDepthRenderList(DrawableObject *o);

		static void RemoveFromDepthRenderList(DrawableObject *o);

		static void SetNearAndFarPlanes(float zNear, float zFar);

	private:
		static GLuint frameBuffer;
		static GLuint depthCubeMap;
		static glm::vec2 zRange;

		static std::vector<DrawableObject*> depthRenderList;

		static void renderDepthRenderList();
	};
}

#endif
