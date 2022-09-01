#include "CircumferenceEnemy.h"
#include "Util.h"
#include "Quaternion.h"
#include "NoHomingEnemyBullet.h"
#include <time.h>
using namespace MCB;


void CircumferenceEnemy::Update()
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

void CircumferenceEnemy::Rotasion()
{

	if (target == nullptr) return;
	nowFrontVec = NORM_FRONT_VEC;
	Vector3D targetVec;
	Vector3D DefoVec = { 0,0,1 };
	targetVec = targetVec.V3Get({ prevPosition.x,prevPosition.y,prevPosition.z }, { target->prevPosition.x , target->prevPosition.y , target->prevPosition.z });
	targetVec.V3Norm();
	Vector3D RotasionAxis = RotasionAxis.GetV3Cross(DefoVec, targetVec);
	RotasionAxis.V3Norm();
	float angle = DefoVec.GetV3Dot(targetVec);
	enemyQ.SetRota(RotasionAxis, angle);
	nowFrontVec = enemyQ.Vector3DRotasion(nowFrontVec, enemyQ);
	nowFrontVec = targetVec;
}

void CircumferenceEnemy::Attack()
{
	attacktime++;
	if (attacktime >= attackResponse)
	{
		std::unique_ptr<EnemyBullet> bullet;
		bullet = std::make_unique<NoHomingEnemyBullet>();
		bullet->Fire({ position.x,position.y,position.z }, nowFrontVec, nullptr);
		bullet->model = bulletModel;
		bullet->rotasion = rotasion;
		bullet->mapTexture = bulletMapTexture;
		bullets.push_back(std::move(bullet));
		attacktime = 0;
	}
}

void CircumferenceEnemy::Move()
{
	rotationAngle += 0.05f;

	if (target && !SetRota)
	{
		Vector3D temp = rotationVec;
		Vector3D tempVa;
		if (target->position.x != target->prevPosition.x || target->position.y != target->prevPosition.y || target->position.z != target->prevPosition.z) tempVa = tempVa.V3Get({ position.x,position.y,position.z }, { target->prevPosition.x , target->prevPosition.y , target->prevPosition.z });
		else tempVa = tempVa.V3Get({ position.x,position.y,position.z }, { target->position.x , target->position.y , target->position.z });
		tempVa.V3Norm();
		rotationVec = rotationVec.GetUpVec({ 0,0,1 }, tempVa);
		if (temp.vec.x != rotationVec.vec.x || temp.vec.y != rotationVec.vec.y || temp.vec.z != rotationVec.vec.z) SetRota = true;
	}
	Quaternion positionQ;
	positionQ = positionQ.SetRotationQuaternion(rotationVec, startPositionVec, rotationAngle);
	Vector3D PositionVec = { positionQ.x,positionQ.y,positionQ.z };
	PositionVec.V3Norm();
	Float3 tempPosition = { position.x,position.y,position.z };
	//position.x = PositionVec.vec.x * distance, position.y = PositionVec.vec.y * distance, position.z = PositionVec.vec.z * distance;
	if (target != nullptr)
	{
		position.x = PositionVec.vec.x * (distance - sinf(rotationAngle) * 10) + target->position.x;
		position.y = PositionVec.vec.y * (distance - sinf(rotationAngle) * 10) + target->position.y;
		position.z = PositionVec.vec.z * (distance - sinf(rotationAngle) * 10) + target->position.z;
	}



}

void CircumferenceEnemy::Init(Player* target, MCB::Float3 position, MCB::Model* model, MCB::Model* bulletModel, int AttackType)
{
	this->target = target;
	this->position.x = position.x;
	this->position.y = position.y;
	this->position.z = position.z;
	this->model = model;
	this->bulletModel = bulletModel;
	this->attackType = AttackType;
	scale = { 10,10,10 };
	startPositionVec.vec.x = this->position.x;
	startPositionVec.vec.y = this->position.y;
	startPositionVec.vec.z = this->position.z;
	rotationAngle = 0;
	attackResponse = 30;
	//startPositionVec.V3Norm();
	if (target)
	{
		Vector3D tempVa = tempVa.V3Get({ startPositionVec.vec.x, startPositionVec.vec.y,startPositionVec.vec.z }, { target->position.x , target->position.y , target->position.z });
		tempVa.V3Norm();
		rotationVec = rotationVec.GetUpVec({ 0,0,1 }, tempVa);
	}
	sprite = sprite.CreateSprite();
}