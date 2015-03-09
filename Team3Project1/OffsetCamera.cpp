#include "OffsetCamera.h"

void OffsetCamera::UpdateCamera() 
{
	// Lock this to the target.
	position = targetEntity->GetOriginPosition();
	// Add offset
	position += targetEntity->GetRight() * offset.x;
	position += targetEntity->GetUp() * offset.y;
	position += targetEntity->GetForward() * -offset.z;

	rotation = targetEntity->GetRotation().ToMatrix() * Quaternion::EulerAnglesToQuaternion(pitchOffset, yawOffset, rollOffset).ToMatrix();
	
	Camera::UpdateCamera();
}