#include "EnemyManager.h"

void EnemyManager::enemyPop(MCB::Object3d* target, MCB::Float3 position, MCB::Model* model, MCB::Model* bulletModel,int attackType)
{
	int PopEnemyNum = 0;
	if (PopEnemyNum == 0)
	{
		std::unique_ptr<TurretEnemy> enemy = std::make_unique<TurretEnemy>();
		enemy->Init(target, position, model, bulletModel,attackType);
		enemys.push_back(std::move(enemy));
	}

}

void EnemyManager::Update()
{
	for (std::unique_ptr<Enemy>& enemy : enemys)
	{
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
