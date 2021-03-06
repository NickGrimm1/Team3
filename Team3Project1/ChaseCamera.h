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
	ChaseCamera(DrawableEntity3D* targetEntity, const T3Vector3& positionOffset, float pitchOffset, float yawOffset, float rollOffset) 
		: OffsetCamera(targetEntity, positionOffset, pitchOffset, yawOffset, rollOffset)
	{ }
	
	void UpdateCamera(); 

private:
	T3Vector3 expectedPosition;
	float expectedPitch;
	float expectedYaw;
	float expectedRoll;
};
