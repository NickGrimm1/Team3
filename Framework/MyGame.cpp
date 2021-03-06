#if WINDOWS_BUILD

#include "MyGame.h"
#include "InertialMatrixHelper.h"
#include "Spring.h"
#include "SpringDemo.h"
#include "Vehicle.h"
#include "Debuff.h"
#include "../Team3Project1/FreeCamera.h"
#include "../Team3Project1/GameStateManager.h"
/*
Creates a really simple scene for our game - A cube robot standing on
a floor. As the module progresses you'll see how to get the robot moving
around in a physically accurate manner, and how to stop it falling
through the floor as gravity is added to the scene. 

You can completely change all of this if you want, it's your game!

*/
int MyGame::newt=0;
int MyGame::dt=0 ;
int MyGame::st=1 ;
int MyGame::g=1;
MyGame::MyGame()	{
	//gameCamera = new Camera(-30.0f,0.0f,T3Vector3(0,450,850));
	gameCamera = new FreeCamera(-30.0f,0.0f,0.0f,T3Vector3(0,350,-800));
	
//	Renderer::GetRenderer().SetCamera(gameCamera);
	Car=new Vehicle(20);
	//DB = new Debuff();
	CubeRobot::CreateCube();

	/*
	We're going to manage the meshes we need in our game in the game class!

	You can do this with textures, too if you want - but you might want to make
	some sort of 'get / load texture' part of the Renderer or OGLRenderer, so as
	to encapsulate the API-specific parts of texture loading behind a class so
	we don't care whether the renderer is OpenGL / Direct3D / using SOIL or 
	something else...
	*/
	cube	= new OBJMesh(MESHDIR"cube.obj");
	quad	= Mesh::GenerateQuad();
	sphere	= new OBJMesh(MESHDIR"ico.obj");
	newCar= new OBJMesh(MESHDIR"NovaCar.obj");

	/*
	A more 'robust' system would check the entities vector for duplicates so as
	to not cause problems...why not give it a go?
	*/
#if WINDOWS_BUILD
//	Background_Music = BuildSoundEntity();
#endif
	GameEntity* quadEntity = BuildQuadEntity(1000.0f);
	quadEntity->GetPhysicsNode().SetPosition(T3Vector3(1000,0,0));
	allEntities.push_back(quadEntity);
	
	v=0;

	//Vehicle = BuildVehicleEntity(100.0f,T3Vector3(-300, 100, -100),T3Vector3(0,0,0));
	//allEntities.push_back(Vehicle);


	GameEntity* cube=BuildCubeEntity(50.f);
	cube->GetPhysicsNode().SetPosition(T3Vector3(1000,0,0));
	cube->GetPhysicsNode().SetUseGravity(false);
	cube->GetPhysicsNode().SetInverseMass(0.0f);

	cube->GetPhysicsNode().SetInverseInertia(InertialMatrixHelper::createImmovableInvInertial());
	allEntities.push_back(cube);





	




	GameEntity* cube2=BuildCubeEntityout(50.f);
	cube2->GetPhysicsNode().SetPosition(T3Vector3(1000,0,1500));
	cube2->GetPhysicsNode().SetUseGravity(false);
	cube2->GetPhysicsNode().SetInverseMass(0.0f);

	cube2->GetPhysicsNode().SetInverseInertia(InertialMatrixHelper::createImmovableInvInertial());
	allEntities.push_back(cube2);

	
	
}

MyGame::~MyGame(void)	{
	/*
	We're done with our assets now, so we can delete them
	*/
	delete cube;
	delete quad;
	delete sphere;
	delete Car;
	//delete DB;
	CubeRobot::DeleteCube();

	//GameClass destructor will destroy your entities for you...
}

