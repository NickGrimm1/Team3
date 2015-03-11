


#include "RacerGame.h"
#include "Mesh.h"
#include "../Framework/Light.h"
#include "GameStateManager.h"
#include "FreeCamera.h"
#include "ChaseCamera.h"
#include "../Framework/ObjMesh.h"
#include "TrackSegment.h"
#include "../Framework/InertialMatrixHelper.h"
//#include "../Framework/Vehicle.h"
#include <math.h>

#include <time.h>
//TODO - remove
#include <iostream>
#include "../Framework/BumpTexture.h"

//#include "DrawableText2D.h"
//#include "GameScreen2D.h"
#include "HudTestScreen.h"

int RacerGame::update =0;
float RacerGame::g=0.0f;
float RacerGame::gx =300.0f;

RacerGame::RacerGame(void)
{
#if WINDOWS_BUILD
	//f=5;
	srand(time(NULL));
	T3Vector3 sp1= T3Vector3(100,0,0);
	T3Vector3 sp2= T3Vector3(200,0,0);
	T3Vector3 sp3= T3Vector3(300,0,0);

	SplinePoint.push_back(sp1);
	SplinePoint.push_back(sp2);
	SplinePoint.push_back(sp3);
	score=0;
	Time=60;
	PlayTime=30;
	timeOrScore=0;

		//hud = new HudTestScreen();
		//GameStateManager::Instance()->AddGameScreen(hud);

		 scoreTexture = GameStateManager::Assets()->LoadTexture(this, TEXTUREDIR"score.jpg", 0);
		 timeTexture = GameStateManager::Assets()->LoadTexture(this, TEXTUREDIR"time.jpg", 0);

#endif
	//Strack = new TrackSegment(SplinePoint[0],SplinePoint[1],SplinePoint[2],5,50.f);
	//TrackSegmentVector.push_back(Strack);
}

RacerGame::~RacerGame(void)
{
#if WINDOWS_BUILD
	delete quad;

	delete light;
#endif
}

void RacerGame::LoadContent() {
#if WINDOWS_BUILD	
	//Mesh* coneMesh = Mesh::GenerateCone(20);
	quad = GameStateManager::Assets()->LoadCylinder(this, 20);
	cylinder = GameStateManager::Assets()->LoadCylinder(this, 20);
	//cout << "Quad Obj = " << quad->GetVertexBuffer() << endl;
	
	ent = new DrawableEntity3D(
		quad, 
		NULL,
		GameStateManager::Assets()->LoadTexture(this, "Grass_Color", 0), 
		GameStateManager::Assets()->LoadTexture(this, "snowflake", 0),
		50.0f, 
		T3Vector3(0,0,0), 
		Quaternion::FromMatrix(T3Matrix4::Rotation(90.0f, T3Vector3(1,0,0))),
		T3Vector3(50,50,1));
	



	
	//cout << "Cone Mesh Obj = " << coneMesh->GetVertexBuffer() << endl;
	//SpotLight::SetConeMesh(coneMesh);
	//Mesh* circleMesh = Mesh::GenerateCircle(20);
	//cout << "Circle Mesh Obj = " << circleMesh->GetVertexBuffer() << endl;
	//SpotLight::SetCircleMesh(Mesh::GenerateCircle(20));
	//Mesh* sphereMesh = new OBJMesh(MESHDIR"sphere.obj");
	//PointLight::SetMesh(sphereMesh);

	
	//light = GameStateManager::Graphics()->AddSpotLight(T3Vector3(0, 0, 0), T3Vector3(0,1,0), T3Vector3(1,0,0), 1.0f, 45.0f, T3Vector4(1,1,1,1), T3Vector4(1,1,1,1), false);
	
	//PointLight* l = GameStateManager::Graphics()->AddPointLight(T3Vector3(0,5,0), 10, T3Vector4(1,1,1,1), T3Vector4(1,1,1,1), false); 


	camera = new FreeCamera();

	/*checkpoint= new CheckPoint(10);
	checkpoint->SetPhysics(10,'c');
	checkpoint->SetType('g');
	checkpoint->GetPhysicsNode().SetPGE(checkpoint);
	AddDrawable(checkpoint);*/
	
	
	
	
	
	

	
	//add road
	
	Texture* grassTex2 = GameStateManager::Assets()->LoadTexture(this, TEXTUREDIR"trackTex.png", 0);
	Shader* sh = GameStateManager::Assets()->LoadShader(this, SHADERDIR"TexturedVertex.glsl", SHADERDIR"TexturedFragment.glsl");
	GameStateManager::Graphics()->GetRenderContext();
	TrackSegment* trackr = new TrackSegment(SplinePoint[0],SplinePoint[1],SplinePoint[2], 5, 50.0);
	
	//push back track
	//TrackSegmentVector.push_back(trackr);
	GameStateManager::Graphics()->DropRenderContext();
	PhysicsNode* proad = new PhysicsNode();
	GameEntity* road= new GameEntity(proad);
		road->SetMesh(trackr);
		road->SetShader(sh);
		road->SetTexture(grassTex2);
		road->SetBumpTexture(NULL);
		road->SetBoundingRadius(800.0f);
		road->SetOriginPosition(T3Vector3(-200,-20,0));
		road->SetRotation(Quaternion::EulerAnglesToQuaternion(0,0,0));
		road->SetScale(T3Vector3(1,1,1));
		road->GetPhysicsNode().SetMass(5);
		road->GetPhysicsNode().SetMesh(trackr);
		road->GetPhysicsNode().SetInverseMass(0);
		road->GetPhysicsNode().SetUseGravity(false);
		road->GetPhysicsNode().SetIsCollide(true);
	    road->GetPhysicsNode().Setcar_wheel(true);
		road->GetPhysicsNode().SetPosition(T3Vector3(-200,-20,0));
		road->GetPhysicsNode().SetInverseInertia(InertialMatrixHelper::createImmovableInvInertial());
		road->ConnectToSystems();
		AddDrawable(road);
	
	/*	road =new Road (5);
		road->SetPhysics(5);*/

	unsigned int size = 5;

	VehiclePhysicsNode* vpn = new VehiclePhysicsNode(size);
	
	//GameStateManager::Physics()->AddNode(vpn);

	car = new Vehicle(5);
	car->SetRotation(Quaternion::EulerAnglesToQuaternion(0,-90,0));
	car->SetType('u');
	cout<<car->GetType()<<endl;
	chasecamera = new ChaseCamera(car,T3Vector3(0,40,130),180,90,0);
	//chasecamera->AddYaw(500);
	//chasecamera->AddPitch(90);
	//car->SetPhysics(5,(vpn->GetCar()));

	vpn->SetPGE(car);
	vpn->getRFW()->SetPGE(car);
	vpn->getRFW()->SetType('z');
	vpn->getLFW()->SetPGE(car);
	vpn->getLFW()->SetType('b');
	vpn->getBRW()->SetPGE(car);
	vpn->getBRW()->SetType('n');
	vpn->getBLW()->SetPGE(car);
	vpn->getBLW()->SetType('m');
	vpn->SetType('f');
	cout<<"35"<<vpn->GetGameEntity()->GetType()<<"45"<<endl;

	car->SetPhysics(5, vpn);
	AddDrawable(car);


	FrontRightTire = new Vehicle_Wheel(5);
	FrontRightTire->SetOriginPosition(T3Vector3(10, -2, 8));
	FrontRightTire->SetPhysics(5, (vpn->getRFW()));
	AddDrawable(FrontRightTire);


	FrontLeftTire = new Vehicle_Wheel(5);
	FrontLeftTire->SetOriginPosition(T3Vector3(10, -2, -5));
	FrontLeftTire->SetPhysics(5, (vpn->getLFW()));
	AddDrawable(FrontLeftTire);


    BackRightTire = new Vehicle_Wheel(5/*,car->GetPhysicsNode()*/);
	BackRightTire->SetOriginPosition(T3Vector3(-11, -2, 8));
	BackRightTire->SetPhysics(5,(vpn->getBRW()));
	AddDrawable(BackRightTire);

	BackLeftTire = new Vehicle_Wheel(5/*,car->GetPhysicsNode()*/);
	BackLeftTire->SetOriginPosition(T3Vector3(-11, -2, -5));
	BackLeftTire->SetPhysics(5, (vpn->getBLW()));
	AddDrawable(BackLeftTire);


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
	f=0;
	b=0;
	temp2=T3Vector3(0,0,0);
	Speed_Rotate = 1.0;
	tempPosition = T3Vector3(0,350,-800);
	PlayerPosition=T3Vector3(500,100,-800);

	camera->SetPosition(T3Vector3(0,10.0f, 80.0f));
	//camera->SetYaw(180.0f);
	GameStateManager::Graphics()->SetCamera(chasecamera);
#endif
	//GameStateManager::Graphics()->SetCamera(chasecamera);
}

