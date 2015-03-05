#pragma once
//#include"../Framework/MyGame.h"
#include "../Framework/PhysicsNode.h"
#include "../Team3Project1/DrawableEntity3D.h"
#include "../Framework/GameEntity.h"

class Gold_cion : public GameEntity {
public:
	Gold_cion(float size);
	~Gold_cion(void);

	
	void SetPhysics(float size);
	


protected:


	PhysicsNode * carPhysicNode;


};