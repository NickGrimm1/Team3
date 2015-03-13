#include "RacerGame.h"
#include "Mesh.h"
#include "../Framework/Light.h"
#include "GameStateManager.h"
#include "FreeCamera.h"
#include "ChaseCamera.h"
#include "../Framework/ObjMesh.h"
#include "TrackSegment.h"
#include "../Framework/InertialMatrixHelper.h"
#include "PauseScreen.h"

#if WINDOWS_BUILD
#include "AudioTestClass.h"
#endif
//#include "../Framework/Vehicle.h"
#include <math.h>

#include <time.h>
//TODO - remove
#include <iostream>
#include "../Framework/BumpTexture.h"

//#include "DrawableText2D.h"
//#include "GameScreen2D.h"



int RacerGame::update =0;
float RacerGame::g=0.0f;
float RacerGame::gx =300.0f;



RacerGame::RacerGame(void)
{

	//f=5;
	srand(time(NULL));
	T3Vector3 sp1= T3Vector3(100.0f,0.0f,0.0f);
	T3Vector3 sp2= T3Vector3(200.0f,0.0f,0.0f);
	T3Vector3 sp3= T3Vector3(300.0f,0.0f,0.0f);

	SplinePoint.push_back(sp1);
	SplinePoint.push_back(sp2);
	SplinePoint.push_back(sp3);
	score=0;
	Time=60.0f;
	PlayTime=20;
	timeOrScore=0;
	isplaystartegine=false;
	isplaylowspdegine=false;
	isplaymidspdegine=false;
	moveenginesound=false;
	carspeediszero=true;
#if WINDOWS_BUILD
	Engine0 = new SoundEmitter();
	Engine1 = new SoundEmitter();
#endif

	//DebugOverlay* debug = new DebugOverlay();
	//GameStateManager::Instance()->AddGameScreen(debug);
		

	//Strack = new TrackSegment(SplinePoint[0],SplinePoint[1],SplinePoint[2],5,50.f);
	//TrackSegmentVector.push_back(Strack);
}

RacerGame::~RacerGame(void)
{

	delete quad;

}

void RacerGame::LoadContent() {
	
	quad = GameStateManager::Assets()->LoadCylinder(this, 20);
	cylinder = GameStateManager::Assets()->LoadCylinder(this, 20);
	
	hud = new HudTestScreen();
	GameStateManager::Instance()->AddGameScreen(hud);
#if WINDOWS_BUILD
	audio= new AudioTestClass();
	GameStateManager::Instance()->AddGameScreen(audio);
#endif
	scoreTexture = GameStateManager::Assets()->LoadTexture(this, "score", 0);
	timeTexture = GameStateManager::Assets()->LoadTexture(this, "time", 0);

	//ent = new DrawableEntity3D(
	//	quad, 
	//	NULL,
	//	GameStateManager::Assets()->LoadTexture(this, TEXTUREDIR"Grass_Color.png", 0), 
	//	GameStateManager::Assets()->LoadTexture(this, TEXTUREDIR"snowflake.png", 0),
	//	50.0f, 
	//	T3Vector3(0,0,0), 
	//	Quaternion::FromMatrix(T3Matrix4::Rotation(90.0f, T3Vector3(1,0,0))),
	//	T3Vector3(50,50,1));
	



	
	//SpotLight::SetConeMesh(coneMesh);
	//Mesh* circleMesh = Mesh::GenerateCircle(20);
	//SpotLight::SetCircleMesh(Mesh::GenerateCircle(20));
	//Mesh* sphereMesh = new OBJMesh(MESHDIR"sphere.obj");
	//PointLight::SetMesh(sphereMesh);

	
	//light = GameStateManager::Graphics()->AddSpotLight(T3Vector3(0, 0, 0), T3Vector3(0,1,0), T3Vector3(1,0,0), 1.0f, 45.0f, T3Vector4(1,1,1,1), T3Vector4(1,1,1,1), false);
	
	//PointLight* l = GameStateManager::Graphics()->AddPointLight(T3Vector3(0,5,0), 10, T3Vector4(1,1,1,1), T3Vector4(1,1,1,1), false); 


	camera = new FreeCamera();
	camera->SetPosition(T3Vector3(40,50,1200));
	//checkpoint= new CheckPoint(10);
	//checkpoint->SetPhysics(10,'c');
	//checkpoint->SetType('g');
	//checkpoint->GetPhysicsNode().SetPGE(checkpoint);
	//AddDrawable(checkpoint);
	
	//add road
	
	//Texture* grassTex2 = GameStateManager::Assets()->LoadTexture(this, "trackTex", 0);
	//GameStateManager::Graphics()->GetRenderContext();
	//TrackSegment* trackr = new TrackSegment(T3Vector3(-100.0f,0.0f,0.0f),T3Vector3(0.0f,0.0f,0.0f),T3Vector3(100.0f,0.0f,0.0f), 5, 50.0f);
	//
	////push back track
	////TrackSegmentVector.push_back(trackr);
	//GameStateManager::Graphics()->DropRenderContext();
	//PhysicsNode* proad = new PhysicsNode();
	//GameEntity* road= new GameEntity(proad);
	//	road->SetMesh(trackr);
	//	road->SetShader(0);
	//	road->SetTexture(grassTex2);
	//	road->SetBumpTexture(NULL);
	//	road->SetBoundingRadius(800.0f);
	//	road->SetOriginPosition(T3Vector3(0.0f,-20.0f,0.0f));
	//	road->SetRotation(Quaternion::EulerAnglesToQuaternion(0.0f,0.0f,0.0f));
	//	road->SetScale(T3Vector3(1.0f,1.0f,1.0f));
	//	road->GetPhysicsNode().SetMass(5.0f);
	//	road->GetPhysicsNode().SetMesh(trackr);
	//	road->GetPhysicsNode().SetInverseMass(0.0f);
	//	road->GetPhysicsNode().SetUseGravity(false);
	//	road->GetPhysicsNode().SetXstart(road->GetPhysicsNode().GetPosition().x-200);
	//	road->GetPhysicsNode().SetXend(road->GetPhysicsNode().GetPosition().x+200);
	//	road->GetPhysicsNode().SetIsCollide(true);
	//    road->GetPhysicsNode().Setcar_wheel(true);
	//	road->GetPhysicsNode().Setplanecollision(true);
	//	road->GetPhysicsNode().SetPosition(T3Vector3(0.0f,-20.0f,0.0f));
	//	road->GetPhysicsNode().SetInverseInertia(InertialMatrixHelper::createImmovableInvInertial());
	//	road->ConnectToSystems();
	//	AddDrawable(road);
	
	/*	road =new Road (5);
		road->SetPhysics(5);*/

	float size = 5;

	//VehiclePhysicsNode* vpn = new VehiclePhysicsNode(size);
	
	//GameStateManager::Physics()->AddNode(vpn);

	car = new Vehicle(5);
	car->SetRotation(Quaternion::EulerAnglesToQuaternion(0.0f,-90.0f,0.0f));
	car->SetType('u');
	cout<<car->GetType()<<endl;
	chasecamera = new ChaseCamera(car,T3Vector3(0.0f,40.0f,130.0f),180.0f,90.0f,0.0f);
	//chasecamera->AddYaw(500);
	//chasecamera->AddPitch(90);
	//car->SetPhysics(5,(vpn->GetCar()));



	car->GetVehiclePhysicsNode()->SetPGE(car);
	car->GetVehiclePhysicsNode()->getRFW()->SetPGE(car);
	car->GetVehiclePhysicsNode()->getRFW()->SetType('z');
	car->GetVehiclePhysicsNode()->getLFW()->SetPGE(car);
	car->GetVehiclePhysicsNode()->getLFW()->SetType('b');
	car->GetVehiclePhysicsNode()->getBRW()->SetPGE(car);
	car->GetVehiclePhysicsNode()->getBRW()->SetType('n');
	car->GetVehiclePhysicsNode()->getBLW()->SetPGE(car);
	car->GetVehiclePhysicsNode()->getBLW()->SetType('m');
	car->GetVehiclePhysicsNode()->SetType('q');
	/*vpn->SetPGE(car);
	vpn->getRFW()->SetPGE(car);
	vpn->getRFW()->SetType('z');
	vpn->getLFW()->SetPGE(car);
	vpn->getLFW()->SetType('b');
	vpn->getBRW()->SetPGE(car);
	vpn->getBRW()->SetType('n');
	vpn->getBLW()->SetPGE(car);
	vpn->getBLW()->SetType('m');
	vpn->SetType('f');*/

	//car->SetPhysics(5.0f, vpn);
	AddDrawable(car);

#if WINDOWS_BUILD
	GameStateManager::Audio()->SetListener(car);
#endif
	//FrontRightTire = new Vehicle_Wheel(5.0f);
	//FrontRightTire->SetOriginPosition(T3Vector3(10.0f, -2.0f, 8.0f));
	//FrontRightTire->SetPhysics(5.0f, (vpn->getRFW()));
	AddDrawable(car->GetFrontLeftTire());


	//FrontLeftTire = new Vehicle_Wheel(5.0f);
	//FrontLeftTire->SetOriginPosition(T3Vector3(10.0f, -2.0f, -5.0f));
	//FrontLeftTire->SetPhysics(5, (vpn->getLFW()));
	AddDrawable(car->GetFrontRightTire());


    //BackRightTire = new Vehicle_Wheel(5.0f/*,car->GetPhysicsNode()*/);
	//BackRightTire->SetOriginPosition(T3Vector3(-11.0f, -2.0f, 8.0f));
	//BackRightTire->SetPhysics(5.0f,(vpn->getBRW()));
	AddDrawable(car->GetBackLeftTire());

	//BackLeftTire = new Vehicle_Wheel(5.0f/*,car->GetPhysicsNode()*/);
	//BackLeftTire->SetOriginPosition(T3Vector3(-11.0f, -2.0f, -5.0f));
	//BackLeftTire->SetPhysics(5.0f, (vpn->getBLW()));
	AddDrawable(car->GetBackRightTire());


/*
	Mesh* carMesh = GameStateManager::Assets()->LoadMesh(this, MESHDIR"NovaCar.obj");
	ent = new DrawableEntity3D(
		GameStateManager::Assets()->LoadMesh(this, MESHDIR"NovaCar.obj"),
		NULL,
		NULL,
		NULL,
		25.0f,
		T3Vector3(-25, 20, 0),
		Quaternion::EulerAnglesToQuaternion(0,0,0),
		T3Vector3(5,5,5));
	//gameEntities.push_back(ent);
	AddDrawable(ent);
	*/
	Start();

	Speed_Player = 2;
	//f=0.0f;
	b=0.0f;
	maxSpeed=300.0f;
	minSpeed=160.0f;
	temp2=T3Vector3(0.0f,0.0f,0.0f);
	Speed_Rotate = 1.0f;
	tempPosition = T3Vector3(0.0f,350.0f,-800.0f);
	PlayerPosition=T3Vector3(500.0f,100.0f,-800.0f);

	//camera->SetPosition(T3Vector3(0.0f,10.0f, 80.0f));
	//camera->SetYaw(180.0f);
	//GameStateManager::Graphics()->SetCamera(camera);

	GameStateManager::Graphics()->SetCamera(chasecamera);
}

