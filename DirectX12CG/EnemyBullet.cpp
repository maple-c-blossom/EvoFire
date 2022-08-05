#include "EnemyBullet.h"

using namespace MCB;


void EnemyBullet::VelocityUpdate()
{
	Float3 targetposition = { target->position.x,target->position.y,target->position.z };
	Vector3D toEnemy;
	toEnemy = toEnemy.V3Get({ position.x,position.y,position.z }, targetposition);
	toEnemy.V3Norm();
	velocity.V3Norm();
	Quaternion velocityQ = { velocity.vec.x,velocity.vec.y,velocity.vec.z,0 };
	Quaternion toEnemyQ = { toEnemy.vec.x,toEnemy.vec.y,toEnemy.vec.z,0 };
	if (lifeTime < maxLifeTime - 10 && !SlerpStop && target != nullptr)
	{
		velocityQ = velocityQ.Slerp(velocityQ, toEnemyQ, t, tUpdateMaxTime);
		velocity.vec = { velocityQ.x,velocityQ.y,velocityQ.z };
	}


}

void EnemyBullet::Update()
{
	position.x += velocity.vec.x * speedOffSet;
	position.y += velocity.vec.y * speedOffSet;
	position.z += velocity.vec.z * speedOffSet;
	//if(speedOffSet > 0 && maxLifeTime - lifeTime < 60)speedOffSet--;
	lifeTime++;

	if (lifeTime >= maxLifeTime)
	{
		deleteFlag = true;
	}
}

void EnemyBullet::BulletHit()
{
	deleteFlag = true;
}

void EnemyBullet::SlerpHit()
{
	SlerpStop = true;
}

void EnemyBullet::Fire(Float3 startPosition, Vector3D frontVec, Object3d* target)
{
	Init();
	speedOffSet = 25;
	position.x = startPosition.x;
	position.y = startPosition.y;
	position.z = startPosition.z;
	velocity = frontVec;
	this->target = target;
}
