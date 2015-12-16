#include "Bagnall.h"
#include "Shader.h"
#include "InitShader.h"
#include "Game.h"
#include "FpsTracker.h"
#include "Camera.h"
#include "Rectangle.h"
#include "Cube.h"
#include "LightSource.h"
#include "Util.h"
#include "UpdateGraph.h"
#include "ModelGraph.h"
#include "RenderGraph.h"
#include "Texture.h"
#include <vector>
#include <iostream>

#include <thread>
#include <ppl.h>

using namespace Bagnall;

SDL_Window *window;
SDL_GLContext glcontext;

Camera *camera;
Cube *sun;
LightSource *lightSource;
Object *cubeContainer;
std::vector<Cube*> cubes;

Object *groundContainer;

UpdateNode *rootUpdateNode;
ModelNode *rootModelNode;

GLuint frameBuffer;
GLuint depthTexture;
GLuint cubemap;

GLuint tempTexture;

void updateProjectionMatrixAndViewport()
{
	int w, h;
	SDL_GetWindowSize(window, &w, &h);
	Game::Projection = perspective(45.0f, w / static_cast<float>(h), 1.0f, Game::ViewDistance);
	Shader::SetProjection(Game::Projection);

	glViewport(0, 0, w, h);
}

void init(void)
{
	Rectangle::Init();
	Cube::Init();
	LightSource::Init();
	Texture::Init();
	Shader::Init();

	// SET UP FRAME BUFFER FOR RENDERING TO CUBE MAP
	frameBuffer = 0;
	glGenFramebuffers(1, &frameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);

	//glGenTextures(1, &tempTexture);
	//glBindTexture(GL_TEXTURE_2D, tempTexture);
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 800, 600, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//glBindTexture(GL_TEXTURE_2D, 0);

	//glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tempTexture, 0);

	// Depth texture. Slower than a depth buffer, but you can sample it later in your shader
	/*glGenTextures(1, &depthTexture);
	glBindTexture(GL_TEXTURE_2D, depthTexture);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, 1024, 1024, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthTexture, 0);

	glDrawBuffer(GL_NONE); // No color buffer is drawn to.*/

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// SET UP CUBE MAP
	glGenTextures(1, &cubemap);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap);

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_BASE_LEVEL, 0);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAX_LEVEL, 0);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGBA8, 1024, 1024, 0, GL_BGRA, GL_UNSIGNED_BYTE, NULL);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGBA8, 1024, 1024, 0, GL_BGRA, GL_UNSIGNED_BYTE, NULL);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGBA8, 1024, 1024, 0, GL_BGRA, GL_UNSIGNED_BYTE, NULL);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGBA8, 1024, 1024, 0, GL_BGRA, GL_UNSIGNED_BYTE, NULL);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGBA8, 1024, 1024, 0, GL_BGRA, GL_UNSIGNED_BYTE, NULL);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGBA8, 1024, 1024, 0, GL_BGRA, GL_UNSIGNED_BYTE, NULL);

	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_Y, cubemap, 0);

	// Always check that our framebuffer is ok
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cerr << "frame buffer error\n";
	}

	//auto v = Shader::Vertices;
	//auto n = Shader::Normals;
	//auto s = Shader::Tangents;
	//auto t = Shader::Binormals;

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

	// CONTAINERS
	cubeContainer = new Object(rootObject);
	groundContainer = new Object(rootObject);
	groundContainer->Translate(vec4(0.0f, 0.0f, -Game::WorldSize / 2.0f, 0.0f));

	// MANY CUBES
	int range = Game::WorldSize - 5.0f;
	for (int i = 0; i < 100; ++i)
	{
		Cube *cube = new Cube(cubeContainer);
		cube->SetPosition(vec4(rand() % range - range / 2.0f, rand() % range - range / 2.0f, rand() % range - range / 2.0f, 1.0f));
		cube->SetTexture(Texture::GetTextureByName("shrek"));
		cube->SetMaterial(Material::Plastic(vec4(0.3f, 0.3f, 0.3f, 1.0f)));
		cube->Scale(5.0f);
		cubes.push_back(cube);
	}

	// MIDDLE CUBE
	Cube *cube = new Cube(cubeContainer);
	//cube->SetTexture(Texture::GetTextureByName("ben"));
	cube->SetTexture(tempTexture);
	cube->SetMaterial(Material::Plastic(vec4(0.3f, 0.3f, 0.3f, 1.0f)));
	cube->Scale(10.0f);
	cubes.push_back(cube);

	// SUN
	sun = new Cube(rootObject);
	sun->SetPosition(vec4(50.0, -250.0, 50.0, 1.0));
	sun->SetEmissionColor(vec4(1.0f, 1.0f, 0.0f, 1.0f));
	sun->SetEmissive(true);
	sun->Scale(10.0f);
	//cubes.push_back(cube);

	// floor
	float floorScale = Game::WorldSize / 50.0f;
	for (int i = 0; i < Game::WorldSize / floorScale; ++i)
	{
		for (int j = 0; j < Game::WorldSize / floorScale; ++j)
		{
			Rectangle *rect = new Rectangle(groundContainer);
			rect->SetPosition(vec4(-Game::WorldSize / 2.0f + floorScale / 2.0f + i * floorScale, -Game::WorldSize / 2.0f + floorScale / 2.0f + j * floorScale, 0.0, 1.0));
			rect->SetTexture(Texture::GetTextureByName("stone_wall"));
			rect->SetMaterial(Material::Plastic(vec4(0.0f, 0.0f, 0.0f, 1.0f)));
			rect->Scale(floorScale);
		}
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
	//cubeContainer->RotateX(1.0f / 10000.0f * FpsTracker::GetFrameTimeMs());
	//cubeContainer->RotateY(1.0f / 10000.0f * FpsTracker::GetFrameTimeMs());
	//cubeContainer->RotateZ(1.0f / 10000.0f * FpsTracker::GetFrameTimeMs());

	//groundContainer->RotateX(1.0f / 10000.0f * FpsTracker::GetFrameTimeMs());

	//for (auto it = cubes.begin(); it != cubes.end(); ++it)
	/*concurrency::parallel_for_each(begin(cubes), end(cubes), [&](Cube *cube)
	{
		cube->RotateX(static_cast<float>(rand()) / RAND_MAX / 1000.0f * FpsTracker::GetFrameTimeMs());
		cube->RotateY(static_cast<float>(rand()) / RAND_MAX / 1000.0f * FpsTracker::GetFrameTimeMs());
		cube->RotateZ(static_cast<float>(rand()) / RAND_MAX / 1000.0f * FpsTracker::GetFrameTimeMs());
	});*/
	concurrency::parallel_for_each(begin(cubes), end(cubes), [&](Cube *cube)
	{
		//cube->RotateX(1.0f / 2000.0f * FpsTracker::GetFrameTimeMs());
		//cube->RotateY(1.0f / 2000.0f * FpsTracker::GetFrameTimeMs());
		//cube->RotateZ(1.0f / 2000.0f * FpsTracker::GetFrameTimeMs());
	}, concurrency::static_partitioner());

	rootUpdateNode->Update();

	rootModelNode->Update();

	sun->SetPosition(camera->GetPosition() - camera->GetLookDirection() * 10.0f);
	//camera->Update();

	return 0;
}

