#if WINDOWS_BUILD

#include "Debuff.h"
#include "Vehicle.h"
//#include "../../nclgl/Keyboard.h"
#include <stdlib.h> 
#include <time.h>  
Debuff::Debuff()
{
	int b=400,a=100,c=401,d=600;
	int SpdBf_pos_x,SpdBf_pos_z,InvBuff_pos_x,InvBuff_pos_z;
	srand((unsigned)time(NULL));
	SpdBf_pos_x=(rand() % (b-a+1))+ a;
	SpdBf_pos_z=(rand() % (b-a+1))+ a;
	InvBuff_pos_x=(rand() % (d-c+1))+ c;
	InvBuff_pos_z=(rand() % (d-c+1))+ c;
	cout<<SpdBf_pos_x<<endl;
	cout<<SpdBf_pos_z<<endl;
	cout<<InvBuff_pos_x<<endl;
	cout<<InvBuff_pos_z<<endl;
	//random the buff position
	SpeedBuffMesh = new OBJMesh(MESHDIR"gas_tank.obj");
	InverseleftrightMesh =new OBJMesh(MESHDIR"ico.obj");
	SpdBf_pos = T3Vector3((float)SpdBf_pos_x,100.0f,(float)SpdBf_pos_z);
	RtBf_pos=T3Vector3((float)InvBuff_pos_x,100.0f,(float)InvBuff_pos_z);
	size = 20.0f;
	SpeedUp= BuildSpeedBuffEntity(size,SpdBf_pos);
	Inverse= BuildInvLeftAndRightEntity(size,RtBf_pos);
	speeduptime=5000.0f;
	//Speed_Player=1;
	//Speed_Rotate = -0.0004;
	//tempPosition = T3Vector3(0,350,-800);
	//Dist2=Car_pos-RtBf_pos;
	LR=true;
}

Debuff::~Debuff(void)
{
	delete SpeedBuffMesh;
	delete SpeedUp;
	delete Inverse;
}


GameEntity* Debuff::BuildSpeedBuffEntity(float radius, T3Vector3 pos) {   
	//SceneNode* test = new SceneNode(SpeedBuffMesh);
	//test->SetModelScale(T3Vector3(radius,radius,radius));
	//test->SetBoundingRadius(radius);
	//test->SetColour(T3Vector4(1,0,0,1));
	//PhysicsNode*p = new PhysicsNode();

	//p->SetUseGravity(false);
	//p->SetPosition(pos);
	////p->SetCollisionVolume(new CollisionSphere(radius));
	//GameEntity*g = new GameEntity(test, p);
	//g->ConnectToSystems();
	//return g;
	return 0;
}

GameEntity* Debuff::BuildInvLeftAndRightEntity(float radius, T3Vector3 pos) {   
	//SceneNode* test = new SceneNode(InverseleftrightMesh);
	//test->SetModelScale(T3Vector3(radius,radius,radius));
	//test->SetBoundingRadius(radius);
	//test->SetColour(T3Vector4(1,0,0,1));
	//PhysicsNode*p = new PhysicsNode();

	//p->SetUseGravity(false);
	//p->SetPosition(pos);
	////p->SetCollisionVolume(new CollisionSphere(radius));
	//GameEntity*g = new GameEntity(test, p);
	//g->ConnectToSystems();
	//return g;
	return 0;
}


void Debuff::UpdateDebuff(Vehicle *car, float msec){
	T3Vector3 Car_pos=car->GetPlayer()->GetPhysicsNode().GetPosition();
	T3Vector3 SpeedUp_pos=SpeedUp->GetPhysicsNode().GetPosition();
	T3Vector3 RtBf_pos=Inverse->GetPhysicsNode().GetPosition();
	T3Vector3 Dist= Car_pos-SpeedUp_pos;
	T3Vector3 Dist2=Car_pos-RtBf_pos;
	if(Dist.Length()<=20.0f)
	{
		SpeedUp->GetPhysicsNode().SetPosition(T3Vector3(0.0f,10000.0f,0.0f));//make entity disappeared
		/*car->GetPlayer()->GetPhysicsNode().SetLinearVelocity(T3Vector3(0,0,50));*/
		car->SetSpeed_Player(8);
	}
	if(car->GetSpeed_Player()==8){
		speeduptime -= msec;
	}
	if (speeduptime<=0.0f)
	{
		speeduptime=5000.0f;
		LR=true;
		car->SetSpeed_Player(1);
	}
	if(Dist2.Length()<=20.0f)
	{
		Inverse->GetPhysicsNode().SetPosition(T3Vector3(0.0f,10000.0f,0.0f));//make entity disappeared
		car->SetSpeed_Player(2);
		LR=false;
	}
	if(car->GetSpeed_Player()==2)
	{
		speeduptime -= msec;
	}
	if(speeduptime<=0)
	{
		speeduptime=5000.0f;
		LR=true;
		car->SetSpeed_Player(1);
	}
}


