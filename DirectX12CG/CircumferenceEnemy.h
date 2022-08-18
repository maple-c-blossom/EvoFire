#pragma once
#include "Enemy.h"
class CircumferenceEnemy:public Enemy
{
public:
	int lifeTime = 0;
	int maxLifeTime = 1200;
	MCB::Vector3D rotationVec;
	float rotationAngle;
	MCB::Vector3D startPositionVec;
	float distance = 500;
	bool SetRota = false;
	void Update() override;
	void Rotasion();
	void Attack();
	void Move();
	void Init(Player* target, MCB::Float3 position, MCB::Model* model, MCB::Model* bulletModel, int AttackType = NoHoming) override;
};

