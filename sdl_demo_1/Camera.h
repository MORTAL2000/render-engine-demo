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
		Camera();

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
		vec4 GetLookDirection();

	private:
		vec3 lookTheta; // rotation values of the camera
		vec4 lookDirection; // look direction vector of the camera
		float zoomOut; // zoom out value

		/**********************************************************************
		Function:		updateGameCamera
		Purpose:		Update the camera matrix in the vertex shader
		**********************************************************************/
		void updateGameCamera();
	};
}

#endif