void RacerGame::Update() { 
	#if WINDOWS_BUILD
	/*if(car->GetCarNode().GetLinearVelocity().Length()>=15)
	{
		car->GetCarNode().SetForce(T3Vector3(0,0,0));
	
	}*/
	//if(((abs(car->GetCarNode().GetLinearVelocity().x)+abs(car->GetCarNode().GetLinearVelocity().z))<0.5)&& f!=0)
	//	
	//{
 // f=0;
 // //cout<<"reset f"<<endl;
	//}
	if(f>0){
		f=f-0.35f;
	if(f<0)
	{
	f=0;
	}
	 T3Matrix4 m4 = car->GetCarNode().GetOrientation().ToMatrix();
				   car->GetCarNode().SetLinearVelocity( m4 *T3Matrix4::Rotation(90,T3Vector3(0,1,0))*f);
	}

	if(f<0){
		f=f+0.35f;
	if(f>0)
	{
	  f=0;
	}
	 T3Matrix4 m4 = car->GetCarNode().GetOrientation().ToMatrix();
				   car->GetCarNode().SetLinearVelocity( m4 *T3Matrix4::Rotation(90,T3Vector3(0,1,0))*f);
	}
	//T3Matrix4 m = T3Matrix4::Rotation(0.016f, T3Vector3(0,1,0));
	//ent->AddRotation(Quaternion::FromMatrix(m));
	if(update==1)
	{
		CreateTrack();

	update=0;
	}

	if(update==2)
	{
		DeleteTrack();

	update=0;
	}
	if((Time-60)==0){
	//cout<<"one sec pass"<<endl;
	SetPlayTime(-1);
	Time=0;
	//cout<<"\n time: pick up delete ="<<pickup.size()<<" delete  \n";
	//cout<<"Play Time = "<<GetPlayTime()<<endl;
	hud->SetScreen(GetScore(),GetPlayTime());
	}
	Time+=1;

	//GameScreen2D::AddDrawable(new DrawableText2D(
	//	0.0f, 
	//	0.0f, 
	//	0, 
	//	0.5f, 
	//	0.1f, 
	//	"HELLO WORLD!!!!!!!!!", 
	//	f,
	//	0,
	//	T3Vector2(0.5f, 0.5f),
	//	T3Vector4(1.0f,1.0f,1.0f,0.7f)));

#endif
}
void RacerGame::Start(){

	Gold_cion * gold_cion= new Gold_cion(8);

	gold_cion->SetOriginPosition(T3Vector3(50,0,0));
	gold_cion->SetRotation(Quaternion::EulerAnglesToQuaternion(0,0,0));
	gold_cion->SetTexture(scoreTexture);

	gold_cion->SetPhysics(8,'p',T3Vector3(50,0,0),Quaternion::EulerAnglesToQuaternion(0,0,0));
	
	gold_cion->SetType('p');                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                      
	gold_cion->GetPhysicsNode().SetPGE(gold_cion);

	//T3Vector4 color(1.f,1.f,1.f,0.f);
	//gold_cion->GetMesh()->SetColour(color);


	AddDrawable(gold_cion);
	pickup.push_back(gold_cion);




	//starting track
	float angle =0;
//		g += 100*(sin(RadToDeg(angle)));
//		gx += 100*(2 * abs(cos(RadToDeg(angle))));
		g += 100*(sin(DegToRad(angle)));
		gx += 100*( 3*abs(cos(DegToRad(angle))));

		T3Vector3 spn= T3Vector3(gx,0,g);

		SplinePoint.push_back(spn);

		//2
//		g += 100*(sin(RadToDeg(angle)));
//		gx += 100*(2 * abs(cos(RadToDeg(angle))));
		//float angle2 = 15;
		 //angle = (rand() % 150) - 75;
		g += 100*(sin(DegToRad(angle)));
		gx += 100*(3* abs(cos(DegToRad(angle))));

		T3Vector3 spn2= T3Vector3(gx,0,g);

		SplinePoint.push_back(spn2);

		//create new track
		
		T3Vector3 avg = (SplinePoint[1] + SplinePoint[2] + SplinePoint[3]) / 3.0f;
		T3Vector3 avg2 = (SplinePoint[2] + SplinePoint[3] + SplinePoint[4]) / 3.0f;

		GameStateManager::Graphics()->GetRenderContext();

			TrackSegment* track = new TrackSegment(SplinePoint[0],SplinePoint[1],SplinePoint[2], 5, 50.0);
	//push back track

	TrackSegmentVector.push_back(track);

		TrackSegment* Strackn = new TrackSegment(SplinePoint[1] - avg,SplinePoint[2] - avg,SplinePoint[3] - avg,5,50.f);

		TrackSegmentVector.push_back(Strackn);
		
		/*TrackSegment* Strackn = new TrackSegment(SplinePoint[1],SplinePoint[2],T3Vector3(400,0,0),5,50.f);
		TrackSegmentVector.push_back(Strackn);*/
	/*	GameStateManager::Graphics()->DropRenderContext();

		GameStateManager::Graphics()->GetRenderContext();*/
		TrackSegment* Strackn2 = new TrackSegment(SplinePoint[2] - avg2,SplinePoint[3] - avg2,SplinePoint[4] - avg2,5,50.f);

		TrackSegmentVector.push_back(Strackn2);
		
		/*TrackSegment* Strackn = new TrackSegment(SplinePoint[1],SplinePoint[2],T3Vector3(400,0,0),5,50.f);
		TrackSegmentVector.push_back(Strackn);*/
		//GameStateManager::Graphics()->DropRenderContext();
		//create end

			Texture* grassTex = GameStateManager::Assets()->LoadTexture(this, TEXTUREDIR"water.jpg", 0);
		/*GameStateManager::Graphics()->GetRenderContext();*/


	GameStateManager::Graphics()->DropRenderContext();


	DrawableEntity3D* ent = new DrawableEntity3D(
		track,
		NULL,
		grassTex,
		NULL,
		800.0f,
		T3Vector3(0,0,0),
		Quaternion::EulerAnglesToQuaternion(0,0,0),
		T3Vector3(1,1,1));
	//gameEntities.push_back(ent);
	AddDrawable(ent);
	allEntities.push_back(ent);

	//add new track to draw
	DrawableEntity3D* ent2 = new DrawableEntity3D(
		Strackn,
		NULL,
		grassTex,
		NULL,
		800.0f,
		//T3Vector3(0,0,0),
		avg,
		Quaternion::EulerAnglesToQuaternion(0,0,0),
		T3Vector3(1,1,1));
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
		/*T3Vector3(0,0,0),*/
		avg2,
		Quaternion::EulerAnglesToQuaternion(0,0,0),
		T3Vector3(1,1,1));
	//gameEntities.push_back(ent);
	AddDrawable(ent3);
	allEntities.push_back(ent3);
	//add 3 end


	T3Vector3 L =  Strackn2->GetTrackCentreLeft();
	T3Vector3 R = Strackn2->GetTrackCentreRight();

	T3Vector3 rl = R-L;
	T3Vector3 lr = L-R;

	T3Vector3 F = T3Vector3(0,0,1); 
	F.Normalise();

	rl.Normalise();

	float cosc= T3Vector3::Dot(rl,F); 
	//float Degc= 
	double degc=acos(cosc);

	float anglec=RadToDeg(degc);
	cout<<anglec<<endl;


	if(L.x<R.x)
	{
	anglec=-anglec;
	}

	cout<<"updated"<<endl;



	/*checkpoint= new CheckPoint(10);
	checkpoint->SetPhysics(10,'c');
	checkpoint->SetType('g');
	checkpoint->GetPhysicsNode().SetPGE(checkpoint);
	AddDrawable(checkpoint);*/

	CheckPoint* checkpoint3= new CheckPoint(10);
	//checkpoint2->SetPhysics(10,'c');
	checkpoint3->SetOriginPosition(SplinePoint[1]);
	checkpoint3->SetRotation(Quaternion::EulerAnglesToQuaternion(0,0,0));

	checkpoint3->SetPhysics(10,'c',SplinePoint[1],Quaternion::EulerAnglesToQuaternion(0,0,0));
	checkpoint3->SetType('g');                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                      
	checkpoint3->GetPhysicsNode().SetPGE(checkpoint3);
	//T3Vector4* color= new T3Vector4(0.5f,0.5f,0.5f,0.2f);
	//checkpoint3->GetMesh()->SetColour(color);
	//checkpoint2->GetPhysicsNode().SetPGE(checkpoint2);
	AddDrawable(checkpoint3);
	checkPoint.push_back(checkpoint3);

	CheckPoint* checkpoint2= new CheckPoint(10);
	//checkpoint2->SetPhysics(10,'c');
	checkpoint2->SetOriginPosition(SplinePoint[3]);
	checkpoint2->SetRotation(Quaternion::EulerAnglesToQuaternion(0,-(anglec),0));

	checkpoint2->SetPhysics(10,'c',SplinePoint[3],Quaternion::EulerAnglesToQuaternion(0,-(anglec),0));
	checkpoint2->SetType('d');                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                      
	checkpoint2->GetPhysicsNode().SetPGE(checkpoint2);
	//checkpoint2->GetPhysicsNode().SetPGE(checkpoint2);
	AddDrawable(checkpoint2);
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
	RemoveDrawable(allEntities[0]);
	RemoveDrawable(allEntities[1]);
	allEntities.erase(allEntities.begin());
	allEntities.erase(allEntities.begin());
	GameStateManager::Graphics()->RemoveDrawable(checkPoint[0]);
	checkPoint[0]->DisconnectFromSystems();
	GameStateManager::Graphics()->RemoveDrawable(checkPoint[1]);
	checkPoint[1]->DisconnectFromSystems();
	//PhysicsNode*  n =checkPoint[0]->GetPhysicsNode();
	//GameStateManager::Physics()->RemoveNode(n);
	//checkPoint[0]->DisconnectFromSystems();
	//checkPoint[1]->DisconnectFromSystems();
	//RemoveDrawable(checkPoint[0]);
	//RemoveDrawable(checkPoint[1]);
	//RemoveDrawable(allEntities[0]);

	//delete checkPoint[0];
	checkPoint.erase(checkPoint.begin());
	//delete checkPoint[0];
	checkPoint.erase(checkPoint.begin());

	//RemoveDrawable(allEntities[2]);
	
	delete TrackSegmentVector[0];
	TrackSegmentVector.erase(TrackSegmentVector.begin());
	delete TrackSegmentVector[0];
	TrackSegmentVector.erase(TrackSegmentVector.begin());
	SplinePoint.erase(SplinePoint.begin());
	SplinePoint.erase(SplinePoint.begin());

		cout<<"\n delete ="<<allEntities.size()<<" delete  \n";
}

