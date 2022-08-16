#include "Player.h"
#include "MCBMatrix.h"


using namespace MCB;
using namespace std;

void Player::PlayerInit(Model* model, Model* bulletModel, Model* missileModel, MCB::Model* laserModel, MCB::Model* bombModel)
{
	speedFront = 0.0f;
	speedRight = 0.0f;
	rotasionSpeed = 0.025f;
	speed = 2.0f;
	Srowspeed = 0.25f;
	maxspeed = 15.0f;
	homingMissileCount = 0;
	laserCount = 0;
	bombCount = 0;
	exp = 0;
	target = nullptr;
	attackTime = attackResponceTime;
	attackResponceTime = 20;
	Level = 1;
	nextLevelExp = 20;
	maxhp = 20;
	hp = maxhp;
	r = 10;
	this->model = model;
	this->bulletModel = bulletModel;
	this->missileModel = missileModel;
	this->laserModel = laserModel;
	this->bombModel = bombModel;
	scale = { 4,4,4 };
	targetRay.radius = 50;
	targetRay.rayVec = nowFrontVec;
	targetRay.StartPosition.x = position.x;
	targetRay.StartPosition.y = position.y;
	targetRay.StartPosition.z = position.z;
	targetRay.range = 50000.0f;
	for (std::unique_ptr<PlayerBullet>& bullet : bullets)
	{
		bullet->deleteFlag = true;
	}
	bullets.remove_if([](std::unique_ptr<PlayerBullet>& bullet) {return bullet->deleteFlag; });

	for (std::unique_ptr<HomingMissile>& missile : homingMissile)
	{
		missile->allDeleteFlag = true;
	}
	homingMissile.remove_if([](std::unique_ptr<HomingMissile>& missile) {return missile->allDeleteFlag; });

	for (std::unique_ptr<Laser>& laser : lasers)
	{
		laser->deleteFlag = true;
	}
	lasers.remove_if([](std::unique_ptr<Laser>& laser) {return laser->deleteFlag; });
}

void Player::Update()
{
	Rotasion();
	Move();
	Attack();
	SPAttack();
	targetRay.rayVec = nowFrontVec;
	targetRay.StartPosition.x = position.x;
	targetRay.StartPosition.y = position.y;
	targetRay.StartPosition.z = position.z;

	for (std::unique_ptr<HomingMissile>& missile : homingMissile)
	{
		missile->SetTarget(target);
	}
	


	for (std::unique_ptr<PlayerBullet>& bullet : bullets)
	{
		bullet->VelocityUpdate();
		bullet->Update();
	}
	bullets.remove_if([](std::unique_ptr<PlayerBullet>& bullet) {return bullet->deleteFlag; });

	for (std::unique_ptr<HomingMissile>& missile : homingMissile)
	{

		missile->VelocityUpdate();
		missile->Update();
	}
	homingMissile.remove_if([](std::unique_ptr<HomingMissile>& missile) {return missile->allDeleteFlag; });

	for (std::unique_ptr<Laser>& laser : lasers)
	{
		laser->Update({ position.x,position.y,position.z }, nowFrontVec);
	}
	lasers.remove_if([](std::unique_ptr<Laser>& laser) {return laser->deleteFlag; });

	for (std::unique_ptr<Bomb>& bomb : bombs)
	{
		bomb->Update();
	}
	bombs.remove_if([](std::unique_ptr<Bomb>& bomb) {return bomb->deleteFlag; });
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

		if (((int)exp == 20 || (int)exp == 40 || (int)exp == 60)&& homingMissileCount < maxHomingMissileCount && !homingMissileGet)
		{
			homingMissileCount++;
			homingMissileGet = true;
		}
		if (((int)exp == 40 || (int)exp == 60) && laserCount < maxLaserCount && !LaserGet)
		{
			laserCount++;
			LaserGet = true;
		}
		if ((int)exp == 60 && bombCount < maxBombCount && !BombGet)
		{
			bombCount++;
			BombGet = true;
		}
}

void Player::SetTarget(Object3d* target)
{
	this->target = target;
}

