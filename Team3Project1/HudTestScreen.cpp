#include "HudTestScreen.h"
#include "DrawableText2D.h"
#include "GameStateManager.h"

HudTestScreen::HudTestScreen(void)
{
}


HudTestScreen::~HudTestScreen(void)
{
}

void HudTestScreen::LoadContent() {
	Font* f = GameStateManager::Assets()->LoadFont(this, TEXTUREDIR"tahoma.tga", 16, 16);

	AddDrawable(new DrawableText2D(
		10, 
		10, 
		0, 
		500, 
		50, 
		"HELLO WORLD!!!!!!!!!", 
		f,
		0,
		Vector2(0.5, 0.5),
		Vector4(1,0,0,0.7f)));

	AddDrawable(new DrawableText2D(
		150, 
		500, 
		0, 
		400, 
		25, 
		"Rotated", 
		f,
		0,
		Vector2(0.5f, 0.5f),
		Vector4(1,1,1,0.4f)));

	AddDrawable(new DrawableText2D(
		150, 
		500, 
		0, 
		400, 
		25, 
		"Rotated", 
		f,
		15,
		Vector2(0.5f, 0.5f),
		Vector4(1,1,1,0.4f)));

	AddDrawable(new DrawableText2D(
		150, 
		500, 
		0, 
		400, 
		25, 
		"Rotated", 
		f,
		30,
		Vector2(0.5f, 0.5f),
		Vector4(1,1,1,0.4f)));

		AddDrawable(new DrawableText2D(
		150, 
		500, 
		0, 
		400, 
		25, 
		"Rotated", 
		f,
		45,
		Vector2(1.0f, 0.5f),
		Vector4(1,1,1,0.4f)));

		AddDrawable(new DrawableText2D(
		150, 
		500, 
		0, 
		400, 
		25, 
		"Rotated", 
		f,
		90,
		Vector2(1.0f, 0.5f),
		Vector4(1,1,1,0.4f)));

	AddDrawable(
		new DrawableTexture2D(
		10, 
		10, 
		1, 
		30, 
		50, 
		GameStateManager::Assets()->LoadTexture(this, TEXTUREDIR"Grass_Color.tga", SOIL_FLAG_MIPMAPS),
		0,
		Vector2(),
		Vector4(1,1,1,0.5)));

	AddDrawable(
		new DrawableTexture2D(
		150, 
		30, 
		-1, 
		50, 
		30, 
		GameStateManager::Assets()->LoadTexture(this, TEXTUREDIR"calvin.bmp", SOIL_FLAG_MIPMAPS),
		30.0,
		Vector2(0.5f, 0.5f),
		Vector4(0,0,1,1.0f)));
}