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

namespace EntryPoint
{
	void StartGame()
	{
		LoadingScreen* loadingScreen = new LoadingScreen();
		GameStateManager::Instance()->AddGameScreen(loadingScreen);

		GameStateManager::Instance()->Start();
	}
}