/*
Here's the base 'skeleton' of your game update loop! You will presumably
want your games to have some sort of internal logic to them, and this
logic will be added to this function.
*/
void MyGame::UpdateGame(float msec) {
	//if(gameCamera) {
	//	bool Inverse=DB->GetLR();
	//	/*if(Inverse==false)
	//	{
	//		Car->UpdatePlayer2(msec);
	//	}
	//	else*/
	//	{
	//		Car->UpdatePlayer1(msec);
	//	}
	//	DB->UpdateDebuff(Car,msec);
	//	
	//	//gameCamera->UpdateCamera(msec);
	//	gameCamera->SetPosition(Car->tempPosition);
	//}
		
	//if (newt==1)
	//{
	//	GT(g*2000.f);
	//	GT3(g*2000.f);
	//	GT2(g*2000.f);
	//	g=g+1;
	//	newt = 0;
	//}

	//if (dt==1)
	//{
	//	allEntities[0]->DisconnectFromSystems();
	//	allEntities[1]->DisconnectFromSystems();
	//	allEntities[2]->DisconnectFromSystems();
	//	allEntities.erase(allEntities.begin());
	//	allEntities.erase(allEntities.begin());
	//	allEntities.erase(allEntities.begin());
	//	/*GT3(g*2000.f);
	//	GT2(g*2000.f);
	//	g=g+1;*/
	//	dt = 0;
	//}


	//if (st==1)
	//{
	//	
	//	st = 0;
	//}

	////if(Window::GetKeyboard()->KeyTriggered(KEYBOARD_A))                                         
	////  {
	////	//  Vehicle->GetPhysicsNode().LookAt (T3Vector3(0,0,0),T3Vector3(-1,0,0));
	////  }
	////if(Window::GetKeyboard()->KeyTriggered(KEYBOARD_D))                                         
	////  {
	////	 //turn right
	////  }
		
	////if(Window::GetKeyboard()->KeyTriggered(KEYBOARD_2))                                         
	////{
	////  
	////  GameEntity* b=BuildCubeEntity(50.0f);
	////  
	////  b->GetPhysicsNode().SetPosition(gameCamera->Camera::GetPosition());
	////  b->GetRenderNode().SetColour(T3Vector4(0.2,0.9,1,1));
	////  b->GetPhysicsNode().SetLinearVelocity(T3Vector3(1,1,1)*T3Vector3(
	////	  -gameCamera->BuildViewMatrix().values[2],
	////	  -gameCamera->BuildViewMatrix().values[6],
	////	  -gameCamera->BuildViewMatrix().values[10]));  
	////  b->GetPhysicsNode().SetInverseMass(1/10.0);
	////  allEntities.push_back(b);
	//// 
	////
	////}

	//for(vector<GameEntity*>::iterator i = allEntities.begin(); i != allEntities.end(); ++i) {
	//	(*i)->Update(msec);
	//}


	//GameStateManager::Physics()->DrawDebug();
}

/*
Makes an entity that looks like a CubeRobot! You'll probably want to modify
this so that you can have different sized robots, with different masses and
so on!
*/
//GameEntity* MyGame::BuildRobotEntity() {
//	GameEntity*g = new GameEntity(new CubeRobot(), new PhysicsNode());
//
//	g->GetPhysicsNode().SetUseGravity(false);
//	g->ConnectToSystems();
//	return g;
//}


//t
void MyGame::GT(float y){
	GameEntity* gt=BuildCubeEntityout(50.f);

	gt->GetPhysicsNode().SetPosition(T3Vector3(1000,0,1500+y));
	gt->GetPhysicsNode().SetUseGravity(false);
	gt->GetPhysicsNode().SetInverseMass(0.0f);


	gt->GetPhysicsNode().SetInverseInertia(InertialMatrixHelper::createImmovableInvInertial());
	allEntities.push_back(gt);
	//GameEntity* cube=BuildCubeEntity(500.f);
}

void MyGame::GT3(float y){
	GameEntity* gt=BuildCubeEntity(50.f);

	gt->GetPhysicsNode().SetPosition(T3Vector3(1000,0,y));
	gt->GetPhysicsNode().SetUseGravity(false);
	gt->GetPhysicsNode().SetInverseMass(0.0f);


	gt->GetPhysicsNode().SetInverseInertia(InertialMatrixHelper::createImmovableInvInertial());
	allEntities.push_back(gt);
	//GameEntity* cube=BuildCubeEntity(500.f);
}



void MyGame::GT2(float y){
	GameEntity* quadEntity = BuildQuadEntity(1000.0f);
	quadEntity->GetPhysicsNode().SetPosition(T3Vector3(1000,0,y));
	allEntities.push_back(quadEntity);
}

//t

/*
Makes a cube. Every game has a crate in it somewhere!
*/
GameEntity* MyGame::BuildCubeEntity(float size) {

	DrawableEntity3D* s = new DrawableEntity3D(GameStateManager::Assets()->LoadMesh(this, MESHDIR"cube.obj"),
		NULL,
		NULL,
		NULL,
		size,
		T3Vector3(-25, 20, 0),
		Quaternion::EulerAnglesToQuaternion(0,0,0),
		T3Vector3(size,size,size));


	PhysicsNode*p = new PhysicsNode();
	
	//p->SetAngularVelocity(T3Vector3(0,0,0.01f));
	p->SetInverseInertia(InertialMatrixHelper::createCuboidInvInertial(1.0f, size, size, size));
	p->SetInverseMass(1.0f);
	p->SetCollisionVolume(new CollisionAABB(T3Vector3(20*size,size,size)));
	//T3Vector3(size*0.5f,size*0.5f,size*0.5f)
	
	GameEntity*g;// = new GameEntity(s, p);
//	g->ConnectToSystems();
	
	return g;
	
}

