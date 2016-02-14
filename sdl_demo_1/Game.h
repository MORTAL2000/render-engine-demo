#ifndef GAME_H
#define GAME_H

#include "Bagnall.h"
#include <string>

namespace Bagnall
{
	class Object;
	class RenderGraph;
	class Camera;
	struct UpdateNode;
	struct ModelNode;

	class Game
	{
	public:
		static float WorldSize;
		static float ViewDistance;
		static float FOV;

		static mat4 ProjectionMatrix;
		static mat4 CameraMatrix;
		static vec4 CameraPosition;

		static Camera *MainCamera;

		static RenderGraph *MainRenderGraph;

		static std::string WindowTitleMessage;

		static int Init();
		static void Cleanup();

		// do this after all vertex data is loaded
		static void SendVertexDataToGPU();

		static void UpdateProjectionMatrixAndViewport();

		static const Object* GetRootObject();

		static void Update();

		static void Draw();

		// only enables full screen atm
		static void ToggleFullScreen();

	private:
		static SDL_Window *window;
		static SDL_GLContext glcontext;

		static Object *rootObject;
		static UpdateNode *rootUpdateNode;
		static ModelNode *rootModelNode;
	};
}

#endif
