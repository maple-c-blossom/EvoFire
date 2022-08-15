#include "HomingEnemyBullet.h"

using namespace MCB;
void HomingEnemyBullet::VelocityUpdate()
{
	if(target == nullptr) return;
	Float3 targetposition = { target->position.x,target->position.y,target->position.z };
	Vector3D toPlayer;
	toPlayer = toPlayer.V3Get({ position.x,position.y,position.z }, targetposition);
	toPlayer.V3Norm();
	velocity.V3Norm();
	Quaternion velocityQ = { velocity.vec.x,velocity.vec.y,velocity.vec.z,0 };
	Quaternion toPlayerQ = { toPlayer.vec.x,toPlayer.vec.y,toPlayer.vec.z,0 };
	if (lifeTime < maxLifeTime - 10 && !SlerpStop && target != nullptr)
	{
		velocityQ = velocityQ.Slerp(velocityQ, toPlayerQ, t, tUpdateMaxTime);
	}

	if ((isfinite(velocityQ.x)) || (isfinite(velocityQ.y)) || (isfinite(velocityQ.z)) || (isfinite(velocityQ.w)))
	{
		velocity.vec = { velocityQ.x,velocityQ.y,velocityQ.z };
		velocity.V3Norm();
	}
	else
	{
		velocity.vec = { toPlayer.vec.x,toPlayer.vec.y,toPlayer.vec.z };
		velocity.V3Norm();
	}

}

void HomingEnemyBullet::Update()
{
	VelocityUpdate();
	position.x += velocity.vec.x * speedOffSet;
	position.y += velocity.vec.y * speedOffSet;
	position.z += velocity.vec.z * speedOffSet;
	//if(speedOffSet > 0 && maxLifeTime - lifeTime < 60)speedOffSet--;
	lifeTime++;
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
	if (lifeTime >= maxLifeTime)
	{
		deleteFlag = true;
	}
}

void HomingEnemyBullet::Fire(Float3 startPosition, Vector3D frontVec, Object3d* target)
{
	Init();
	speedOffSet = 25;
	position.x = startPosition.x;
	position.y = startPosition.y;
	position.z = startPosition.z;
	velocity = frontVec;
	this->target = target;
	lifeTime = 0;
	t = tUpdateMaxTime;
}
