#pragma once
#include "Enemy.h"

class RushEnemy :public Enemy
{
public:
	int lifeTime = 0;
	int maxLifeTime = 1200;
	void Update() override;
	void Move();
	void Rotasion();
	void Attack();
	void Init(MCB::Object3d* target, MCB::Float3 position, MCB::Model* model, MCB::Model* bulletModel, int AttackType = NoHoming) override;

};

