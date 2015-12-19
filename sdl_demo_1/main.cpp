#include "Bagnall.h"
#include "Shader.h"
#include "InitShader.h"
#include "Game.h"
#include "FpsTracker.h"
#include "Camera.h"
#include "Rectangle.h"
#include "Cube.h"
#include "Sphere.h"
#include "Skybox.h"
#include "LightSource.h"
#include "Util.h"
#include "UpdateGraph.h"
#include "ModelGraph.h"
#include "RenderGraph.h"
#include "Texture.h"
#include "Shadow.h"
#include "EnvironmentMap.h"
#include <vector>
#include <iostream>

#include <thread>
#include <ppl.h>

using namespace Bagnall;

SDL_Window *window;
SDL_GLContext glcontext;

Camera *camera;
Sphere *sun;
//Cube *cameraCube;
Sphere *player;
LightSource *lightSource;
Object *cubeContainer;
std::vector<Cube*> cubes;

Object *groundContainer;

UpdateNode *rootUpdateNode;
ModelNode *rootModelNode;

//GLuint frameBuffer;
//GLuint depthBuffer;
GLuint colorCubemap;
//GLuint depthCubemap;

GLuint tempTexture;

//const int ENVIRONMENT_MAP_SIZE = 1024;
//const int SHADOW_MAP_SIZE = 1024;

void updateProjectionMatrixAndViewport()
{
	int w, h;
	SDL_GetWindowSize(window, &w, &h);
	Game::Projection = perspective(static_cast<float>(M_PI) / 4.0f, w / static_cast<float>(h), 1.0f, Game::ViewDistance);
	Shader::SetProjection(Game::Projection);

	glViewport(0, 0, w, h);
}

