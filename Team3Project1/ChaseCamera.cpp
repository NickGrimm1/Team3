#pragma once
#include "ChaseCamera.h"

void ChaseCamera::UpdateCamera() 
{
	// Get new target position.
	expectedPosition = targetEntity->GetOriginPosition();
	// Add offset
	expectedPosition += targetEntity->GetRight() * offset.x;
	expectedPosition += targetEntity->GetUp() * offset.y;
	expectedPosition += targetEntity->GetForward() * -offset.z;
	position = MathHelper::Lerp(position, expectedPosition, 0.05f);

	T3Matrix4 expectedRotation = targetEntity->GetRotation().ToMatrix() * Quaternion::EulerAnglesToQuaternion(pitchOffset, yawOffset, rollOffset).ToMatrix();
	rotation = MathHelper::Lerp(rotation, expectedRotation, 0.05f);

	Camera::UpdateCamera();
}