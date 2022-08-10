#include "Laser.h"
#include "Util.h"
using namespace MCB;

void Laser::Update(MCB::Float3 startPosition, MCB::Vector3D frontVec)
{
	if (lifeTime < maxLifeTime - rudiusDownMaxTime)
	{
		if (rangeUpMaxTime > rangeUpTime) rangeUpTime++;
		if (rudiusDownMaxTime > rudiusDownTime) rudiusDownTime++;
	}
	else
	{
		if (rudiusDownTime > 0) rudiusDownTime--;
	}
	laser.range = OutQuad(0, maxRange, rangeUpMaxTime, rangeUpTime);
	laser.radius = InQuad(0, MaxRudius, rudiusDownMaxTime, rudiusDownTime);
	laser.StartPosition = startPosition;
	laser.rayVec = frontVec;
	position.x = laser.StartPosition.x + laser.rayVec.vec.x * (laser.range / 2);
	position.y = laser.StartPosition.y + laser.rayVec.vec.y * (laser.range / 2);
	position.z = laser.StartPosition.z + laser.rayVec.vec.z * (laser.range / 2);
	lifeTime++;
	if (lifeTime > maxLifeTime)
	{
		deleteFlag = true;
	}


	scale.z = laser.range;
	scale.x = laser.radius;
	scale.y = laser.radius;
}

void Laser::Fire(MCB::Float3 startPosition, MCB::Vector3D frontVec, MCB::Model* laserModel)
{
	Init();
	laser.StartPosition = startPosition;
	position.x = startPosition.x;
	position.y = startPosition.y;
	position.z = startPosition.z;
	laser.rayVec = frontVec;
	rangeUpTime;
	model = laserModel;
	laser.radius = 0;
	scale = { 1,1,1 };
}
