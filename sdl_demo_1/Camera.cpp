#include "Camera.h"
#include "Bagnall.h"
#include "Util.h"
#include "Game.h"
#include "Shader.h"
#include "FpsTracker.h"

namespace Bagnall
{
	// PUBLIC

	Camera::Camera()
	{
		lookTheta = vec3();
		lookDirection = vec4(0.0f, 1.0f, 0.0f, 0.0f);
		zoomOut = 0.0;
	}

	void Camera::Update()
	{
		updateGameCamera();
	}

	void Camera::SetZoomOut(float z)
	{
		zoomOut = z;
	}

	vec4 Camera::GetLookDirection()
	{
		return lookDirection;
	}

	// PRIVATE

	void Camera::updateGameCamera()
	{
		vec4 lookPos = position + lookDirection;

		vec4 zoomOffset = -(normalize(lookPos - position)) * zoomOut;

		//Game::Camera = lookAt(vec3(position + zoomOffset), vec3(lookPos), vec3(0.0f, 0.0f, 1.0f));
		Game::Camera = Util::LookAt(vec3(position + zoomOffset), vec3(lookPos), vec3(0.0f, 0.0f, 1.0f));
		Shader::SetCamera(Game::Camera);

		Game::CameraPosition = position + zoomOffset;
		Shader::SetCameraPosition(Game::CameraPosition);
	}
}