void RacerGame::Update() { 

	GameStateManager::Input()->GetActiveController();
	

	/*if(car->GetCarNode().GetLinearVelocity().x>0)
	{
		FrontRightTire->GetPhysicsNode().SetAngularVelocity(T3Vector3(-car->GetCarNode().GetLinearVelocity().Length()*0.5f,0,0) );
		FrontLeftTire->GetPhysicsNode().SetAngularVelocity(T3Vector3(-car->GetCarNode().GetLinearVelocity().Length()*0.5f,0,0));
		BackRightTire->GetPhysicsNode().SetAngularVelocity(T3Vector3(-car->GetCarNode().GetLinearVelocity().Length()*0.5f,0,0));
		BackLeftTire->GetPhysicsNode().SetAngularVelocity(T3Vector3(-car->GetCarNode().GetLinearVelocity().Length()*0.5f,0,0));
	}
	if(car->GetCarNode().GetLinearVelocity().x<0)
	{
	    FrontRightTire->GetPhysicsNode().SetAngularVelocity(T3Vector3(car->GetCarNode().GetLinearVelocity().Length()*0.5f,0,0));
		FrontLeftTire->GetPhysicsNode().SetAngularVelocity(T3Vector3(car->GetCarNode().GetLinearVelocity().Length()*0.5f,0,0));
		BackRightTire->GetPhysicsNode().SetAngularVelocity(T3Vector3(car->GetCarNode().GetLinearVelocity().Length()*0.5f,0,0));
		BackLeftTire->GetPhysicsNode().SetAngularVelocity(T3Vector3(car->GetCarNode().GetLinearVelocity().Length()*0.5f,0,0));
	
	}*/

	for (unsigned int i = 0; i < pickup.size(); i++) {
		//PhysicsNode pn = pickup[i]->GetPhysicsNode();
		pickup[i]->GetPhysicsNode().SetOrientation(Quaternion::AxisAngleToQuaterion(T3Vector3(0.0f, 1.0f, 0.0f), 0.5f) * pickup[i]->GetPhysicsNode().GetOrientation());
		
		if (pickup[i]->GetUpDown()) {
			if (pickup[i]->GetPhysicsNode().GetPosition().y < 8.0f)
				pickup[i]->GetPhysicsNode().SetPosition(pickup[i]->GetPhysicsNode().GetPosition() + T3Vector3(0.0f, 0.05f, 0.0f));
			else {
				pickup[i]->FlipUpDown();
				//pn.SetPosition(pn.GetPosition + T3Vector3(0.0f, -1.0f, 0.0f));
	}
		}
		else {
			if (pickup[i]->GetPhysicsNode().GetPosition().y > 3.0f)
				pickup[i]->GetPhysicsNode().SetPosition(pickup[i]->GetPhysicsNode().GetPosition() + T3Vector3(0.0f, -0.05f, 0.0f));
			else {
				pickup[i]->FlipUpDown();
				//pn.SetPosition(pn.GetPosition + T3Vector3(0.0f, 1.0f, 0.0f));
			}
		}
	}
	
	
	//if(((abs(car->GetCarNode().GetLinearVelocity().x)+abs(car->GetCarNode().GetLinearVelocity().z))<0.5)&& f!=0)
	//	
	//{
 // f=0;
 // //cout<<"reset f"<<endl;
	//}
	if(car->GetCarNode().GetLinearVelocity()!=T3Vector3(0,0,0))
	{
		carspeediszero=false;
	}
	if(isplaystartegine==false&&car->GetCarNode().GetLinearVelocity()!=T3Vector3(0,0,0))
	{
#if WINDOWS_BUILD
		GameStateManager::Audio()->StopSound(audio->Getsoundemitter());
#endif
		isplaystartegine=true;
	}

	if(isplaylowspdegine==false&&car->GetCarNode().GetLinearVelocity().Length()<170&&car->GetCarNode().GetLinearVelocity()!=T3Vector3(0,0,0))
	{
#if WINDOWS_BUILD
		Sound* engine0=GameStateManager::Audio()->GetSound(SOUNDSDIR"lowspeedengine.wav");
		Engine0=GameStateManager::Audio()->PlaySoundA (engine0,SOUNDPRIORITY_ALWAYS,true, true);
#endif
		isplaylowspdegine=true;
	}
	if(isplaymidspdegine==false&&car->GetCarNode().GetLinearVelocity().Length()>=170)
	{
#if WINDOWS_BUILD
		GameStateManager::Audio()->StopSound(Engine0);
		Sound* engine1=GameStateManager::Audio()->GetSound(SOUNDSDIR"midspeed.wav");
		Engine1=GameStateManager::Audio()->PlaySoundA (engine1,SOUNDPRIORITY_ALWAYS,true, true);
#endif
		isplaymidspdegine=true;
	}

	if(carspeediszero==false&&moveenginesound==false&&car->GetCarNode().GetLinearVelocity()==T3Vector3(0,0,0))
	{
#if WINDOWS_BUILD
		GameStateManager::Audio()->StopSound(Engine1);
#endif
		moveenginesound=true;
	}

	/*if(car->GetVehiclePhysicsNode()->GetF()>0){
		car->GetVehiclePhysicsNode()->SetF(car->GetVehiclePhysicsNode()->GetF()-0.35);
	if(car->GetVehiclePhysicsNode()->GetF()<0)
	{
	car->GetVehiclePhysicsNode()->SetF(0);
	}
	 T3Matrix4 m4 = car->GetCarNode().GetOrientation().ToMatrix();
				   car->GetCarNode().SetLinearVelocity( m4 *T3Matrix4::Rotation(90,T3Vector3(0,1,0))*car->GetVehiclePhysicsNode()->GetF());
	}

	if(car->GetVehiclePhysicsNode()->GetF()<0){
		car->GetVehiclePhysicsNode()->SetF(car->GetVehiclePhysicsNode()->GetF()+0.35);
	if(car->GetVehiclePhysicsNode()->GetF()>0)
	{
	 car->GetVehiclePhysicsNode()->SetF(0);
	}
	 T3Matrix4 m4 = car->GetCarNode().GetOrientation().ToMatrix();
				   car->GetCarNode().SetLinearVelocity( m4 *T3Matrix4::Rotation(90,T3Vector3(0,1,0))*car->GetVehiclePhysicsNode()->GetF());
	}*/


	//T3Matrix4 m = T3Matrix4::Rotation(0.016f, T3Vector3(0,1,0));
	//ent->AddRotation(Quaternion::FromMatrix(m));
	cout << "About to check Update" << endl;
	if(update==1)
	{
		cout << "Update == 1, creating new track" << endl;
		CreateTrack();
		cout << "New Track Created" << endl;
	update=0;
	}

	if(update==2)
	{
		//DeleteTrack();

	update=0;
	}

	if((Time-60)==0){
	SetPlayTime(-1);
	Time=0;
	if(GetPlayTime()<0){
		GameOver();
		cout<<"game over"<<endl;
	}

	hud->SetScreen(GetScore(),GetPlayTime(),car->GetVehiclePhysicsNode()->GetF());


	}
	Time+=1;
}
void RacerGame::SetMinSpeed(float value){
minSpeed+=value;
	car->GetVehiclePhysicsNode()->SetMinSpeed(GetMinSpeed());	
}

