#include "LoadingScreen.h"
#include "GameStateManager.h"
#include "GraphicsCommon.h"
#include "GraphicsTestScreen.h"
#include "HudTestScreen.h"
#include "MainMenu.h"

void LoadingScreen::LoadContent() {
	isLoading = true;
	GameStateManager::Graphics()->EnableLoadingIcon(true);
}

void LoadingScreen::Update() {
	if (isLoading) {
//		GraphicsTestScreen* game = new GraphicsTestScreen();
//		GameStateManager::Instance()->AddGameScreen(game);

//		HudTestScreen* hud = new HudTestScreen();
//		GameStateManager::Instance()->AddGameScreen(hud);
		
//		GameStateManager::Graphics()->EnableLoadingIcon(false);
//		isLoading = false;
	
		MainMenu* menu = new MainMenu();
		GameStateManager::Instance()->AddGameScreen(menu);
	
		GameStateManager::Graphics()->EnableLoadingIcon(false);

		//GameStateManager::RemoveGameScreen(this);
		isLoading = false;
	}
}

void LoadingScreen::UnloadContent() {

}