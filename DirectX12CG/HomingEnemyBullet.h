#pragma once
#include "EnemyBullet.h"
class HomingEnemyBullet:public EnemyBullet
{
public:
	void VelocityUpdate();
	void Update() override;
	void Fire(MCB::Float3 startPosition, MCB::Vector3D frontVec, Object3d* target) override;

};

