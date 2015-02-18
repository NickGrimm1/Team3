/**
<summary>Allows the actual game to start on top of the framework.
Called buy the GSM once the framework has initialised.
Not to be with main() which starts the framework.

Author: Nick Grimm
Version: 0.0.3 06/02/2015.</summary>
*/

#include "GraphicsTestScreen.h"

namespace EntryPoint
{
	void StartGame()
	{
		// TODO: Get a new gameScreen up for splash/menu etc.

		GraphicsTestScreen* game = new GraphicsTestScreen();
		GameStateManager::Instance()->AddGameScreen(game);
		GameStateManager::Instance()->Start();
	}
}