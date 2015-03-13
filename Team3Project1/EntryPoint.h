/**
<summary>Allows the actual game to start on top of the framework.
Called buy the GSM once the framework has initialised.
Not to be with main() which starts the framework.

Initial Author: Nick Grimm
Main Contributors: Derek Kelly, Sherif Salem
Version: 1.0.0 06/02/2015.</summary>
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
		LoadingScreen* loadingScreen = new LoadingScreen();
		GameStateManager::Instance()->AddGameScreen(loadingScreen);

		//MainMenu* main = new MainMenu();
		//GameStateManager::Instance()->AddGameScreen(main);

		//MainMenu* main = new MainMenu();
		//GameStateManager::Instance()->AddGameScreen(main);

		/*PauseScreen* pause = new PauseScreen();
		GameStateManager::Instance()->AddGameScreen(pause);*/

		/*HudTestScreen* hud = new HudTestScreen();
		GameStateManager::Instance()->AddGameScreen(hud);*/

		/*DebugOverlay* debug = new DebugOverlay();
		GameStateManager::Instance()->AddGameScreen(debug);*/

		//RacerGame* game = new RacerGame();
		//GameStateManager::Instance()->AddGameScreen(game);
		//GameStateManager::Instance()->Start();


		//AudioTestClass* audio = new AudioTestClass();
		//GameStateManager::Instance()->AddGameScreen(audio);

		//GraphicsTestScreen* game = new GraphicsTestScreen();
	//	GameStateManager::Instance()->AddGameScreen(game);
		
		//NetworkTest* network = new NetworkTest();
		//GameStateManager::Instance()->AddGameScreen(network);
		//cout << "Entry Point: Let's Start The Game!" << endl;
		//RacerGame* game = new RacerGame();
		////cout << "Entry Point: Racer Game Initialised" << endl;
		//PhysicsEngine::SetGame(game);
		//GameStateManager::Instance()->AddGameScreen(game);
		//cout << "Entry Point: Added Game to GameStateManager" << endl;
		
		//cout << "Entry Point: Set Physics for the Game" << endl;

		//RacerGame* game = new RacerGame();
		//GameStateManager::Instance()->AddGameScreen(game);

		GameStateManager::Instance()->Start();
	}
}