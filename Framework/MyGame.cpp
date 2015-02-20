#include "MyGame.h"
#include "InertialMatrixHelper.h"
#include "Spring.h"
#include "SpringDemo.h"
#include "Vehicle.h"
#include "Debuff.h"
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
	//gameCamera = new Camera(-30.0f,0.0f,Vector3(0,450,850));
	gameCamera = new Camera(-30.0f,0.0f,Vector3(0,350,-800));
	
	Renderer::GetRenderer().SetCamera(gameCamera);
	Car=new Vehicle();
	DB = new Debuff();
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
	newCar= new OBJMesh(MESHDIR"Nova Car.obj");

	/*
	A more 'robust' system would check the entities vector for duplicates so as
	to not cause problems...why not give it a go?
	*/
	Background_Music = BuildSoundEntity();
	GameEntity* quadEntity = BuildQuadEntity(1000.0f);
	quadEntity->GetPhysicsNode().SetPosition(Vector3(1000,0,0));
	allEntities.push_back(quadEntity);
	
	v=0;

	//Vehicle = BuildVehicleEntity(100.0f,Vector3(-300, 100, -100),Vector3(0,0,0));
	//allEntities.push_back(Vehicle);


	GameEntity* cube=BuildCubeEntity(50.f);
	cube->GetPhysicsNode().SetPosition(Vector3(1000,0,0));
	cube->GetPhysicsNode().SetUseGravity(false);
	cube->GetPhysicsNode().SetInverseMass(0.0f);

	cube->GetPhysicsNode().SetInverseInertia(InertialMatrixHelper::createImmovableInvInertial());
	allEntities.push_back(cube);





	




	GameEntity* cube2=BuildCubeEntityout(50.f);
	cube2->GetPhysicsNode().SetPosition(Vector3(1000,0,1500));
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
	delete DB;
	CubeRobot::DeleteCube();

	//GameClass destructor will destroy your entities for you...
}

/*
Here's the base 'skeleton' of your game update loop! You will presumably
want your games to have some sort of internal logic to them, and this
logic will be added to this function.
*/
void MyGame::UpdateGame(float msec) {
	if(gameCamera) {
		bool Inverse=DB->GetLR();
		if(Inverse==false)
		{
			Car->UpdatePlayer2(msec);
		}
		else
		{
			Car->UpdatePlayer1(msec);
		}
		DB->UpdateDebuff(Car,msec);
		
		//gameCamera->UpdateCamera(msec);
		gameCamera->SetPosition(Car->tempPosition);
	}

	if (newt==1)
	{
		GT(g*2000.f);
		GT3(g*2000.f);
		GT2(g*2000.f);
		g=g+1;
		newt = 0;
	}

	if (dt==1)
	{
		allEntities[0]->DisconnectFromSystems();
		allEntities[1]->DisconnectFromSystems();
		allEntities[2]->DisconnectFromSystems();
		allEntities.erase(allEntities.begin());
		allEntities.erase(allEntities.begin());
		allEntities.erase(allEntities.begin());
		//cout<<"\n a2aaaa="<<allEntities.size()<<"a2aaaa \n";
		/*GT3(g*2000.f);
		GT2(g*2000.f);
		g=g+1;*/
		dt = 0;
	}


	if (st==1)
	{
		
		//cout<<"\n a3aaaa="<<allEntities.size()<<"a3aaaa \n";
		st = 0;
	}

	if(Window::GetKeyboard()->KeyTriggered(KEYBOARD_A))                                         
	  {
		//  Vehicle->GetPhysicsNode().LookAt (Vector3(0,0,0),Vector3(-1,0,0));
	  }
	if(Window::GetKeyboard()->KeyTriggered(KEYBOARD_D))                                         
	  {
		 //turn right
	  }

	if(Window::GetKeyboard()->KeyTriggered(KEYBOARD_2))                                         
	{
	  
	  GameEntity* b=BuildCubeEntity(50.0f);
	  
	  b->GetPhysicsNode().SetPosition(gameCamera->Camera::GetPosition());
	  b->GetRenderNode().SetColour(Vector4(0.2,0.9,1,1));
	  b->GetPhysicsNode().SetLinearVelocity(Vector3(1,1,1)*Vector3(
		  -gameCamera->BuildViewMatrix().values[2],
		  -gameCamera->BuildViewMatrix().values[6],
		  -gameCamera->BuildViewMatrix().values[10]));  
	  b->GetPhysicsNode().SetInverseMass(1/10.0);
	  allEntities.push_back(b);
	 
	
	}

	for(vector<GameEntity*>::iterator i = allEntities.begin(); i != allEntities.end(); ++i) {
		(*i)->Update(msec);
	}


	PhysicsSystem::GetPhysicsSystem().DrawDebug();
}

