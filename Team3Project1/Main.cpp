#include "GameStateManager.h"
#include "EntryPoint.h"

int main()
{
	if (!GameStateManager::Initialize())
		return 1;
	GameStateManager::Instance()->Start();


	EntryPoint::StartGame();

	return 0;
}