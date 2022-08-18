#pragma once
#include "Object3d.h"
#include "EnemyBullet.h"
class Boss :public  MCB::Object3d
{
private:
	enum Status
	{
		HomingTurret,
		HomingRush,
		HomingCircumference
	};
	int attackTime = 0;
	int attackResponceTime = 20;
	int Level = 1;
	int nextLevelExp = 20;
	int maxhp = 200;
	int hp = maxhp;
	MCB::Object3d* playerPtr = nullptr;
public:
	float r = 10;
	std::list<std::unique_ptr<EnemyBullet>> bullets;
	MCB::Quaternion enemyQ = { 0,0,0,1 };
	MCB::Vector3D rightVec;
	MCB::Vector3D UpVec;
	void Update();
	void Move();
	void Rotasion();
	void Attack();
	void Init(MCB::Object3d* target, MCB::Float3 position, MCB::Model* model, MCB::Model* bulletModel);
};

