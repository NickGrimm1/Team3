#pragma once
//#include"../Framework/MyGame.h"
#include "../Framework/PhysicsNode.h"
#include "../Team3Project1/DrawableEntity3D.h"
#include "../Framework/GameEntity.h"
#include "../Framework/InertialMatrixHelper.h"

class Gold_cion : public GameEntity {
public:
	Gold_cion(float size);
	~Gold_cion(void);

	
	void SetPhysics(float size);
	void SetPhysics(float size,char type,T3Vector3 position,const Quaternion& rotation);


protected:


	PhysicsNode * carPhysicNode;


};