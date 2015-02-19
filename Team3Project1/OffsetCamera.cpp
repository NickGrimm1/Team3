#include "OffsetCamera.h"

void OffsetCamera::UpdateCamera() 
{
	// Lock this to the target.
	position = targetEntity->GetOriginPosition() + offset;
	rotation = targetEntity->GetRotation().ToMatrix() * Quaternion::EulerAnglesToQuaternion(pitchOffset, yawOffset, rollOffset).ToMatrix();

	Camera::UpdateCamera();
}