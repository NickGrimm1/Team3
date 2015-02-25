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
//		HudTestScreen* hud = new HudTestScreen();
//		GameStateManager::Instance()->AddGameScreen(hud);
		
//		GraphicsTestScreen* game = new GraphicsTestScreen();
//		GameStateManager::Instance()->AddGameScreen(game);
		
		GameStateManager::Graphics()->EnableLoadingIcon(false);
		isLoading = false;
	}
}

void LoadingScreen::UnloadContent() {

}