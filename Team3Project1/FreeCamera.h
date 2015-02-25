/**
<summary>A somewhat basic 6DOF camera. Useful for RTS cameras.

Author: Nick Grimm
Version: 0.0.5 11/02/2015.</summary>
*/
#pragma once
#include "../Framework/Camera.h"

class FreeCamera : public Camera
{
public:
	FreeCamera(float pitch = 0, float yaw = 0, float roll = 0, T3Vector3 position = T3Vector3(), T3Vector3 target = T3Vector3(0, 0, -1)) 
		: Camera(NULL, pitch, yaw, roll, position, target)
	{ }
	
	void UpdateCamera() 
	{
		// This doesn't do anything in this camera type since position/rotation will be set by their respective methods.
		Camera::UpdateCamera();
	}
};