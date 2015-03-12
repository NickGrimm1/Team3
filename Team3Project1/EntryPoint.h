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
		LoadingScreen* loadingScreen = new LoadingScreen();
		GameStateManager::Instance()->AddGameScreen(loadingScreen);

		/*GraphicsTestScreen* game = new GraphicsTestScreen();
		GameStateManager::Instance()->AddGameScreen(game);*/

		//MainMenu* main = new MainMenu();
		//GameStateManager::Instance()->AddGameScreen(main);

		/*PauseScreen* pause = new PauseScreen();
		GameStateManager::Instance()->AddGameScreen(pause);*/

		/*HudTestScreen* hud = new HudTestScreen();
		GameStateManager::Instance()->AddGameScreen(hud);*/

		/*DebugOverlay* debug = new DebugOverlay();
		GameStateManager::Instance()->AddGameScreen(debug);*/

		/*RacerGame* game = new RacerGame();
		GameStateManager::Instance()->AddGameScreen(game);
		GameStateManager::Instance()->Start();*/


		//AudioTestClass* audio = new AudioTestClass();
		//GameStateManager::Instance()->AddGameScreen(audio);


		//NetworkTest* network = new NetworkTest();
		//GameStateManager::Instance()->AddGameScreen(network);

		//RacerGame* game = new RacerGame();
		//GameStateManager::Instance()->AddGameScreen(game);
		//PhysicsEngine::SetGame(game);

		//VehicleTestingScreen* physicsgame = new VehicleTestingScreen();
		//GameStateManager::Instance()->AddGameScreen(physicsgame);

		GameStateManager::Instance()->Start();
	}
}