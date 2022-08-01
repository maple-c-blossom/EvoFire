#pragma once
#include "Object3d.h"
#include "Input.h"

class Player:public MCB::Object3d
{
private:
	MCB::Input* input = MCB::Input::GetInstance();
	float speed = 5.0f;

public:
	void Update();
	void Move();
};

