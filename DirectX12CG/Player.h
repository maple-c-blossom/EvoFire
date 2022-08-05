#pragma once
#include "Object3d.h"
#include "Input.h"
#include "PlayerBullet.h"

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

	int homingMissileCount = 0;
	int laserCount = 0;
	int bombCount = 0;

	const int maxHomingMissileCount = 3;
	const int maxLaserCount = 2;
	const int maxBombCount = 1;
	int exp = 0;
	Object3d* target;
	int attackTime = 0;
	int attackResponceTime = 20;
	int Level = 1;
	int nextLevelExp = 20;
	int maxhp = 20;
	int hp = maxhp;

public:
	float r = 10;
	MCB::Model* bulletModel = nullptr;
	std::list<std::unique_ptr<PlayerBullet>> bullets;
	MCB::Quaternion playerQ = {0,0,0,1};
	MCB::Vector3D rightVec;
	MCB::Vector3D UpVec;
	void PlayerInit();
	void Update();
	void Move();
	void Rotasion();
	void Attack();
	void GetSPAttack();
	void SetTarget(Object3d* target);
	void GetExp(int expPoint);
	void LevelUp();
};