void init(void)
{
	srand(time(NULL));

	Rectangle::Init();
	Cube::Init();
	Sphere::Init();
	Skybox::Init();
	LightSource::Init();
	Texture::Init();
	Shader::Init();
	Shadow::Init();
	EnvironmentMap::Init();

	// ENABLE SHADOWS
	Game::MainRenderGraph->SetShadowsEnabled(true);

	colorCubemap = EnvironmentMap::GenerateCubeMap();

	// SET UP FRAME BUFFER FOR RENDERING TO CUBE MAP
	/*frameBuffer = 0;
	glGenFramebuffers(1, &frameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
	glEnable(GL_DEPTH_TEST);*/

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

	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, SHADOW_MAP_SIZE, SHADOW_MAP_SIZE, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthTexture, 0);

	glDrawBuffer(GL_NONE); // No color buffer is drawn to.*/

	// SET UP COLOR CUBE MAP
	//glGenTextures(1, &colorCubemap);
	//glBindTexture(GL_TEXTURE_CUBE_MAP, colorCubemap);

	//glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_BASE_LEVEL, 0);
	//glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAX_LEVEL, 0);
	//glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	//glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGBA8, ENVIRONMENT_MAP_SIZE, ENVIRONMENT_MAP_SIZE, 0, GL_BGRA, GL_UNSIGNED_BYTE, NULL);
	//glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGBA8, ENVIRONMENT_MAP_SIZE, ENVIRONMENT_MAP_SIZE, 0, GL_BGRA, GL_UNSIGNED_BYTE, NULL);
	//glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGBA8, ENVIRONMENT_MAP_SIZE, ENVIRONMENT_MAP_SIZE, 0, GL_BGRA, GL_UNSIGNED_BYTE, NULL);
	//glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGBA8, ENVIRONMENT_MAP_SIZE, ENVIRONMENT_MAP_SIZE, 0, GL_BGRA, GL_UNSIGNED_BYTE, NULL);
	//glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGBA8, ENVIRONMENT_MAP_SIZE, ENVIRONMENT_MAP_SIZE, 0, GL_BGRA, GL_UNSIGNED_BYTE, NULL);
	//glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGBA8, ENVIRONMENT_MAP_SIZE, ENVIRONMENT_MAP_SIZE, 0, GL_BGRA, GL_UNSIGNED_BYTE, NULL);

	//glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	//// SET UP DEPTH BUFFER FOR FBO
	//glGenRenderbuffers(1, &depthBuffer);
	//glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
	//glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, SHADOW_MAP_SIZE, SHADOW_MAP_SIZE);
	//glBindRenderbuffer(GL_RENDERBUFFER, 0);

	////glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_Y, colorCubemap, 0);

	////// Always check that our framebuffer is ok
	////if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	////{
	////	std::cerr << "frame buffer error after color cube map initialiation\n";
	////}

	//// SET UP DEPTH CUBE MAP
	//glGenTextures(1, &depthCubemap);
	//glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubemap);

	//glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_BASE_LEVEL, 0);
	//glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAX_LEVEL, 0);
	//glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
	//glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);

	//glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_DEPTH_COMPONENT16, SHADOW_MAP_SIZE, SHADOW_MAP_SIZE, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	//glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_DEPTH_COMPONENT16, SHADOW_MAP_SIZE, SHADOW_MAP_SIZE, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	//glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_DEPTH_COMPONENT16, SHADOW_MAP_SIZE, SHADOW_MAP_SIZE, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	//glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_DEPTH_COMPONENT16, SHADOW_MAP_SIZE, SHADOW_MAP_SIZE, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	//glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_DEPTH_COMPONENT16, SHADOW_MAP_SIZE, SHADOW_MAP_SIZE, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	//glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_DEPTH_COMPONENT16, SHADOW_MAP_SIZE, SHADOW_MAP_SIZE, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

	//glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	//glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_CUBE_MAP_POSITIVE_Y, depthCubemap, 0);

	//// Always check that our framebuffer is ok
	//if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	//{
	//	std::cerr << "frame buffer error after depth cube map initialiation\n";
	//}

	//glBindFramebuffer(GL_FRAMEBUFFER, 0);

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
	camera->SetZoomOut(10.0f);

	lightSource = LightSource::ObtainLightSource();
	lightSource->ambient = vec4(1.0, 1.0, 1.0, 1.0);
	lightSource->diffuse = vec4(1.0, 1.0, 1.0, 1.0);
	lightSource->specular = vec4(1.0, 1.0, 1.0, 1.0);
	//lightSource->position = vec4(0.0f, 0.0f, Game::WorldSize * 0.25f, 1.0);
	lightSource->position = vec4(0.0f, 0.0f, 0.0f, 1.0);
	lightSource->UpdateMatrix();
	LightSource::UpdateLightSourceMatricesInShaders();

	// CONTAINERS
	cubeContainer = new Object(rootObject);
	groundContainer = new Object(rootObject);
	groundContainer->Translate(vec4(0.0f, 0.0f, -Game::WorldSize / 2.0f, 0.0f));

	char *textureNames[] = { "shrek", "dowm_furry", "isaac_final_form", "costanza" };

	// MANY CUBES
	int range = Game::WorldSize - 5.0f;
	for (int i = 0; i < 100; ++i)
	{
		Cube *cube = new Cube(cubeContainer);
		cube->SetPosition(vec4(rand() % range - range / 2.0f, rand() % range - range / 2.0f, rand() % range - range / 2.0f, 1.0f));
		//cube->SetTexture(Texture::GetTextureByName("shrek"));
		cube->SetTexture(Texture::GetTextureByName(textureNames[rand() % 4]));
		cube->SetMaterial(Material::Plastic(vec4(0.3f, 0.3f, 0.3f, 1.0f)));
		//cube->SetMaterial(Material::Plastic(vec4(1.0f, 0.0f, 0.0f, 1.0f)));
		cube->Scale(5.0f);
		cubes.push_back(cube);
		//cube->Cull();
	}

	// MIDDLE CUBE
	Cube *cube = new Cube(cubeContainer);
	cube->SetPosition(vec4(0.0f, 0.0f, -Game::WorldSize * 0.25f, 1.0));
	cube->SetTexture(Texture::GetTextureByName("isaac_final_form"));
	//cube->SetTexture(tempTexture);
	//cube->SetEmissive(true);
	cube->SetMaterial(Material::Plastic(vec4(0.3f, 0.3f, 0.3f, 1.0f)));
	cube->Scale(10.0f);
	cubes.push_back(cube);

	// SUN
	//sun = new Cube(rootObject);
	sun = new Sphere(rootObject);
	sun->SetPosition(lightSource->position);
	sun->SetEmissionColor(vec4(1.0f, 1.0f, 0.0f, 1.0f));
	sun->SetEmissive(true);
	sun->Scale(5.0f);
	//cubes.push_back(cube);

	//// CAMERA CUBE
	//cameraCube = new Cube(camera);
	//cameraCube->SetMaterial(Material::Chrome());
	//cameraCube->SetPosition(vec4(0.0, 5.0f, 0.0, 1.0));
	////cameraCube->SetEmissionColor(vec4(1.0f, 1.0f, 0.0f, 1.0f));
	////cameraCube->SetEmissive(true);
	//cameraCube->Scale(1.0f);

	// PLAYER
	player = new Sphere(camera);
	//player->SetMaterial(Material::Chrome());
	//player->SetMaterial(Material::Plastic(vec4(0.25f, 0.25f, 0.25f, 1.0f)));
	player->SetMaterial(Material::Plastic(vec4(0.0f, 0.0f, 0.0f, 1.0f)));
	player->SetCubeMap(colorCubemap);
	//player->SetPosition(vec4(0.0, 5.0f, 0.0, 1.0));
	//player->SetEmissionColor(vec4(0.2f, 0.5f, 0.3f, 1.0f));
	//player->SetEmissive(true);
	player->Scale(1.0f);

	// GIANT SKY CUBE
	/*Skybox *skybox = new Skybox(rootObject);
	skybox->SetTexture(Texture::GetTextureByName("shrek"));
	skybox->SetMaterial(Material::WhiteRubber());
	skybox->SetScale(Game::WorldSize);*/

	// FLOOR
	float floorScale = Game::WorldSize / 10.0f;
	for (int i = 0; i < Game::WorldSize / floorScale; ++i)
	{
		for (int j = 0; j < Game::WorldSize / floorScale; ++j)
		{
			Rectangle *rect = new Rectangle(groundContainer);
			rect->SetPosition(vec4(-Game::WorldSize / 2.0f + floorScale / 2.0f + i * floorScale, -Game::WorldSize / 2.0f + floorScale / 2.0f + j * floorScale, 0.0f, 1.0));
			rect->SetTexture(Texture::GetTextureByName("stone_wall"));
			rect->SetMaterial(Material::Plastic(vec4(0.0f, 0.0f, 0.0f, 1.0f)));
			rect->Scale(floorScale);
		}
	}

	// CEILING
	for (int i = 0; i < Game::WorldSize / floorScale; ++i)
	{
		for (int j = 0; j < Game::WorldSize / floorScale; ++j)
		{
			Rectangle *rect = new Rectangle(groundContainer);
			rect->SetPosition(vec4(-Game::WorldSize / 2.0f + floorScale / 2.0f + i * floorScale, -Game::WorldSize / 2.0f + floorScale / 2.0f + j * floorScale, Game::WorldSize, 1.0));
			rect->SetTexture(Texture::GetTextureByName("stone_wall"));
			rect->SetMaterial(Material::Plastic(vec4(0.0f, 0.0f, 0.0f, 1.0f)));
			rect->Scale(floorScale);
			rect->SetRotationX(M_PI);
		}
	}

	// WALLS
	// SOUTH
	Object *southWall = new Object(rootObject);
	for (int i = 0; i < Game::WorldSize / floorScale; ++i)
	{
		for (int j = 0; j < Game::WorldSize / floorScale; ++j)
		{
			Rectangle *rect = new Rectangle(southWall);
			rect->SetPosition(vec4(-Game::WorldSize / 2.0f + floorScale / 2.0f + i * floorScale, -Game::WorldSize / 2.0f + floorScale / 2.0f + j * floorScale, 0.0f, 1.0));
			rect->SetTexture(Texture::GetTextureByName("stone_wall"));
			rect->SetMaterial(Material::Plastic(vec4(0.0f, 0.0f, 0.0f, 1.0f)));
			rect->Scale(floorScale);
		}
	}
	southWall->SetRotationX(M_PI / 2.0);
	southWall->Translate(vec4(0.0, -Game::WorldSize / 2.0f, 0.0, 0.0));

	// NORTH
	Object *northWall = new Object(rootObject);
	for (int i = 0; i < Game::WorldSize / floorScale; ++i)
	{
		for (int j = 0; j < Game::WorldSize / floorScale; ++j)
		{
			Rectangle *rect = new Rectangle(northWall);
			rect->SetPosition(vec4(-Game::WorldSize / 2.0f + floorScale / 2.0f + i * floorScale, -Game::WorldSize / 2.0f + floorScale / 2.0f + j * floorScale, 0.0f, 1.0));
			rect->SetTexture(Texture::GetTextureByName("stone_wall"));
			rect->SetMaterial(Material::Plastic(vec4(0.0f, 0.0f, 0.0f, 1.0f)));
			rect->Scale(floorScale);
		}
	}
	northWall->SetRotationX(-M_PI / 2.0);
	northWall->Translate(vec4(0.0, Game::WorldSize / 2.0f, 0.0, 0.0));

	// WEST
	Object *westWall = new Object(rootObject);
	for (int i = 0; i < Game::WorldSize / floorScale; ++i)
	{
		for (int j = 0; j < Game::WorldSize / floorScale; ++j)
		{
			Rectangle *rect = new Rectangle(westWall);
			rect->SetPosition(vec4(-Game::WorldSize / 2.0f + floorScale / 2.0f + i * floorScale, -Game::WorldSize / 2.0f + floorScale / 2.0f + j * floorScale, 0.0f, 1.0));
			rect->SetTexture(Texture::GetTextureByName("stone_wall"));
			rect->SetMaterial(Material::Plastic(vec4(0.0f, 0.0f, 0.0f, 1.0f)));
			rect->Scale(floorScale);
		}
	}
	westWall->SetRotationX(-M_PI / 2.0);
	westWall->SetRotationZ(-M_PI / 2.0);
	westWall->Translate(vec4(-Game::WorldSize / 2.0f, 0.0, 0.0, 0.0));

	// EAST
	Object *eastWall = new Object(rootObject);
	for (int i = 0; i < Game::WorldSize / floorScale; ++i)
	{
		for (int j = 0; j < Game::WorldSize / floorScale; ++j)
		{
			Rectangle *rect = new Rectangle(eastWall);
			rect->SetPosition(vec4(-Game::WorldSize / 2.0f + floorScale / 2.0f + i * floorScale, -Game::WorldSize / 2.0f + floorScale / 2.0f + j * floorScale, 0.0f, 1.0));
			rect->SetTexture(Texture::GetTextureByName("stone_wall"));
			rect->SetMaterial(Material::Plastic(vec4(0.0f, 0.0f, 0.0f, 1.0f)));
			rect->Scale(floorScale);
		}
	}
	eastWall->SetRotationX(-M_PI / 2.0);
	eastWall->SetRotationZ(M_PI / 2.0);
	eastWall->Translate(vec4(Game::WorldSize / 2.0f, 0.0, 0.0, 0.0));
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
	cubeContainer->RotateZ(1.0f / 10000.0f * FpsTracker::GetFrameTimeMs());

	//groundContainer->RotateX(1.0f / 10000.0f * FpsTracker::GetFrameTimeMs());

	//for (auto it = cubes.begin(); it != cubes.end(); ++it)
	concurrency::parallel_for_each(begin(cubes), end(cubes), [&](Cube *cube)
	{
		cube->RotateX(static_cast<float>(rand()) / RAND_MAX / 1000.0f * FpsTracker::GetFrameTimeMs());
		cube->RotateY(static_cast<float>(rand()) / RAND_MAX / 1000.0f * FpsTracker::GetFrameTimeMs());
		cube->RotateZ(static_cast<float>(rand()) / RAND_MAX / 1000.0f * FpsTracker::GetFrameTimeMs());
	});
	//concurrency::parallel_for_each(begin(cubes), end(cubes), [&](Cube *cube)
	//{
	//	cube->RotateX(1.0f / 2000.0f * FpsTracker::GetFrameTimeMs());
	//	cube->RotateY(1.0f / 2000.0f * FpsTracker::GetFrameTimeMs());
	//	cube->RotateZ(1.0f / 2000.0f * FpsTracker::GetFrameTimeMs());
	//}, concurrency::static_partitioner());

	rootUpdateNode->Update();

	rootModelNode->Update();

	//sun->SetPosition(camera->GetPosition() - camera->GetLookDirection() * 10.0f);
	//cameraCube->SetPosition(camera->GetPosition() - camera->GetLookDirection() * 10.0f);
	//camera->Update();

	return 0;
}

