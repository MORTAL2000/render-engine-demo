#ifndef SHADOW_H
#define SHADOW_H

#define SHADOW_MAP_SIZE 1024
//#define SHADOW_MAP_SIZE 2048

#include <GL/glew.h>
#include <glm/vec3.hpp>
#include <vector>

namespace Bagnall
{
	class DrawableObject;

	class Shadow
	{
	public:
		static void Init();

		static void RenderShadowMap(const glm::vec3& sourcePos, DrawableObject* source, float zNear, float zFar);

		static void SendToGPU();

		static void AddToDepthRenderList(DrawableObject *o);

		static void RemoveFromDepthRenderList(DrawableObject *o);

	private:
		static GLuint frameBuffer;
		static GLuint depthCubeMap;

		static std::vector<DrawableObject*> depthRenderList;

		static void renderDepthRenderList();
	};
}

#endif
