#include "Game.h"
#include "RenderGraph.h"

namespace Bagnall
{
	// PUBLIC

	float Game::WorldSize = 100.0f;
	float Game::ViewDistance;

	mat4 Game::Projection;
	mat4 Game::Camera;
	vec4 Game::CameraPosition;

	RenderGraph* Game::GameRenderGraph = new RenderGraph();
}
