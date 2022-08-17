#pragma once
#include "Object3d.h"

class EnemyBullet: public MCB::Object3d
{
protected:
	MCB::Vector3D velocity = {};
	MCB::Object3d* target = nullptr;
public:
	float speedOffSet = 5;
	int lifeTime = 0;
	int maxLifeTime = 120;
	int damage = 1;
	int t = 2;
	const int defoT = 2;
	int tUpdateMaxTime = 10;
	int tUpdateTime = 5;
	int tUpdateNowTime = 0;
	float r = 10;
	float slerpStopR = 30 + r;
	bool deleteFlag = false;
	bool SlerpStop = false;
	//void VelocityUpdate();
	virtual void Update() = 0;
	void BulletHit();
	void SlerpHit();
	virtual void Fire(MCB::Float3 startPosition, MCB::Vector3D frontVec, Object3d* target) = 0;
};

