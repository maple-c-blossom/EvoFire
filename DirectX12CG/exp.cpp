#include "exp.h"


using namespace MCB;
using namespace std;

void Exp::ExpInit(float Speed, MCB::Float3 position, MCB::Vector3D velosity,MCB::Object3d* playerPtr)
{
	this->startSpeed = Speed;
	this->position = { position.x,position.y,position.z };
	this->velocity = velosity;
	this->velocity.V3Norm();
	this->playerPtr = playerPtr;
	time = 0;
	stopSlerp = false;
}

void Exp::Update()
{
	
	if(maxTime > time) time++;
	if (time > maxTime) time = maxTime;
	speed = MCB::OutQuad(startSpeed, 0, maxTime, time);
	position.x += velocity.vec.x * speed;
	position.y += velocity.vec.y * speed;
	position.z += velocity.vec.z * speed;

	if (speed <= 0 && sinFlag)
	{
		if (sinTimer > 360) sinTimer = 0;
		sinTimer++;
		position.y = sin(ConvertRadius(sinTimer)) * sinOffSet + prevPos.y;
	}
	else
	{
		prevPos.x = position.x;
		prevPos.y = position.y;
		prevPos.z = position.z;
	}

	lifeTime++;
	if (lifeTime >= maxLifeTime)
	{
		deleteFlag = true;
	}
}

void Exp::ExpDraw()
{
	Draw();
}

void Exp::GetExp()
{
	deleteFlag = true;
}

void Exp::StopSlerp()
{
	stopSlerp = true;
}

void Exp::ExpApproach()
{
	sinFlag = false;
	Float3 targetposition = { playerPtr->position.x,playerPtr->position.y,playerPtr->position.z };
	Vector3D toPlayer;
	toPlayer = toPlayer.V3Get({ position.x,position.y,position.z }, targetposition);
	float toPlayerLenge = toPlayer.V3Len();
	toPlayer.V3Norm();
	velocity.V3Norm();
	Quaternion velocityQ = { velocity.vec.x,velocity.vec.y,velocity.vec.z,0 };
	Quaternion toPlayerQ = { toPlayer.vec.x,toPlayer.vec.y,toPlayer.vec.z,0 };
	velocityQ = velocityQ.Slerp(velocityQ, toPlayerQ, 2, 10);
	if (((isfinite(velocityQ.x)) || (isfinite(velocityQ.y)) || (isfinite(velocityQ.z)) || (isfinite(velocityQ.w))) && !stopSlerp)
	{
		velocity.vec = { velocityQ.x,velocityQ.y,velocityQ.z };
		velocity.V3Norm();
	}
	else
	{
		velocity.vec = { toPlayer.vec.x,toPlayer.vec.y,toPlayer.vec.z };
		velocity.V3Norm();
	}
	float approachSpeed = (speedOffSet - toPlayerLenge / 300);
	if (approachSpeed < 0) approachSpeed = 1;
	position.x += velocity.vec.x * approachSpeed;
	position.y += velocity.vec.y * approachSpeed;
	position.z += velocity.vec.z * approachSpeed;



}

