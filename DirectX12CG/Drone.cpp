#include "Drone.h"
using namespace MCB;
using namespace std;


void Drone::VelosityUpdate()
{
	Vector3D toPlayer;
	toPlayer = toPlayer.V3Get({ position.x,position.y,position.z }, targetPosition);
	toPlayer.V3Norm();
	velosity.V3Norm();
	Quaternion velocityQ = { velosity.vec.x,velosity.vec.y,velosity.vec.z,0 };
	Quaternion toPlayerQ = { toPlayer.vec.x,toPlayer.vec.y,toPlayer.vec.z,0 };
	velocityQ = velocityQ.Slerp(velocityQ, toPlayerQ, t, tUpdateMaxTime);

	if ((isfinite(velocityQ.x)) || (isfinite(velocityQ.y)) || (isfinite(velocityQ.z)) || (isfinite(velocityQ.w)))
	{
		velosity.vec = { velocityQ.x,velocityQ.y,velocityQ.z };
		velosity.V3Norm();
	}
	else
	{
		velosity.vec = { toPlayer.vec.x,toPlayer.vec.y,toPlayer.vec.z };
		velosity.V3Norm();
	}

}

void Drone::Update()
{

}

void Drone::Move()
{
	
	rotationAngle += 0.01f;
	Quaternion positionQ;
	positionQ = positionQ.SetRotationQuaternion(rotationVec, startPositionVec, rotationAngle);
	Vector3D PositionVec = { positionQ.x,positionQ.y,positionQ.z };
	PositionVec.V3Norm();
	Float3 tempPosition = { position.x,position.y,position.z };
	//position.x = PositionVec.vec.x * distance, position.y = PositionVec.vec.y * distance, position.z = PositionVec.vec.z * distance;

	if (BossPtr != nullptr)
	{
		nearTheBossPosition.x = PositionVec.vec.x * distance + BossPtr->position.x;
		nearTheBossPosition.y = PositionVec.vec.y * distance + BossPtr->position.y;
		nearTheBossPosition.z = PositionVec.vec.z * distance + BossPtr->position.z;
	}
	else
	{
		deleteFlag = true;
		if (target->GetTarget() == this)
		{
			target->SetTarget(nullptr);
		}
	}
	startPosition.x = nearTheBossPosition.x;
	startPosition.y = nearTheBossPosition.y;
	startPosition.z = nearTheBossPosition.z;
	if (FireFlag)
	{
		shootTimer++;
		tUpdateNowTime++;
		if (tUpdateNowTime >= tUpdateTime)
		{

			t--;
			if (t < 1)
			{
				t = 1;
			}
			tUpdateNowTime = 0;
		}
		VelosityUpdate();
		position.x += velosity.vec.x * speed;
		position.y += velosity.vec.y * speed;
		position.z += velosity.vec.z * speed;
		if (position.x == targetPosition.x && position.y == targetPosition.y && position.z == targetPosition.z || maxShootTime > maxShootTime)
		{
			backFlag = true;
			targetPosition = nearTheBossPosition;

		}

		if (backFlag && position.x == nearTheBossPosition.x && position.y == nearTheBossPosition.y && position.z == nearTheBossPosition.z)
		{
			position.x = nearTheBossPosition.x;
			position.y = nearTheBossPosition.y;
			position.z = nearTheBossPosition.z;
			FireFlag = false;
			maxShootTime = 0;
		}
	}
	else
	{
		position.x = nearTheBossPosition.x;
		position.y = nearTheBossPosition.y;
		position.z = nearTheBossPosition.z;
	}
	
}

void Drone::Init(Player* target, MCB::Float3 position, MCB::Model* model, int AttackType)
{

	this->target = target;
	this->position.x = position.x;
	this->position.y = position.y;
	this->position.z = position.z;
	this->model = model;
	this->attackType = AttackType;
	scale = { 10,10,10 };

	startPositionVec.vec.x = this->position.x;
	startPositionVec.vec.y = this->position.y;
	startPositionVec.vec.z = this->position.z;

	if (BossPtr != nullptr)
	{
		Vector3D bossFrontVec = BossPtr->nowFrontVec;
		bossFrontVec.V3Norm();
		rotationVec = rotationVec.GetUpVec({ 0,0,1 }, bossFrontVec);
	}
}

void Drone::Break(int Damage)
{
	hp -= Damage;
	if (hp < 0) deleteFlag = true;
}

void Drone::Shot()
{
	FireFlag = true;
}