void RacerGame::Start(){

	Gold_cion * gold_cion= new Gold_cion(20.0f);
	gold_cion->SetOriginPosition(T3Vector3(450.0f,0.0f,0.0f));
	gold_cion->SetRotation(Quaternion::EulerAnglesToQuaternion(0.0f,0.0f,0.0f));
	gold_cion->SetPhysics(8.0f,'p',T3Vector3(450.0f,0.0f,0.0f),Quaternion::EulerAnglesToQuaternion(0.0f,0.0f,0.0f));
	gold_cion->SetType('p');                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                      
	gold_cion->GetPhysicsNode().SetPGE(gold_cion);
	AddDrawable(gold_cion);
	pickup.push_back(gold_cion);


	//starting track
	float angle =0.0f;
//		g += 100*(sin(RadToDeg(angle)));
//		gx += 100*(2 * abs(cos(RadToDeg(angle))));
		g += 100.0f*(sin(DegToRad(angle)));
		gx += 100.0f*( 3.0f*abs(cos(DegToRad(angle))));

		T3Vector3 spn= T3Vector3(gx,0.0f,g);

		SplinePoint.push_back(spn);

		//2
//		g += 100*(sin(RadToDeg(angle)));
//		gx += 100*(2 * abs(cos(RadToDeg(angle))));
		//float angle2 = 15;
		 //angle = (rand() % 150) - 75;
		g += 100.0f*(sin(DegToRad(angle)));
		gx += 100.0f*(3* abs(cos(DegToRad(angle))));

		T3Vector3 spn2= T3Vector3(gx,0.0f,g);

		SplinePoint.push_back(spn2);

		//create new track
		
		T3Vector3 avg = (SplinePoint[1] + SplinePoint[2] + SplinePoint[3]) / 3.0f;
		T3Vector3 avg2 = (SplinePoint[2] + SplinePoint[3] + SplinePoint[4]) / 3.0f;

		TrackSegment* track = (TrackSegment*) GameStateManager::Assets()->LoadTrackSegment(SplinePoint[0],SplinePoint[1],SplinePoint[2], 5.0f, 50.0f);
		//	TrackSegment* track = new TrackSegment(SplinePoint[0],SplinePoint[1],SplinePoint[2], 5.0f, 50.0f);
	//push back track

	TrackSegmentVector.push_back(track);


		//TrackSegment* Strackn = new TrackSegment(SplinePoint[1] - avg,SplinePoint[2] - avg,SplinePoint[3] - avg,5.0f,50.f);
		TrackSegment* Strackn = (TrackSegment*) GameStateManager::Assets()->LoadTrackSegment(SplinePoint[1] - avg,SplinePoint[2] - avg,SplinePoint[3] - avg, 5.0f, 50.0f);
		TrackSegmentVector.push_back(Strackn);
		
		/*TrackSegment* Strackn = new TrackSegment(SplinePoint[1],SplinePoint[2],T3Vector3(400,0,0),5,50.f);
		TrackSegmentVector.push_back(Strackn);*/
	/*	GameStateManager::Graphics()->DropRenderContext();

		GameStateManager::Graphics()->GetRenderContext();*/
		//TrackSegment* Strackn2 = new TrackSegment(SplinePoint[2] - avg2,SplinePoint[3] - avg2,SplinePoint[4] - avg2,5.0f,50.f);
		TrackSegment* Strackn2 = (TrackSegment*) GameStateManager::Assets()->LoadTrackSegment(SplinePoint[2] - avg2,SplinePoint[3] - avg2,SplinePoint[4] - avg2, 5.0f, 50.0f);
		TrackSegmentVector.push_back(Strackn2);
		
		/*TrackSegment* Strackn = new TrackSegment(SplinePoint[1],SplinePoint[2],T3Vector3(400,0,0),5,50.f);
		TrackSegmentVector.push_back(Strackn);*/
		//GameStateManager::Graphics()->DropRenderContext();
		//create end

			Texture* grassTex = GameStateManager::Assets()->LoadTexture(this, "trackTex", 0);
		/*GameStateManager::Graphics()->GetRenderContext();*/




	/*DrawableEntity3D* ent = new DrawableEntity3D(
		track,
		NULL,
		grassTex,
		NULL,
		800.0f,
		T3Vector3(0.0f,0.0f,0.0f),
		Quaternion::EulerAnglesToQuaternion(0.0f,0.0f,0.0f),
		T3Vector3(1.0f,1.0f,1.0f));
	//gameEntities.push_back(ent);
	AddDrawable(ent);
	allEntities.push_back(ent);*/


	//test road physics
	PhysicsNode* proad = new PhysicsNode();
	GameEntity* road= new GameEntity(proad);
		road->SetMesh(track);
		road->SetShader(0);
		road->SetTexture(grassTex);
		road->SetBumpTexture(NULL);
		road->SetBoundingRadius(800.0f);
		road->SetOriginPosition(T3Vector3(0.0f,0.0f,0.0f));
		road->SetRotation(Quaternion::EulerAnglesToQuaternion(0.0f,0.0f,0.0f));
		road->SetScale(T3Vector3(1.0f,1.0f,1.0f));
		road->GetPhysicsNode().SetMass(5.0f);
		road->GetPhysicsNode().SetMesh(track);
		road->GetPhysicsNode().SetInverseMass(0.0f);
		road->GetPhysicsNode().SetUseGravity(false);
		road->GetPhysicsNode().SetXstart(road->GetPhysicsNode().GetPosition().x-200);
		road->GetPhysicsNode().SetXend(road->GetPhysicsNode().GetPosition().x+200);
		road->GetPhysicsNode().SetIsCollide(true);
	    road->GetPhysicsNode().Setcar_wheel(true);
		road->GetPhysicsNode().Setplanecollision(true);
		road->GetPhysicsNode().SetPosition(T3Vector3(0.0f,0.0f,0.0f));
		road->GetPhysicsNode().SetInverseInertia(InertialMatrixHelper::createImmovableInvInertial());
		road->ConnectToSystems();
		AddDrawable(road);
		allEntities.push_back(road);
	//test end















	//add new track to draw
	/*DrawableEntity3D* ent2 = new DrawableEntity3D(
		Strackn,
		NULL,
		grassTex,
		NULL,
		800.0f,
		//T3Vector3(0,0,0),
		avg,
		Quaternion::EulerAnglesToQuaternion(0.0f,0.0f,0.0f),
		T3Vector3(1,1,1));
	//gameEntities.push_back(ent);
	AddDrawable(ent2);
	allEntities.push_back(ent2);*/
	//add end

	//test road physics
	PhysicsNode* proad2 = new PhysicsNode();
	GameEntity* road2= new GameEntity(proad2);
		road2->SetMesh(Strackn);
		road2->SetShader(0);
		road2->SetTexture(grassTex);
		road2->SetBumpTexture(NULL);
		road2->SetBoundingRadius(800.0f);
		road2->SetOriginPosition(avg);
		road2->SetRotation(Quaternion::EulerAnglesToQuaternion(0.0f,0.0f,0.0f));
		road2->SetScale(T3Vector3(1.0f,1.0f,1.0f));
		road2->GetPhysicsNode().SetMass(5.0f);
		road2->GetPhysicsNode().SetMesh(Strackn);
		road2->GetPhysicsNode().SetInverseMass(0.0f);
		road2->GetPhysicsNode().SetUseGravity(false);
		road2->GetPhysicsNode().SetXstart(road2->GetPhysicsNode().GetPosition().x-200);
		road2->GetPhysicsNode().SetXend(road2->GetPhysicsNode().GetPosition().x+200);
		road2->GetPhysicsNode().SetIsCollide(true);
	    road2->GetPhysicsNode().Setcar_wheel(true);
		road2->GetPhysicsNode().Setplanecollision(true);
		road2->GetPhysicsNode().SetPosition(avg);
		road2->GetPhysicsNode().SetInverseInertia(InertialMatrixHelper::createImmovableInvInertial());
		road2->ConnectToSystems();
		AddDrawable(road2);
		allEntities.push_back(road2);
	//test end


	//add3
	/*DrawableEntity3D* ent3 = new DrawableEntity3D(
		Strackn2,
		NULL,
		grassTex,
		NULL,
		800.0f,
		//T3Vector3(0,0,0),
		avg2,
		Quaternion::EulerAnglesToQuaternion(0.0f,0.0f,0.0f),
		T3Vector3(1,1,1));
	//gameEntities.push_back(ent);
	AddDrawable(ent3);
	allEntities.push_back(ent3);*/
	//add 3 end
	//test road physics
	PhysicsNode* proad3 = new PhysicsNode();
	GameEntity* road3= new GameEntity(proad3);
		road3->SetMesh(Strackn2);
		road3->SetShader(0);
		road3->SetTexture(grassTex);
		road3->SetBumpTexture(NULL);
		road3->SetBoundingRadius(800.0f);
		road3->SetOriginPosition(avg2);
		road3->SetRotation(Quaternion::EulerAnglesToQuaternion(0.0f,0.0f,0.0f));
		road3->SetScale(T3Vector3(1.0f,1.0f,1.0f));
		road3->GetPhysicsNode().SetMass(5.0f);
		road3->GetPhysicsNode().SetMesh(Strackn2);
		road3->GetPhysicsNode().SetInverseMass(0.0f);
		road3->GetPhysicsNode().SetUseGravity(false);
		road3->GetPhysicsNode().SetXstart(road3->GetPhysicsNode().GetPosition().x-200);
		road3->GetPhysicsNode().SetXend(road3->GetPhysicsNode().GetPosition().x+200);
		road3->GetPhysicsNode().SetIsCollide(true);
	    road3->GetPhysicsNode().Setcar_wheel(true);
		road3->GetPhysicsNode().Setplanecollision(true);
		road3->GetPhysicsNode().SetPosition(avg2);
		road3->GetPhysicsNode().SetInverseInertia(InertialMatrixHelper::createImmovableInvInertial());
		road3->ConnectToSystems();
		AddDrawable(road3);
		allEntities.push_back(road3);
	//test end




	T3Vector3 L =  Strackn2->GetTrackCentreLeft();
	T3Vector3 R = Strackn2->GetTrackCentreRight();

	T3Vector3 rl = R-L;
	T3Vector3 lr = L-R;

	T3Vector3 F = T3Vector3(0.0f,0.0f,1.0f); 
	F.Normalise();

	rl.Normalise();

	float cosc= T3Vector3::Dot(rl,F); 
	//float Degc= 
	double degc=acos(cosc);

	float anglec=RadToDeg(degc);


	if(L.x<R.x)
	{
	anglec=-anglec;
	}




	/*checkpoint= new CheckPoint(10);
	checkpoint->SetPhysics(10,'c');
	checkpoint->SetType('g');
	checkpoint->GetPhysicsNode().SetPGE(checkpoint);
	AddDrawable(checkpoint);*/

	CheckPoint* checkpoint3= new CheckPoint(10.0f);
	//checkpoint2->SetPhysics(10,'c');
	checkpoint3->SetOriginPosition(SplinePoint[1]+T3Vector3(0,1,0));
	checkpoint3->SetRotation(Quaternion::EulerAnglesToQuaternion(0.0f,0.0f,0.0f));

	checkpoint3->SetPhysics(10.0f,'c',SplinePoint[1],Quaternion::EulerAnglesToQuaternion(0.0f,0.0f,0.0f));
	checkpoint3->SetType('g');                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                      
	checkpoint3->GetPhysicsNode().SetPGE(checkpoint3);
	//T3Vector4* color= new T3Vector4(0.5f,0.5f,0.5f,0.2f);
	//checkpoint3->GetMesh()->SetColour(color);
	//checkpoint2->GetPhysicsNode().SetPGE(checkpoint2);
	//AddDrawable(checkpoint3);
	checkPoint.push_back(checkpoint3);

	CheckPoint* checkpoint2= new CheckPoint(10.0f);
	//checkpoint2->SetPhysics(10,'c');
	checkpoint2->SetOriginPosition(SplinePoint[3]+T3Vector3(0,1,0));
	checkpoint2->SetRotation(Quaternion::EulerAnglesToQuaternion(0.0f,-(anglec),0.0f));

	checkpoint2->SetPhysics(10.0f,'c',SplinePoint[3],Quaternion::EulerAnglesToQuaternion(0.0f,-(anglec),0.0f));
	checkpoint2->SetType('d');                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                      
	checkpoint2->GetPhysicsNode().SetPGE(checkpoint2);
	//checkpoint2->GetPhysicsNode().SetPGE(checkpoint2);
	//AddDrawable(checkpoint2);
	checkPoint.push_back(checkpoint2);
	//starting track
}

