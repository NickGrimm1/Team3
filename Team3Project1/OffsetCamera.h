/**
<summary>An offset camera. Clamped to it's parent entity. Useful for FPS cameras/inside vehicle cameras.

Author: Nick Grimm
Version: 0.0.5 11/02/2015.</summary>
*/
#pragma once
#include "../Framework/Camera.h"

class OffsetCamera : public Camera
{
public:
	OffsetCamera(DrawableEntity3D* targetEntity, const T3Vector3& positionOffset, float pitchOffset, float yawOffset, float rollOffset) 
		: Camera(pitchOffset, yawOffset, rollOffset, targetEntity->GetOriginPosition() + positionOffset, targetEntity->GetForward()), targetEntity(targetEntity), offset(positionOffset), pitchOffset(pitchOffset), yawOffset(yawOffset), rollOffset(rollOffset)
	{ }
	
	T3Vector3 GetPositionOffset() const { return offset; }
	void SetPositionOffset(const T3Vector3& value) { offset = value; }
	float GetPitchOffset() const { return pitchOffset; }
	void SetPitchOffset(float value) { pitchOffset = value; }
	float GetYawOffset() const { return yawOffset; }
	void SetYawOffset(float value) { yawOffset = value; }
	float GetRollOffset() const { return rollOffset; }
	void SetRollOffset(float value) { rollOffset = value; }

	void UpdateCamera();
protected:
	T3Vector3 offset;
	float pitchOffset;
	float yawOffset;
	float rollOffset;
	DrawableEntity3D* targetEntity;
};