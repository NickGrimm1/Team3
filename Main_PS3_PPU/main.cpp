#include <stdio.h>
#include <stdlib.h>	

#include <string>
#include <iostream>

#include <sys/process.h>

#include "Renderer.h"
#include "Input.h"
#include "timer.h"
#include "../Team3Project1/FreeCamera.h"
#include "../Framework/SceneNode.h"
#include "../Team3Project1/Mesh.h"
//#pragma comment(lib, "Team3Project1.a")
SYS_PROCESS_PARAM(1001, 0x10000)

bool done = false;
Renderer renderer;
SceneNode *root;

Camera* camera;

void start_button()		{
	done = true;
	std::cout << "Pressed start button!" << std::endl;
}

void select_button()		{
	camera->SetPosition(T3Vector3(0,0,10));
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
	Input::Initialise();
	//If the start button is pressed, call this function!
	Input::SetPadFunction(INPUT_START,	start_button);
	Input::SetPadFunction(INPUT_START,	select_button);

	//Make a new quad mesh, and set its texture to a newcastle logo
	Mesh* m = Mesh::GenerateQuad();
	//m->SetDefaultTexture(*GCMRenderer::LoadGTF("/ncl.gtf"));
	
	//Create a new scenenode
	root = new SceneNode();

	SceneNode* logo = new SceneNode();
	logo->SetMesh(m); //tell it to draw our new quad

	logo->SetTransform(/*Matrix4::rotationX(DegToRad(-90)) * */T3Matrix4::Scale(T3Vector3(100,100,100)));

	root->AddChild(logo);


	renderer.SetRootNode(root); //Set our new SceneNode as the root for our Renderer

	//We need a new camera!
	camera = new FreeCamera();	
	//camera->SetControllingPad(JOYPAD_A);	//Controlled via joypad A
	camera->SetPosition(T3Vector3(0,0,100)); //And set back slightly so we can see the node at the origin

	renderer.SetCamera(camera);	//Set the current renderer camera

	Timer gameTime;

	while(!done) {
		Input::UpdateJoypad();	//Receive latest joypad input for all joypads

		float msec = (float)gameTime.GetTimedMS();

		camera->UpdateCamera();

		root->Update(msec);	//Update our scene hierarchy. This bit is new (previously the renderer handled it)

		renderer.RenderScene();	//Render the scene
	}
	//If we get here, joypad A has had its start button pressed

	std::cout << "Quitting..." << std::endl;

	//delete m->GetDefaultTexture();
	delete m;
	delete root;
	Input::Destroy();

	return 0;
}