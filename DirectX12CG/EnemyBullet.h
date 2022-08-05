#pragma once
#include "Object3d.h"

class EnemyBullet: public MCB::Object3d
{
private:
	int damage = 1;
	MCB::Vector3D velocity = {};
	float speedOffSet = 5;
	int lifeTime = 0;
	int maxLifeTime = 120;
	MCB::Object3d* target = nullptr;
public:
	int t = 2;
	const int defoT = 2;
	int tUpdateMaxTime = 10;
	int tUpdateTime = 5;
	int tUpdateNowTime = 0;
	float r = 10;
	float slerpStopR = 30 + r;
	bool deleteFlag = false;
	bool SlerpStop = false;
	void VelocityUpdate();
	void Update();
	void BulletHit();
	void SlerpHit();
	void Fire(MCB::Float3 startPosition, MCB::Vector3D frontVec, Object3d* target);
};

