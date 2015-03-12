#pragma once
//#include"../Framework/MyGame.h"
#include "../Framework/PhysicsNode.h"
#include "../Team3Project1/DrawableEntity3D.h"
#include "../Framework/GameEntity.h"

class CheckPoint : public GameEntity {
public:
	CheckPoint(float size);
	~CheckPoint(void);
	void SetPhysics(float size,char type);
	
	void SetPhysics(float size,char type,T3Vector3 position,const Quaternion& rotation);
	


protected:


	PhysicsNode * carPhysicNode;


};