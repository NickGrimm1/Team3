#include "LoadingScreen.h"
#include "GameStateManager.h"
#include "GraphicsCommon.h"
#include "GraphicsTestScreen.h"
#include "HudTestScreen.h"
#include "MainMenu.h"
#include "PauseScreen.h"

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
		isLoading = false;
		paused = false;
	}
}
#if WINDOWS_BUILD
void LoadingScreen::KeyboardEvent(KeyboardEvents::EventType type, KeyboardEvents::Key key) {
	if (!isLoading) {
		switch (type) {
		case KeyboardEvents::KEY_PRESS:
			switch (key) {
			case KeyboardEvents::KEYBOARD_PERIOD:
				debugEnabled = !debugEnabled;
				GameStateManager::Debug()->EnableDebugOverlay(debugEnabled);
				break;
			}
			break;
		}
	}
}
#endif
void LoadingScreen::GamepadEvent(GamepadEvents::PlayerIndex playerID, GamepadEvents::EventType type, GamepadEvents::Button button) 
{
	if (type == GamepadEvents::BUTTON_PRESS)
	{
		if(button == GamepadEvents::INPUT_START)
		{
			if (paused) {
				GameStateManager::Resume();
			}
			else {
				GameStateManager::Pause();
			}
			paused = !paused;
		}
		else if (button == GamepadEvents::INPUT_SELECT)
		{
			debugEnabled = !debugEnabled;
			GameStateManager::Debug()->EnableDebugOverlay(debugEnabled);
		}
	}
}