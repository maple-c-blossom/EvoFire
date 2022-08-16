#include "GuardEnemy.h"
#include "Util.h"
#include "Quaternion.h"

using namespace MCB;

GuardEnemy::GuardEnemy(Enemy* guardTarget)
{
	guardPtr = guardTarget;
}

void GuardEnemy::Update()
{
	if (guardPtr != nullptr)
	{
		if (guardPtr->deleteFlag)
		{
			guardPtr = nullptr;
		}
	}
	Move();
}

void GuardEnemy::Rotasion()
{

}

void GuardEnemy::Attack()
{

}

void GuardEnemy::Move()
{
	rotationAngle += 0.01f;
	Quaternion positionQ;
	positionQ = positionQ.SetRotationQuaternion(rotationVec, startPositionVec, rotationAngle);
	Vector3D PositionVec = { positionQ.x,positionQ.y,positionQ.z };
	PositionVec.V3Norm();
	Float3 tempPosition = { position.x,position.y,position.z };
	position.x = PositionVec.vec.x * distance, position.y = PositionVec.vec.y * distance, position.z = PositionVec.vec.z * distance;
	if (guardPtr != nullptr)
	{
		position.x += guardPtr->position.x;
		position.y += guardPtr->position.y;
		position.z += guardPtr->position.z;
	}
	else
	{
		deleteFlag = true;
	}


}

void GuardEnemy::Init(MCB::Object3d* target, MCB::Float3 position, MCB::Model* model, MCB::Model* bulletModel, int AttackType)
{
	this->target = target;
	this->position.x = position.x;
	this->position.y = position.y;
	this->position.z = position.z;
	this->model = model;
	this->bulletModel = bulletModel;
	this->attackType = AttackType;
	scale = { 10,10,10 };
	startPositionVec.vec = position;
	startPositionVec.V3Norm();
	if (guardPtr)
	{
		Vector3D tempVa = tempVa.V3Get(startPositionVec.vec, { guardPtr->position.x,guardPtr->position.y,guardPtr->position.z });
		tempVa.V3Norm();
		Vector3D tempVb = tempVb.GetRightVec(tempVa);
		//rotationVec = rotationVec.GetUpVec(tempVb, tempVa);
		rotationVec = tempVb;
	}

}

void GuardEnemy::SetGuardTarget(Enemy* guardTarget)
{
	
}
