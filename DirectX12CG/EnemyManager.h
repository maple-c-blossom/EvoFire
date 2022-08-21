#pragma once
#include "TurretEnemy.h"
#include "GuardEnemy.h"
#include "RushEnemy.h"
#include <memory>
#include <list>
class EnemyManager
{
public:
	static enum PopEnemyNum
	{
		Turret,
		Guard,
		Rash,
		Circumference
	};
	std::list<std::unique_ptr<Enemy>>enemys;
	void enemyPop(Player* target, MCB::Float3 position, MCB::Model* model, MCB::Model* bulletModel,MCB::Texture* mapTexture,MCB::Texture* bulletmapTexture, int PopEnemy = Turret,int attackType = Enemy::NoHoming,Enemy* guardTarget = nullptr);
	void Update();
	void Draw();
	void UpdateMatrix(MCB::View view, MCB::Projection proj);
};

