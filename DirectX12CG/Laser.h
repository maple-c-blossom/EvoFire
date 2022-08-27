#pragma once
#include "Object3d.h"
#include "Collider.h"
class Laser : public MCB::Object3d
{
private:
	int lifeTime = 0;
	int maxLifeTime = 120;
public:
	int damage = 5;
	MCB::Ray laser;
	bool deleteFlag = false;
	float maxRange = 100.0f;
	int rangeUpTime = 0;
	int rangeUpMaxTime = 10;
	float MaxRudius = 10;
	int rudiusDownTime = 0;
	int rudiusDownMaxTime = 10;

	void Update(MCB::Float3 startPosition, MCB::Vector3D frontVec);
	void Fire(MCB::Float3 startPosition, MCB::Vector3D frontVec,MCB::Model* laserModel);

};