//void renderToColorCubeMap()
//{
//	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, frameBuffer);
//	//glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthTexture);
//
//	glViewport(0, 0, ENVIRONMENT_MAP_SIZE, ENVIRONMENT_MAP_SIZE);
//
//	//vec3 projPos = vec3(0.0f, 0.0f, 15.0f);
//	//vec3 projPos = vec3(camera->GetPosition() + camera->GetLookDirection() * camera->GetZoomOut());
//	vec3 projPos = vec3(camera->GetPosition());
//	player->DisableRender();
//
//	float zNear = 1.0f;
//	float zFar = Game::ViewDistance;
//	mat4 projection = perspective(static_cast<float>(M_PI) / 2.0f, 1.0f, zNear, zFar);
//	Shader::SetProjection(projection);
//
//	glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);
//
//	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_Y, colorCubemap, 0);
//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//	Shader::SetCamera(lookAt(projPos, projPos + vec3(0.0f, 1.0f, 0.0f), vec3(0.0f, 0.0f, 1.0f)));
//	Game::GameRenderGraph->Render();
//
//	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, colorCubemap, 0);
//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//	Shader::SetCamera(lookAt(projPos, projPos + vec3(0.0f, -1.0f, 0.0f), vec3(0.0f, 0.0f, -1.0f)));
//	Game::GameRenderGraph->Render();
//
//	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X, colorCubemap, 0);
//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//	Shader::SetCamera(lookAt(projPos, projPos + vec3(1.0f, 0.0f, 0.0f), vec3(0.0f, -1.0f, 0.0f)));
//	Game::GameRenderGraph->Render();
//
//	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_NEGATIVE_X, colorCubemap, 0);
//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//	Shader::SetCamera(lookAt(projPos, projPos + vec3(-1.0f, 0.0f, 0.0f), vec3(0.0f, -1.0f, 0.0f)));
//	Game::GameRenderGraph->Render();
//
//	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_Z, colorCubemap, 0);
//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//	Shader::SetCamera(lookAt(projPos, projPos + vec3(0.0f, 0.0f, 1.0f), vec3(0.0f, -1.0f, 0.0f)));
//	Game::GameRenderGraph->Render();
//
//	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, colorCubemap, 0);
//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//	Shader::SetCamera(lookAt(projPos, projPos + vec3(0.0f, 0.0f, -1.0f), vec3(0.0f, -1.0f, 0.0f)));
//	Game::GameRenderGraph->Render();
//
//	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
//
//	glBindTexture(GL_TEXTURE_CUBE_MAP, colorCubemap);
//	glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
//	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
//
//	player->EnableRender();
//
//	Shader::SetCamera(Game::Camera);
//	updateProjectionMatrixAndViewport();
//}

