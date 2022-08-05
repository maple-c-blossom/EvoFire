#pragma once
#include "Object3d.h"
class Boss :public  MCB::Object3d
{
private:
	int hp = 200;
public:
	float r = 10;
};

