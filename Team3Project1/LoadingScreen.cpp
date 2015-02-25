#include "LoadingScreen.h"
#include "GameStateManager.h"
#include "GraphicsCommon.h"

void LoadingScreen::LoadContent() {
	refreshTex = GameStateManager::Assets()->LoadTexture(this, TEXTUREDIR"refresh.png", 0);
	refreshIcon = new DrawableTexture2D(
		SCREEN_WIDTH - 45,
		SCREEN_HEIGHT - 65,
		1,
		25,
		25,
		refreshTex,
		0,
		T3Vector2(),
		T3Vector4(1, 1, 1, 1.0));
	AddDrawable(refreshIcon);
}

void LoadingScreen::Update() {
	refreshIcon->SetRotation(refreshIcon->GetRotation() + 0.0001f);
}

void LoadingScreen::UnloadContent() {
	GameStateManager::Assets()->UnloadTexture(this, TEXTUREDIR"refresh.tga");
}