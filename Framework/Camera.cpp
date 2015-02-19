#include "Camera.h"

/*
Polls the camera for keyboard / mouse movement.
Should be done once per frame! Pass it the msec since
last frame (default value is for simplicities sake...)
*/
void Camera::UpdateCamera()	
{
	if(pitch < 0) {
		pitch += 360.0f;
	}
	if(pitch > 360.0f) {
		pitch -= 360.0f;
	}

	if(yaw < 0) {
		yaw += 360.0f;
	}
	if(yaw > 360.0f) {
		yaw -= 360.0f;
	}

	if(roll < 0) {
		roll += 360.0f;
	}
	if(roll > 360.0f) {
		roll -= 360.0f;
	}

	rotatedTarget = position + (rotation * originalTarget);
	rotatedUp = rotation * originalUp;
	rotatedRight = rotation * originalRight;
}

/*
Generates a view matrix for the camera's viewpoint. This matrix can be sent
straight to the shader...it's already an 'inverse camera' matrix.
*/
Matrix4 Camera::BuildViewMatrix()	{
	//Why do a complicated matrix inversion, when we can just generate the matrix
	//using the negative values ;). The matrix multiplication order is important!
	return	Matrix4::BuildViewMatrix(position, rotatedTarget, rotatedUp);
};
