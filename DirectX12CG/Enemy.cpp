#include "Enemy.h"
using namespace MCB;
using namespace std;

void Enemy::Update()
{
	Rotasion();
	Move();
	Attack();
	for (std::unique_ptr<EnemyBullet>& bullet : bullets)
	{
		if (target != nullptr)
		{
			bullet->VelocityUpdate();
		}
		bullet->Update();
	}
	bullets.remove_if([](std::unique_ptr<EnemyBullet>& bullet) {return bullet->deleteFlag; });

}

void Enemy::Move()
{

}

void Enemy::Rotasion()
{
	nowFrontVec = NORM_FRONT_VEC;
	Vector3D targetVec;
	Vector3D DefoVec = { 0,0,1 };
	targetVec.V3Get({ position.x,position.y,position.z }, { target->position.x,target->position.y,target->position.z });
	Vector3D RotasionAxis = RotasionAxis.GetV3Cross(DefoVec, targetVec);
	float angle = DefoVec.GetV3Dot(targetVec);

	enemyQ.SetRota(RotasionAxis, angle);
	nowFrontVec = enemyQ.Vector3DRotasion(nowFrontVec, enemyQ);
}

void Enemy::Attack()
{
	attacktime++;
	if (attacktime >= attackResponse)
	{
		std::unique_ptr<EnemyBullet> bullet = std::make_unique<EnemyBullet>();
		bullet->Fire({ position.x,position.y,position.z }, nowFrontVec, target);
		bullet->model = bulletModel;
		bullet->scale = { 6,6,6 };
		bullet->rotasion = rotasion;
		bullets.push_back(std::move(bullet));
		attacktime = 0;
	}
}

void Enemy::Init(MCB::Object3d* target, MCB::Float3 position, MCB::Model* model, MCB::Model* bulletModel)
{
	this->target = target;
	this->position.x = position.x;
	this->position.y = position.y;
	this->position.z = position.z;
	this->model = model;
	this->bulletModel = bulletModel;
	scale = { 10,10,10 };
}

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

