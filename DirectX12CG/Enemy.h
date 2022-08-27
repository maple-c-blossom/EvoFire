#pragma once
#include "Object3d.h"
#include "EnemyBullet.h"
#include "Player.h"
#include "Sprite.h"
#include "Texture.h"

class Enemy: public MCB::Object3d
{
protected:

	float speedFront = 0.0f;
	float speedRight = 0.0f;
	float rotasionSpeed = 0.025f;
	float speed = 2.0f;
	float Srowspeed = 0.25f;
	float maxspeed = 15.0f;
	int attackResponse = 60;
	int attacktime = 0;
	float Angle = 0;
	Player* target;
	int maxhp = 2;
	int hp = maxhp;
	int attackType = 0;
	int lifeTime = 0;
	int maxTime = 1200;
public:
	MCB::Sprite sprite;
	MCB::Float3 prevPosition;
	MCB::Texture* mapTexture;
	MCB::Texture* bulletMapTexture;
	static enum AttaxkType
	{
		NoHoming,
		Homing
	};
	float expPoint = 2 / 20.0f;
	float r = 10;
	bool deleteFlag = false;
	MCB::Model* bulletModel = nullptr;
	std::list<std::unique_ptr<EnemyBullet>> bullets;
	MCB::Quaternion enemyQ = { 0,0,0,1 };
	MCB::Vector3D rightVec;
	MCB::Vector3D UpVec;
	virtual void Update() = 0;
	//void Move();
	//void Rotasion();
	//void Attack();
	virtual void Init(Player* target,MCB::Float3 position,MCB::Model* model,MCB::Model* bulletModel,int AttackType = NoHoming) = 0;
	void Deth(int Damage);
	void AllMatrixUpdate(MCB::View view, MCB::Projection proj);
	void AllDraw();
	
};