void RacerGame::DeleteTrack(){
	    /*allEntities[0]->DisconnectFromSystems();
		
		allEntities[1]->DisconnectFromSystems();
		
		allEntities[2]->DisconnectFromSystems();

		allEntities[3]->DisconnectFromSystems();
		
		allEntities.erase(allEntities.begin());
		allEntities.erase(allEntities.begin());
		allEntities.erase(allEntities.begin());
		allEntities.erase(allEntities.begin());*/
	if(pickup.size()>1){
		//cout<<"\n pick up delete ="<<pickup.size()<<" delete  \n";
	GameStateManager::Graphics()->RemoveDrawable(pickup[0]);
	pickup[0]->DisconnectFromSystems();
	pickup.erase(pickup.begin());
	}

	GameStateManager::Graphics()->RemoveDrawable(allEntities[0]);
	allEntities[0]->DisconnectFromSystems();

	GameStateManager::Graphics()->RemoveDrawable(allEntities[1]);
	allEntities[1]->DisconnectFromSystems();


	allEntities.erase(allEntities.begin());
	allEntities.erase(allEntities.begin());

	/*RemoveDrawable(allEntities[0]);
	RemoveDrawable(allEntities[1]);
	allEntities.erase(allEntities.begin());
	allEntities.erase(allEntities.begin());*/
	
	//GameStateManager::Graphics()->RemoveDrawable(checkPoint[1]);
	//checkPoint[1]->DisconnectFromSystems();
	//PhysicsNode*  n =checkPoint[0]->GetPhysicsNode();
	//GameStateManager::Physics()->RemoveNode(n);
	//checkPoint[0]->DisconnectFromSystems();
	//checkPoint[1]->DisconnectFromSystems();
	//RemoveDrawable(checkPoint[0]);
	//RemoveDrawable(checkPoint[1]);
	//RemoveDrawable(allEntities[0]);

	//delete checkPoint[0];
	
	//delete checkPoint[0];
	//checkPoint.erase(checkPoint.begin());

	//RemoveDrawable(allEntities[2]);
	
	delete TrackSegmentVector[0];
	TrackSegmentVector.erase(TrackSegmentVector.begin());
	delete TrackSegmentVector[0];
	TrackSegmentVector.erase(TrackSegmentVector.begin());
	SplinePoint.erase(SplinePoint.begin());
	SplinePoint.erase(SplinePoint.begin());
}

