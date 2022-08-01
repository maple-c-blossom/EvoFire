#include "Player.h"

void Player::Update()
{
	Move();

}

void Player::Move()
{

	if (input->IsKeyDown(DIK_W))
	{
		if (speedFront <= 5)speedFront += 0.5f;
		else speedFront = 5;
	}

	if (input->IsKeyDown(DIK_S))
	{
		if (speedFront >= -5)speedFront -= 0.5f;
		else speedFront = -5;
	}

	if(!input->IsKeyDown(DIK_S) && !input->IsKeyDown(DIK_W))
	{
		if (speedFront > 0)speedFront -= 0.25f;
		else if (speedFront < 0)speedFront += 0.25f;
		else speedFront = 0;
	}


	if (input->IsKeyDown(DIK_D))
	{
		if (speedRight <= 5)speedRight += 0.5f;
		else speedRight = 5;
	}

	if (input->IsKeyDown(DIK_A))
	{
		if (speedRight >= -5)speedRight -= 0.5f;
		else speedRight = -5;
	}

	if (!input->IsKeyDown(DIK_D) && !input->IsKeyDown(DIK_A))
	{
		if (speedRight > 0)speedRight -= 0.25f;
		else if (speedRight < 0)speedRight += 0.25f;
		else speedRight = 0;
	}

	MCB::Vector3D rightVec;
	rightVec = rightVec.GetRightVec(nowFrontVec);

	position.x += nowFrontVec.vec.x * speedFront;
	position.y += nowFrontVec.vec.y * speedFront;
	position.z += nowFrontVec.vec.z * speedFront;
	position.x += rightVec.vec.x * speedRight;
	position.y += rightVec.vec.y * speedRight;
	position.z += rightVec.vec.z * speedRight;
}