//void renderToDepthCubeMap()
//{
//	sun->DisableRender();
//	//sun->Cull();
//
//	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, frameBuffer);
//	glDrawBuffer(GL_NONE);
//	glReadBuffer(GL_NONE);
//
//	glViewport(0, 0, SHADOW_MAP_SIZE, SHADOW_MAP_SIZE);
//
//	//glCullFace(GL_BACK);
//
//	Shader::SetOnlyDepth(true);
//
//	float zNear = 2.0f;
//	float zFar = Game::ViewDistance;
//	mat4 projection = perspective(static_cast<float>(M_PI) / 2.0f, 1.0f, zNear, zFar);
//	Shader::SetProjection(projection);
//	Shader::SetShadowZRange(vec2(zNear, zFar));
//
//	//vec3 projPos = vec3(0.0f, 0.0f, 15.0f);
//	vec3 projPos = vec3(lightSource->position);
//
//	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_CUBE_MAP_POSITIVE_Y, depthCubemap, 0);
//	glClear(GL_DEPTH_BUFFER_BIT);
//	Shader::SetCamera(lookAt(projPos, projPos + vec3(0.0f, 1.0f, 0.0f), vec3(0.0f, 0.0f, 1.0f)));
//	Game::GameRenderGraph->Render();
//
//	Shader::SetProjection(projection);
//
//	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, depthCubemap, 0);
//	glClear(GL_DEPTH_BUFFER_BIT);
//	//Shader::SetProjection(projection * scale(vec3(-1, -1, 1)));
//	Shader::SetCamera(lookAt(projPos, projPos + vec3(0.0f, -1.0f, 0.0f), vec3(0.0f, 0.0f, -1.0f)));
//	Game::GameRenderGraph->Render();
//
//	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_CUBE_MAP_POSITIVE_X, depthCubemap, 0);
//	glClear(GL_DEPTH_BUFFER_BIT);
//	//Shader::SetProjection(projection * scale(vec3(1, 1, 1)));
//	Shader::SetCamera(lookAt(projPos, projPos + vec3(1.0f, 0.0f, 0.0f), vec3(0.0f, -1.0f, 0.0f)));
//	Game::GameRenderGraph->Render();
//
//	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_CUBE_MAP_NEGATIVE_X, depthCubemap, 0);
//	glClear(GL_DEPTH_BUFFER_BIT);
//	//Shader::SetProjection(projection * scale(vec3(1, 1, 1)));
//	Shader::SetCamera(lookAt(projPos, projPos + vec3(-1.0f, 0.0f, 0.0f), vec3(0.0f, -1.0f, 0.0f)));
//	Game::GameRenderGraph->Render();
//
//	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_CUBE_MAP_POSITIVE_Z, depthCubemap, 0);
//	glClear(GL_DEPTH_BUFFER_BIT);
//	//Shader::SetProjection(projection * scale(vec3(1, 1, 1)));
//	Shader::SetCamera(lookAt(projPos, projPos + vec3(0.0f, 0.0f, 1.0f), vec3(0.0f, -1.0f, 0.0f)));
//	Game::GameRenderGraph->Render();
//
//	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, depthCubemap, 0);
//	glClear(GL_DEPTH_BUFFER_BIT);
//	//Shader::SetProjection(projection * scale(vec3(1, 1, 1)));
//	Shader::SetCamera(lookAt(projPos, projPos + vec3(0.0f, 0.0f, -1.0f), vec3(0.0f, -1.0f, 0.0f)));
//	Game::GameRenderGraph->Render();
//
//	glDrawBuffer(GL_FRONT);
//	glReadBuffer(GL_FRONT);
//
//	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
//
//	Shader::SetOnlyDepth(false);
//	Shader::SetCamera(Game::Camera);
//	updateProjectionMatrixAndViewport();
//
//	sun->EnableRender();
//}

void draw()
{
	//renderToColorCubeMap();
	EnvironmentMap::Render(colorCubemap, vec3(camera->GetPosition()), player, 2.0f, Game::ViewDistance);
	Shadow::RenderShadowMap(vec3(lightSource->position), sun, 2.0, Game::ViewDistance);

	updateProjectionMatrixAndViewport();

	// clear the window
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Shader::SetUseCubeMap(true);
	//glActiveTexture(GL_TEXTURE2);
	//glBindTexture(GL_TEXTURE_CUBE_MAP, colorCubemap);

	// draw stuff
	Game::MainRenderGraph->Render();

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