void RacerGame::CreateTrack(){
//create a new control point
		float angle = (rand() % 120) - 60.0f;
		//float angle = 0;
//		g += 100*(sin(RadToDeg(angle)));
//		gx += 100*(2 * abs(cos(RadToDeg(angle))));
		g += 100.0f*(sin(DegToRad(angle)));
		gx += 100.0f*( 3.0f*abs(cos(DegToRad(angle))));

		T3Vector3 spn= T3Vector3(gx,0.0f,g);

		SplinePoint.push_back(spn);

		//2
//		g += 100*(sin(RadToDeg(angle)));
//		gx += 100*(2 * abs(cos(RadToDeg(angle))));
		//float angle2 = 15;
		 //angle = (rand() % 150) - 75;
		g += 100.0f*(sin(DegToRad(angle)));
		gx += 100.0f*(3.0f* abs(cos(DegToRad(angle))));

		T3Vector3 spn2= T3Vector3(gx,0.0f,g);

		SplinePoint.push_back(spn2);

		//create new track
		
		T3Vector3 avg = (SplinePoint[3] + SplinePoint[4] + SplinePoint[5]) / 3.0f;
		T3Vector3 avg2 = (SplinePoint[4] + SplinePoint[5] + SplinePoint[6]) / 3.0f;


		//TrackSegment* Strackn = new TrackSegment(SplinePoint[3] - avg,SplinePoint[4] - avg,SplinePoint[5] - avg,5.0f,50.f);
		TrackSegment* Strackn = (TrackSegment*) GameStateManager::Assets()->LoadTrackSegment(SplinePoint[3] - avg,SplinePoint[4] - avg,SplinePoint[5] - avg, 5.0f, 50.0f);
		TrackSegmentVector.push_back(Strackn);
		
		/*TrackSegment* Strackn = new TrackSegment(SplinePoint[1],SplinePoint[2],T3Vector3(400,0,0),5,50.f);
		TrackSegmentVector.push_back(Strackn);*/
	/*	GameStateManager::Graphics()->DropRenderContext();

		GameStateManager::Graphics()->GetRenderContext();*/
		//TrackSegment* Strackn2 = new TrackSegment(SplinePoint[4] - avg2,SplinePoint[5] - avg2,SplinePoint[6] - avg2,5.0f,50.f);
		TrackSegment* Strackn2 = (TrackSegment*) GameStateManager::Assets()->LoadTrackSegment(SplinePoint[4] - avg2,SplinePoint[5] - avg2,SplinePoint[6] - avg2, 5.0f, 50.0f);
		TrackSegmentVector.push_back(Strackn2);
		
		/*TrackSegment* Strackn = new TrackSegment(SplinePoint[1],SplinePoint[2],T3Vector3(400,0,0),5,50.f);
		TrackSegmentVector.push_back(Strackn);*/
		//GameStateManager::Graphics()->DropRenderContext();
		//create end

			Texture* grassTex = GameStateManager::Assets()->LoadTexture(this, "trackTex", 0);
		/*GameStateManager::Graphics()->GetRenderContext();*/
	




	//test road physics
	PhysicsNode* proad2 = new PhysicsNode();
	GameEntity* road2= new GameEntity(proad2);
		road2->SetMesh(Strackn);
		road2->SetShader(0);
		road2->SetTexture(grassTex);
		road2->SetBumpTexture(NULL);
		road2->SetBoundingRadius(800.0f);
		road2->SetOriginPosition(avg);
		road2->SetRotation(Quaternion::EulerAnglesToQuaternion(0.0f,0.0f,0.0f));
		road2->SetScale(T3Vector3(1.0f,1.0f,1.0f));
		road2->GetPhysicsNode().SetMass(5.0f);
		road2->GetPhysicsNode().SetMesh(Strackn);
		road2->GetPhysicsNode().SetInverseMass(0.0f);
		road2->GetPhysicsNode().SetUseGravity(false);
		road2->GetPhysicsNode().SetXstart(road2->GetPhysicsNode().GetPosition().x-200);
		road2->GetPhysicsNode().SetXend(road2->GetPhysicsNode().GetPosition().x+200);
		road2->GetPhysicsNode().SetIsCollide(true);
	    road2->GetPhysicsNode().Setcar_wheel(true);
		road2->GetPhysicsNode().Setplanecollision(true);
		road2->GetPhysicsNode().SetPosition(avg);
		road2->GetPhysicsNode().SetInverseInertia(InertialMatrixHelper::createImmovableInvInertial());
		road2->ConnectToSystems();
		AddDrawable(road2);
		allEntities.push_back(road2);
	//test end
	

	//add3
	/*DrawableEntity3D* ent3 = new DrawableEntity3D(
		Strackn2,
		NULL,
		grassTex,
		NULL,
		800.0f,
		//T3Vector3(0,0,0),
		avg2,
		Quaternion::EulerAnglesToQuaternion(0.0f,0.0f,0.0f),
		T3Vector3(1,1,1));
	//gameEntities.push_back(ent);
	AddDrawable(ent3);
	allEntities.push_back(ent3);*/
	//add 3 end
	//test road physics
	PhysicsNode* proad3 = new PhysicsNode();
	GameEntity* road3= new GameEntity(proad3);
		road3->SetMesh(Strackn2);
		road3->SetShader(0);
		road3->SetTexture(grassTex);
		road3->SetBumpTexture(NULL);
		road3->SetBoundingRadius(800.0f);
		road3->SetOriginPosition(avg2);
		road3->SetRotation(Quaternion::EulerAnglesToQuaternion(0.0f,0.0f,0.0f));
		road3->SetScale(T3Vector3(1.0f,1.0f,1.0f));
		road3->GetPhysicsNode().SetMass(5.0f);
		road3->GetPhysicsNode().SetMesh(Strackn2);
		road3->GetPhysicsNode().SetInverseMass(0.0f);
		road3->GetPhysicsNode().SetUseGravity(false);
		road3->GetPhysicsNode().SetXstart(road3->GetPhysicsNode().GetPosition().x-200);
		road3->GetPhysicsNode().SetXend(road3->GetPhysicsNode().GetPosition().x+200);
		road3->GetPhysicsNode().SetIsCollide(true);
	    road3->GetPhysicsNode().Setcar_wheel(true);
		road3->GetPhysicsNode().Setplanecollision(true);
		road3->GetPhysicsNode().SetPosition(avg2);
		road3->GetPhysicsNode().SetInverseInertia(InertialMatrixHelper::createImmovableInvInertial());
		road3->ConnectToSystems();
		AddDrawable(road3);
		allEntities.push_back(road3);
	//test end

	
	/*
	//add new track to draw
	DrawableEntity3D* ent2 = new DrawableEntity3D(
		Strackn,
		NULL,
		grassTex,
		NULL,
		800.0f,
		//T3Vector3(0,0,0),
		avg,
		Quaternion::EulerAnglesToQuaternion(0.0f,0.0f,0.0f),
		T3Vector3(1.0f,1.0f,1.0f));
	//gameEntities.push_back(ent);
	AddDrawable(ent2);
	allEntities.push_back(ent2);
	//add end

	//add3
	DrawableEntity3D* ent3 = new DrawableEntity3D(
		Strackn2,
		NULL,
		grassTex,
		NULL,
		800.0f,
		//T3Vector3(0,0,0),
		avg2,
		Quaternion::EulerAnglesToQuaternion(0.0f,0.0f,0.0f),
		T3Vector3(1.0f,1.0f,1.0f));
	//gameEntities.push_back(ent);
	AddDrawable(ent3);
	allEntities.push_back(ent3);
	//add 3 end*/


	T3Vector3 L =  Strackn2->GetTrackCentreLeft();
	T3Vector3 R = Strackn2->GetTrackCentreRight();

	T3Vector3 rl = R-L;
	T3Vector3 lr = L-R;

	T3Vector3 F = T3Vector3(0.0f,0.0f,1.0f); 
	F.Normalise();

	rl.Normalise();

	float cosc= T3Vector3::Dot(rl,F); 
	//float Degc= 
	double degc=acos(cosc);

	float anglec=RadToDeg(degc);


	if(L.x<R.x)
	{
	anglec=-anglec;
	}



	//angle2
	int move=22;
	float angleCreate=GetCreateAngle();
	
	if(angleCreate<0){
	move=-22;
	}
	//angle2


	
	CheckPoint* checkpoint3= new CheckPoint(10.0f);
	//checkpoint2->SetPhysics(10,'c');
	checkpoint3->SetOriginPosition(SplinePoint[3]+T3Vector3(33.0f,1.0f,move));
	checkpoint3->SetRotation(Quaternion::EulerAnglesToQuaternion(0.0f,-(angleCreate),0.0f));

	checkpoint3->SetPhysics(10.0f,'c',SplinePoint[3]+T3Vector3(33.0f,1.0f,move),Quaternion::EulerAnglesToQuaternion(0.0f,-(angleCreate),0.0f));
	checkpoint3->SetType('g');                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                      
	checkpoint3->GetPhysicsNode().SetPGE(checkpoint3);
	//checkpoint2->GetPhysicsNode().SetPGE(checkpoint2);
	//AddDrawable(checkpoint3);
	checkPoint.push_back(checkpoint3);

	/*checkpoint= new CheckPoint(10);
	checkpoint->SetPhysics(10,'c');
	checkpoint->SetType('g');
	checkpoint->GetPhysicsNode().SetPGE(checkpoint);
	AddDrawable(checkpoint);*/

	CheckPoint* checkpoint2= new CheckPoint(10.0f);
	//checkpoint2->SetPhysics(10,'c');
	checkpoint2->SetOriginPosition(SplinePoint[5]+T3Vector3(0,1,0));
	checkpoint2->SetRotation(Quaternion::EulerAnglesToQuaternion(0.0f,-(anglec),0.0f));

	checkpoint2->SetPhysics(10.0f,'c',SplinePoint[5]+T3Vector3(0,1,0),Quaternion::EulerAnglesToQuaternion(0.0f,-(anglec),0.0f));
	checkpoint2->SetType('d');                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                      
	checkpoint2->GetPhysicsNode().SetPGE(checkpoint2);
	//checkpoint2->GetPhysicsNode().SetPGE(checkpoint2);
	//AddDrawable(checkpoint2);
	checkPoint.push_back(checkpoint2);

	if(GettimeOrScore()!=4){
	Gold_cion * gold_cion= new Gold_cion(20.0f);

	gold_cion->SetOriginPosition(SplinePoint[4]+T3Vector3((rand() % 10)-5.0f,0.0f,(rand() % 10)-5.0f));
	//gold_cion->SetOriginPosition(SplinePoint[4]+T3Vector3(0,3,0));
	gold_cion->SetRotation(Quaternion::EulerAnglesToQuaternion(0.0f,0.0f,0.0f));

	gold_cion->SetPhysics(8.0f,'p',SplinePoint[4]+T3Vector3((rand() % 10)-5.0f,0.0f,(rand() % 10)-5.0f),Quaternion::EulerAnglesToQuaternion(0.0f,0.0f,0.0f));
	
	gold_cion->SetType('p');                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                      
	gold_cion->GetPhysicsNode().SetPGE(gold_cion);

	AddDrawable(gold_cion);
	pickup.push_back(gold_cion);
	}
	if(GettimeOrScore()==4){
	Gold_cion * gold_cion= new Gold_cion(20.0f);

	gold_cion->SetOriginPosition(SplinePoint[4]+T3Vector3((rand() % 10)-5.0f,0.0f,(rand() % 10)-5.0f));
	gold_cion->SetRotation(Quaternion::EulerAnglesToQuaternion(0.0f,0.0f,0.0f));

	gold_cion->SetPhysics(8.0f,'p',SplinePoint[4]+T3Vector3((rand() % 10)-5.0f,0.0f,(rand() % 10)-5.0f),Quaternion::EulerAnglesToQuaternion(0.0f,0.0f,0.0f));
	
	gold_cion->SetType('t');                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                      
	gold_cion->GetPhysicsNode().SetPGE(gold_cion);

	AddDrawable(gold_cion);
	 SettimeOrScore(-(GettimeOrScore()));

	 pickup.push_back(gold_cion);
	}

}

