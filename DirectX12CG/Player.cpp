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
	else if(!input->IsKeyDown(DIK_S))
	{
		if (speedFront >= 0)speedFront -= 0.5f;
		else speedFront = 0;
	}

	if (input->IsKeyDown(DIK_S))
	{
		if (speedFront >= -5)speedFront -= 0.5f;
		else speedFront = -5;
	}
	else if (!input->IsKeyDown(DIK_W))
	{
		if (speedFront <= 0)speedFront += 0.5f;
		else speedFront = 0;
	}


	if (input->IsKeyDown(DIK_D))
	{
		if (speedRight <= 5)speedRight += 0.5f;
		else speedRight = 5;
	}
	else if (!input->IsKeyDown(DIK_A))
	{
		if (speedRight >= 0)speedRight -= 0.5f;
		else speedRight = 0;
	}


	if (input->IsKeyDown(DIK_A))
	{
		if (speedRight >= -5)speedRight -= 0.5f;
		else speedRight = -5;
	}
	else if (!input->IsKeyDown(DIK_D))
	{
		if (speedRight <= 0)speedRight += 0.5f;
		else speedRight = 0;
	}

	
}
