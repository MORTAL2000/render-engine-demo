#ifndef SHADOW_H
#define SHADOW_H

#define SHADOW_MAP_SIZE 1024
//#define SHADOW_MAP_SIZE 2048

#include <GL/glew.h>
#include <glm/vec3.hpp>

namespace Bagnall
{
	class DrawableObject;

	class Shadow
	{
	public:
		static void Init();

		static void Render(const glm::vec3& sourcePos, DrawableObject* source, float zNear, float zFar);

		static void SendToGPU();

	private:
		static GLuint frameBuffer;
		static GLuint depthCubeMap;
	};
}

#endif