float RacerGame::GetCreateAngle(){
    T3Vector3 L =  TrackSegmentVector[2]->GetTrackCentreLeft();
	T3Vector3 R =  TrackSegmentVector[2]->GetTrackCentreRight();

	T3Vector3 rl = R-L;
	T3Vector3 lr = L-R;

	T3Vector3 F = T3Vector3(0.0f,0.0f,1.0f); 
	F.Normalise();

	rl.Normalise();

	float cosc= T3Vector3::Dot(rl,F); 
	//float Degc= 
	double degc=acos(cosc);

	float anglec2=RadToDeg(degc);


	if(L.x<R.x)
	{
	anglec2=-anglec2;
	}

	return anglec2;
}


#if WINDOWS_BUILD
void RacerGame::KeyboardEvent(KeyboardEvents::EventType type, KeyboardEvents::Key key) {

	float R=0.707106829f;
	switch (type) {
	case KeyboardEvents::KEY_DOWN:
	case KeyboardEvents::KEY_HELD:
		switch (key) {


case KeyboardEvents::KEYBOARD_7:
		{
				//camera->AddMovement(T3Vector3(1,0,0));
			GameStateManager::Audio()->PlaySoundA(GameStateManager::Audio()->GetSound (SOUNDSDIR"bgm2_42sec.wav"),SOUNDPRIORTY_LOW,false);

			      // f=f+1.8;
			car->GetVehiclePhysicsNode()->SetF(car->GetVehiclePhysicsNode()->GetF()+1.8);
			if(car->GetVehiclePhysicsNode()->GetF()>GetMaxSpeed()) {
				car->GetVehiclePhysicsNode()->SetF(GetMaxSpeed());
		       }
				   T3Matrix4 m4 = car->GetCarNode().GetOrientation().ToMatrix();
				   car->GetCarNode().SetLinearVelocity( m4 *T3Matrix4::Rotation(90,T3Vector3(0,1,0))*car->GetVehiclePhysicsNode()->GetF());
			 break;
		}
		case KeyboardEvents::KEYBOARD_8:
			{//camera->AddMovement(T3Vector3(0,0,1));
				// f=f-3.0;
				car->GetVehiclePhysicsNode()->SetF(car->GetVehiclePhysicsNode()->GetF()-3.0);
				if(car->GetVehiclePhysicsNode()->GetF() < (GetMinSpeed())) {
					car->GetVehiclePhysicsNode()->SetF(GetMinSpeed());
		}
           T3Matrix4 m4 = car->GetCarNode().GetOrientation().ToMatrix();
				   car->GetCarNode().SetLinearVelocity( m4 *T3Matrix4::Rotation(90,T3Vector3(0,1,0))*car->GetVehiclePhysicsNode()->GetF());
			}
			break;
		case KeyboardEvents::KEYBOARD_A:
			{//camera->AddMovement(T3Vector3(1,0,0));


				if(car->GetCarNode().GetLinearVelocity().x>=0){
			  car->GetCarNode().SetAngularVelocity(T3Vector3(0,Speed_Rotate,0));
				car->GetFrontLeftTire()->GetPhysicsNode().SetOrientation(car->GetCarNode().GetOrientation());
				car->GetFrontRightTire()->GetPhysicsNode().SetOrientation(car->GetCarNode().GetOrientation());
				car->GetBackLeftTire()->GetPhysicsNode().SetOrientation(car->GetCarNode().GetOrientation());
				car->GetBackRightTire()->GetPhysicsNode().SetOrientation(car->GetCarNode().GetOrientation());
				}
				if(car->GetCarNode().GetLinearVelocity().x<0){
			  car->GetCarNode().SetAngularVelocity(T3Vector3(0,-Speed_Rotate,0));
					 car->GetFrontLeftTire()->GetPhysicsNode().SetOrientation(car->GetCarNode().GetOrientation());
					 car->GetFrontRightTire()->GetPhysicsNode().SetOrientation(car->GetCarNode().GetOrientation());
					 car->GetBackLeftTire()->GetPhysicsNode().SetOrientation(car->GetCarNode().GetOrientation());
				     car->GetBackRightTire()->GetPhysicsNode().SetOrientation(car->GetCarNode().GetOrientation());
				}

				 T3Matrix4 m4 = car->GetCarNode().GetOrientation().ToMatrix();
				   car->GetCarNode().SetLinearVelocity( m4 *T3Matrix4::Rotation(90,T3Vector3(0,1,0))*car->GetVehiclePhysicsNode()->GetF());


	//	}		
		
			
			}
			break;
		case KeyboardEvents::KEYBOARD_D:
			{//camera->AddMovement(T3Vector3(-1,0,0));
			
		if(car->GetCarNode().GetLinearVelocity().x>=0){
			 car->GetFrontLeftTire()->GetPhysicsNode().SetOrientation(car->GetCarNode().GetOrientation());
		     car->GetFrontRightTire()->GetPhysicsNode().SetOrientation(car->GetCarNode().GetOrientation());
			 car->GetBackLeftTire()->GetPhysicsNode().SetOrientation(car->GetCarNode().GetOrientation());
		     car->GetBackRightTire()->GetPhysicsNode().SetOrientation(car->GetCarNode().GetOrientation());
			  car->GetCarNode().SetAngularVelocity(T3Vector3(0,-Speed_Rotate,0));
		      }
				if(car->GetCarNode().GetLinearVelocity().x<0){
		    car->GetFrontLeftTire()->GetPhysicsNode().SetOrientation(car->GetCarNode().GetOrientation());
		    car->GetFrontRightTire()->GetPhysicsNode().SetOrientation(car->GetCarNode().GetOrientation());
			car->GetBackLeftTire()->GetPhysicsNode().SetOrientation(car->GetCarNode().GetOrientation());
		    car->GetBackRightTire()->GetPhysicsNode().SetOrientation(car->GetCarNode().GetOrientation());
			  car->GetCarNode().SetAngularVelocity(T3Vector3(0,Speed_Rotate,0));
		}			

	 T3Matrix4 m4 = car->GetCarNode().GetOrientation().ToMatrix();
				   car->GetCarNode().SetLinearVelocity( m4 *T3Matrix4::Rotation(90,T3Vector3(0,1,0))*car->GetVehiclePhysicsNode()->GetF());
	


			}
			break;
		case KeyboardEvents::KEYBOARD_SHIFT:
			camera->AddMovement(T3Vector3(0,1,0));
			break;
		case KeyboardEvents::KEYBOARD_SPACE:
			{camera->AddMovement(T3Vector3(0,-1,0));
			 temp1 =  car->GetCarNode().GetLinearVelocity();
			  car->GetCarNode().SetUseGravity(TRUE);
			 temp1.y = 0.4;
			  car->GetCarNode().SetLinearVelocity(temp1);	
			
			}
			break;
			//sam 
			case KeyboardEvents::KEYBOARD_I:
			camera->AddMovement(T3Vector3(0,0,-10));
			break;
		case KeyboardEvents::KEYBOARD_K:
			camera->AddMovement(T3Vector3(0,0,10));
			break;
		case KeyboardEvents::KEYBOARD_J:
			camera->AddMovement(T3Vector3(-10,0,0));
			break;
		case KeyboardEvents::KEYBOARD_L:
			camera->AddMovement(T3Vector3(10,0,0));
			break;
		case KeyboardEvents::KEYBOARD_U:
			camera->AddMovement(T3Vector3(0,10,0));
			break;
		case KeyboardEvents::KEYBOARD_O:
			camera->AddMovement(T3Vector3(0,-10,0));
			break;
		case KeyboardEvents::KEYBOARD_LEFT:
			camera->AddYaw(1);
			break;
		case KeyboardEvents::KEYBOARD_RIGHT:
			camera->AddYaw(-1);
			break;
		case KeyboardEvents::KEYBOARD_UP:
			camera->AddPitch(1);
			break;
		case KeyboardEvents::KEYBOARD_DOWN:
			camera->AddPitch(-1);
			break;
		case KeyboardEvents::KEYBOARD_Q:
			camera->AddRoll(-1);
			break;
		case KeyboardEvents::KEYBOARD_E:
			camera->AddRoll(1);
			break;
			//sam

		}
		
		case KeyboardEvents::KEY_PRESS:
			switch (key) {
			case KeyboardEvents::KEYBOARD_ESCAPE:
				GameStateManager::Pause();
				break;

			}
		
	}

}
#endif
#if WINDOWS_BUILD
void RacerGame::MouseMoved(T3Vector2& star, T3Vector2& finish) {

	T3Vector2 amount = finish - star;
	
	camera->AddPitch(10*(-amount.y));
	camera->AddYaw(10*(amount.x));

}
#endif

