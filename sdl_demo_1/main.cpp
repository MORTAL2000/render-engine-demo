#include "Bagnall.h"
#include "Shader.h"
#include "InitShader.h"
#include "Game.h"
#include "FpsTracker.h"
#include "Camera.h"
#include "Cube.h"
#include "LightSource.h"
#include "Util.h"
#include <vector>
#include <iostream>

using namespace Bagnall;

SDL_Window *window;
SDL_GLContext glcontext;

Camera *camera;
LightSource *lightSource;
Cube *cube;

UpdateNode *rootUpdateNode;
ModelNode *rootModelNode;

void init(void)
{
	Cube::Init();
	LightSource::Init();
	Shader::Init();

	glEnable(GL_DEPTH_TEST);

	//glClearColor(1.0, 1.0, 1.0, 1.0); // white background
	glClearColor(0.5, 0.5, 0.5, 1.0);

	// initialize projection matrix
	Game::ViewDistance = 100.0;
	int w, h;
	SDL_GetWindowSize(window, &w, &h);
	Game::Projection = perspective(45.0f, w/static_cast<float>(h), 1.0f, Game::ViewDistance);
	//Game::Projection = Util::Perspective(45.0f, w/h, 1.0f, Game::ViewDistance);
	//Game::Projection = ortho(-50.0f, 50.0f, -50.0f, 50.0f, -50.0f, 50.0f);
	Shader::SetProjection(Game::Projection);

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

	cube = new Cube(rootObject);
	cube->SetMaterial(Material::Plastic(vec4(0.3f, 0.3f, 0.3f, 1.0f)));
	cube->Scale(10.0f);
}

//----------------------------------------------------------------------------

bool holdingMouseClick = false;
int update()
{
	FpsTracker::Update();

	SDL_Event ev;
	while (SDL_PollEvent(&ev))
	{
		camera->InputEvent(ev);

		if (ev.type == SDL_QUIT)
		{
			return 1;
		}
		else if (ev.type == SDL_KEYDOWN)
		{
			switch (ev.key.keysym.sym)
			{
			case SDLK_ESCAPE:
				return 1;
				break;
			}
		}
		else if (ev.type == SDL_MOUSEBUTTONDOWN)
		{
			holdingMouseClick = true;
		}
		else if (ev.type == SDL_MOUSEBUTTONUP)
		{
			holdingMouseClick = false;
		}
		else if (ev.type == SDL_MOUSEMOTION)
		{
			//
		}
	}

	if (holdingMouseClick)
	{
		//
	}

	// update stuff

	SDL_SetWindowTitle(window, Util::ToString(FpsTracker::GetFps()).c_str());

	cube->RotateX(static_cast<float>(rand()) / RAND_MAX / 1000.0f);
	cube->RotateY(static_cast<float>(rand()) / RAND_MAX / 1000.0f);
	cube->RotateZ(static_cast<float>(rand()) / RAND_MAX / 1000.0f);
	/*cube->Update();
	cube->UpdateModel();*/

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

	cube->Draw();

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
		std::cerr << "ünable to set relative mouse mode.\n";

	std::cout << "all systems are go bro\n";

	while (!update())
	{
		draw();
	}

	SDL_GL_DeleteContext(glcontext);

	SDL_Quit();
	return 0;
}