GameEntity* MyGame::BuildCubeEntityout(float size) {

	DrawableEntity3D* s = new DrawableEntity3D(GameStateManager::Assets()->LoadMesh(this, MESHDIR"cube.obj"),
		NULL,
		NULL,
		NULL,
		size,
		T3Vector3(-25, 20, 0),
		Quaternion::EulerAnglesToQuaternion(0,0,0),
		T3Vector3(size,size,size));

	PhysicsNode*p = new PhysicsNode();
	
	//p->SetAngularVelocity(T3Vector3(0,0,0.01f));
	p->SetInverseInertia(InertialMatrixHelper::createCuboidInvInertial(1.0f, size, size, size));
	p->SetInverseMass(1.0f);
	p->SetCollisionVolume(new CollisionBBAA(T3Vector3(20*size,size,size)));
	//T3Vector3(size*0.5f,size*0.5f,size*0.5f)
	
	GameEntity*g;// = new GameEntity(s, p);
//	g->ConnectToSystems();
	
	return g;
	
}
/*
Makes a sphere.
*/



GameEntity* MyGame::BuildSphereEntity(float radius) {
	DrawableEntity3D* s = new DrawableEntity3D(GameStateManager::Assets()->LoadMesh(this, MESHDIR"sphere.obj"),
		NULL,
		NULL,
		NULL,
		radius,
		T3Vector3(-25, 20, 0),
		Quaternion::EulerAnglesToQuaternion(0,0,0),
		T3Vector3(radius,radius,radius));


	PhysicsNode*p = new PhysicsNode();
	
	p->SetAngularVelocity(T3Vector3(0,0,0.01f));
	p->SetInverseInertia(InertialMatrixHelper::createSphereInvInertial(1.0f, radius));
	p->SetInverseMass(1.0f);
	p->SetCollisionVolume(new CollisionSphere(radius));
	/*CollisionSphere& cSphere = *(CollisionSphere*)p->GetCollisionVolume();  
	float r = cSphere.GetRadius();
	Xstart=p->GetPosition().x-r;
	Xend=p->GetPosition().x+r;
	p->SetXstart(Xstart);
	p->SetXend(Xend);*/
	
	GameEntity*g ;//= new GameEntity(s, p);
//	g->ConnectToSystems();
	
	return g;
}

/*
Makes a flat quad, initially oriented such that we can use it as a simple
floor. 
*/
GameEntity* MyGame::BuildQuadEntity(float size) {
	DrawableEntity3D* ent;
	for (unsigned int i = 0; i < 8; i++) {
		for (unsigned int j = 0; j < 8; j++) {
			ent = new DrawableEntity3D(
				quad,
				NULL,
				GameStateManager::Assets()->LoadTexture(this, "Grass_Color", SOIL_FLAG_MIPMAPS),
				NULL,
				size,
				T3Vector3(-350.0f + i * 100.0f,0,-350.0f + j * 100.0f),
				Quaternion::FromMatrix(T3Matrix4::Rotation(90.0f, T3Vector3(1,0,0))),
				T3Vector3(size,size,size));
		}
	}

	//Oh if only we had a set texture function...we could make our brick floor again WINK WINK


	PhysicsNode*p = new PhysicsNode(Quaternion::AxisAngleToQuaterion(T3Vector3(1,0,0), 90.0f), T3Vector3());
	p->SetUseGravity(false);
	p->SetInverseMass(0.0f);
	//p->SetPosition(T3Vector3(0,0,0));
	p->SetInverseInertia(InertialMatrixHelper::createImmovableInvInertial());
	p->SetCollisionVolume(new CollisionPlane(T3Vector3(0,1,0), 0));
	GameEntity*g;// = new GameEntity(ent, p);
//	g->ConnectToSystems();
	return g;
}

#if WINDOWS_BUILD
//SoundEntity * MyGame :: BuildSoundEntity () {
//	 float size = 300 + ( rand ()%300);
//
//	 SceneNode * s = new SceneNode ( sphere );
//
//	 s-> SetModelScale ( T3Vector3 ( size , size , size ));
//	 s-> SetBoundingRadius ( size );
//	 s-> SetColour ( T3Vector4 (1 ,1 ,1 ,0.6)); // Make node transparent , too
//	 // Pick a sound from a list of filenames
//	 //Sound * snd = SoundManager :: GetSound ( soundNames [ rand ()% NUM_SOUNDS ]);
//	 Sound * snd = SoundManager :: GetSound ("../../Sounds/14615__man__canon.wav");
//	 // and set it on a new SoundEntity
//	 SoundEntity * g=new SoundEntity(snd,s,NULL);
//	 g->GetSoundEmitter().SetLooping(FALSE);
//	 //GameEntity * g = new GameEntity ( snd , s , NULL );
//	 // Randomly place it in the world somewhere
//	 T3Vector3 randpos = T3Vector3 (( rand ()%10)*256 , 0.0f ,( rand ()%10)*256);
//	 randpos -= T3Vector3 (( rand ()%10)*256 , 0.0f ,( rand ()%10)*256);
//
//	 s -> SetTransform ( T3Matrix4 :: Translation ( randpos ));
//	 // Connect it to all of our core systems
////	 g -> ConnectToSystems ();
//
//	 return g ;
// }
#endif

#endif
