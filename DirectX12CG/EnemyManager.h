#pragma once
#include "Enemy.h"
#include <memory>
#include <list>
class EnemyManager
{
public:
	std::list<std::unique_ptr<Enemy>>enemys;
	void enemyPop(MCB::Object3d* target, MCB::Float3 position, MCB::Model* model, MCB::Model* bulletModel);
	void Update();
	void Draw();
	void UpdateMatrix(MCB::View view, MCB::Projection proj);
};

