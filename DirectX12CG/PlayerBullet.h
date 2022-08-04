#pragma once
#include "Object3d.h"
#include "Collider.h"

class PlayerBullet :public MCB::Object3d
{
private:
	static const int BULLET_NUM = 3;
	int damage = 1;
	MCB::Vector3D velocity = {};
	float speedOffSet = 50;
	int lifeTime = 0;
	int maxLifeTime = 120;
public:
	int t = 2;
	const int defoT = 2;
	int tUpdateMaxTime = 10;
	int tUpdateTime = 10;
	int tUpdateNowTime = 0;
	float r = 10;
	bool deleteFlag = false;
	void VelocityUpdate(MCB::Vector3D targetPosition);
	void Update();
	void BulletHit();
	void Fire(MCB::Float3 startPosition, MCB::Vector3D frontVec);
};