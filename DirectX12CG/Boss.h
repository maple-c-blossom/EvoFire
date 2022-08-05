#pragma once
#include "Object3d.h"
class Boss :public  MCB::Object3d
{
private:
	int attackTime = 0;
	int attackResponceTime = 20;
	int Level = 1;
	int nextLevelExp = 20;
	int maxhp = 200;
	int hp = maxhp;
	MCB::Object3d* playerPtr = nullptr;
public:
	float r = 10;

	void Update();
};