/*
Makes an entity that looks like a CubeRobot! You'll probably want to modify
this so that you can have different sized robots, with different masses and
so on!
*/
GameEntity* MyGame::BuildRobotEntity() {
	GameEntity*g = new GameEntity(new CubeRobot(), new PhysicsNode());

	g->GetPhysicsNode().SetUseGravity(false);
	g->ConnectToSystems();
	return g;
}


//t
void MyGame::GT(float y){
	GameEntity* gt=BuildCubeEntityout(50.f);

	gt->GetPhysicsNode().SetPosition(Vector3(1000,0,1500+y));
	gt->GetPhysicsNode().SetUseGravity(false);
	gt->GetPhysicsNode().SetInverseMass(0.0f);


	gt->GetPhysicsNode().SetInverseInertia(InertialMatrixHelper::createImmovableInvInertial());
	allEntities.push_back(gt);
	cout<<"\n aaaa="<<allEntities.size()<<"aaaa \n";
	//GameEntity* cube=BuildCubeEntity(500.f);
}

void MyGame::GT3(float y){
	GameEntity* gt=BuildCubeEntity(50.f);

	gt->GetPhysicsNode().SetPosition(Vector3(1000,0,y));
	gt->GetPhysicsNode().SetUseGravity(false);
	gt->GetPhysicsNode().SetInverseMass(0.0f);


	gt->GetPhysicsNode().SetInverseInertia(InertialMatrixHelper::createImmovableInvInertial());
	allEntities.push_back(gt);
	cout<<"\n aaaa="<<allEntities.size()<<"aaaa \n";
	//GameEntity* cube=BuildCubeEntity(500.f);
}



void MyGame::GT2(float y){
	GameEntity* quadEntity = BuildQuadEntity(1000.0f);
	quadEntity->GetPhysicsNode().SetPosition(Vector3(1000,0,y));
	allEntities.push_back(quadEntity);
	cout<<"\n aaaa="<<allEntities.size()<<"aaaa \n";
}

//t

/*
Makes a cube. Every game has a crate in it somewhere!
*/
GameEntity* MyGame::BuildCubeEntity(float size) {
	/*GameEntity*g = new GameEntity(new SceneNode(cube), new PhysicsNode());
	g->ConnectToSystems();

	SceneNode &test = g->GetRenderNode();

	test.SetModelScale(Vector3(size,size,size));
	test.SetBoundingRadius(size);*/
	SceneNode* s = new SceneNode(cube);

	s->SetModelScale(Vector3(20*size,size,size));
	s->SetBoundingRadius(size);
	s->SetColour(Vector4(0.2,0.2,0.5,1));                                                                                                
	PhysicsNode*p = new PhysicsNode();
	
	//p->SetAngularVelocity(Vector3(0,0,0.01f));
	p->SetInverseInertia(InertialMatrixHelper::createCuboidInvInertial(1.0f, size, size, size));
	p->SetInverseMass(1.0f);
	p->SetCollisionVolume(new CollisionAABB(Vector3(20*size,size,size)));
	//Vector3(size*0.5f,size*0.5f,size*0.5f)
	
	GameEntity*g = new GameEntity(s, p);
	g->ConnectToSystems();
	
	return g;
	
}