void Player::GetExp(float expPoint)
{
	int beforeExp = (int)exp;
	exp += expPoint;
	if ((int)exp != beforeExp)
	{
		homingMissileGet = false;
		LaserGet = false;
		BombGet = false;
	}
	GetSPAttack();
	LevelUp();
}

void Player::LevelUp()
{
	if (exp >= nextLevelExp)
	{
		exp = 0;
		Level++;
		maxhp = maxhp + ((Level - 1) * 10);
		hp = maxhp;
		nextLevelExp = nextLevelExp + (20 * (Level - 1));
	}
}

void Player::SPAttack()
{
	if ((input->IsKeyDown(DIK_LSHIFT) && input->IsKeyTrigger(DIK_C)) || 
		(input->gamePad->IsButtonDown(GAMEPAD_LB) && input->gamePad->IsButtonTrigger(GAMEPAD_A)))
	{
		if (homingMissileCount > 0)
		{
			homingMissileCount--;
			std::unique_ptr<HomingMissile> HM = std::make_unique<HomingMissile>();
			HM->Fire({ position.x,position.y,position.z }, nowFrontVec, target, missileModel);
			for (int i = 0; i < 3; i++)
			{
				HM->homingMissiles[i].scale = { 6,6,6 };
				HM->homingMissiles[i].rotasion = rotasion;
			}
			homingMissile.push_back(std::move(HM));
			attackTime = 0;
		}
	}

	if ((input->IsKeyDown(DIK_LSHIFT) && input->IsKeyTrigger(DIK_V)) ||
		(input->gamePad->IsButtonDown(GAMEPAD_LB) && input->gamePad->IsButtonTrigger(GAMEPAD_B)))
	{
		if (laserCount > 0)
		{
			laserCount--;
			std::unique_ptr<Laser> LS = std::make_unique<Laser>();
			LS->Fire({ position.x,position.y,position.z }, nowFrontVec, laserModel);
			lasers.push_back(std::move(LS));
			attackTime = 0;
		}
	}

	if ((input->IsKeyDown(DIK_LSHIFT) && input->IsKeyTrigger(DIK_B)) ||
		(input->gamePad->IsButtonDown(GAMEPAD_LB) && input->gamePad->IsButtonTrigger(GAMEPAD_Y)))
	{
		if (bombCount > 0)
		{
			bombCount--;
			std::unique_ptr<Bomb> BM = std::make_unique<Bomb>();
			BM->Fire(nowFrontVec,{ position.x,position.y,position.z }, bombModel);
			bombs.push_back(std::move(BM));
			attackTime = 0;
		}
	}

}

void Player::AllDraw()
{
	Draw();
	for (std::unique_ptr<PlayerBullet>& bullet : bullets) { bullet->Draw(); }
	for (std::unique_ptr<HomingMissile>& missile : homingMissile) 
	{
		for (int i = 0; i < 3; i++)
		{
			missile->homingMissiles[i].Draw(); 
		}
	}
	for (std::unique_ptr<Laser>& laser : lasers) { laser->Draw(); }
	for (std::unique_ptr<Bomb>& bomb : bombs) { bomb->Draw(); }
}

void Player::AllMatrixUpdate(MCB::View viewmat, MCB::Projection projmat)
{
	MatrixUpdata(viewmat, projmat,playerQ);

	for (std::unique_ptr<PlayerBullet>& bullet : bullets)
	{
		bullet->MatrixUpdata(viewmat, projmat);
	}

	for (std::unique_ptr<HomingMissile>& missile : homingMissile)
	{
		for (int i = 0; i < 3; i++)
		{
			missile->homingMissiles[i].MatrixUpdata(viewmat, projmat);
		}
	}
	for (std::unique_ptr<Laser>& laser : lasers)
	{
		laser->MatrixUpdata(viewmat, projmat, playerQ);
	}

	for (std::unique_ptr<Bomb>& bomb : bombs)
	{
		bomb->MatrixUpdata(viewmat, projmat);
	}
}

void Player::EnemyBulletHit(int damege)
{
	hp -= damege;
}

MCB::Object3d* Player::GetTarget()
{
	return target;
}

void Player::SetHomingTarget(Object3d* target)
{
	homingTarget = target;
}

MCB::Object3d* Player::GetHomingTarget()
{
	return homingTarget;
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
