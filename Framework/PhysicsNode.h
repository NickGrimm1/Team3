/******************************************************************************
Class:PhysicsNode
Implements:
Author:Rich Davison	<richard.davison4@newcastle.ac.uk>, Callum Rhodes <c.g.rhodes@ncl.ac.uk> and YOU!
Description: This class represents the physical properties of your game's
entities - their position, orientation, mass, collision volume, and so on.
Most of the first few tutorials will be based around adding code to this class
in order to add correct physical integration of velocity / acceleration etc to
your game objects. 


In addition to the tutorial code, this class contains a pointer to a SceneNode.
This pointer is to the 'graphical' representation of your game object, just 
like the SceneNode's used in the graphics module. However, instead of 
calculating positions etc as part of the SceneNode, it should instead be up
to your 'physics' representation to determine - so we calculate a transform
matrix for your SceneNode here, and apply it to the pointer. 

Your SceneNode should still have an Update function, though, in order to
update any graphical effects applied to your object - anything that will end
up modifying a uniform in a shader should still be the job of the SceneNode. 

Note that since the SceneNode can still have children, we can represent our
entire CubeRobot with a single PhysicsNode, and a single SceneNode 'root'.

-_-_-_-_-_-_-_,------,   
_-_-_-_-_-_-_-|   /\_/\   NYANYANYAN
-_-_-_-_-_-_-~|__( ^ .^) /
_-_-_-_-_-_-_-""  ""   

*//////////////////////////////////////////////////////////////////////////////


#pragma once

#include "Quaternion.h"
#include "T3Vector3.h"
#include "T3Matrix4.h"
#include "SceneNode.h"
#include "../Team3Project1/DrawableEntity3D.h"

#include "CollisionVolume.h"
//#include "GameEntity.h"

using namespace std;
//#define LINEAR_VELOCITY_DAMP 0.988
#define LINEAR_VELOCITY_DAMP 1
#define ANGULAR_VELOCITY_DAMP 0.7
#define LINEAR_VELOCITY_MIN 0.00001

class GameEntity;

class PhysicsNode	{
public:
	PhysicsNode(void);
	PhysicsNode(Quaternion orientation, T3Vector3 position);
	~PhysicsNode(void);

	T3Vector3		GetPosition() 			{ return m_position;}
	T3Vector3		GetLinearVelocity() const	{ return m_linearVelocity;}
	
	float		GetInverseMass() const		{return m_invMass;}
	float		GetMass() const				{return 1.0f/m_invMass;}

	T3Matrix4		GetInverseInertia() const	{ return m_invInertia; }

	Quaternion	GetOrientation() const		{ return m_orientation;}
	T3Vector3		GetAngularVelocity() const	{ return m_angularVelocity;}

	CollisionVolume* GetCollisionVolume() const { return vol; }

	T3Matrix4		BuildTransform();

	virtual void		Update(float msec);

	void	SetPosition(T3Vector3 vec) { m_position = vec; }
	void	SetLinearVelocity(T3Vector3 vec)		{ m_linearVelocity = vec;}
	
	void	SetMass(float mass) {m_invMass = 1.0f/mass;}
	void	SetInverseMass(float invMass) {m_invMass = invMass;}

	void	SetInverseInertia(T3Matrix4 invInertia) {m_invInertia = invInertia;}
	
	void	SetOrientation(Quaternion q) { m_orientation = q; }
	void	SetAngularVelocity(T3Vector3 vec) { m_angularVelocity = vec; }

	void	SetTarget(DrawableEntity3D *s) { target = s;}
	DrawableEntity3D* GetTarget() {return target;}
	void	SetUseGravity(bool value) { useGravity = value; }
	void	SetCollisionVolume(CollisionVolume* vol) { this->vol = vol; }

	bool Getplanecollision(){return planecollision;};
	void Setplanecollision(bool Planecollision){planecollision =Planecollision;};

	bool GetIsDrop(){return isDrop;}
	void SetIsDrop(bool isdrop) {isDrop = isdrop;}

	T3Vector3 GetFriction() {return m_friction;}
	void    SetFriction(T3Vector3 friction)  {m_friction=friction;}

	T3Vector3	GetForce()	{ return m_force;}
	T3Vector3	GetTorque() { return m_torque;}
	void        SetTorque(T3Vector3 torque) { m_torque=torque;}

	void AddForce(T3Vector3 point, T3Vector3 force);

	void SetWorldPoints(vector<T3Vector3>& newWorldPoint ) { this->newWorldPoints=newWorldPoint; };
	vector<T3Vector3> GetWorldPoints() {return newWorldPoints;};

	vector<T3Vector3> newWorldPoints;


	vector<T3Vector3> Points;


	static const T3Vector3 gravity;
	float Xstart;
	float Xend;
	float GetXstart(){return Xstart;};
	float GetXend(){return Xend;};
	void  SetXstart(float xstart) {Xstart=xstart;};
	void  SetXend(float xend) {Xend=xend;};
	void  SetForce(T3Vector3 x)    {m_force=x;};
	float Ystart;
	float Yend;
	float GetYstart(){return Ystart;};
	float GetYend(){return Yend;};
	void  SetYstart(float Ystart) {Ystart=Ystart;};
	void  SetYend(float Yend) {Yend=Yend;};
	T3Vector3 GetFathestPointInDirection(PhysicsNode& shape1,T3Vector3 d);

	bool GetIsCollide(){ return isCollide;};
	void SetIsCollide(bool iscollide){ isCollide=iscollide;}

	virtual int GetNunmberVertex() {return physicsMesh->GetNumVertices();}
	virtual Vertex* GetPhysicsVertex() {return (physicsMesh != NULL) ? physicsMesh->GetVertices() : NULL;}
	bool Getcar_wheel(){ return car_wheel;};
	void Setcar_wheel(bool Car_wheel){ car_wheel=Car_wheel;}

	Mesh* GetPhysicsMesh() {return physicsMesh;}
	void SetMesh(Mesh* mesh) {physicsMesh = mesh;}


	void SetType(char type){Type=type;}
	char GetType(){return Type;};

	void SetPGE(GameEntity* pge){parentGameEntity=pge;}
	GameEntity* GetGameEntity() {return parentGameEntity;}

	bool useGravity;
	bool isCollide;
	bool isDrop;
	bool car_wheel;
	bool planecollision;

	Mesh* physicsMesh;

	char Type;

	//<---------LINEAR-------------->
	T3Vector3		m_position;
	T3Vector3		m_linearVelocity;
	T3Vector3		m_force;
	float		m_invMass;
	T3Vector3     m_friction;

	//<----------ANGULAR--------------->
	Quaternion  m_orientation;
	T3Vector3		m_angularVelocity;
	T3Vector3		m_torque;
	T3Matrix4     m_invInertia;

	T3Vector3  u;    // unit vector reflects the car's orientation
	T3Vector3  T_engine;  // torque of the engine at a given rpm
	T3Vector3  F;  //drive force
	float xg;  //gear ratio
	float xd;  //differential ratio
	float Rw;  //wheel radius
	float n;   //transmission efficiency

	DrawableEntity3D*	target;
	CollisionVolume* vol;

	GameEntity* parentGameEntity;
//	Function* callback;
//	GameClass* interestedObject;

//	void onCollide(PhysicsNode* collidesWith) {interestedObject->callback(collidesWith);}
};

