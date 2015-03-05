#if WINDOWS_BUILD

#pragma once
#include"MyGame.h"

class Vehicle;
class Debuff{
public:
	Debuff();
	~Debuff(void);
	void UpdateDebuff(Vehicle *car, float msec);
	void SwitchleftAndright(Vehicle*Player);
	GameEntity* BuildSpeedBuffEntity(float radius, T3Vector3 pos);
	GameEntity* BuildInvLeftAndRightEntity(float radius, T3Vector3 pos);
	//T3Vector3 tempPosition;
	//T3Vector3 GetDist2(){return Dist2;}
	bool GetLR(){return LR;}
	/*void SetCarPosition(T3Vector3 pos){Car_pos=pos;}
	T3Vector3 GetCarPosition(){return Car_pos;}*/
	//GameEntity* BuildBuffEntity(float radius, T3Vector3 pos);
protected:
	Mesh* SpeedBuffMesh;
	Mesh* InverseleftrightMesh;
	T3Vector3 SpdBf_pos;
	T3Vector3 RtBf_pos;
	//T3Vector3 Car_pos;
	//T3Vector3 Dist2;
	GameEntity* SpeedUp;
	GameEntity* Inverse;
	float size;
	float speeduptime;
	/*T3Vector3 temp1;
	int Speed_Player;
	float Speed_Rotate;*/
	bool LR;
};

#endif