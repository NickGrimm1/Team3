#pragma once
//#include"../Framework/MyGame.h"
#include "../Framework/PhysicsNode.h"
#include "../Team3Project1/DrawableEntity3D.h"
#include "../Framework/GameEntity.h"

class CheckPoint : public GameEntity {
public:
	CheckPoint(float size){};
	~CheckPoint(void){};

	
	void SetPhysics(float size);
	


protected:


	PhysicsNode * carPhysicNode;


};