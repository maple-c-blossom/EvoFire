#include "PlayerBullet.h"
#include "Quaternion.h"
using namespace MCB;

void PlayerBullet::VelocityUpdate()
{
	if (lifeTime < 3) return;
	if (target == nullptr) return;
	Float3 targetposition = { target->position.x,target->position.y,target->position.z };
	Vector3D toEnemy;
	toEnemy = toEnemy.V3Get({ position.x,position.y,position.z }, targetposition);
	toEnemy.V3Norm();
	velocity.V3Norm();
	Quaternion velocityQ = { velocity.vec.x,velocity.vec.y,velocity.vec.z,0 };
	Quaternion toEnemyQ = { toEnemy.vec.x,toEnemy.vec.y,toEnemy.vec.z,0 };

	if (lifeTime < maxLifeTime - 10 && !SlerpStop)
	{
		velocityQ = velocityQ.Slerp(velocityQ, toEnemyQ, t, tUpdateMaxTime);
	}

	if ((isfinite(velocityQ.x)) || (isfinite(velocityQ.y)) || (isfinite(velocityQ.z)) || (isfinite(velocityQ.w)))
	{
		velocity.vec = { velocityQ.x,velocityQ.y,velocityQ.z };
		velocity.V3Norm();
	}
	else
	{
		velocity.vec = { toEnemy.vec.x,toEnemy.vec.y,toEnemy.vec.z };
		velocity.V3Norm();
	}


}

void PlayerBullet::Update()
{
	position.x += velocity.vec.x * speedOffSet;
	position.y += velocity.vec.y * speedOffSet;
	position.z += velocity.vec.z * speedOffSet;
	//if(speedOffSet > 0 && maxLifeTime - lifeTime < 60)speedOffSet--;
	lifeTime++;
	if (lifeTime > 3)
	{
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
	}
	if (lifeTime >= maxLifeTime)
	{
		deleteFlag = true;
	}

	sprite.SpriteUpdate(sprite);
}

void PlayerBullet::BulletHit()
{
	deleteFlag = true;
}

void PlayerBullet::SlerpHit()
{
	SlerpStop = true;
}

void PlayerBullet::SetTarget(MCB::Object3d* target)
{
	this->target = target;
}

void PlayerBullet::Fire(Float3 startPosition, Vector3D frontVec,Object3d* target)
{
	Init();
	speedOffSet = 50;
	position.x = startPosition.x;
	position.y = startPosition.y;
	position.z = startPosition.z;
	velocity = frontVec;
	this->target = target;
	t = tUpdateMaxTime;
	sprite = sprite.CreateSprite();
}
