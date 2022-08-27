#pragma once
#include "Object3d.h"
#include "Player.h"

class Drone :public MCB::Object3d
{
public:
	static enum AttaxkType
	{
		NoHoming,
		Homing
	};
	float rotationAngle;
	MCB::Float3 nearTheBossPosition;
	float speedFront = 0.0f;
	float speedRight = 0.0f;
	float rotasionSpeed = 0.025f;
	float maxSpeed = 50.0f;
	float speed = maxSpeed;
	int attackResponse = 60;
	int attacktime = 0;
	float Angle = 0;
	Player* target;
	MCB::Object3d* BossPtr;
	int maxhp = 20;
	int hp = maxhp;
	int attackType = 0;
	MCB::Float3 prevPosition;
	float expPoint = 2 / 20.0f;
	float r = 15;
	float damage = 5;
	bool deleteFlag = false;
	MCB::Quaternion enemyQ = { 0,0,0,1 };
	MCB::Vector3D rightVec;
	MCB::Vector3D UpVec;
	MCB::Vector3D rotationVec;
	MCB::Vector3D startPositionVec;
	bool SetRota = false;
	float distance = 250;
	bool FireFlag;
	bool prevFire;
	bool goFlag;
	bool backFlag;
	int timer = 0;
	int shootTimer = 0;
	int maxShootTime = 60;
	MCB::Sprite sprite;
	MCB::Texture* mapTexture;
	MCB::Float3 startPosition;
	MCB::Float3 targetPosition;
	MCB::Vector3D velosity;
	int t = 2;
	const int defoT = 2;
	int tUpdateMaxTime = 60;
	int tUpdateTime = 5;
	int tUpdateNowTime = 0;
	bool yourFront = true;


	void VelosityUpdate();
	void Update();
	void Move();
	void Rotasion();
	void Attack();
	void Init(Player* target, MCB::Float3 position, MCB::Model* model, int AttackType = NoHoming);
	void Break(int Damage);
	void Shot();
	
};
