#include"Vehicle.h"


Vehicle::Vehicle(){
	PlayerMesh = new OBJMesh(MESHDIR"Nova Car.obj");
	
	BuffMesh = new OBJMesh(MESHDIR"ico.obj");

	//TT =  SOIL_load_OGL_texture ("../../Texture/footballpitch.jpg", SOIL_LOAD_AUTO , SOIL_CREATE_NEW_ID , SOIL_FLAG_MIPMAPS);
	PlayerPosition=T3Vector3(500,100,-800);

	Player = BuildPlayerEntity(20.0f,PlayerPosition);
	

	Buff = BuildBuffEntity(10,T3Vector3(200,100,-200));

	Speed_Player = 2;
	f=0;
	temp2=T3Vector3(0,0,0);
	Speed_Rotate = -0.0004;

	tempPosition = T3Vector3(0,350,-800);
}

Vehicle::~Vehicle(void){
	delete PlayerMesh;
	delete Player;
}

void Vehicle::UpdatePlayer1(float msec){
//void Vehicle::UpdatePlayer1(float msec,float forward, float sideways) {
	PlayerPosition = Player->GetPhysicsNode().GetPosition();

	if(Player->GetPhysicsNode().GetOrientation().z<-0.1||Player->GetPhysicsNode().GetOrientation().z>0.1){
		Player->GetPhysicsNode().SetAngularVelocity(T3Vector3(0,0,0));
	}

	if(Player->GetPhysicsNode().GetPosition().y<=100){
		temp1 = Player->GetPhysicsNode().GetLinearVelocity();
		temp1.y = 0;
		Player->GetPhysicsNode().SetLinearVelocity(temp1);
		Player->GetPhysicsNode().SetUseGravity(false);
	}

	/*if(Window::GetKeyboard()->KeyDown(KEYBOARD_A)) {
		if(Player->GetPhysicsNode().GetOrientation().z>=-0.13){
			Player->GetPhysicsNode().SetAngularVelocity(T3Vector3(0,0,Speed_Rotate));
		}			
		temp1 = Player->GetPhysicsNode().GetLinearVelocity();
		temp1.x=0.1*Speed_Player;
		Player->GetPhysicsNode().SetLinearVelocity(temp1);
		tempPosition.x = PlayerPosition.x;

	}*/

	/*if(Window::GetKeyboard()->KeyDown(KEYBOARD_D)) {
		if(Player->GetPhysicsNode().GetOrientation().z<=0.13){
			Player->GetPhysicsNode().SetAngularVelocity(T3Vector3(0,0,-Speed_Rotate));
		}
		temp1 = Player->GetPhysicsNode().GetLinearVelocity();
		temp1.x=-0.1*Speed_Player;
		Player->GetPhysicsNode().SetLinearVelocity(temp1);
		tempPosition.x = PlayerPosition.x;
	}*/


	/*if(Window::GetKeyboard()->KeyHeld(KEYBOARD_W)) {

	    f=f+0.01;
		Player->GetPhysicsNode().SetForce(T3Vector3(0,0,0.3+f));                                                                        
		tempPosition.z = PlayerPosition.z-1000;
		tempPosition.y = PlayerPosition.y+800;
	
	}*/

	tempPosition.z = PlayerPosition.z-1000;
	tempPosition.y = PlayerPosition.y+800;
	

	//if(Window::GetKeyboard()->KeyHeld(KEYBOARD_S)) {

	//	f=f+0.01;
	//	Player->GetPhysicsNode().SetForce(T3Vector3(0,0,-1-f)); 
	//	tempPosition.z = PlayerPosition.z-1000;
	//	tempPosition.y = PlayerPosition.y+800;

	//}

	/*if(!Window::GetKeyboard()->KeyDown(KEYBOARD_D)&&!Window::GetKeyboard()->KeyDown(KEYBOARD_A)) {		
		temp1 = Player->GetPhysicsNode().GetLinearVelocity();
		if(Player->GetPhysicsNode().GetOrientation().z<-0.002||Player->GetPhysicsNode().GetOrientation().z>0.002){
			if(Player->GetPhysicsNode().GetOrientation().z<0){
				Player->GetPhysicsNode().SetAngularVelocity(T3Vector3(0,0,-Speed_Rotate));
				
			}
			if(Player->GetPhysicsNode().GetOrientation().z>0){
				Player->GetPhysicsNode().SetAngularVelocity(T3Vector3(0,0,Speed_Rotate));	
			}
		}
		else{
			Player->GetPhysicsNode().SetAngularVelocity(T3Vector3(0,0,0));
			temp1.x=0;
			Player->GetPhysicsNode().SetLinearVelocity(temp1);
		}
		tempPosition.x = PlayerPosition.x;
	}*/
	
	/*if(Player->GetPhysicsNode().GetLinearVelocity().y==0){
		if(Window::GetKeyboard()->KeyDown(KEYBOARD_SPACE)){
			temp1 = Player->GetPhysicsNode().GetLinearVelocity();
			Player->GetPhysicsNode().SetUseGravity(TRUE);
			temp1.y = 0.4;
			Player->GetPhysicsNode().SetLinearVelocity(temp1);	
		}
	}*/

	

}

