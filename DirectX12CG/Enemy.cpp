#include "Enemy.h"
using namespace MCB;
using namespace std;



void Enemy::Deth(int Damage)
{
	hp -= Damage;
	if(hp < 0) deleteFlag = true;
}

void Enemy::AllMatrixUpdate(MCB::View view, MCB::Projection proj)
{
	MatrixUpdata(view, proj, enemyQ);
	for (std::unique_ptr<EnemyBullet>& bullet : bullets)
	{
		bullet->MatrixUpdata(view, proj);
	}
}

void Enemy::AllDraw()
{
	Draw();
	for (std::unique_ptr<EnemyBullet>& bullet : bullets)
	{
		bullet->Draw();
	}
}