void RacerGame::GamepadEvent(GamepadEvents::PlayerIndex playerID, GamepadEvents::EventType type, GamepadEvents::Button button)
{
	//cout << "RacerGame: Button Pressed" << endl;
	if(type == GamepadEvents::BUTTON_PRESS && button == GamepadEvents::INPUT_DPAD_UP)
	{

		car->GetVehiclePhysicsNode()->SetF(car->GetVehiclePhysicsNode()->GetF()+1.8);
		if(car->GetVehiclePhysicsNode()->GetF()>GetMaxSpeed())
		{
			car->GetVehiclePhysicsNode()->SetF(GetMaxSpeed());
		}
		T3Matrix4 m4 = car->GetCarNode().GetOrientation().ToMatrix();
		car->GetCarNode().SetLinearVelocity( m4 *T3Matrix4::Rotation(90,T3Vector3(0,1,0))*car->GetVehiclePhysicsNode()->GetF());
			
		//cout<<"Current Velocity: " << car->GetCarNode().GetLinearVelocity() << endl << endl << endl;
	}
	if(type == GamepadEvents::BUTTON_DOWN && button == GamepadEvents::INPUT_DPAD_UP)
	{
		car->GetVehiclePhysicsNode()->SetF(car->GetVehiclePhysicsNode()->GetF()+1.8);
		if(car->GetVehiclePhysicsNode()->GetF()>GetMaxSpeed())
		{
			car->GetVehiclePhysicsNode()->SetF(GetMaxSpeed());
		}
		T3Matrix4 m4 = car->GetCarNode().GetOrientation().ToMatrix();
		car->GetCarNode().SetLinearVelocity( m4 *T3Matrix4::Rotation(90,T3Vector3(0,1,0))*car->GetVehiclePhysicsNode()->GetF());
	
		//cout<<"Current Velocity: " << car->GetCarNode().GetLinearVelocity() << endl << endl << endl;
	}
};
	
