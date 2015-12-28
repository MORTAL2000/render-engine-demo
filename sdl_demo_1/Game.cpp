#include "Game.h"
#include "RenderGraph.h"
#include "Rectangle.h"
#include "Cube.h"
#include "Sphere.h"
#include "Skybox.h"
#include "LightSource.h"
#include "Texture.h"
#include "Shadow.h"
#include "EnvironmentMap.h"
#include "Schematic.h"
#include "Shader.h"
#include "Camera.h"
#include "Util.h"
#include "FpsTracker.h"
#include <iostream>

namespace Bagnall
{
	// PUBLIC

	float Game::WorldSize = 2000.0f;
	//float Game::WorldSize = 10000.0f;
	float Game::ViewDistance;
	float Game::FOV = M_PI / 4.0f;

	mat4 Game::ProjectionMatrix;
	mat4 Game::CameraMatrix;
	vec4 Game::CameraPosition;

	Camera* Game::MainCamera;

	RenderGraph* Game::MainRenderGraph = new RenderGraph();

	std::string Game::WindowTitleMessage = "";

	int Game::Init()
	{
		srand(time(NULL));

		if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_AUDIO) != 0)
		{
			std::cerr << "Failed to initialize SDL.\n";
			SDL_Quit();
			system("pause");
			return 1;
		}

		// 4x AA
		SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
		SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);

		window = SDL_CreateWindow(
			"SDL2/OpenGL Demo", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480,
			SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

		// Create an OpenGL context associated with the window.
		glcontext = SDL_GL_CreateContext(window);

		// enable AA
		glEnable(GL_MULTISAMPLE);

		//SDL_GL_MakeCurrent(window, glcontext);
		glewInit();

		if (SDL_SetRelativeMouseMode(SDL_TRUE) == -1)
			std::cerr << "unable to set relative mouse mode.\n";

		Rectangle::Init();
		Cube::Init();
		Sphere::Init();
		Skybox::Init();
		LightSource::Init();
		Texture::Init();
		Shadow::Init();
		EnvironmentMap::Init();
		Schematic::Init();

		rootUpdateNode = rootObject->GetUpdateNode();
		rootModelNode = rootObject->GetModelNode();

		MainCamera = new Camera(rootObject);

		glEnable(GL_DEPTH_TEST);

		glEnable(GL_PRIMITIVE_RESTART);
		glPrimitiveRestartIndex(UINT_MAX);

		Game::ViewDistance = Game::WorldSize * 1.6f;

		// initialize projection matrix
		Game::UpdateProjectionMatrixAndViewport();

		// SET SHADOW NEAR AND FAR PLANES
		Shadow::SetZRange(2.0f, Game::ViewDistance);

		std::cout << "Puddi initialization complete.\n";

		return 0;
	}

	void Game::Cleanup()
	{
		Schematic::Cleanup();

		SDL_GL_DeleteContext(glcontext);
		SDL_DestroyWindow(window);
		SDL_Quit();
	}

	void Game::SendVertexDataToGPU()
	{
		Shader::Init();

		// clear all vertex data on cpu side
		Shader::Vertices.clear();
		Shader::Normals.clear();
		Shader::Tangents.clear();
		Shader::Binormals.clear();
		Shader::TextureCoordinates.clear();
		Shader::VertexIndices.clear();
		Shader::Vertices.resize(0);
		Shader::Normals.resize(0);
		Shader::Tangents.resize(0);
		Shader::Binormals.resize(0);
		Shader::TextureCoordinates.resize(0);
		Shader::VertexIndices.resize(0);
	}

	void Game::UpdateProjectionMatrixAndViewport()
	{
		int w, h;
		SDL_GetWindowSize(window, &w, &h);
		Game::ProjectionMatrix = perspective(static_cast<float>(M_PI) / 4.0f, w / static_cast<float>(h), 1.0f, Game::ViewDistance);
		Shader::SetProjection(Game::ProjectionMatrix);

		glViewport(0, 0, w, h);
	}

	const Object* Game::GetRootObject()
	{
		return rootObject;
	}

	void Game::Update()
	{
		FpsTracker::Update();

		SDL_SetWindowTitle(window, Util::ToString(FpsTracker::GetFps()).c_str());

		rootUpdateNode->Update();

		rootModelNode->Update();
	}

	void Game::Draw()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		Game::MainRenderGraph->Render();

		SDL_GL_SwapWindow(window);
		glFlush();
	}

	void Game::ToggleFullScreen()
	{
		SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
	}

	// PRIVATE

	SDL_Window* Game::window;
	SDL_GLContext Game::glcontext;

	Object* Game::rootObject = new Object(nullptr);
	UpdateNode* Game::rootUpdateNode;
	ModelNode* Game::rootModelNode;
}
