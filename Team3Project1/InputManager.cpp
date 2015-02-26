#include "InputManager.h"
#if WINDOWS_BUILD
#include "../Framework/Window.h"
#endif

InputManager* InputManager::instance = NULL;

void InputManager::Run()
{
	isRunning = true;
	while(isRunning)
	{
#if WINDOWS_BUILD
		while (Window::GetWindow().GetTimer()->GetMS() - lastFrameTimeStamp < INPUT_TIME) { ; } // Fix the timestep
		float msec = Window::GetWindow().GetTimer()->GetMS() - lastFrameTimeStamp;
		lastFrameTimeStamp = Window::GetWindow().GetTimer()->GetMS();
#endif
	}
}