void renderToCubeMap()
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, frameBuffer);
	//glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthTexture);

	glViewport(0, 0, 1024, 1024);

	//glCullFace(GL_FRONT);

	//for (int i = 0; i < 1; ++i)
	//{
	//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//	//glActiveTexture(GL_TEXTURE0);
	//	//glBindTexture(GL_TEXTURE_2D, Texture::GetTextureByName("ben"));

	//	//glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, cubemap, 0);
	//	//glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, Texture::GetTextureByName("ben"), 0);
	//	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tempTexture, 0);
	//	glViewport(0, 0, 800, 600);
	//	Game::Projection = perspective(45.0f, 800.0f / 600.0f, 1.0f, Game::ViewDistance) * scale(vec3(1, -1, 1));
	//	Shader::SetProjection(Game::Projection);
	//	Shader::SetCamera(lookAt(vec3(camera->GetPosition()), vec3(camera->GetPosition() - camera->GetLookDirection()), vec3(0.0f, 0.0f, 1.0f)));
	//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//	Game::GameRenderGraph->Render();
	//}

	//Shader::SetProjection(perspective(90.0f, 1.0f, 1.0f, Game::ViewDistance) * scale(vec3(1, -1, 1)));

	vec3 projPos = vec3(0.0f, 0.0f, 15.0f);

	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_Y, cubemap, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	Shader::SetProjection(perspective(90.0f, 1.0f, 1.0f, Game::ViewDistance) * scale(vec3(1, 1, 1)));
	Shader::SetCamera(lookAt(projPos, projPos + vec3(0.0f, 1.0f, 0.0f), vec3(0.0f, 0.0f, 1.0f)));
	Game::GameRenderGraph->Render();

	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, cubemap, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	Shader::SetProjection(perspective(90.0f, 1.0f, 1.0f, Game::ViewDistance) * scale(vec3(-1, -1, 1)));
	Shader::SetCamera(lookAt(projPos, projPos + vec3(0.0f, -1.0f, 0.0f), vec3(0.0f, 0.0f, 1.0f)));
	Game::GameRenderGraph->Render();

	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X, cubemap, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	Shader::SetProjection(perspective(90.0f, 1.0f, 1.0f, Game::ViewDistance) * scale(vec3(1, 1, 1)));
	Shader::SetCamera(lookAt(projPos, projPos + vec3(1.0f, 0.0f, 0.0f), vec3(0.0f, -1.0f, 0.0f)));
	Game::GameRenderGraph->Render();

	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_NEGATIVE_X, cubemap, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	Shader::SetProjection(perspective(90.0f, 1.0f, 1.0f, Game::ViewDistance) * scale(vec3(1, 1, 1)));
	Shader::SetCamera(lookAt(projPos, projPos + vec3(-1.0f, 0.0f, 0.0f), vec3(0.0f, -1.0f, 0.0f)));
	Game::GameRenderGraph->Render();

	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_Z, cubemap, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	Shader::SetProjection(perspective(90.0f, 1.0f, 1.0f, Game::ViewDistance) * scale(vec3(1, 1, 1)));
	Shader::SetCamera(lookAt(projPos, projPos + vec3(0.0f, 0.0f, 1.0f), vec3(0.0f, -1.0f, 0.0f)));
	Game::GameRenderGraph->Render();

	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, cubemap, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	Shader::SetProjection(perspective(90.0f, 1.0f, 1.0f, Game::ViewDistance) * scale(vec3(1, 1, 1)));
	Shader::SetCamera(lookAt(projPos, projPos + vec3(0.0f, 0.0f, -1.0f), vec3(0.0f, -1.0f, 0.0f)));
	Game::GameRenderGraph->Render();

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

	updateProjectionMatrixAndViewport();
}

void draw()
{
	// clear the window
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	Shader::SetUseCubeMap(true);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap);

	// draw stuff
	Game::GameRenderGraph->Render();

	Shader::SetUseCubeMap(false);

	//glBindTexture(GL_TEXTURE_2D, Texture::GetTextureByName("ben"));
	////glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 960, 960, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
	//glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 0, 0, 960, 960, 0);

	renderToCubeMap();

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

	init();

	if (SDL_SetRelativeMouseMode(SDL_TRUE) == -1)
		std::cerr << "unable to set relative mouse mode.\n";

	std::cout << "all systems are go bro\n";

	while (!update())
	{
		draw();
	}

	SDL_GL_DeleteContext(glcontext);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}
