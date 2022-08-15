#include "EnemyBullet.h"

using namespace MCB;


void EnemyBullet::BulletHit()
{
	deleteFlag = true;
}

void EnemyBullet::SlerpHit()
{
	SlerpStop = true;
}

