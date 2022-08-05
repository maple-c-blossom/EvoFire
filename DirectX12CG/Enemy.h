#pragma once
#include "Object3d.h"
#include "EnemyBullet.h"


class Enemy: public MCB::Object3d
{
private:
	float speedFront = 0.0f;
	float speedRight = 0.0f;
	float rotasionSpeed = 0.025f;
	float speed = 2.0f;
	float Srowspeed = 0.25f;
	float maxspeed = 15.0f;

	int attackResponse = 60;
	int attacktime = 0;
	int expPoint = 1;
	float Angle = 0;
	Object3d* target;
public:
	float r = 10;
	bool deleteFlag = false;
	MCB::Model* bulletModel = nullptr;
	std::list<std::unique_ptr<EnemyBullet>> bullets;
	MCB::Quaternion enemyQ = { 0,0,0,1 };
	MCB::Vector3D rightVec;
	MCB::Vector3D UpVec;
	void Update();
	void Move();
	void Rotasion();
	void Attack();
	void Init(MCB::Object3d* target,MCB::Float3 position,MCB::Model* model,MCB::Model* bulletModel);
	void Deth();
	void AllMatrixUpdate(MCB::View view, MCB::Projection proj);
	void AllDraw();
};

