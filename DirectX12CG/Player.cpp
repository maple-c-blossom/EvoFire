#include "Player.h"
#include "MCBMatrix.h"


using namespace MCB;
using namespace std;

void Player::Update()
{
	Rotasion();
	Move();
	GetSPAttack();
	Attack();
	for (std::unique_ptr<PlayerBullet>& bullet : bullets)
	{
		if (target != nullptr)
		{
			bullet->VelocityUpdate();
		}
		bullet->Update();
	}
	bullets.remove_if([](std::unique_ptr<PlayerBullet>& bullet) {return bullet->deleteFlag; });

}

void Player::Move()
{
	if (input->IsKeyDown(DIK_W))
	{
		if (speedFront <= maxspeed)speedFront += speed;
		else speedFront = maxspeed;
	}

	if (input->IsKeyDown(DIK_S))
	{
		if (speedFront >= -maxspeed)speedFront -= speed;
		else speedFront = -maxspeed;
	}

	if (input->gamePad->LStick.y)
	{
		float accelerator = speed;
		accelerator *= input->gamePad->LStick.y;
		if (speedFront <= maxspeed && speedFront >= -maxspeed)speedFront += accelerator;
		else if(speedFront >= maxspeed) speedFront = maxspeed;
		else if(speedFront <= -maxspeed)speedFront = -maxspeed;
	}

	if(!input->IsKeyDown(DIK_S) && !input->IsKeyDown(DIK_W) && !input->gamePad->LStick.y)
	{
		if (speedFront > 0)speedFront -= Srowspeed;
		else if (speedFront < 0)speedFront += Srowspeed;
		else speedFront = 0;
	}


	if (input->IsKeyDown(DIK_D))
	{
		if (speedRight <= maxspeed)speedRight += speed;
		else speedRight = maxspeed;
	}

	if (input->IsKeyDown(DIK_A))
	{
		if (speedRight >= -maxspeed)speedRight -= speed;
		else speedRight = -maxspeed;
	}

	if (input->gamePad->LStick.x)
	{
		float accelerator = speed;
		accelerator *= input->gamePad->LStick.x;
		if (speedRight <= maxspeed && speedRight >= -maxspeed)speedRight += accelerator;
		else if (speedRight >= maxspeed) speedRight = maxspeed;
		else if (speedRight <= -maxspeed)speedRight = -maxspeed;
	}


	if (!input->IsKeyDown(DIK_D) && !input->IsKeyDown(DIK_A) && !input->gamePad->LStick.x)
	{
		if (speedRight > 0)speedRight -= Srowspeed;
		else if (speedRight < 0)speedRight += Srowspeed;
		else speedRight = 0;
	}


	
	position.x += nowFrontVec.vec.x * speedFront;
	//position.y += nowFrontVec.vec.y * speedFront;
	position.z += nowFrontVec.vec.z * speedFront;
	position.x += rightVec.vec.x * speedRight;
	//position.y += rightVec.vec.y * speedRight;
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
		input->IsKeyDown(DIK_E) || input->IsKeyDown(DIK_Q) ||
		input->gamePad->RStick.y || input->gamePad->RStick.x ||
		input->gamePad->LTrriger.y || input->gamePad->RTrriger.y )
	{
		Quaternion PitchQ;
		PitchQ.SetRota(rightVec, angle.z);
		Quaternion YawQ;
		YawQ.SetRota(UpVec, angle.y);
		Quaternion RollQ;
		RollQ.SetRota(nowFrontVec, angle.x);

		if (input->IsKeyDown(DIK_UP) || input->IsKeyDown(DIK_DOWN) || input->gamePad->RStick.y)
		{
			if (input->IsKeyDown(DIK_UP))
			{
				angle.x += rotasionSpeed;
			}

			if (input->IsKeyDown(DIK_DOWN))
			{
				angle.x -= rotasionSpeed;
			}

			if (input->gamePad->RStick.y)
			{
				float angleSpeed = rotasionSpeed * input->gamePad->RStick.y;
				angle.x += angleSpeed;
			}

			PitchQ.SetRota(rightVec, angle.x);
		}

		//if (input->IsKeyDown(DIK_RIGHT) || input->IsKeyDown(DIK_LEFT) || 
		//	input->gamePad->LTrriger.y || input->gamePad->RTrriger.y)
		//{
		//	if (input->IsKeyDown(DIK_RIGHT))
		//	{
		//		angle.z += rotasionSpeed;
		//	}

		//	if (input->IsKeyDown(DIK_LEFT))
		//	{
		//		angle.z -= rotasionSpeed;
		//	}

		//	if (input->gamePad->RTrriger.y)
		//	{
		//		float angleSpeed = input->gamePad->RTrriger.y * rotasionSpeed;
		//		angle.z += angleSpeed;
		//	}

		//	if (input->gamePad->LTrriger.y)
		//	{
		//		float angleSpeed = input->gamePad->LTrriger.y * rotasionSpeed;
		//		angle.z -= angleSpeed;
		//	}

		//	RollQ.SetRota(nowFrontVec, angle.z);
		//}

		if (input->IsKeyDown(DIK_E) || input->IsKeyDown(DIK_Q) || input->gamePad->RStick.x)
		{
			if (input->IsKeyDown(DIK_E))
			{
				angle.y += rotasionSpeed;
			}

			if (input->IsKeyDown(DIK_Q))
			{
				angle.y -= rotasionSpeed;
			}
			if (input->gamePad->RStick.x)
			{
				float angleSpeed = rotasionSpeed * input->gamePad->RStick.x;
				angle.y += angleSpeed;
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

void Player::Attack()
{
	attackTime++;
	if ((input->IsKeyDown(DIK_SPACE) || input->gamePad->IsButtonDown(GAMEPAD_X)) && attackTime >= attackResponceTime)
	{
		std::unique_ptr<PlayerBullet> bullet = std::make_unique<PlayerBullet>();
		bullet->Fire({ position.x,position.y,position.z }, nowFrontVec,target);
		bullet->model = bulletModel;
		bullet->scale = {6,6,6};
		bullet->rotasion = rotasion;
		bullets.push_back(std::move(bullet));
		attackTime = 0;
	}
}

void Player::GetSPAttack()
{
	if (exp % 20 == 0 && homingMissileCount < maxHomingMissileCount)
	{
		homingMissileCount++;
	}
	if (exp % 40 == 0 && laserCount < maxLaserCount)
	{
		laserCount++;
	}
	if (exp % 60 == 0 && bombCount < maxBombCount)
	{
		bombCount++;
	}

}

void Player::SetTarget(Object3d* target)
{
	this->target = target;
}




//自由に飛び回るタイプ
//void Player::Move()
//{
//	if (input->IsKeyDown(DIK_W))
//	{
//		if (speedFront <= maxspeed)speedFront += speed;
//		else speedFront = maxspeed;
//	}
//
//	if (input->IsKeyDown(DIK_S))
//	{
//		if (speedFront >= -maxspeed)speedFront -= speed;
//		else speedFront = -maxspeed;
//	}
//
//	if (input->gamePad->LStick.y)
//	{
//		float accelerator = speed;
//		accelerator *= input->gamePad->LStick.y;
//		if (speedFront <= maxspeed && speedFront >= -maxspeed)speedFront += accelerator;
//		else if (speedFront >= maxspeed) speedFront = maxspeed;
//		else if (speedFront <= -maxspeed)speedFront = -maxspeed;
//	}
//
//	if (!input->IsKeyDown(DIK_S) && !input->IsKeyDown(DIK_W) && !input->gamePad->LStick.y)
//	{
//		if (speedFront > 0)speedFront -= Srowspeed;
//		else if (speedFront < 0)speedFront += Srowspeed;
//		else speedFront = 0;
//	}
//
//
//	if (input->IsKeyDown(DIK_D))
//	{
//		if (speedRight <= maxspeed)speedRight += speed;
//		else speedRight = maxspeed;
//	}
//
//	if (input->IsKeyDown(DIK_A))
//	{
//		if (speedRight >= -maxspeed)speedRight -= speed;
//		else speedRight = -maxspeed;
//	}
//
//	if (input->gamePad->LStick.x)
//	{
//		float accelerator = speed;
//		accelerator *= input->gamePad->LStick.x;
//		if (speedRight <= maxspeed && speedRight >= -maxspeed)speedRight += accelerator;
//		else if (speedRight >= maxspeed) speedRight = maxspeed;
//		else if (speedRight <= -maxspeed)speedRight = -maxspeed;
//	}
//
//
//	if (!input->IsKeyDown(DIK_D) && !input->IsKeyDown(DIK_A) && !input->gamePad->LStick.x)
//	{
//		if (speedRight > 0)speedRight -= Srowspeed;
//		else if (speedRight < 0)speedRight += Srowspeed;
//		else speedRight = 0;
//	}
//
//
//
//	position.x += nowFrontVec.vec.x * speedFront;
//	position.y += nowFrontVec.vec.y * speedFront;
//	position.z += nowFrontVec.vec.z * speedFront;
//	position.x += rightVec.vec.x * speedRight;
//	position.y += rightVec.vec.y * speedRight;
//	position.z += rightVec.vec.z * speedRight;
//}
//
//void Player::Rotasion()
//{
//	Float3 angle = { 0,0,0 };
//	rightVec.vec = { 1,0,0 };
//	UpVec.vec = { 0,1,0 };
//	nowFrontVec = NORM_FRONT_VEC;
//
//	if (input->IsKeyDown(DIK_UP) || input->IsKeyDown(DIK_DOWN) ||
//		input->IsKeyDown(DIK_RIGHT) || input->IsKeyDown(DIK_LEFT) ||
//		input->IsKeyDown(DIK_E) || input->IsKeyDown(DIK_Q) ||
//		input->gamePad->RStick.y || input->gamePad->RStick.x ||
//		input->gamePad->LTrriger.y || input->gamePad->RTrriger.y)
//	{
//		Quaternion PitchQ;
//		PitchQ.SetRota(rightVec, angle.z);
//		Quaternion YawQ;
//		YawQ.SetRota(UpVec, angle.y);
//		Quaternion RollQ;
//		RollQ.SetRota(nowFrontVec, angle.x);
//
//		if (input->IsKeyDown(DIK_UP) || input->IsKeyDown(DIK_DOWN) || input->gamePad->RStick.y)
//		{
//			if (input->IsKeyDown(DIK_UP))
//			{
//				angle.x += rotasionSpeed;
//			}
//
//			if (input->IsKeyDown(DIK_DOWN))
//			{
//				angle.x -= rotasionSpeed;
//			}
//
//			if (input->gamePad->RStick.y)
//			{
//				float angleSpeed = rotasionSpeed * input->gamePad->RStick.y;
//				angle.x += angleSpeed;
//			}
//
//			PitchQ.SetRota(rightVec, angle.x);
//		}
//
//		if (input->IsKeyDown(DIK_RIGHT) || input->IsKeyDown(DIK_LEFT) ||
//			input->gamePad->LTrriger.y || input->gamePad->RTrriger.y)
//		{
//			if (input->IsKeyDown(DIK_RIGHT))
//			{
//				angle.z += rotasionSpeed;
//			}
//
//			if (input->IsKeyDown(DIK_LEFT))
//			{
//				angle.z -= rotasionSpeed;
//			}
//
//			if (input->gamePad->RTrriger.y)
//			{
//				float angleSpeed = input->gamePad->RTrriger.y * rotasionSpeed;
//				angle.z += angleSpeed;
//			}
//
//			if (input->gamePad->LTrriger.y)
//			{
//				float angleSpeed = input->gamePad->LTrriger.y * rotasionSpeed;
//				angle.z -= angleSpeed;
//			}
//
//			RollQ.SetRota(nowFrontVec, angle.z);
//		}
//
//		if (input->IsKeyDown(DIK_E) || input->IsKeyDown(DIK_Q) || input->gamePad->RStick.x)
//		{
//			if (input->IsKeyDown(DIK_E))
//			{
//				angle.y += rotasionSpeed;
//			}
//
//			if (input->IsKeyDown(DIK_Q))
//			{
//				angle.y -= rotasionSpeed;
//			}
//			if (input->gamePad->RStick.x)
//			{
//				float angleSpeed = rotasionSpeed * input->gamePad->RStick.x;
//				angle.y += angleSpeed;
//			}
//
//			YawQ.SetRota(UpVec, angle.y);
//		}
//
//		Quaternion NewQ = { 0,0,0,1 };
//		NewQ = playerQ.GetCartesianProduct(RollQ, PitchQ);
//		NewQ = playerQ.GetCartesianProduct(NewQ, YawQ);
//		playerQ = playerQ.GetCartesianProduct(playerQ, NewQ);
//	}
//
//	nowFrontVec = playerQ.Vector3DRotasion(nowFrontVec, playerQ);
//	rightVec = playerQ.Vector3DRotasion(rightVec, playerQ);
//	UpVec = playerQ.Vector3DRotasion(UpVec, playerQ);
//
//
//}