void RacerGame::CreateTrack(){
//create a new control point
		float angle = (rand() % 120) - 60;
		//float angle = 0;
//		g += 100*(sin(RadToDeg(angle)));
//		gx += 100*(2 * abs(cos(RadToDeg(angle))));
		g += 100*(sin(DegToRad(angle)));
		gx += 100*( 3*abs(cos(DegToRad(angle))));

		T3Vector3 spn= T3Vector3(gx,0,g);

		SplinePoint.push_back(spn);

		//2
//		g += 100*(sin(RadToDeg(angle)));
//		gx += 100*(2 * abs(cos(RadToDeg(angle))));
		//float angle2 = 15;
		 //angle = (rand() % 150) - 75;
		g += 100*(sin(DegToRad(angle)));
		gx += 100*(3* abs(cos(DegToRad(angle))));

		T3Vector3 spn2= T3Vector3(gx,0,g);

		SplinePoint.push_back(spn2);

		//create new track
		
		T3Vector3 avg = (SplinePoint[3] + SplinePoint[4] + SplinePoint[5]) / 3.0f;
		T3Vector3 avg2 = (SplinePoint[4] + SplinePoint[5] + SplinePoint[6]) / 3.0f;

		GameStateManager::Graphics()->GetRenderContext();
		TrackSegment* Strackn = new TrackSegment(SplinePoint[3] - avg,SplinePoint[4] - avg,SplinePoint[5] - avg,5,50.f);

		TrackSegmentVector.push_back(Strackn);
		
		/*TrackSegment* Strackn = new TrackSegment(SplinePoint[1],SplinePoint[2],T3Vector3(400,0,0),5,50.f);
		TrackSegmentVector.push_back(Strackn);*/
	/*	GameStateManager::Graphics()->DropRenderContext();

		GameStateManager::Graphics()->GetRenderContext();*/
		TrackSegment* Strackn2 = new TrackSegment(SplinePoint[4] - avg2,SplinePoint[5] - avg2,SplinePoint[6] - avg2,5,50.f);

		TrackSegmentVector.push_back(Strackn2);
		
		/*TrackSegment* Strackn = new TrackSegment(SplinePoint[1],SplinePoint[2],T3Vector3(400,0,0),5,50.f);
		TrackSegmentVector.push_back(Strackn);*/
		//GameStateManager::Graphics()->DropRenderContext();
		//create end

			Texture* grassTex = GameStateManager::Assets()->LoadTexture(this, TEXTUREDIR"water.jpg", 0);
		/*GameStateManager::Graphics()->GetRenderContext();*/
	

	GameStateManager::Graphics()->DropRenderContext();


	

	//add new track to draw
	DrawableEntity3D* ent2 = new DrawableEntity3D(
		Strackn,
		NULL,
		grassTex,
		NULL,
		800.0f,
		//T3Vector3(0,0,0),
		avg,
		Quaternion::EulerAnglesToQuaternion(0,0,0),
		T3Vector3(1,1,1));
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
		/*T3Vector3(0,0,0),*/
		avg2,
		Quaternion::EulerAnglesToQuaternion(0,0,0),
		T3Vector3(1,1,1));
	//gameEntities.push_back(ent);
	AddDrawable(ent3);
	allEntities.push_back(ent3);
	//add 3 end


	T3Vector3 L =  Strackn2->GetTrackCentreLeft();
	T3Vector3 R = Strackn2->GetTrackCentreRight();

	T3Vector3 rl = R-L;
	T3Vector3 lr = L-R;

	T3Vector3 F = T3Vector3(0,0,1); 
	F.Normalise();

	rl.Normalise();

	float cosc= T3Vector3::Dot(rl,F); 
	//float Degc= 
	double degc=acos(cosc);

	float anglec=RadToDeg(degc);
	cout<<anglec<<endl;


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

	cout<<"updated"<<endl;

	
	CheckPoint* checkpoint3= new CheckPoint(10);
	//checkpoint2->SetPhysics(10,'c');
	checkpoint3->SetOriginPosition(SplinePoint[3]+T3Vector3(33,0,move));
	checkpoint3->SetRotation(Quaternion::EulerAnglesToQuaternion(0,-(angleCreate),0));

	checkpoint3->SetPhysics(10,'c',SplinePoint[3]+T3Vector3(33,0,move),Quaternion::EulerAnglesToQuaternion(0,-(angleCreate),0));
	checkpoint3->SetType('g');                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                      
	checkpoint3->GetPhysicsNode().SetPGE(checkpoint3);
	//checkpoint2->GetPhysicsNode().SetPGE(checkpoint2);
	AddDrawable(checkpoint3);
	checkPoint.push_back(checkpoint3);

	/*checkpoint= new CheckPoint(10);
	checkpoint->SetPhysics(10,'c');
	checkpoint->SetType('g');
	checkpoint->GetPhysicsNode().SetPGE(checkpoint);
	AddDrawable(checkpoint);*/

	CheckPoint* checkpoint2= new CheckPoint(10);
	//checkpoint2->SetPhysics(10,'c');
	checkpoint2->SetOriginPosition(SplinePoint[5]);
	checkpoint2->SetRotation(Quaternion::EulerAnglesToQuaternion(0,-(anglec),0));

	checkpoint2->SetPhysics(10,'c',SplinePoint[5],Quaternion::EulerAnglesToQuaternion(0,-(anglec),0));
	checkpoint2->SetType('d');                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                      
	checkpoint2->GetPhysicsNode().SetPGE(checkpoint2);
	//checkpoint2->GetPhysicsNode().SetPGE(checkpoint2);
	AddDrawable(checkpoint2);
	checkPoint.push_back(checkpoint2);

	if(GettimeOrScore()!=3){
	Gold_cion * gold_cion= new Gold_cion(8);

	gold_cion->SetOriginPosition(SplinePoint[4]+T3Vector3((rand() % 10)-5,0,(rand() % 10)-5));
	gold_cion->SetRotation(Quaternion::EulerAnglesToQuaternion(0,0,0));

	gold_cion->SetTexture(scoreTexture);

	gold_cion->SetPhysics(8,'p',SplinePoint[4],Quaternion::EulerAnglesToQuaternion(0,0,0));
	
	gold_cion->SetType('p');                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                      
	gold_cion->GetPhysicsNode().SetPGE(gold_cion);

	AddDrawable(gold_cion);
	pickup.push_back(gold_cion);
	}
	if(GettimeOrScore()==3){
	Gold_cion * gold_cion= new Gold_cion(8);

	gold_cion->SetOriginPosition(SplinePoint[4]+T3Vector3((rand() % 10)-5,0,(rand() % 10)-5));
	gold_cion->SetRotation(Quaternion::EulerAnglesToQuaternion(0,0,0));

	gold_cion->SetTexture(timeTexture);

	gold_cion->SetPhysics(8,'p',SplinePoint[4],Quaternion::EulerAnglesToQuaternion(0,0,0));
	
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

	T3Vector3 F = T3Vector3(0,0,1); 
	F.Normalise();

	rl.Normalise();

	float cosc= T3Vector3::Dot(rl,F); 
	//float Degc= 
	double degc=acos(cosc);

	float anglec2=RadToDeg(degc);
	cout<<anglec2<<endl;


	if(L.x<R.x)
	{
	anglec2=-anglec2;
	}

	return anglec2;
}


