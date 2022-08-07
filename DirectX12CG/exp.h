#pragma once
#include "Object3d.h"
class Exp: public MCB::Object3d
{
public:
	float rudius = 5;
	int ExpPoint = 1;
	float speed = 0;
	float startSpeed;
	int time = 0;
	int maxTime = 40;
	bool deleteFlag = false;
	MCB::Vector3D velocity = {0,0,1};
	MCB::Object3d* playerPtr = nullptr;
	float speedOffSet = 10;
	float lifeTime = 0;
	float maxLifeTime = 60 * 60;

	void ExpInit(float Speed, MCB::Float3 position,MCB::Vector3D velosity,MCB::Object3d* playerPtr);
	void Update();
	void ExpDraw();
	void GetExp();
	void ExpApproach();

};

