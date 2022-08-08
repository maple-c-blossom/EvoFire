#pragma once
#include "Object3d.h"
#include "Collider.h"
#include "Util.h"
class HomingMissile
{
private:
	static const int MissileCount = 3;
	int damage = 50;
	MCB::Vector3D velocity[MissileCount] = {  };
	float speedOffSet = 5;
	int lifeTime[MissileCount] = {0,0,0};
	int homingTime[MissileCount] = {0,0,0};
	static const int maxLifeTime = 120;
	MCB::Object3d* target[MissileCount] = {nullptr,nullptr,nullptr};
public:
	MCB::Object3d homingMissiles[MissileCount];
	int t[MissileCount] = {2,2,2};
	const int defoT = 2;
	int tUpdateMaxTime = 10;
	int tUpdateTime[MissileCount] = {5,5,5};
	int tUpdateNowTime[MissileCount] = {};
	float r = 10;
	float slerpStopR = 15 + r;
	bool deleteFlag[MissileCount] = {false,false,false};

	bool AllDeleteFlag = false;
	bool SlerpStop[MissileCount] = { false,false,false };
	void VelocityUpdate();
	void Update();
	void BulletHit(int number);
	void SetSlerp(int number,bool flag);
	void Fire(MCB::Float3 startPosition, MCB::Vector3D frontVec,MCB::Object3d* target,MCB::Model* model);
	void SetTarget(MCB::Object3d* target);
	void SetTarget(MCB::Object3d* target,int number);
};

