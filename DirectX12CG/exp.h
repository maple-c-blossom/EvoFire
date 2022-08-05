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
	int maxTime = 20;
	bool deleteFlag = false;
	MCB::Vector3D velosity;
	void Init(float Speed, MCB::Float3 position,MCB::Vector3D velosity);
	void Update();
	void ExpDraw();
	void GetExp();

};

