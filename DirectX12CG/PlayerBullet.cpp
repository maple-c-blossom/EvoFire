#include "PlayerBullet.h"
#include "Quaternion.h"
using namespace MCB;

void PlayerBullet::Update( MCB::Vector3D targetPosition)
{
	//for (int i = 0; i < BULLET_NUM; i++)
	//{
	//	Vector3D toEnemy;
	//	toEnemy = toEnemy.V3Get(bullet[i].centerPosition, targetPosition.vec);

	//	toEnemy.V3Norm();
	//	velocity[i].V3Norm();
	//	Quaternion velocityQ = { velocity[i].vec.x,velocity[i].vec.y,velocity[i].vec.z,0 };
	//	Quaternion toEnemyQ = { toEnemy.vec.x,toEnemy.vec.y,toEnemy.vec.z,0 };
	//	velocityQ = velocityQ.Slerp(velocityQ, toEnemyQ, 2, 10);
	//	velocity[i].vec = { velocityQ.x,velocityQ.y,velocityQ.z };

	//}

}

void PlayerBullet::Update()
{
	bullet.centerPosition.x += velocity.vec.x * speedOffSet;
	bullet.centerPosition.y += velocity.vec.y * speedOffSet;
	bullet.centerPosition.z += velocity.vec.z * speedOffSet;
	position = { bullet.centerPosition.x,bullet.centerPosition.y,bullet.centerPosition.z };
	lifeTime++;

	if (lifeTime >= maxLifeTime)
	{
		deleteFlag = true;
	}
}

void PlayerBullet::Fire(Float3 startPosition, Vector3D frontVec)
{
	Init();
	
	bullet.centerPosition.x = startPosition.x;
	bullet.centerPosition.y = startPosition.y;
	bullet.centerPosition.z = startPosition.z;
	position = { bullet.centerPosition.x,bullet.centerPosition.y,bullet.centerPosition.z };
	velocity = frontVec;
}
