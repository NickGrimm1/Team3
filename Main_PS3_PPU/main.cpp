#pragma comment(lib, "Team3Project1.a")
#include <stdio.h>
#include <stdlib.h>	
#include <string>
#include <iostream>
#include <cell/gcm.h>
#include <sys/process.h>

#include "Renderer.h"
#include "Input.h"
#include "../Framework/GameTimer.h"
#include "../Team3Project1/FreeCamera.h"
#include "../Framework/SceneNode.h"
#include "../Team3Project1/Mesh.h"
#include "../Team3Project1/GameStateManager.h"
#include "../Team3Project1/EntryPoint.h"
SYS_PROCESS_PARAM(1001, 0x10000)

//bool done = false;
//Renderer renderer;
//SceneNode *root;


//Camera* camera;

void start_button()		{
	//done = true;
	std::cout << "Pressed start button!" << std::endl;
}

void select_button()		{
	//camera->SetPosition(T3Vector3(0,0,10));
	//camera->SetPitch(0.0f);
	//camera->SetYaw(0.0f);
}


/*
Here's a quick example program. It'll load up a single SceneNode
scene, containing a textured quad, with a camera controllable by
the joypad. Pretty basic, but as with Graphics for Games, its
everything you need to get started and doing something more 
interesting with the Playstation!

*/
int main(void)	{
	std::cout << "All-New intro to GCM V1.0!\n" << std::endl;

	//Start off by initialising the Input system
//	Input::Initialise();
//	std::cout << "Input initialised"<<std::endl;
//	//If the start button is pressed, call this function!
//	Input::SetPadFunction(INPUT_START,	start_button);
//	Input::SetPadFunction(INPUT_START,	select_button);
	//std::cout  << "pad functions set"<<std::endl;
	//Make a new quad mesh, and set its texture to a newcastle logo
//	Mesh* m = Mesh::GenerateQuad();
//	std::cout << "quad generated" << std::endl;
	//m->SetDefaultTexture(*GCMRenderer::LoadGTF("/ncl.gtf"));
	
	//Create a new scenenode
	//root = new SceneNode();
//	std::cout <<"Root scenenode declared" << std::endl;
	//SceneNode* logo = new SceneNode();
	//std::cout << "logo scenenode declared" <<std::endl;
//	logo->SetMesh(m); //tell it to draw our new quad
	//std::cout << "logo mesh set" << std::endl;
	//logo->SetTransform(/*T3Matrix4::Rotation((float)DegToRad(0), T3Vector3::UnitX()) **/ T3Matrix4::Scale(T3Vector3(1,2,1)));
	//std::cout << "logo transform set" << std::endl;
	//root->AddChild(logo);
	//std::cout << "root addChild logo" <<std::endl;

	//renderer.SetRootNode(root); //Set our new SceneNode as the root for our Renderer
	//cout << "Set root as renderer root node" << endl;
	////We need a new camera!
	//camera = new FreeCamera();	
	//cout << "new camera set" << endl;
	////camera->SetControllingPad(JOYPAD_A);	//Controlled via joypad A
	//camera->SetPosition(T3Vector3(0,5,100)); //And set back slightly so we can see the node at the origin
	//cout << "camera position set" << endl;
	//renderer.SetCamera(camera);	//Set the current renderer camera

	GameTimer gameTime;
	
	//while(!done) {
	//	//Input::UpdateJoypad();	//Receive latest joypad input for all joypads

	//	//float msec = (float)gameTime.GetTimedMS();

	//	//camera->UpdateCamera();

	//	//root->Update(msec);	//Update our scene hierarchy. This bit is new (previously the renderer handled it)

	//	//renderer.RenderScene();	//Render the scene
	//}
	//If we get here, joypad A has had its start button pressed

	//std::cout << "Quitting..." << std::endl;

	////delete m->GetDefaultTexture();
	//delete m;
	//delete root;
	//Input::Destroy();
	if (!GameStateManager::Initialize())
		return 1;

	EntryPoint::StartGame();
	cout <<"FINISHED!"<<endl;
	
	return 0;
}