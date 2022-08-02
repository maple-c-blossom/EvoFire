#pragma once
#include "Object3d.h"
#include "Input.h"


class Player:public MCB::Object3d
{
private:
	MCB::Input* input = MCB::Input::GetInstance();
	float speedFront = 0.0f;
	float speedRight = 0.0f;
	float rotasionSpeed = 0.025f;
public:
	MCB::Quaternion playerQ = {0,0,0,1};
	MCB::Vector3D rightVec;
	MCB::Vector3D UpVec;
	void Update();
	void Move();
	void Rotasion();
};

