#include "DebugManager.h"
#include "GameStateManager.h"

DebugManager* DebugManager::instance = NULL;

void DebugManager::EnableDebugOverlay(bool value) {
	if (value && debugOverlay == NULL) {
		debugOverlay = new DebugOverlay();
		GameStateManager::AddGameScreen(debugOverlay);
	}
	else if (!value && debugOverlay != NULL) {
		GameStateManager::RemoveGameScreen(debugOverlay);
		//delete debugOverlay; - Remove screen deletes screen
		debugOverlay = NULL;
	}
}