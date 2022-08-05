#include "exp.h"
#include "Util.h"

void Exp::Init(float Speed, MCB::Float3 position, MCB::Vector3D velosity)
{
	this->startSpeed = Speed;
	this->position = { position.x,position.y,position.z };
	this->velosity = velosity;
	this->velosity.V3Norm();

	time = 0;
}

void Exp::Update()
{
	
	if(maxTime > time) time++;
	if (time > maxTime) time = maxTime;
	speed = MCB::Lerp(startSpeed, 0, maxTime, time);
	position.x = velosity.vec.x * speed;
	position.y = velosity.vec.y * speed;
	position.z = velosity.vec.z * speed;
}

void Exp::ExpDraw()
{
	Draw();
}

void Exp::GetExp()
{
	deleteFlag = true;
}
