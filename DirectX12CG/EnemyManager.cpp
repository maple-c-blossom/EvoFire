#include "EnemyManager.h"

void EnemyManager::enemyPop(MCB::Object3d* target, MCB::Float3 position, MCB::Model* model, MCB::Model* bulletModel)
{
	std::unique_ptr<Enemy> enemy = std::make_unique<Enemy>();
	enemy->Init(target, position, model, bulletModel);
	enemys.push_back(std::move(enemy));

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
