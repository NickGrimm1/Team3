#pragma comment(lib, "Team3Project1.a")
#include <stdio.h>
#include <stdlib.h>	
#include <string>
#include <iostream>
#include <cell/gcm.h>
#include <sys/process.h>

#include "Renderer.h"
#include "../Framework/GameTimer.h"
#include "../Team3Project1/FreeCamera.h"
#include "../Framework/SceneNode.h"
#include "../Team3Project1/Mesh.h"
#include "../Team3Project1/GameStateManager.h"
#include "../Team3Project1/EntryPoint.h"
SYS_PROCESS_PARAM(1001, 0x10000)


/*
Here's a quick example program. It'll load up a single SceneNode
scene, containing a textured quad, with a camera controllable by
the joypad. Pretty basic, but as with Graphics for Games, its
everything you need to get started and doing something more 
interesting with the Playstation!

*/
int main(void)	
{
	if (!GameStateManager::Initialize())
		return 1;

	EntryPoint::StartGame();
	
	return 0;
}
