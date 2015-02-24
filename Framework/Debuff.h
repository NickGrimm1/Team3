#pragma once
#include"MyGame.h"

class Vehicle;
class Debuff{
public:
	Debuff();
	~Debuff(void);
	void UpdateDebuff(Vehicle *car, float msec);
	void SwitchleftAndright(Vehicle*Player);
	GameEntity* BuildSpeedBuffEntity(float radius, Vector3 pos);
	GameEntity* BuildInvLeftAndRightEntity(float radius, Vector3 pos);
	//Vector3 tempPosition;
	//Vector3 GetDist2(){return Dist2;}
	bool GetLR(){return LR;}
	/*void SetCarPosition(Vector3 pos){Car_pos=pos;}
	Vector3 GetCarPosition(){return Car_pos;}*/
	//GameEntity* BuildBuffEntity(float radius, Vector3 pos);
protected:
	Mesh* SpeedBuffMesh;
	Mesh* InverseleftrightMesh;
	Vector3 SpdBf_pos;
	Vector3 RtBf_pos;
	//Vector3 Car_pos;
	//Vector3 Dist2;
	GameEntity* SpeedUp;
	GameEntity* Inverse;
	float size;
	float speeduptime;
	/*Vector3 temp1;
	int Speed_Player;
	float Speed_Rotate;*/
	bool LR;
};