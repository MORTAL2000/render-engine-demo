#include "Bagnall.h"
#include "Shader.h"
#include "InitShader.h"
#include "Game.h"
#include "FpsTracker.h"
#include "Camera.h"
#include "Cube.h"
#include "LightSource.h"
#include "Util.h"
#include "UpdateGraph.h"
#include "ModelGraph.h"
#include "RenderGraph.h"
#include <vector>
#include <iostream>

#include <thread>
#include <ppl.h>

using namespace Bagnall;

SDL_Window *window;
SDL_GLContext glcontext;

Camera *camera;
LightSource *lightSource;
Object *cubeContainer;
std::vector<Cube*> cubes;

UpdateNode *rootUpdateNode;
ModelNode *rootModelNode;

void updateProjectionMatrixAndViewport()
{
	int w, h;
	SDL_GetWindowSize(window, &w, &h);
	Game::Projection = perspective(45.0f, w / static_cast<float>(h), 1.0f, Game::ViewDistance);
	//Game::Projection = Util::Perspective(45.0f, w/h, 1.0f, Game::ViewDistance);
	//Game::Projection = ortho(-50.0f, 50.0f, -50.0f, 50.0f, -50.0f, 50.0f);
	Shader::SetProjection(Game::Projection);

	glViewport(0, 0, w, h);
}

void init(void)
{
	Cube::Init();
	LightSource::Init();
	Shader::Init();

	glEnable(GL_DEPTH_TEST);

	//glClearColor(1.0, 1.0, 1.0, 1.0); // white background
	glClearColor(0.5, 0.5, 0.5, 1.0);

	Game::ViewDistance = Game::WorldSize * 2.0f;

	// initialize projection matrix
	updateProjectionMatrixAndViewport();

	// create root object
	Object *rootObject = new Object(NULL);
	rootUpdateNode = rootObject->GetUpdateNode();
	rootModelNode = rootObject->GetModelNode();

	camera = new Camera(rootObject);
	camera->SetPosition(vec4(0.0f, -25.0f, 0.0f, 1.0f));
	camera->LookAt(vec4(0.0f, 0.0f, 0.0f, 1.0f));

	lightSource = LightSource::ObtainLightSource();
	lightSource->ambient = vec4(1.0, 1.0, 1.0, 1.0);
	lightSource->diffuse = vec4(1.0, 1.0, 1.0, 1.0);
	lightSource->specular = vec4(1.0, 1.0, 1.0, 1.0);
	lightSource->position = vec4(50.0, -250.0, 50.0, 0.0);
	lightSource->UpdateMatrix();
	LightSource::UpdateLightSourceMatricesInShaders();

	cubeContainer = new Object(rootObject);

	int range = Game::WorldSize - 5.0f;
	for (int i = 0; i < 25000; ++i)
	{
		Cube *cube = new Cube(cubeContainer);
		cube->SetPosition(vec4(rand() % range - range / 2.0f, rand() % range - range / 2.0f, rand() % range - range / 2.0f, 1.0f));
		cube->SetMaterial(Material::Plastic(vec4(0.3f, 0.3f, 0.3f, 1.0f)));
		cube->Scale(5.0f);
		cubes.push_back(cube);
	}
}

//----------------------------------------------------------------------------

bool holdingMouseClick = false;
int update()
{
	FpsTracker::Update();

	SDL_Event ev;
	while (SDL_PollEvent(&ev))
	{
		// send event to camera
		camera->InputEvent(ev);

		// quit
		if (ev.type == SDL_QUIT)
		{
			return 1;
		}
		// key press
		else if (ev.type == SDL_KEYDOWN)
		{
			switch (ev.key.keysym.sym)
			{
			case SDLK_ESCAPE:
				return 1;
				break;
			case SDLK_f:
				SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
				break;
			}
		}
		// mouse click
		else if (ev.type == SDL_MOUSEBUTTONDOWN)
		{
			holdingMouseClick = true;
		}
		// mouse release
		else if (ev.type == SDL_MOUSEBUTTONUP)
		{
			holdingMouseClick = false;
		}
		// mouse motion
		else if (ev.type == SDL_MOUSEMOTION)
		{
			//
		}
		// window event
		else if (ev.type == SDL_WINDOWEVENT)
		{
			updateProjectionMatrixAndViewport();
		}
	}

	if (holdingMouseClick)
	{
		//
	}

	// update stuff

	SDL_SetWindowTitle(window, Util::ToString(FpsTracker::GetFps()).c_str());

	/*cubeContainer->RotateX(static_cast<float>(rand()) / RAND_MAX / 10000.0f * FpsTracker::GetFrameTimeMs());
	cubeContainer->RotateY(static_cast<float>(rand()) / RAND_MAX / 10000.0f * FpsTracker::GetFrameTimeMs());
	cubeContainer->RotateZ(static_cast<float>(rand()) / RAND_MAX / 10000.0f * FpsTracker::GetFrameTimeMs());*/
	cubeContainer->RotateX(1.0f / 10000.0f * FpsTracker::GetFrameTimeMs());
	cubeContainer->RotateY(1.0f / 10000.0f * FpsTracker::GetFrameTimeMs());
	cubeContainer->RotateZ(1.0f / 10000.0f * FpsTracker::GetFrameTimeMs());

	////for (auto it = cubes.begin(); it != cubes.end(); ++it)
	//concurrency::parallel_for_each(begin(cubes), end(cubes), [&](Cube *cube)
	//{
	//	cube->RotateX(static_cast<float>(rand()) / RAND_MAX / 1000.0f * FpsTracker::GetFrameTimeMs());
	//	cube->RotateY(static_cast<float>(rand()) / RAND_MAX / 1000.0f * FpsTracker::GetFrameTimeMs());
	//	cube->RotateZ(static_cast<float>(rand()) / RAND_MAX / 1000.0f * FpsTracker::GetFrameTimeMs());
	//});
	concurrency::parallel_for_each(begin(cubes), end(cubes), [&](Cube *cube)
	{
		cube->RotateX(1.0f / 1000.0f * FpsTracker::GetFrameTimeMs());
		cube->RotateY(1.0f / 1000.0f * FpsTracker::GetFrameTimeMs());
		cube->RotateZ(1.0f / 1000.0f * FpsTracker::GetFrameTimeMs());
	}, concurrency::static_partitioner());

	rootUpdateNode->Update();

	rootModelNode->Update();

	//camera->Update();

	return 0;
}

void draw()
{
	// clear the window
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// draw stuff
	Game::GameRenderGraph->Render();

	//cube->Draw();

	SDL_GL_SwapWindow(window);
	glFlush();
}

int main(int argc, char **argv)
{
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_AUDIO) != 0)
	{
		std::cerr << "Failed to initialize SDL.\n";
		SDL_Quit();
		system("pause");
		return 1;
	}

	window = SDL_CreateWindow(
		"SDL2/OpenGL Demo", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480,
		SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

	// Create an OpenGL context associated with the window.
	glcontext = SDL_GL_CreateContext(window);

	//SDL_GL_MakeCurrent(window, glcontext);
	glewInit();

	init();

	if (SDL_SetRelativeMouseMode(SDL_TRUE) == -1)
		std::cerr << "unable to set relative mouse mode.\n";

	std::cout << "all systems are go bro\n";

	while (!update())
	{
		draw();
	}

	SDL_GL_DeleteContext(glcontext);

	SDL_Quit();
	return 0;
}