void Debuff::SwitchleftAndright(Vehicle *Player){
	/*T3Vector3 PlayerPosition = Player->GetPlayer()->GetPhysicsNode().GetPosition();

	if(Player->GetPlayer()->GetPhysicsNode().GetOrientation().z<-0.1||Player->GetPlayer()->GetPhysicsNode().GetOrientation().z>0.1){
		Player->GetPlayer()->GetPhysicsNode().SetAngularVelocity(T3Vector3(0,0,0));
	}

	if(Player->GetPlayer()->GetPhysicsNode().GetPosition().y<=100){
		temp1 = Player->GetPlayer()->GetPhysicsNode().GetLinearVelocity();
		temp1.y = 0;
		Player->GetPlayer()->GetPhysicsNode().SetLinearVelocity(temp1);
		Player->GetPlayer()->GetPhysicsNode().SetUseGravity(FALSE);
	}

	if(Window::GetKeyboard()->KeyDown(KEYBOARD_D)) {
		if(Player->GetPlayer()->GetPhysicsNode().GetOrientation().z>=-0.13){
			Player->GetPlayer()->GetPhysicsNode().SetAngularVelocity(T3Vector3(0,0,Speed_Rotate));
		}			
		temp1 = Player->GetPlayer()->GetPhysicsNode().GetLinearVelocity();
		temp1.x=0.1*Speed_Player;
		Player->GetPlayer()->GetPhysicsNode().SetLinearVelocity(temp1);
		tempPosition.x = PlayerPosition.x;
	}

	if(Window::GetKeyboard()->KeyDown(KEYBOARD_A)) {
		if(Player->GetPlayer()->GetPhysicsNode().GetOrientation().z<=0.13){
			Player->GetPlayer()->GetPhysicsNode().SetAngularVelocity(T3Vector3(0,0,-Speed_Rotate));
		}
		temp1 = Player->GetPlayer()->GetPhysicsNode().GetLinearVelocity();
		temp1.x=-0.1*Speed_Player;
		Player->GetPlayer()->GetPhysicsNode().SetLinearVelocity(temp1);
		tempPosition.x = PlayerPosition.x;
	}

	if(Window::GetKeyboard()->KeyDown(KEYBOARD_W)) {
		temp1 = Player->GetPlayer()->GetPhysicsNode().GetLinearVelocity();
		temp1.z=0.1*Speed_Player;
		Player->GetPlayer()->GetPhysicsNode().SetLinearVelocity(temp1);             
		tempPosition.z = PlayerPosition.z-400;
	}

	if(Window::GetKeyboard()->KeyDown(KEYBOARD_S)) {
		temp1 = Player->GetPlayer()->GetPhysicsNode().GetLinearVelocity();
		temp1.z=-0.1*Speed_Player;
		Player->GetPlayer()->GetPhysicsNode().SetLinearVelocity(temp1);             
		tempPosition.z = PlayerPosition.z-400;
	}

	if(!Window::GetKeyboard()->KeyDown(KEYBOARD_D)&&!Window::GetKeyboard()->KeyDown(KEYBOARD_A)) {		
		temp1 = Player->GetPlayer()->GetPhysicsNode().GetLinearVelocity();
		if(Player->GetPlayer()->GetPhysicsNode().GetOrientation().z<-0.002||Player->GetPlayer()->GetPhysicsNode().GetOrientation().z>0.002){
			if(Player->GetPlayer()->GetPhysicsNode().GetOrientation().z<0){
				Player->GetPlayer()->GetPhysicsNode().SetAngularVelocity(T3Vector3(0,0,-Speed_Rotate));
				
			}
			if(Player->GetPlayer()->GetPhysicsNode().GetOrientation().z>0){
				Player->GetPlayer()->GetPhysicsNode().SetAngularVelocity(T3Vector3(0,0,Speed_Rotate));
			}
		}
		else{
			Player->GetPlayer()->GetPhysicsNode().SetAngularVelocity(T3Vector3(0,0,0));
			temp1.x=0;
			Player->GetPlayer()->GetPhysicsNode().SetLinearVelocity(temp1);
		}
		tempPosition.x = PlayerPosition.x;
	}
	if(!Window::GetKeyboard()->KeyDown(KEYBOARD_W)&&!Window::GetKeyboard()->KeyDown(KEYBOARD_S)) {		
		temp1 = Player->GetPlayer()->GetPhysicsNode().GetLinearVelocity();
		temp1.z=0;
		Player->GetPlayer()->GetPhysicsNode().SetLinearVelocity(temp1);
	}
	if(Player->GetPlayer()->GetPhysicsNode().GetLinearVelocity().y==0){
		if(Window::GetKeyboard()->KeyDown(KEYBOARD_SPACE)){
			temp1 = Player->GetPlayer()->GetPhysicsNode().GetLinearVelocity();
			Player->GetPlayer()->GetPhysicsNode().SetUseGravity(TRUE);
			temp1.y = 0.4;
			Player->GetPlayer()->GetPhysicsNode().SetLinearVelocity(temp1);	
		}
	}*/
}

#endif