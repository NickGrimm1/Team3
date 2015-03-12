/**
<summary>Allows the actual game to start on top of the framework.
Called buy the GSM once the framework has initialised.
Not to be with main() which starts the framework.

Author: Nick Grimm
Version: 0.0.3 06/02/2015.</summary>
*/

#include "GraphicsCommon.h"
#include "LoadingScreen.h"
#if WINDOWS_BUILD
#include "AudioTestClass.h"
#include "NetworkTest.h"
#endif

// FOR TESTING PURPOSES - REMOVE IN FINAL BUILD
#include "GraphicsTestScreen.h"
#include "HudTestScreen.h"
#include "DebugOverlay.h"
#include "MainMenu.h"
#include "PauseScreen.h"
#include "RacerGame.h"
#include "VehicleTestingScreen.h"
namespace EntryPoint
{
	void StartGame()
	{
		// TODO: Get a new gameScreen up for splash/menu etc.
		//LoadingScreen* loadingScreen = new LoadingScreen();
		//GameStateManager::Instance()->AddGameScreen(loadingScreen);

		//MainMenu* main = new MainMenu();
		//GameStateManager::Instance()->AddGameScreen(main);

		//GraphicsTestScreen* graphics = new GraphicsTestScreen();
		//GameStateManager::AddGameScreen(graphics);

		RacerGame* game = new RacerGame();
		GameStateManager::Physics()->SetGame(game);
		GameStateManager::Instance()->AddGameScreen(game);

		GameStateManager::Instance()->Start();
	}
}