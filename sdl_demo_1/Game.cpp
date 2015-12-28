#include "Game.h"
#include "RenderGraph.h"

namespace Bagnall
{
	// PUBLIC

	float Game::WorldSize = 500.0f;
	//float Game::WorldSize = 10000.0f;
	float Game::ViewDistance;

	mat4 Game::Projection;
	mat4 Game::Camera;
	vec4 Game::CameraPosition;

	RenderGraph* Game::MainRenderGraph = new RenderGraph();
}
