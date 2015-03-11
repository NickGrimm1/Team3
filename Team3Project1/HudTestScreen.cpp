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
	Font* f = GameStateManager::Assets()->LoadFont(this, "tahoma", 16, 16);
#if WINDOWS_BUILD
	Texture* grassTex = GameStateManager::Assets()->LoadTexture(this, "Grass_Color", SOIL_FLAG_MIPMAPS);
#endif
#if PS3_BUILD
	Texture* grassTex = GameStateManager::Assets()->LoadTexture(this, "Grass_Color", 0);
#endif
	Texture* calvinTex = GameStateManager::Assets()->LoadTexture(this, "calvin", 0);

	AddDrawable(new DrawableText2D(
		0.0f, 
		0.0f, 
		0, 
		0.5f, 
		0.1f, 
		"HELLO WORLD!!!!!!!!!", 
		f,
		0,
		T3Vector2(0.5f, 0.5f),
		T3Vector4(1.0f,1.0f,1.0f,0.7f)));

	AddDrawable(new DrawableText2D(
		0.3f, 
		0.7f, 
		0, 
		0.4f, 
		0.1f, 
		"Rotated", 
		f,
		0,
		T3Vector2(0.5f, 0.5f),
		T3Vector4(1.0f,1.0f,1.0f,1.0f)));

	AddDrawable(new DrawableText2D(
		0.3f, 
		0.7f, 
		0, 
		0.4f, 
		0.1f, 
		"Rotated", 
		f,
		15,
		T3Vector2(0.5f, 0.5f),
		T3Vector4(1.0f,1.0f,1.0f,0.4f)));

	AddDrawable(new DrawableText2D(
		0.3f, 
		0.7f, 
		0, 
		0.4f, 
		0.1f, 
		"Rotated", 
		f,
		30,
		T3Vector2(0.5f, 0.5f),
		T3Vector4(1.0f,1.0f,1.0f,0.4f)));

	AddDrawable(new DrawableText2D(
		0.3f, 
		0.7f, 
		0, 
		0.4f, 
		0.1f, 
		"Rotated", 
		f,
		45,
		T3Vector2(1.0f, 0.5f),
		T3Vector4(1.0f,1.0f,1.0f,0.4f)));

	AddDrawable(new DrawableText2D(
		0.3f, 
		0.7f, 
		0, 
		0.4f, 
		0.1f, 
		"Rotated", 
		f,
		90,
		T3Vector2(1.0f, 0.5f),
		T3Vector4(1.0f,1.0f,1.0f,0.4f)));

	AddDrawable(
		new DrawableTexture2D(
		0.05f, 
		0.05f, 
		1, 
		0.05f, 
		0.05f, 
		grassTex,
		0,
		T3Vector2(),
		T3Vector4(1.0f,1.0f,1.0f,0.5f)));

	AddDrawable(
		new DrawableTexture2D(
		0.25f, 
		0.05f, 
		-1, 
		0.05f, 
		0.05f, 
		calvinTex,
		30.0,
		T3Vector2(0.5f, 0.5f),
		T3Vector4(1.0f,1.0f,0.0f,0.1f)));
}

void HudTestScreen::UnloadContent()
{
	GameStateManager::Assets()->UnloadFont(this, "tahoma");
	GameStateManager::Assets()->UnloadTexture(this, "Grass_Color");
	GameStateManager::Assets()->UnloadTexture(this, "calvin");
}