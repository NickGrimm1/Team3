#include "Camera.h"

/*
Polls the camera for keyboard / mouse movement.
Should be done once per frame! Pass it the msec since
last frame (default value is for simplicities sake...)
*/
void Camera::UpdateCamera()	{
	//Bounds check the pitch, to be between straight up and straight down ;)
	pitch = min(pitch,90.0f);
	pitch = max(pitch,-90.0f);

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

	rotation = Quaternion::EulerAnglesToQuaternion(pitch, yaw, roll).ToMatrix();

	rotatedTarget = position + (rotation * originalTarget);
	rotatedUp = rotation * originalUp;
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
