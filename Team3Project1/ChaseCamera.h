/**
<summary>A chase camera. Attached to it's parent entity,
but will gradually interpolate it's position to look at the parent.
Useful for outside vehicle cameras.

Author: Nick Grimm
Version: 0.0.5 11/02/2015.</summary>
*/
#pragma once
#include "OffsetCamera.h"
#include "MathHelper.h"

class ChaseCamera : public OffsetCamera
{
public:
	ChaseCamera(DrawableEntity3D* targetEntity, const Vector3& positionOffset, float pitchOffset, float yawOffset, float rollOffset) 
		: OffsetCamera(targetEntity, positionOffset, pitchOffset, yawOffset, rollOffset)
	{ }
	
	void UpdateCamera() 
	{
		// Get new target position
		expectedPosition = targetEntity->GetOriginPosition() +  offset;
		Vector3 pitchYawRoll = targetEntity->GetRotation().GetEulerAngles();
		expectedPitch = pitchYawRoll.x + pitchOffset;
		expectedYaw = pitchYawRoll.y + yawOffset;
		expectedRoll = pitchYawRoll.z + rollOffset;

		// Linearly interpolate to this position
		pitch = MathHelper::Lerp(pitch, expectedPitch, 0.05f);
		yaw = MathHelper::Lerp(yaw, expectedYaw, 0.05f);
		roll = MathHelper::Lerp(roll, expectedRoll, 0.05f);
		position = MathHelper::Lerp(position, expectedPosition, 0.05f);

		Camera::UpdateCamera();
	}
private:
	Vector3 expectedPosition;
	float expectedPitch;
	float expectedYaw;
	float expectedRoll;
};
