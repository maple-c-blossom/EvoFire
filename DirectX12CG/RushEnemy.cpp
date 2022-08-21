#include "RushEnemy.h"
#include "NoHomingEnemyBullet.h"
using namespace MCB;

void RushEnemy::Update()
{
	prevPosition.x = position.x, prevPosition.y = position.y, prevPosition.z = position.z;
	lifeTime++;
	if (lifeTime > maxLifeTime)
	{
		deleteFlag = true;
		if (target->GetTarget() == this)
		{
			target->SetTarget(nullptr);
		}
	}
	if (!deleteFlag)
	{
		Move();
		Rotasion();
		Attack();
	}
	for (std::unique_ptr<EnemyBullet>& bullet : bullets)
	{
		bullet->Update();
	}
	bullets.remove_if([](std::unique_ptr<EnemyBullet>& bullet) {return bullet->deleteFlag; });
	
}

void RushEnemy::Rotasion()
{

	if (target == nullptr) return;
	Vector3D targetVec = nowFrontVec;
	nowFrontVec = NORM_FRONT_VEC;
	Vector3D DefoVec = { 0,0,1 };
	Vector3D RotasionAxis = RotasionAxis.GetV3Cross(DefoVec, targetVec);
	RotasionAxis.V3Norm();
	float angle = DefoVec.GetV3Dot(targetVec);
	enemyQ.SetRota(RotasionAxis, angle);
	nowFrontVec = targetVec;

}

void RushEnemy::Move()
{
	if (target == nullptr) return;
	Float3 targetposition = { target->position.x,target->position.y,target->position.z };
	Vector3D toPlayer;
	toPlayer = toPlayer.V3Get({ position.x,position.y,position.z }, targetposition);
	toPlayer.V3Norm();
	nowFrontVec.V3Norm();
	Quaternion velocityQ = { nowFrontVec.vec.x,nowFrontVec.vec.y,nowFrontVec.vec.z,0 };
	Quaternion toPlayerQ = { toPlayer.vec.x,toPlayer.vec.y,toPlayer.vec.z,0 };
	if (lifeTime < maxLifeTime - 10 && target != nullptr)
	{
		velocityQ = velocityQ.Slerp(velocityQ, toPlayerQ, 5, 10);
	}

	if ((isfinite(velocityQ.x)) || (isfinite(velocityQ.y)) || (isfinite(velocityQ.z)) || (isfinite(velocityQ.w)))
	{
		nowFrontVec.vec = { velocityQ.x,velocityQ.y,velocityQ.z };
		nowFrontVec.V3Norm();
	}
	else
	{
		nowFrontVec.vec = { toPlayer.vec.x,toPlayer.vec.y,toPlayer.vec.z };
		nowFrontVec.V3Norm();
	}
	position.x += nowFrontVec.vec.x * 15;
	position.y += nowFrontVec.vec.y * 15;
	position.z += nowFrontVec.vec.z * 15;
}

void RushEnemy::Attack()
{
	attacktime++;
	if (attacktime >= attackResponse)
	{
		std::unique_ptr<EnemyBullet> bullet;
		bullet = std::make_unique<NoHomingEnemyBullet>();
		bullet->Fire({ position.x,position.y,position.z }, nowFrontVec, nullptr);
		bullet->model = bulletModel;
		bullet->scale = { 6,6,6 };
		bullet->rotasion = rotasion;
		bullet->mapTexture = bulletMapTexture;
		bullets.push_back(std::move(bullet));
		attacktime = 0;
	}
}

void RushEnemy::Init(Player* target, MCB::Float3 position, MCB::Model* model, MCB::Model* bulletModel, int AttackType)
{

	this->target = target;
	this->position.x = position.x;
	this->position.y = position.y;
	this->position.z = position.z;
	this->model = model;
	this->bulletModel = bulletModel;
	this->attackType = AttackType;
	this->maxhp = 2;
	this->hp = maxhp;
	scale = { 10,10,10 };
	sprite = sprite.CreateSprite();
}
