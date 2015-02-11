#include "GameStateManager.h"

int main()
{
	if (!GameStateManager::Initialize())
		return 1;

	return 0;
}