#pragma once
#include "Object3d.h"
#include "Collider.h"
#include "Sprite.h"

class PlayerBullet :public MCB::Object3d
{
private:
	MCB::Vector3D velocity = {};
	float speedOffSet = 50;
	int lifeTime = 0;
	int maxLifeTime = 120;
	MCB::Object3d* target = nullptr;
public:
	
	int damage = 1;
	int t = 2;
	const int defoT = 2;
	int tUpdateMaxTime = 10;
	int tUpdateTime = 50;
	int tUpdateNowTime = 0;
	float r = 10;
	float slerpStopR = 15 + r;
	bool deleteFlag = false;
	bool SlerpStop = false;
	MCB::Sprite sprite;
	void VelocityUpdate();
	void Update();
	void BulletHit();
	void SlerpHit();
	void SetTarget(MCB::Object3d* target);
	void Fire(MCB::Float3 startPosition, MCB::Vector3D frontVec, Object3d* target);
};