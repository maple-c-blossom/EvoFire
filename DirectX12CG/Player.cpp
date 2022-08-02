#include "Player.h"
#include "MCBMatrix.h"


using namespace MCB;
void Player::Update()
{
	Rotasion();
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

	
	position.x += nowFrontVec.vec.x * speedFront;
	position.y += nowFrontVec.vec.y * speedFront;
	position.z += nowFrontVec.vec.z * speedFront;
	position.x += rightVec.vec.x * speedRight;
	position.y += rightVec.vec.y * speedRight;
	position.z += rightVec.vec.z * speedRight;
}

void Player::Rotasion()
{
	Float3 angle = { 0,0,0 };
	rightVec.vec = { 1,0,0 };
	UpVec.vec = { 0,1,0 };
	nowFrontVec = NORM_FRONT_VEC;

	if (input->IsKeyDown(DIK_UP) || input->IsKeyDown(DIK_DOWN) ||
		input->IsKeyDown(DIK_RIGHT) || input->IsKeyDown(DIK_LEFT) ||
		input->IsKeyDown(DIK_E) || input->IsKeyDown(DIK_Q))
	{
		Quaternion PitchQ;
		PitchQ.SetRota(rightVec, angle.z);
		Quaternion YawQ;
		YawQ.SetRota(UpVec, angle.y);
		Quaternion RollQ;
		RollQ.SetRota(nowFrontVec, angle.x);

		if (input->IsKeyDown(DIK_UP) || input->IsKeyDown(DIK_DOWN))
		{
			if (input->IsKeyDown(DIK_UP))
			{
				angle.x += rotasionSpeed;
			}

			if (input->IsKeyDown(DIK_DOWN))
			{
				angle.x -= rotasionSpeed;
			}

			PitchQ.SetRota(rightVec, angle.x);
		}

		if (input->IsKeyDown(DIK_RIGHT) || input->IsKeyDown(DIK_LEFT))
		{
			if (input->IsKeyDown(DIK_RIGHT))
			{
				angle.z += rotasionSpeed;
			}

			if (input->IsKeyDown(DIK_LEFT))
			{
				angle.z -= rotasionSpeed;
			}
			RollQ.SetRota(nowFrontVec, angle.z);
		}

		if (input->IsKeyDown(DIK_E) || input->IsKeyDown(DIK_Q))
		{
			if (input->IsKeyDown(DIK_E))
			{
				angle.y += rotasionSpeed;
			}

			if (input->IsKeyDown(DIK_Q))
			{
				angle.y -= rotasionSpeed;
			}
			YawQ.SetRota(UpVec, angle.y);
		}

		Quaternion NewQ = { 0,0,0,1 };
		NewQ = playerQ.GetCartesianProduct(RollQ, PitchQ);
		NewQ = playerQ.GetCartesianProduct(NewQ, YawQ);
		playerQ = playerQ.GetCartesianProduct(playerQ, NewQ);
	}

	nowFrontVec = playerQ.Vector3DRotasion(nowFrontVec, playerQ);
	rightVec = playerQ.Vector3DRotasion(rightVec, playerQ);
	UpVec = playerQ.Vector3DRotasion(UpVec, playerQ);


}
