#pragma once
#include "Object3d.h"
#include "Collider.h"
class Bomb:public MCB::Object3d
{
private:
	int lifeTime = 0;
	int maxLifeTime = 300;
public:
	int damage = 500;
	bool deleteFlag = false;
	float maxRudius = 1000.0f;
	int rudiusUpTime = 0;
	int rudiusUpMaxTime = 100;
	int rudiusDownTime = 0;
	int rudiusDownMaxTime = 10;
	float speedOffset = 40;
	float r = 0;
	MCB::Vector3D velosity;
	void Update();
	void Fire(MCB::Vector3D velosity,MCB::Float3 startPosition, MCB::Model* bombModel);

};