//void Vehicle::UpdatePlayer2(float msec){
//	PlayerPosition = Player->GetPhysicsNode().GetPosition();
//
//	if(Player->GetPhysicsNode().GetOrientation().z<-0.1||Player->GetPhysicsNode().GetOrientation().z>0.1){
//		Player->GetPhysicsNode().SetAngularVelocity(T3Vector3(0,0,0));
//	}
//
//	if(Player->GetPhysicsNode().GetPosition().y<=100){
//		temp1 = Player->GetPhysicsNode().GetLinearVelocity();
//		temp1.y = 0;
//		Player->GetPhysicsNode().SetLinearVelocity(temp1);
//		Player->GetPhysicsNode().SetUseGravity(FALSE);
//	}
//
//	if(Window::GetKeyboard()->KeyDown(KEYBOARD_D)) {
//		if(Player->GetPhysicsNode().GetOrientation().z>=-0.13){
//			Player->GetPhysicsNode().SetAngularVelocity(T3Vector3(0,0,Speed_Rotate));
//		}			
//		temp1 = Player->GetPhysicsNode().GetLinearVelocity();
//		temp1.x=0.1*Speed_Player;
//		Player->GetPhysicsNode().SetLinearVelocity(temp1);
//		tempPosition.x = PlayerPosition.x;
//
//	}
//
//	if(Window::GetKeyboard()->KeyDown(KEYBOARD_A)) {
//		if(Player->GetPhysicsNode().GetOrientation().z<=0.13){
//			Player->GetPhysicsNode().SetAngularVelocity(T3Vector3(0,0,-Speed_Rotate));
//		}
//		temp1 = Player->GetPhysicsNode().GetLinearVelocity();
//		temp1.x=-0.1*Speed_Player;
//		Player->GetPhysicsNode().SetLinearVelocity(temp1);
//		tempPosition.x = PlayerPosition.x;
//	}
//
//
//	if(Window::GetKeyboard()->KeyHeld(KEYBOARD_W)) {
//
//	    f=f+0.01;
//		Player->GetPhysicsNode().SetForce(T3Vector3(0,0,1+f));                                                                        
//		tempPosition.z = PlayerPosition.z-1000;
//		tempPosition.y = PlayerPosition.y+800;
//	
//	}
//
//	tempPosition.z = PlayerPosition.z-1000;
//	tempPosition.y = PlayerPosition.y+800;
//	
//
//	if(Window::GetKeyboard()->KeyHeld(KEYBOARD_S)) {
//
//		f=f+0.01;
//		Player->GetPhysicsNode().SetForce(T3Vector3(0,0,-1-f)); 
//		tempPosition.z = PlayerPosition.z-1000;
//		tempPosition.y = PlayerPosition.y+800;
//
//	}
//
//	if(!Window::GetKeyboard()->KeyDown(KEYBOARD_D)&&!Window::GetKeyboard()->KeyDown(KEYBOARD_A)) {		
//		temp1 = Player->GetPhysicsNode().GetLinearVelocity();
//		if(Player->GetPhysicsNode().GetOrientation().z<-0.002||Player->GetPhysicsNode().GetOrientation().z>0.002){
//			if(Player->GetPhysicsNode().GetOrientation().z<0){
//				Player->GetPhysicsNode().SetAngularVelocity(T3Vector3(0,0,-Speed_Rotate));
//				
//			}
//			if(Player->GetPhysicsNode().GetOrientation().z>0){
//				Player->GetPhysicsNode().SetAngularVelocity(T3Vector3(0,0,Speed_Rotate));	
//			}
//		}
//		else{
//			Player->GetPhysicsNode().SetAngularVelocity(T3Vector3(0,0,0));
//			temp1.x=0;
//			Player->GetPhysicsNode().SetLinearVelocity(temp1);
//		}
//		tempPosition.x = PlayerPosition.x;
//	}
//	
//	if(Player->GetPhysicsNode().GetLinearVelocity().y==0){
//		if(Window::GetKeyboard()->KeyDown(KEYBOARD_SPACE)){
//			temp1 = Player->GetPhysicsNode().GetLinearVelocity();
//			Player->GetPhysicsNode().SetUseGravity(TRUE);
//			temp1.y = 0.4;
//			Player->GetPhysicsNode().SetLinearVelocity(temp1);	
//		}
//	}
//
//	if(Player->GetPhysicsNode().GetLinearVelocity()==T3Vector3(0,0,0))
//	{
//
//	}
//
//}

/*
Makes a Player. Every game has a crate in it somewhere!
*/


GameEntity* Vehicle::BuildPlayerEntity(float size, T3Vector3 pos) {   
	SceneNode* test = new SceneNode(PlayerMesh);
	test->SetModelScale(T3Vector3(size,size,size));
	test->SetBoundingRadius(size);
	test->SetColour(T3Vector4(1,1,0.5,1));
	//test->SetTexture(TT);
	PhysicsNode*p = new PhysicsNode();

	p->SetUseGravity(false);
	//pos = T3Vector3(0,100,-400);
	p->SetPosition(pos);
	//p->SetCollisionVolume(new CollisionSphere(size));   
									 
	
	p->SetMass(400);
	p->SetCollisionVolume(new CollisionAABB(T3Vector3(size,size,size)));


	GameEntity*g = new GameEntity(test, p);
	g->ConnectToSystems();
	return g;
}

GameEntity* Vehicle::BuildBuffEntity(float radius, T3Vector3 pos) {   
	SceneNode* test = new SceneNode(BuffMesh);
	test->SetModelScale(T3Vector3(radius,radius,radius));
	test->SetBoundingRadius(radius);
	test->SetColour(T3Vector4(1,0,0,1));
	PhysicsNode*p = new PhysicsNode();

	p->SetUseGravity(false);
	p->SetPosition(pos);

	GameEntity*g = new GameEntity(test, p);
	g->ConnectToSystems();
	return g;
}