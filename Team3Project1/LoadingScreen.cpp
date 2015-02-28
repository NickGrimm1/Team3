#include "LoadingScreen.h"
#include "GameStateManager.h"
#include "GraphicsCommon.h"
#include "GraphicsTestScreen.h"
#include "HudTestScreen.h"

void LoadingScreen::LoadContent() {
	isLoading = true;
}

void LoadingScreen::Update() {
	if (isLoading) {
		GraphicsTestScreen* game = new GraphicsTestScreen();
		GameStateManager::Instance()->AddGameScreen(game);

		HudTestScreen* hud = new HudTestScreen();
		GameStateManager::Instance()->AddGameScreen(hud);
		
		GameStateManager::Graphics()->EnableLoadingIcon(false);
		isLoading = false;
		
		GameStateManager::Instance()->RemoveGameScreen(this);
	}
}

void LoadingScreen::UnloadContent() {

}