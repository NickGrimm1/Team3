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
	Texture* grassTex = GameStateManager::Assets()->LoadTexture(this, TEXTUREDIR"Grass_Color.jpg", SOIL_FLAG_MIPMAPS);
	Texture* calvinTex = GameStateManager::Assets()->LoadTexture(this, TEXTUREDIR"calvin.bmp", 0);

	AddDrawable(new DrawableText2D(
		0.0, 
		0.0, 
		0, 
		0.5, 
		0.1, 
		"HELLO WORLD!!!!!!!!!", 
		f,
		0,
		T3Vector2(0.5, 0.5),
		T3Vector4(1,1,1,0.7f)));

	AddDrawable(new DrawableText2D(
		0.3f, 
		0.7f, 
		0, 
		0.4f, 
		0.1, 
		"Rotated", 
		f,
		0,
		T3Vector2(0.5f, 0.5f),
		T3Vector4(1,1,1,1.0f)));

	AddDrawable(new DrawableText2D(
		0.3, 
		0.7, 
		0, 
		0.4, 
		0.1, 
		"Rotated", 
		f,
		15,
		T3Vector2(0.5f, 0.5f),
		T3Vector4(1,1,1,0.4f)));

	AddDrawable(new DrawableText2D(
		0.3, 
		0.7, 
		0, 
		0.4, 
		0.1, 
		"Rotated", 
		f,
		30,
		T3Vector2(0.5f, 0.5f),
		T3Vector4(1,1,1,0.4f)));

	AddDrawable(new DrawableText2D(
		0.3, 
		0.7, 
		0, 
		0.4, 
		0.1, 
		"Rotated", 
		f,
		45,
		T3Vector2(1.0f, 0.5f),
		T3Vector4(1,1,1,0.4f)));

	AddDrawable(new DrawableText2D(
		0.3, 
		0.7, 
		0, 
		0.4, 
		0.1, 
		"Rotated", 
		f,
		90,
		T3Vector2(1.0f, 0.5f),
		T3Vector4(1,1,1,0.4f)));

	AddDrawable(
		new DrawableTexture2D(
		0.05, 
		0.05, 
		1, 
		0.05, 
		0.05, 
		grassTex,
		0,
		T3Vector2(),
		T3Vector4(1,1,1,0.5)));

	AddDrawable(
		new DrawableTexture2D(
		0.25, 
		0.05, 
		-1, 
		0.05, 
		0.05, 
		calvinTex,
		30.0,
		T3Vector2(0.5f, 0.5f),
		T3Vector4(1,1,0,0.1f)));
}

void HudTestScreen::UnloadContent()
{
	GameStateManager::Assets()->UnloadFont(this, TEXTUREDIR"tahoma.tga");
	GameStateManager::Assets()->UnloadTexture(this, TEXTUREDIR"Grass_Color.jpg");
	GameStateManager::Assets()->UnloadTexture(this, TEXTUREDIR"calvin.bmp");
}