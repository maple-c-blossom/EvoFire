#pragma once
#include "Enemy.h"
class GuardEnemy:public Enemy
{
public:
	GuardEnemy(Enemy* guardTarget);
	MCB::Vector3D rotationVec;
	float rotationAngle;
	MCB::Vector3D startPositionVec;
	float distance= 80;
	Enemy* guardPtr = nullptr;
	void Update() override;
	void Rotasion();
	void Attack();
	void Move();
	void Init(MCB::Object3d* target, MCB::Float3 position, MCB::Model* model, MCB::Model* bulletModel, int AttackType = NoHoming) override;
	void SetGuardTarget(Enemy* guardTarget);
};

