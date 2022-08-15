#pragma once
#include "EnemyBullet.h"
class NoHomingEnemyBullet:public EnemyBullet
{
public:
	void Update() override;
	void Fire(MCB::Float3 startPosition, MCB::Vector3D frontVec, Object3d* target) override;

};

