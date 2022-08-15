#pragma once
#include "Enemy.h"
class TurretEnemy :public Enemy
{
public:


	void Update() override;
	void Rotasion();
	void Attack();
	void Init(MCB::Object3d* target, MCB::Float3 position, MCB::Model* model, MCB::Model* bulletModel, int AttackType = NoHoming) override;

};

