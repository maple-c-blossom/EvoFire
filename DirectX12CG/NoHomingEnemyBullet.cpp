#include "NoHomingEnemyBullet.h"
using namespace MCB;

void NoHomingEnemyBullet::Update()
{
	position.x += velocity.vec.x * speedOffSet;
	position.y += velocity.vec.y * speedOffSet;
	position.z += velocity.vec.z * speedOffSet;
	lifeTime++;
	if (lifeTime >= maxLifeTime)
	{
		deleteFlag = true;
	}

}

void NoHomingEnemyBullet::Fire(Float3 startPosition, Vector3D frontVec, Object3d* target)
{
	Init();
	speedOffSet =25;
	position.x = startPosition.x;
	position.y = startPosition.y;
	position.z = startPosition.z;
	this->target = target;
	if (target == nullptr)
	{
		velocity.V3Norm();
		velocity = frontVec;
	}
	else
	{
		velocity = velocity.V3Get(startPosition, { target->position.x,target->position.y,target->position.z });
		velocity.V3Norm();
	}
	lifeTime = 0;
	t = tUpdateMaxTime;
	sprite = sprite.CreateSprite();
	scale = { 12,12,12 };
}