#if WINDOWS_BUILD
//void RacerGame::KeyboardEvent(KeyboardEvents::EventType type, KeyboardEvents::Key key) {
//
//
//	PlayerPosition = car->GetPhysicsNode().GetPosition();
//
//
//	switch (type) {
//	case KeyboardEvents::KEY_DOWN:
//	case KeyboardEvents::KEY_HELD:
//		switch (key) {
//
//		case KeyboardEvents::KEYBOARD_W:
//			{
//				//camera->AddMovement(T3Vector3(1,0,0));
//			 f=f+1;
//			// car->GetPhysicsNode().SetForce(T3Vector3(10+f,0,0));
//
//
//
//				if(FrontRightTire->GetPhysicsNode().GetOrientation().y==0)
//		{
//		  
//		car->GetPhysicsNode().SetForce(T3Vector3(f /*+ 100.0f*/,0,0));   
//	
//
////		    f=f+100;
//		}
//		if(FrontRightTire->GetPhysicsNode().GetOrientation().y<0)
//		{
//		float angle=-0.5/90*FrontRightTire->GetPhysicsNode().GetOrientation().y;
//
//		float l= FrontRightTire->GetPhysicsNode().GetLinearVelocity().Length();
//
//		float x=l*sin(angle/2/3.14);
//		float z=l*cos(angle/2/3.14);
//	    f=f+100;
//		//Player->GetPhysicsNode().SetForce(Vector3(0,0,0.3+f));  
//		car->GetPhysicsNode().SetForce(T3Vector3(x+f,0,z+f));
//		
//	
//	/*	tempPosition.x = PlayerPosition.x;*/
//		/*tempPosition.z = PlayerPosition.z-1000;
//		tempPosition.y = PlayerPosition.y+800;
//*/
//		
//		}
//
//		if(FrontRightTire->GetPhysicsNode().GetOrientation().y>0)
//		{
//		float angle=0.5/90*FrontRightTire->GetPhysicsNode().GetOrientation().y;
//
//		float l= car->GetPhysicsNode().GetLinearVelocity().Length();
//
//		float x=-l*sin(angle/2/3.14);
//		float z=l*cos(angle/2/3.14);
//	    f=f+100;
//
//		car->GetPhysicsNode().SetForce(T3Vector3(z+f,0,x-f));
//	
//
//				
//		}
//			
//		/*	tempPosition.z = PlayerPosition.z-1000;
//		     tempPosition.y = PlayerPosition.y+800;*/
//
//
//			}
//			 break;
//			
//		case KeyboardEvents::KEYBOARD_S:
//			{//camera->AddMovement(T3Vector3(0,0,1));
//			 f=f+100;
//		     car->GetPhysicsNode().SetForce(T3Vector3(-10-f,0,0)); 
//		  /*   tempPosition.z = PlayerPosition.z-1000;
//		     tempPosition.y = PlayerPosition.y+800;*/
//
//
//
//			}
//			break;
//		case KeyboardEvents::KEYBOARD_A:
//			{//camera->AddMovement(T3Vector3(1,0,0));
//			/*if(car->GetPhysicsNode().GetOrientation().z>=-0.13)
//			 {
//			    car->GetPhysicsNode().SetAngularVelocity(T3Vector3(0,0,Speed_Rotate));
//		     }			
//		     temp1 = car->GetPhysicsNode().GetLinearVelocity();
//		     temp1.x=0.1*Speed_Player;
//		     car->GetPhysicsNode().SetLinearVelocity(temp1);
//		     tempPosition.x = PlayerPosition.x;
//			*/
//		    //	f=f+100;
//
//			if(car->GetPhysicsNode().GetOrientation().y<=0.8){
//			 FrontLeftTire->GetPhysicsNode().SetAngularVelocity(T3Vector3(0,-Speed_Rotate+0.002,0));
//			if( FrontLeftTire->GetPhysicsNode().GetOrientation().y>=0.2||FrontRightTire->GetPhysicsNode().GetOrientation().y>=0.2)
//			{
//			FrontLeftTire->GetPhysicsNode().SetAngularVelocity(T3Vector3(0,0,0));
//			FrontRightTire->GetPhysicsNode().SetAngularVelocity(T3Vector3(0,0,0));
//			}
//		     FrontRightTire->GetPhysicsNode().SetAngularVelocity(T3Vector3(0,-Speed_Rotate+0.002,0));
//			 BackLeftTire->GetPhysicsNode().SetAngularVelocity(T3Vector3(0,-Speed_Rotate,0));
//		     BackRightTire->GetPhysicsNode().SetAngularVelocity(T3Vector3(0,-Speed_Rotate,0));
//			 car->GetPhysicsNode().SetAngularVelocity(T3Vector3(0,-Speed_Rotate,0));
//			 cout<<car->GetPhysicsNode().GetOrientation().y<<endl;
//		 
//		//	 	
//		//
//		//float angle=-0.5/90*FrontRightTire->GetPhysicsNode().GetOrientation().z;
//
//		//float l= car->GetPhysicsNode().GetLinearVelocity().Length();
//
//		//float x=l*sin(angle/2/3.14);
//		//float z=l*cos(angle/2/3.14);
//	 //   f=f+100;
//		//Player->GetPhysicsNode().SetForce(Vector3(0,0,0.3+f));  
//	//	car->GetPhysicsNode().SetForce(T3Vector3(x+f,0,z+f));
//		}		
//		
//			
//			}
//			break;
//		case KeyboardEvents::KEYBOARD_D:
//			{//camera->AddMovement(T3Vector3(-1,0,0));
//			/*  if(car->GetPhysicsNode().GetOrientation().z<=0.13)
//			  {
//			    car->GetPhysicsNode().SetAngularVelocity(T3Vector3(0,0,-Speed_Rotate));
//		      }
//		      temp1 = car->GetPhysicsNode().GetLinearVelocity();
//		      temp1.x=-0.1*Speed_Player;
//		      car->GetPhysicsNode().SetLinearVelocity(temp1);
//		      tempPosition.x = PlayerPosition.x;*/
//
//
//				if(FrontRightTire->GetPhysicsNode().GetOrientation().y>=-0.8){
//		
//			 FrontLeftTire->GetPhysicsNode().SetAngularVelocity(T3Vector3(0,Speed_Rotate-0.002,0));
//		     FrontRightTire->GetPhysicsNode().SetAngularVelocity(T3Vector3(0,Speed_Rotate-0.002,0));
//			 BackLeftTire->GetPhysicsNode().SetAngularVelocity(T3Vector3(0,Speed_Rotate,0));
//		     BackRightTire->GetPhysicsNode().SetAngularVelocity(T3Vector3(0,Speed_Rotate,0));
//			 car->GetPhysicsNode().SetAngularVelocity(T3Vector3(0,Speed_Rotate,0));
//		}			
//
//
//			}
//			break;
//		case KeyboardEvents::KEYBOARD_SHIFT:
//			camera->AddMovement(T3Vector3(0,1,0));
//			break;
//		case KeyboardEvents::KEYBOARD_SPACE:
//			{camera->AddMovement(T3Vector3(0,-1,0));
//			 temp1 = car->GetPhysicsNode().GetLinearVelocity();
//			 car->GetPhysicsNode().SetUseGravity(TRUE);
//			 temp1.y = 0.4;
//			 car->GetPhysicsNode().SetLinearVelocity(temp1);	
//			
//			}
//			break;
//		case KeyboardEvents::KEYBOARD_I:
//			camera->AddMovement(T3Vector3(0,0,-1));
//			break;
//		case KeyboardEvents::KEYBOARD_K:
//			camera->AddMovement(T3Vector3(0,0,1));
//			break;
//		case KeyboardEvents::KEYBOARD_J:
//			camera->AddMovement(T3Vector3(-1,0,0));
//			break;
//		case KeyboardEvents::KEYBOARD_L:
//			camera->AddMovement(T3Vector3(1,0,0));
//			break;
//		case KeyboardEvents::KEYBOARD_U:
//			camera->AddMovement(T3Vector3(0,1,0));
//			break;
//		case KeyboardEvents::KEYBOARD_O:
//			camera->AddMovement(T3Vector3(0,-1,0));
//			break;
//		case KeyboardEvents::KEYBOARD_LEFT:
//			camera->AddYaw(1);
//			break;
//		case KeyboardEvents::KEYBOARD_RIGHT:
//			camera->AddYaw(-1);
//			break;
//		case KeyboardEvents::KEYBOARD_UP:
//			camera->AddPitch(1);
//			break;
//		case KeyboardEvents::KEYBOARD_DOWN:
//			camera->AddPitch(-1);
//			break;
//		case KeyboardEvents::KEYBOARD_Q:
//			camera->AddRoll(-1);
//			break;
//		case KeyboardEvents::KEYBOARD_E:
//			camera->AddRoll(1);
//			break;
//		
//		}
//		
//	}
//}
#endif
#if WINDOWS_BUILD
void RacerGame::KeyboardEvent(KeyboardEvents::EventType type, KeyboardEvents::Key key) {

	float R=0.707106829f;
	switch (type) {
	case KeyboardEvents::KEY_DOWN:
	case KeyboardEvents::KEY_HELD:
		switch (key) {

//		case KeyboardEvents::KEYBOARD_W:
//		{
//				//camera->AddMovement(T3Vector3(1,0,0));
//
//		
//				/*cout<<"bRbbbbbbbbbbbbbbbbbbbb"<<R<<endl;
//				
//				cout<<"bYbbbbbbbbbbbbbbbbbbbb"<<car->GetCarNode().GetOrientation().y<<endl;
//				cout<<"bEbbbbbbbbbbbbbbbbbbbb"<<((car->GetCarNode().GetOrientation().y)+R)<<endl;*/
//
//				//float E=R+(car->GetCarNode().GetOrientation().y);
//			   //f=1200;
//			   f=300;
//				//  car->GetCarNode().SetForce(T3Vector3(10+f,0,0));
//			   if(((car->GetCarNode().GetOrientation().y)+R)==0)
//		       {
//				   //cout<<"b1bbbbbbbbbbbbbbbbbbbb"<<(car->GetCarNode().GetOrientation().y)+R<<endl;
//				   car->GetCarNode().SetForce(T3Vector3(f /*+ 100.0f*/,0,0));   
//					//		    f=f+100;
//		       }
//			 // cout<<car->GetCarNode().GetLinearVelocity().Length ()<<endl;
//
//			   //cout<<car->GetCarNode().GetLinearVelocity().Length()<<endl;
//				if(((car->GetCarNode().GetOrientation().y)+R)<0)
//				{
//					//cout<<"b2bbbbbbbbbbbbbbbbbbbb"<<car->GetCarNode().GetOrientation().y<<endl;
//					float angle=-0.5/90*((car->GetCarNode().GetOrientation().y)+R);
//					float l=  car->GetCarNode().GetLinearVelocity().Length();
//					float x=l*sin(angle/2/3.14);
//					float z=l*cos(angle/2/3.14);
//				   // f=f+5;
//					//Player->GetPhysicsNode().SetForce(Vector3(0,0,0.3+f));  
//					 car->GetCarNode().SetForce(T3Vector3(x+f,0,z+f));
//					/*	tempPosition.x = PlayerPosition.x;*/
//						/*tempPosition.z = PlayerPosition.z-1000;
//						tempPosition.y = PlayerPosition.y+800;
//					*/
//		
//				}
//
//				if(((car->GetCarNode().GetOrientation().y)+R)>0)
//				{
//					//cout<<"b3bbbbbbbbbbbbbbbbbbbb"<<((car->GetCarNode().GetOrientation().y)+0.707107)<<endl;
//					float angle=0.5/90*((car->GetCarNode().GetOrientation().y)+R);
//					float l=  car->GetCarNode().GetLinearVelocity().Length();
//					float x=-l*sin(angle/2/3.14);
//					float z=l*cos(angle/2/3.14);
//					//f=f+5;
//					 car->GetCarNode().SetForce(T3Vector3(z+f,0,x-f));	
//				}
//			
//				 /*	tempPosition.z = PlayerPosition.z-1000;
//					 tempPosition.y = PlayerPosition.y+800;*/
//
//						/*	if(f>4000)
//							{
//								f=3800;
//							}*/
//
//		}
//			 break;
//			
//		case KeyboardEvents::KEYBOARD_S:
//			{//camera->AddMovement(T3Vector3(0,0,1));
//			/* f=f+100;
//		      car->GetCarNode().SetForce(T3Vector3(-10-f,0,0)); */
//		  /*   tempPosition.z = PlayerPosition.z-1000;
//		     tempPosition.y = PlayerPosition.y+800;*/
//
//
//
//				// f=f+5;
//			//  car->GetCarNode().SetForce(T3Vector3(10+f,0,0));
//
//
//
//				if(((car->GetCarNode().GetOrientation().y)+R)==0)
//		{
//		  
//		 car->GetCarNode().SetForce(T3Vector3(-f ,0,0));   
//	
//
////		    f=f+100;
//		}
//		if(((car->GetCarNode().GetOrientation().y)+R)<0)
//		{
//		float angle=-0.5/90*((car->GetCarNode().GetOrientation().y)+R);
//
//		float l=  car->GetCarNode().GetLinearVelocity().Length();
//
//		float x=l*sin(angle/2/3.14);
//		float z=l*cos(angle/2/3.14);
//	    //f=f+5;
//		//Player->GetPhysicsNode().SetForce(Vector3(0,0,0.3+f));  
//		 car->GetCarNode().SetForce(T3Vector3(-(x+f),0,-(z+f)));
//		
//	
//	/*	tempPosition.x = PlayerPosition.x;*/
//		/*tempPosition.z = PlayerPosition.z-1000;
//		tempPosition.y = PlayerPosition.y+800;
//*/
//		
//		}
//
//		if(((car->GetCarNode().GetOrientation().y)+R)>0)
//		{
//		float angle=0.5/90*((car->GetCarNode().GetOrientation().y)+R);
//
//		float l=  car->GetCarNode().GetLinearVelocity().Length();
//
//		float x=-l*sin(angle/2/3.14);
//		float z=l*cos(angle/2/3.14);
//	    //f=f+5;
//
//		 car->GetCarNode().SetForce(T3Vector3(-(z+f),0,-(x-f)));
//	
//
//				
//		}
//
//
//
//			}
//			break;
//		case KeyboardEvents::KEYBOARD_A:
//			{//camera->AddMovement(T3Vector3(1,0,0));
//			/*if( car->GetCarNode().GetOrientation().z>=-0.13)
//			 {
//			     car->GetCarNode().SetAngularVelocity(T3Vector3(0,0,Speed_Rotate));
//		     }			
//		     temp1 =  car->GetCarNode().GetLinearVelocity();
//		     temp1.x=0.1*Speed_Player;
//		      car->GetCarNode().SetLinearVelocity(temp1);
//		     tempPosition.x = PlayerPosition.x;
//			*/
//		    //	f=f+100;
//
//			if( ((car->GetCarNode().GetOrientation().y)+R)<=0.6){
//
//				// car->GetCarNode().SetOrientation( car->GetCarNode().GetOrientation()+Quaternion::FromMatrix(T3Matrix4::Rotation(30,T3Vector3(0,-1,0))));
//
//			 FrontLeftTire->GetPhysicsNode().SetAngularVelocity(T3Vector3(0,-Speed_Rotate-0.001,0));
//			/*if( FrontLeftTire->GetPhysicsNode().GetOrientation().y>=0.2||FrontRightTire->GetPhysicsNode().GetOrientation().y>=0.2)
//			{
//			FrontLeftTire->GetPhysicsNode().SetAngularVelocity(T3Vector3(0,0,0));
//			FrontRightTire->GetPhysicsNode().SetAngularVelocity(T3Vector3(0,0,0));
//			}*/
//		     FrontRightTire->GetPhysicsNode().SetAngularVelocity(T3Vector3(0,-Speed_Rotate-0.001,0));
//			 BackLeftTire->GetPhysicsNode().SetAngularVelocity(T3Vector3(0,-Speed_Rotate,0));
//		     BackRightTire->GetPhysicsNode().SetAngularVelocity(T3Vector3(0,-Speed_Rotate,0));
//			  car->GetCarNode().SetAngularVelocity(T3Vector3(0,-Speed_Rotate,0));
//
//		
//			// cout<< car->GetCarNode().GetOrientation().y<<endl;
//			/* float angle=-0.5/90*FrontRightTire->GetPhysicsNode().GetOrientation().y;
//
//		    float l=  car->GetCarNode().GetLinearVelocity().Length();
//
//	    	float x=l*sin(angle/2/3.14);
//	   float z=l*cos(angle/2/3.14);
//	       f=f+100;*/
//		////Player->GetPhysicsNode().SetForce(Vector3(0,0,0.3+f));  
//		// car->GetCarNode().SetForce(T3Vector3(x+f,0,z+f));
//		//	 cout<< car->GetCarNode().GetOrientation().y<<endl;
//			 
//		
//		
//			 	
//		
//		/*float angle=-0.5/90*FrontRightTire->GetPhysicsNode().GetOrientation().y;
//		float l=  car->GetCarNode().GetLinearVelocity().Length();
//
//		float x=l*sin(angle/2/3.14);
//		float z=l*cos(angle/2/3.14);
//	    f=f+5;
//
//		 car->GetCarNode().SetForce(T3Vector3(x+f,0,-(z+f)));*/
//		}		
//		
//			
//			}
//			break;
//		case KeyboardEvents::KEYBOARD_D:
//			{//camera->AddMovement(T3Vector3(-1,0,0));
//			/*  if( car->GetCarNode().GetOrientation().z<=0.13)
//			  {
//			     car->GetCarNode().SetAngularVelocity(T3Vector3(0,0,-Speed_Rotate));
//		      }
//		      temp1 =  car->GetCarNode().GetLinearVelocity();
//		      temp1.x=-0.1*Speed_Player;
//		       car->GetCarNode().SetLinearVelocity(temp1);
//		      tempPosition.x = PlayerPosition.x;*/
//
//
//				if( ((car->GetCarNode().GetOrientation().y)+R)>=-0.6){
//		
//			 FrontLeftTire->GetPhysicsNode().SetAngularVelocity(T3Vector3(0,Speed_Rotate-0.001,0));
//		     FrontRightTire->GetPhysicsNode().SetAngularVelocity(T3Vector3(0,Speed_Rotate-0.001,0));
//			 BackLeftTire->GetPhysicsNode().SetAngularVelocity(T3Vector3(0,Speed_Rotate,0));
//		     BackRightTire->GetPhysicsNode().SetAngularVelocity(T3Vector3(0,Speed_Rotate,0));
//			  car->GetCarNode().SetAngularVelocity(T3Vector3(0,Speed_Rotate,0));
//			 //cout<< car->GetCarNode().GetOrientation().y<<endl;
//		}			
//				/*float angle=0.5/90*FrontRightTire->GetPhysicsNode().GetOrientation().y;
//
//		       float l=  car->GetCarNode().GetLinearVelocity().Length();
//
//		     float x=-l*sin(angle/2/3.14);
//		float z=l*cos(angle/2/3.14);
//	    f=f+5;
//
//		 car->GetCarNode().SetForce(T3Vector3(z+f,0,-(x-f)));*/
//	
//
//

case KeyboardEvents::KEYBOARD_7:
		{
				//camera->AddMovement(T3Vector3(1,0,0));
			GameStateManager::Audio()->PlaySoundA(GameStateManager::Audio()->GetSound (SOUNDSDIR"bgm2_42sec.wav"),SOUNDPRIORTY_LOW,false);

			       f=f+1.8;
				   if(f>350){
				   f=350;
		       }

		
				   T3Matrix4 m4 = car->GetCarNode().GetOrientation().ToMatrix();
				   car->GetCarNode().SetLinearVelocity( m4 *T3Matrix4::Rotation(90,T3Vector3(0,1,0))*f);

			

			
			 break;
		}
		case KeyboardEvents::KEYBOARD_8:
			{//camera->AddMovement(T3Vector3(0,0,1));
		
				 f=f-3.0;
				    if(f<(-90)){
				   f=-90;
		}
				 
           T3Matrix4 m4 = car->GetCarNode().GetOrientation().ToMatrix();
				   car->GetCarNode().SetLinearVelocity( m4 *T3Matrix4::Rotation(90,T3Vector3(0,1,0))*f);
				 
				   
				   

				



			}
			break;
		case KeyboardEvents::KEYBOARD_A:
			{//camera->AddMovement(T3Vector3(1,0,0));


				if(car->GetCarNode().GetLinearVelocity().x>=0){
				FrontLeftTire->GetPhysicsNode().SetAngularVelocity(T3Vector3(0,Speed_Rotate,0));
		     FrontRightTire->GetPhysicsNode().SetAngularVelocity(T3Vector3(0,Speed_Rotate,0));
			 BackLeftTire->GetPhysicsNode().SetAngularVelocity(T3Vector3(0,Speed_Rotate,0));
		     BackRightTire->GetPhysicsNode().SetAngularVelocity(T3Vector3(0,Speed_Rotate,0));
			  car->GetCarNode().SetAngularVelocity(T3Vector3(0,Speed_Rotate,0));
				}
				if(car->GetCarNode().GetLinearVelocity().x<0){
				FrontLeftTire->GetPhysicsNode().SetAngularVelocity(T3Vector3(0,-Speed_Rotate,0));
		     FrontRightTire->GetPhysicsNode().SetAngularVelocity(T3Vector3(0,-Speed_Rotate,0));
			 BackLeftTire->GetPhysicsNode().SetAngularVelocity(T3Vector3(0,-Speed_Rotate,0));
		     BackRightTire->GetPhysicsNode().SetAngularVelocity(T3Vector3(0,-Speed_Rotate,0));
			  car->GetCarNode().SetAngularVelocity(T3Vector3(0,-Speed_Rotate,0));
				}

				 T3Matrix4 m4 = car->GetCarNode().GetOrientation().ToMatrix();
				   car->GetCarNode().SetLinearVelocity( m4 *T3Matrix4::Rotation(90,T3Vector3(0,1,0))*f);


	//	}		
		
			
			}
			break;
		case KeyboardEvents::KEYBOARD_D:
			{//camera->AddMovement(T3Vector3(-1,0,0));
			
		if(car->GetCarNode().GetLinearVelocity().x>=0){
			 FrontLeftTire->GetPhysicsNode().SetAngularVelocity(T3Vector3(0,-Speed_Rotate,0));
		     FrontRightTire->GetPhysicsNode().SetAngularVelocity(T3Vector3(0,-Speed_Rotate,0));
			 BackLeftTire->GetPhysicsNode().SetAngularVelocity(T3Vector3(0,-Speed_Rotate,0));
		     BackRightTire->GetPhysicsNode().SetAngularVelocity(T3Vector3(0,-Speed_Rotate,0));
			  car->GetCarNode().SetAngularVelocity(T3Vector3(0,-Speed_Rotate,0));
		      }
				if(car->GetCarNode().GetLinearVelocity().x<0){
				FrontLeftTire->GetPhysicsNode().SetAngularVelocity(T3Vector3(0,Speed_Rotate,0));
		     FrontRightTire->GetPhysicsNode().SetAngularVelocity(T3Vector3(0,Speed_Rotate,0));
			 BackLeftTire->GetPhysicsNode().SetAngularVelocity(T3Vector3(0,Speed_Rotate,0));
		     BackRightTire->GetPhysicsNode().SetAngularVelocity(T3Vector3(0,Speed_Rotate,0));
			  car->GetCarNode().SetAngularVelocity(T3Vector3(0,Speed_Rotate,0));
		}			

	 T3Matrix4 m4 = car->GetCarNode().GetOrientation().ToMatrix();
				   car->GetCarNode().SetLinearVelocity( m4 *T3Matrix4::Rotation(90,T3Vector3(0,1,0))*f);
	


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