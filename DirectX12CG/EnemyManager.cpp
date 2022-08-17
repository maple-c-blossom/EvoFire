#include "EnemyManager.h"

void EnemyManager::enemyPop(MCB::Object3d* target, MCB::Float3 position, MCB::Model* model, MCB::Model* bulletModel, int PopEnemy, int attackType, Enemy* guardTarget)
{

	std::unique_ptr<Enemy> enemy;
	if (PopEnemy == Turret)
	{
		enemy = std::make_unique<TurretEnemy>();
	}
	else if (PopEnemy == Guard)
	{
		enemy = std::make_unique<GuardEnemy>(guardTarget);

	}
	else if (PopEnemy == Rash)
	{
		enemy = std::make_unique<RushEnemy>();
	}
	enemy->Init(target, position, model, bulletModel,attackType);
	enemys.push_back(std::move(enemy));

}

void EnemyManager::Update()
{
	for (std::unique_ptr<Enemy>& enemy : enemys)
	{
		if (enemys.size() > 30)
		{
			enemy.get()->deleteFlag = true;
			continue;
		}
		enemy.get()->Update();
	}
	enemys.remove_if([](std::unique_ptr<Enemy>& enemy) {return enemy->deleteFlag; });
}

void EnemyManager::Draw()
{
	for (std::unique_ptr<Enemy>& enemy : enemys)
	{
		enemy.get()->AllDraw();
	}
}

void EnemyManager::UpdateMatrix(MCB::View view, MCB::Projection proj)
{
	for (std::unique_ptr<Enemy>& enemy : enemys)
	{
		enemy.get()->AllMatrixUpdate(view,proj);
	}
}
