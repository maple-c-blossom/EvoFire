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
	if (guardPtr && !SetRota)
	{
		Vector3D temp = rotationVec;
		Vector3D tempVa;
		if(guardPtr->position.x != guardPtr->prevPosition.x || guardPtr->position.y != guardPtr->prevPosition.y || guardPtr->position.z != guardPtr->prevPosition.z) tempVa = tempVa.V3Get({ position.x,position.y,position.z }, { guardPtr->prevPosition.x , guardPtr->prevPosition.y , guardPtr->prevPosition.z });
		else tempVa = tempVa.V3Get({ position.x,position.y,position.z }, { guardPtr->position.x , guardPtr->position.y , guardPtr->position.z });
		tempVa.V3Norm();
		rotationVec = rotationVec.GetUpVec({ 0,0,1 }, tempVa);
		if(temp.vec.x != rotationVec.vec.x || temp.vec.y != rotationVec.vec.y || temp.vec.z != rotationVec.vec.z) SetRota = true;
	}
	Quaternion positionQ;
	positionQ = positionQ.SetRotationQuaternion(rotationVec, startPositionVec, rotationAngle);
	Vector3D PositionVec = { positionQ.x,positionQ.y,positionQ.z };
	PositionVec.V3Norm();
	Float3 tempPosition = { position.x,position.y,position.z };
	//position.x = PositionVec.vec.x * distance, position.y = PositionVec.vec.y * distance, position.z = PositionVec.vec.z * distance;
	if (guardPtr != nullptr)
	{
		position.x = PositionVec.vec.x * distance + guardPtr->position.x;
		position.y = PositionVec.vec.y * distance + guardPtr->position.y;
		position.z = PositionVec.vec.z * distance + guardPtr->position.z;
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
	startPositionVec.vec.x = this->position.x;
	startPositionVec.vec.y = this->position.y;
	startPositionVec.vec.z = this->position.z;
	rotationAngle = 0;
	//startPositionVec.V3Norm();
	if (guardPtr)
	{
		Vector3D tempVa = tempVa.V3Get({ startPositionVec.vec.x, startPositionVec.vec.y,startPositionVec.vec.z }, { guardPtr->position.x , guardPtr->position.y , guardPtr->position.z });
		tempVa.V3Norm();
		rotationVec = rotationVec.GetUpVec({0,0,1}, tempVa);
	}

}

void GuardEnemy::SetGuardTarget(Enemy* guardTarget)
{
	
}
