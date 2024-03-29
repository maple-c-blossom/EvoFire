#include "HomingMissile.h"

using namespace MCB;

void HomingMissile::VelocityUpdate()
{
	for (int i = 0; i < MissileCount; i++)
	{
		if (deleteFlag[i] || target[i] == nullptr)
		{
			continue;
		}
		Float3 targetposition = { target[i]->position.x,target[i]->position.y,target[i]->position.z};
		Vector3D toEnemy;
		toEnemy = toEnemy.V3Get({ homingMissiles[i].position.x,homingMissiles[i].position.y,homingMissiles[i].position.z }, targetposition);
		toEnemy.V3Norm();
		velocity[i].V3Norm();
		Quaternion velocityQ = { velocity[i].vec.x,velocity[i].vec.y,velocity[i].vec.z,0};
		Quaternion toEnemyQ = { toEnemy.vec.x,toEnemy.vec.y,toEnemy.vec.z,0 };

		if ( lifeTime[i] > 10 && lifeTime[i] < maxLifeTime - 10 && !slerpStop[i])
		{
			velocityQ = velocityQ.Slerp(velocityQ, toEnemyQ, t[i], tUpdateMaxTime);
		}

		if (((isfinite(velocityQ.x)) || (isfinite(velocityQ.y)) || (isfinite(velocityQ.z)) || (isfinite(velocityQ.w))) && 
			lifeTime[i] > 10 && lifeTime[i] < maxLifeTime - 10 && !slerpStop[i])
		{
			velocity[i].vec = {velocityQ.x,velocityQ.y,velocityQ.z};
			velocity[i].V3Norm();
		}
		else if(lifeTime[i] > 10 )
		{
			velocity[i].vec = {toEnemy.vec.x,toEnemy.vec.y,toEnemy.vec.z};
			velocity[i].V3Norm();
		}


	}

}

void HomingMissile::Update()
{
	for (int i = 0; i < MissileCount; i++)
	{
		if (deleteFlag[i])
		{
			continue;
		}
		lifeTime[i]++;
		if (homingTime[i] < maxHomingTime)
		{
			homingTime[i]++;

		}
		if (maxSpeed[i] <= speed[i])
		{
			speed[i] = maxSpeed[i];
		}
		else if (maxSpeed[i] != prevMaxSpeed[i])
		{
			homingTime[i] = 0;
			speed[i] = InQuad(speed[i], maxSpeed[i], maxHomingTime, homingTime[i]);
		}
		else
		{
			speed[i] = InQuad(5, maxSpeed[i], maxHomingTime, homingTime[i]);
		}
		homingMissiles[i].position.x += velocity[i].vec.x * speed[i];
		homingMissiles[i].position.y += velocity[i].vec.y * speed[i];
		homingMissiles[i].position.z += velocity[i].vec.z * speed[i];
		//if(speedOffSet > 0 && maxLifeTime - lifeTime < 60)speedOffSet--;
		tUpdateNowTime[i]++;
		if (tUpdateNowTime[i] >= tUpdateTime[i])
		{
			t[i]--;
			if (t[i] < 1)
			{
				t[i] = 1;
			}
			tUpdateNowTime[i] = 0;
		}
		if (lifeTime[i] >= maxLifeTime)
		{
			deleteFlag[i] = true;
		}
	}

	if (deleteFlag[0] && deleteFlag[1] && deleteFlag[2])
	{
		allDeleteFlag = true;
	}
}

void HomingMissile::BulletHit(int number)
{
	if (number >= MissileCount) return;
	deleteFlag[number] = true;
}

void HomingMissile::SetSlerp(int number,bool flag)
{
	if (number >= MissileCount) return;
	slerpStop[number] = flag;
}

void HomingMissile::Fire(Float3 startPosition, Vector3D frontVec, Object3d* target, Model* model)
{
	for (int i = 0; i < MissileCount; i++)
	{
		homingMissiles[i].Init();
		speedOffSet = 50;
		homingMissiles[i].position.x = startPosition.x;
		homingMissiles[i].position.y = startPosition.y;
		homingMissiles[i].position.z = startPosition.z;
		homingMissiles[i].model = model;
		this->target[i] = target;
		t[i] = tUpdateMaxTime;
	}
	velocity[0] = frontVec.GetRightVec(frontVec);
	velocity[1] = -frontVec.GetRightVec(frontVec);
	velocity[2] = frontVec.GetUpVec(velocity[0],frontVec);
}

void HomingMissile::SetTarget(MCB::Object3d* target)
{

		for (int i = 0; i < MissileCount; i++)
		{
			if (this->target[i] == target)
			{
				break;
			}
			if (this->target[i] != target)
			{
				this->target[i] = target;
				homingTime[i] = maxHomingTime / 2;
			}
		}
}

void HomingMissile::SetTarget(MCB::Object3d* target, int number)
{
	if (number >= MissileCount || this->target[number] == target) return;
	if (this->target[number] != target)
	{
		this->target[number] = target;
		homingTime[number] = maxHomingTime / 2;
	}
}

void HomingMissile::SetMaxSpeed(int speed)
{
	for (int i = 0; i < MissileCount; i++)
	{
		prevMaxSpeed[i] = maxSpeed[i];
		maxSpeed[i] = speed;

	}
}

void HomingMissile::SetMaxSpeed(int speed, int number)
{
	prevMaxSpeed[number] = maxSpeed[number];
	maxSpeed[number] = speed;
}

int HomingMissile::GetMaxSpeed(int number)
{
	return maxSpeed[number];
}