GameEntity* MyGame::BuildCubeEntityout(float size) {
	/*GameEntity*g = new GameEntity(new SceneNode(cube), new PhysicsNode());
	g->ConnectToSystems();

	SceneNode &test = g->GetRenderNode();

	test.SetModelScale(Vector3(size,size,size));
	test.SetBoundingRadius(size);*/
	SceneNode* s = new SceneNode(cube);

	s->SetModelScale(Vector3(20*size,size,size));
	s->SetBoundingRadius(size);
	s->SetColour(Vector4(1,0.2,0.5,1));                                                                                                
	PhysicsNode*p = new PhysicsNode();
	
	//p->SetAngularVelocity(Vector3(0,0,0.01f));
	p->SetInverseInertia(InertialMatrixHelper::createCuboidInvInertial(1.0f, size, size, size));
	p->SetInverseMass(1.0f);
	p->SetCollisionVolume(new CollisionBBAA(Vector3(20*size,size,size)));
	//Vector3(size*0.5f,size*0.5f,size*0.5f)
	
	GameEntity*g = new GameEntity(s, p);
	g->ConnectToSystems();
	
	return g;
	
}
/*
Makes a sphere.
*/

 GameEntity* MyGame::BuildVehicleEntity(float size,Vector3 pos, Vector3 vel)
	  {
SceneNode* s = new SceneNode(sphere);

	s->SetColour(Vector4(0.2,0.2,0.5,1));
	PhysicsNode*p = new PhysicsNode();
	p->SetPosition(pos);
	p->SetLinearVelocity(vel);
	p->SetAngularVelocity(Vector3(0,0,0.01f));
	p->SetInverseInertia(InertialMatrixHelper::createSphereInvInertial(1.0f, size));
	p->SetInverseMass(1.0f);
	p->SetCollisionVolume(new CollisionSphere(size));
	GameEntity*g = new GameEntity(s, p);
	g->ConnectToSystems();

	SceneNode &test = g->GetRenderNode();

	test.SetModelScale(Vector3(size,size,size));
	test.SetBoundingRadius(size);

	return g;
}

GameEntity* MyGame::BuildSphereEntity(float radius) {
	SceneNode* s = new SceneNode(sphere);

	s->SetModelScale(Vector3(radius,radius,radius));
	s->SetBoundingRadius(radius);
	s->SetColour(Vector4(0.2,0.2,0.5,1));
	PhysicsNode*p = new PhysicsNode();
	
	p->SetAngularVelocity(Vector3(0,0,0.01f));
	p->SetInverseInertia(InertialMatrixHelper::createSphereInvInertial(1.0f, radius));
	p->SetInverseMass(1.0f);
	p->SetCollisionVolume(new CollisionSphere(radius));
	/*CollisionSphere& cSphere = *(CollisionSphere*)p->GetCollisionVolume();  
	float r = cSphere.GetRadius();
	Xstart=p->GetPosition().x-r;
	Xend=p->GetPosition().x+r;
	p->SetXstart(Xstart);
	p->SetXend(Xend);*/
	
	GameEntity*g = new GameEntity(s, p);
	g->ConnectToSystems();
	
	return g;
}

/*
Makes a flat quad, initially oriented such that we can use it as a simple
floor. 
*/
GameEntity* MyGame::BuildQuadEntity(float size) {
	SceneNode* s = new SceneNode(quad);

	s->SetModelScale(Vector3(size,size,size));
	//Oh if only we had a set texture function...we could make our brick floor again WINK WINK
	s->SetBoundingRadius(size);

	PhysicsNode*p = new PhysicsNode(Quaternion::AxisAngleToQuaterion(Vector3(1,0,0), 90.0f), Vector3());
	p->SetUseGravity(false);
	p->SetInverseMass(0.0f);
	//p->SetPosition(Vector3(0,0,0));
	p->SetInverseInertia(InertialMatrixHelper::createImmovableInvInertial());
	p->SetCollisionVolume(new CollisionPlane(Vector3(0,1,0), 0));
	GameEntity*g = new GameEntity(s, p);
	g->ConnectToSystems();
	return g;
}


SoundEntity * MyGame :: BuildSoundEntity () {
	 float size = 300 + ( rand ()%300);

	 SceneNode * s = new SceneNode ( sphere );

	 s-> SetModelScale ( Vector3 ( size , size , size ));
	 s-> SetBoundingRadius ( size );
	 s-> SetColour ( Vector4 (1 ,1 ,1 ,0.6)); // Make node transparent , too
	 // Pick a sound from a list of filenames
	 //Sound * snd = SoundManager :: GetSound ( soundNames [ rand ()% NUM_SOUNDS ]);
	 Sound * snd = SoundManager :: GetSound ("../../Sounds/14615__man__canon.wav");
	 // and set it on a new SoundEntity
	 SoundEntity * g=new SoundEntity(snd,s,NULL);
	 g->GetSoundEmitter().SetLooping(FALSE);
	 //GameEntity * g = new GameEntity ( snd , s , NULL );
	 // Randomly place it in the world somewhere
	 Vector3 randpos = Vector3 (( rand ()%10)*256 , 0.0f ,( rand ()%10)*256);
	 randpos -= Vector3 (( rand ()%10)*256 , 0.0f ,( rand ()%10)*256);

	 s -> SetTransform ( Matrix4 :: Translation ( randpos ));
	 // Connect it to all of our core systems
	 g -> ConnectToSystems ();

	 return g ;
 }
