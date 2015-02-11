#include "OffsetCamera.h"

void OffsetCamera::UpdateCamera() 
{
	// Lock this to the target.
	position = targetEntity->GetOriginPosition() +  offset;
	Vector3 pitchYawRoll = targetEntity->GetRotation().GetEulerAngles();
	pitch = pitchYawRoll.x + pitchOffset;
	yaw = pitchYawRoll.y + yawOffset;
	roll = pitchYawRoll.z + rollOffset;

	Camera::UpdateCamera();
}