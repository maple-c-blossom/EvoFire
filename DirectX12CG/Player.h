#pragma once
#include "Object3d.h"
#include "Input.h"
#include "PlayerBullet.h"

class Player:public MCB::Object3d
{
private:
	MCB::Input* input = MCB::Input::GetInstance();
	float speedFront = 0.0f;
	float speedRight = 0.0f;
	float rotasionSpeed = 0.025f;
	float speed = 1.0f;
	float Srowspeed = 0.25f;
	float maxspeed = 15.0f;
public:
	MCB::Model* bulletModel = nullptr;
	std::list<std::unique_ptr<PlayerBullet>> bullets;
	MCB::Quaternion playerQ = {0,0,0,1};
	MCB::Vector3D rightVec;
	MCB::Vector3D UpVec;
	void Update();
	void Move();
	void Rotasion();
	void Attack();
};

