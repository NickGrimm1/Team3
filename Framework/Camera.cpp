#include "Camera.h"

/*
Polls the camera for keyboard / mouse movement.
Should be done once per frame! Pass it the msec since
last frame (default value is for simplicities sake...)
*/
void Camera::UpdateCamera()	
{
	rotatedTarget = rotation * originalTarget;
	rotatedUp = rotation * originalUp;
	rotatedRight = rotation * originalRight;
}

/*
Generates a view matrix for the camera's viewpoint. This matrix can be sent
straight to the shader...it's already an 'inverse camera' matrix.
*/
T3Matrix4 Camera::BuildViewMatrix()	{
	//Why do a complicated matrix inversion, when we can just generate the matrix
	//using the negative values ;). The matrix multiplication order is important!
	return	T3Matrix4::BuildViewMatrix(position, position + rotatedTarget, rotatedUp);
};
