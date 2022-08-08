#pragma once
#include "Object3d.h"
#include "Input.h"
#include "PlayerBullet.h"
#include "HomingMissile.h"

class Player:public MCB::Object3d
{
private:
	MCB::Input* input = MCB::Input::GetInstance();
	float speedFront = 0.0f;
	float speedRight = 0.0f;
	float rotasionSpeed = 0.025f;
	float speed = 2.0f;
	float Srowspeed = 0.25f;
	float maxspeed = 15.0f;

public:
	int homingMissileCount = 0;
	int laserCount = 0;
	int bombCount = 0;

private:
	const int maxHomingMissileCount = 3;
	const int maxLaserCount = 2;
	const int maxBombCount = 1;
	Object3d* target = nullptr;
	Object3d* homingTarget = nullptr;
	int attackTime = 0;
	int attackResponceTime = 20;
	int maxhp = 20;
	int hp = maxhp;

public:
	float exp = 0;
	int Level = 1;
	int nextLevelExp = 20;
	float r = 10;
	bool homingMissileGet = false;
	bool LaserGet = false;
	bool BombGet = false;
	MCB::Model* bulletModel = nullptr;
	MCB::Model* missileModel = nullptr;
	std::list<std::unique_ptr<PlayerBullet>> bullets;
	std::list<std::unique_ptr<HomingMissile>> homingMissile;
	MCB::Quaternion playerQ = {0,0,0,1};
	MCB::Vector3D rightVec;
	MCB::Vector3D UpVec;
	void PlayerInit(MCB::Model* model, MCB::Model* bulletModel, MCB::Model* missileModel);
	void Update();
	void Move();
	void Rotasion();
	void Attack();
	void GetSPAttack();
	void SetTarget(Object3d* target);
	void GetExp(float expPoint);
	void LevelUp();
	void SPAttack();
	void AllDraw();
	MCB::Object3d* GetTarget();
	void SetHomingTarget(Object3d* target);
	MCB::Object3d* GetHomingTarget();

};

