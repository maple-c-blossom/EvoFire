#include "TurretEnemy.h"
#include "HomingEnemyBullet.h"
#include "NoHomingEnemyBullet.h"
using namespace MCB;

#include "Input.h"

void TurretEnemy::Update()
{
	lifeTime++;
	if (maxTime < lifeTime)
	{
		deleteFlag = true;
	}
	prevPosition.x = position.x;
	prevPosition.y = position.y;
	prevPosition.z = position.z;
	Rotasion();
	Attack();
	for (std::unique_ptr<EnemyBullet>& bullet : bullets)
	{
		bullet->Update();
	}
	bullets.remove_if([](std::unique_ptr<EnemyBullet>& bullet) {return bullet->deleteFlag; });

}

void TurretEnemy::Rotasion()
{
	if (target == nullptr) return;
	nowFrontVec = NORM_FRONT_VEC;
	Vector3D targetVec;
	Vector3D DefoVec = NORM_FRONT_VEC;
	targetVec = targetVec.V3Get({ position.x,position.y,position.z }, { target->position.x,target->position.y,target->position.z });
	targetVec.V3Norm();
	Vector3D RotasionAxis = RotasionAxis.GetV3Cross(DefoVec, targetVec);
	RotasionAxis.V3Norm();
	float angle = DefoVec.GetV3Dot(targetVec);
	enemyQ.SetRota(RotasionAxis, angle);
	nowFrontVec = enemyQ.Vector3DRotasion(nowFrontVec, enemyQ);
}

void TurretEnemy::Attack()
{
	attacktime++;
	if (attacktime >= attackResponse)
	{
		std::unique_ptr<EnemyBullet> bullet;
		if (attackType == Homing)
		{
			bullet = std::make_unique<HomingEnemyBullet>();
			bullet->Fire({ position.x,position.y,position.z }, nowFrontVec, target);
		}
		else if (attackType == NoHoming)
		{
			bullet = std::make_unique<NoHomingEnemyBullet>();
			bullet->Fire({ position.x,position.y,position.z }, nowFrontVec, target);
		}
		bullet->model = bulletModel;
		bullet->rotasion = rotasion;
		bullet->mapTexture = bulletMapTexture;
		bullets.push_back(std::move(bullet));
		attacktime = 0;
	}
}

void TurretEnemy::Init(Player* target, MCB::Float3 position, MCB::Model* model, MCB::Model* bulletModel, int AttackType)
{
	this->target = target;
	this->position.x = position.x;
	this->position.y = position.y;
	this->position.z = position.z;
	this->model = model;
	this->bulletModel = bulletModel;
	this->attackType = AttackType;
	scale = { 10,10,10 };
	sprite = sprite.CreateSprite();
}