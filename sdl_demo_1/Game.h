#ifndef GAME_H
#define GAME_H

#include "Bagnall.h"

namespace Bagnall
{
	class Game
	{
	public:
		static float ViewDistance;

		static mat4 Projection;
		static mat4 Camera;
		static vec4 CameraPosition;
	};
}

#endif