void RacerGame::GamepadAnalogueDisplacement(GamepadEvents::PlayerIndex playerID, GamepadEvents::AnalogueControl analogueControl, T3Vector2& amount)
{
	//cout << "RacerGame: analogue moved" << endl;
	if(analogueControl == GamepadEvents::LEFT_STICK)
	{
		car->GetVehiclePhysicsNode()->SetF(car->GetVehiclePhysicsNode()->GetF()+1.8);
		if(car->GetVehiclePhysicsNode()->GetF()>GetMaxSpeed())
		{
			car->GetVehiclePhysicsNode()->SetF(GetMaxSpeed());
		}
		T3Matrix4 m4 = car->GetCarNode().GetOrientation().ToMatrix();
		car->GetCarNode().SetLinearVelocity( m4 *T3Matrix4::Rotation(90,T3Vector3(0,1,0))*car->GetVehiclePhysicsNode()->GetF());
			
	//	cout<<"Current Velocity: " << car->GetCarNode().GetLinearVelocity() << endl << endl << endl;
			 
	}
	if(analogueControl == GamepadEvents::RIGHT_STICK)
	{
		car->GetVehiclePhysicsNode()->SetF(car->GetVehiclePhysicsNode()->GetF()+1.8);
		if(car->GetVehiclePhysicsNode()->GetF()>GetMaxSpeed())
		{
			car->GetVehiclePhysicsNode()->SetF(GetMaxSpeed());
		}
		T3Matrix4 m4 = car->GetCarNode().GetOrientation().ToMatrix();
		car->GetCarNode().SetLinearVelocity( m4 *T3Matrix4::Rotation(90,T3Vector3(0,1,0))*car->GetVehiclePhysicsNode()->GetF());
	
		//cout<<"Current Velocity: " << car->GetCarNode().GetLinearVelocity() << endl << endl << endl;
		
	}
	
};

void RacerGame::Pause() {
	camera->Pause();
	GameStateManager::Physics()->Pause();
	inputEnabled = false;
	GameStateManager::AddGameScreen(new PauseScreen(this, hud));
	
}

void RacerGame::Resume() {
	camera->Resume();
	GameStateManager::Physics()->Resume();
	inputEnabled = true;
}

void RacerGame::UnloadContent() 
{
#if WINDOWS_BUILD
	GameStateManager::Audio()->SetListener(NULL);
#endif
}
