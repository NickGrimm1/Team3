#pragma once

#include "DrawableTexture2D.h"
#include "GraphicsCommon.h"

class RefreshIcon : public DrawableTexture2D {

public:
	RefreshIcon(Texture* tex, float rotSpeed) :
		DrawableTexture2D(
		SCREEN_WIDTH - 45,
		SCREEN_HEIGHT - 65,
		1,
		25,
		25,
		tex,
		0,
		Vector2(),
		Vector4(1, 1, 1, 1.0))
	{
		rotationSpeed = rotSpeed;
	}

	virtual float GetRotation() {
		SetRotation(rotation + rotationSpeed);
		return rotation;		
	}

protected:
	float rotationSpeed;
};