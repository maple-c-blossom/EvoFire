#pragma once
#include "Enemy.h"
class GuardEnemy:public Enemy
{
public:
	Object3d* guardPtr = nullptr;
	void Update() override;
	void Rotasion();
	void Attack();
	void Move();
	void Init(MCB::Object3d* target, MCB::Float3 position, MCB::Model* model, MCB::Model* bulletModel, int AttackType = NoHoming) override;
};

