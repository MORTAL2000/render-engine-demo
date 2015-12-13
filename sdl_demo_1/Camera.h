#ifndef CAMERA_H
#define CAMERA_H

#include "Object.h"

namespace Bagnall
{
	class Player;

	class Camera : public Object
	{
	public:
		/**********************************************************************
		Function:		Camera constructor
		Purpose:		Create a camera object
		**********************************************************************/
		Camera(Object *par);

		/**********************************************************************
		Function:		Update
		Purpose:		Update the camera object
		**********************************************************************/
		void Update();

		/**********************************************************************
		Function:		SetZoomOut
		Purpose:		Set the zoom out value.
		Parameters:		float z - new zoom out value
		**********************************************************************/
		void SetZoomOut(float z);

		/**********************************************************************
		Function:		GetLookDirection
		Purpose:		Getter for lookDirection
		**********************************************************************/
		vec4 GetLookDirection() const;

		void SetLookDirection(const vec3& lookDir);

		void LookAt(const vec4& pos);

		void RotateX(float x);

		void InputEvent(SDL_Event ev);

	private:
		bool holdingW, holdingA, holdingS, holdingD;
		vec3 lookTheta; // rotation values of the camera
		vec4 lookDirection; // look direction vector of the camera
		mat4 lookRotation;
		float zoomOut; // zoom out value

		void move();

		/**********************************************************************
		Function:		updateGameCamera
		Purpose:		Update the camera matrix in the vertex shader
		**********************************************************************/
		void updateGameCamera();
	};
}

#endif
