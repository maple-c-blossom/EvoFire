#pragma once
#include "Enemy.h"
#include <memory>
#include <list>
class EnemyManager
{
public:
	std::list<std::unique_ptr<Enemy>>enemys;

};

