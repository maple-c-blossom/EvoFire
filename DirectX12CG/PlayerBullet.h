#pragma once
#include "Object3d.h"
#include "Collider.h"

class PlayerBullet :public MCB::Object3d
{
private:
	static const int BULLET_NUM = 3;
	int damage = 1;
	MCB::Sphere bullet = {};
	MCB::Vector3D velocity = {};
	float r = 10;
	float speedOffSet = 50;
	int lifeTime = 0;
	int maxLifeTime = 60;
public:
	bool deleteFlag = false;
	void Update(MCB::Vector3D targetPosition);
	void Update();
	void Fire(MCB::Float3 startPosition, MCB::Vector3D frontVec);
};