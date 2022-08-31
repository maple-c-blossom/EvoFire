#include "Boss.h"
#include "Util.h"

using namespace MCB;
using namespace std;

void Boss::Init(Player* target, MCB::Float3 position, MCB::Model* model, MCB::Model* bulletModel, int AttackType)
{
}

void Boss::AllMatrixUpdate(MCB::View& view, MCB::Projection& proj)
{
	MatrixUpdata(view, proj,BossQ);
	enemys.UpdateMatrix(view, proj);
	for (std::unique_ptr<Drone>& d : drones)
	{
		d->MatrixUpdata(view, proj);
	}
	for (std::unique_ptr<EnemyBullet>& bullet : bullets)
	{
		bullet->MatrixUpdata(view, proj);
	}

}

void Boss::AllDraw()
{
	if ( !(imotalTimer % 2) )
	{
		Draw();
	}

	enemys.Draw();
	for (std::unique_ptr<Drone>& d : drones)
	{
		d->Draw();
	}
	for (std::unique_ptr<EnemyBullet>& bullet : bullets)
	{
		bullet->Draw();
	}
}

void Boss::Update()
{
	if(imotalTimer > 0)imotalTimer--;
	if (imotalTimer <= 0 && imotalFlag)
	{
		imotalFlag = false;
	}
	if (!isPop)
	{
		Move();
		Rotasion();
	}
	Attack();
	for (std::unique_ptr<Drone>& d : drones)
	{
		d->Update();
	}
	drones.remove_if([](std::unique_ptr<Drone>& d) {return d->deleteFlag; });
	enemys.Update();
	for (std::unique_ptr<EnemyBullet>& bullet : bullets)
	{
		bullet->Update();
	}
	bullets.remove_if([](std::unique_ptr<EnemyBullet>& bullet) {return bullet->deleteFlag; });

}

void Boss::FireDrone()
{
	dromeAttacktimer++;
	if (dromeAttacktimer > dromeAttacktime)
	{
		if (drones.size() < 2)
		{
			if (drones.size() <= 0)
			{
				unique_ptr<Drone> d = make_unique<Drone>();
				d->BossPtr = this;
				d->Init(playerPtr, {nowFrontVec.vec.x * d->distance,nowFrontVec.vec.y * d->distance ,nowFrontVec.vec.z * d->distance }, DroneModel);
				d->yourFront = true;
				d->mapTexture = droneMapTex;
				drones.push_back(move(d));
			
			}
			if (drones.size() == 1)
			{
				if (drones.begin()->get()->yourFront)
				{
					unique_ptr<Drone> d = make_unique<Drone>();
					d->BossPtr = this;
					d->Init(playerPtr, { -nowFrontVec.vec.x * d->distance ,-nowFrontVec.vec.y * d->distance ,-nowFrontVec.vec.z * d->distance }, DroneModel);
					d->yourFront = false;
					d->mapTexture = droneMapTex;
					drones.push_back(move(d));
				}
				else
				{
					unique_ptr<Drone> d = make_unique<Drone>();
					d->BossPtr = this;
					d->Init(playerPtr, { nowFrontVec.vec.x * d->distance,nowFrontVec.vec.y * d->distance ,nowFrontVec.vec.z * d->distance }, DroneModel);
					d->yourFront = true;
					drones.push_back(move(d));
				}
			}
		}
		else
		{
			if (fireDroneIsyorFront)
			{
				fireDroneIsyorFront = false;
				for (std::unique_ptr<Drone>& d : drones)
				{
					if (!d->yourFront)
					{
						d->Shot();
						break;
					}
				}


			}
			else
			{
				fireDroneIsyorFront = true;

				for (std::unique_ptr<Drone>& d : drones)
				{
					if (d->yourFront)
					{
						d->Shot();
						break;
					}
				}
			}
		}
		dromeAttacktimer = 0;
	}

	for (std::unique_ptr<Drone>& d : drones)
	{

		if (d->FireFlag)
		{
			droneAttacked = true;
			break;
		}
		droneAttacked = false;
	}

}
void Boss::PopEnemy()
{
	enemyPopTimer++;
	if (enemyPopTimer > enemyPopTime)
	{
		isPop = true;
		if (isPop)
		{
			popTimer++;
			model->material.constMapMaterial->color = { 1,0.5f,0.5f,1 };
		}
		if (popTimer > popTime)
		{
			for (int k = 0; k < enemysPopNum; k++)
			{
				int EnemyBulletType = 0;
				int EnemyType = 1;
				Enemy* guradPtr = nullptr;
				if (GetRand(0, 1))
				{
					EnemyBulletType = Enemy::Homing;
				}
				else
				{
					EnemyBulletType = Enemy::NoHoming;
				}

				switch (GetRand(1, 4))
				{
				case 1:
					EnemyType = EnemyManager::Turret;

					break;
				case 2:
					EnemyType = EnemyManager::Rash;

					break;
				case 3:
					EnemyType = EnemyManager::Guard;
					if (enemys.enemys.size() > 0)
					{
						int i = GetRand(0, enemys.enemys.size());
						if (i >= enemys.enemys.size())
						{
							guradPtr = this;
						}
						else
						{
							auto itr = enemys.enemys.begin();
							for (int a = 0; a < i; a++)
							{
								itr++;
							}
							guradPtr = itr->get();;
						}
					}
					break;
				case 4:
					EnemyType = EnemyManager::Circumference;

					break;

				default:

					break;
				}
				enemys.enemyPop(playerPtr, { (float)GetRand(-500,500) + position.x,0,(float)GetRand(-500,500) + position.z },
					EnemyModel, EnemyBModel, EnemyMapTex, EnemyBMapTex, EnemyType, EnemyBulletType, guradPtr);


			}
			enemyPopTimer = 0;
			popTimer = 0;
			isPop = false;
			model->material.constMapMaterial->color = { 1,1.f,1.f,1 };
		}
	}
}
void Boss::Move()
{
	countResetTimer++;

	rotationPosMoveTimer++;
	if (countResetTimer > countResetMaxTime)
	{
		hitCount = 0;
	}
	
	if (rotationPosMoveTimer >= maxRotationPosMoveTimer * 2 / 3)
	{
		model->material.constMapMaterial->color = { 0.0,0.0,1.f,1 };
	}
	else
	{
		model->material.constMapMaterial->color = { 1,1.f,1.f,1 };
	}
	if (rotationPosMoveTimer > maxRotationPosMoveTimer)
	{
		rotationPosMoveTimer = 0;
		int i = hitCount % 17;
		if (hitCount <= 0)
		{
			i = GetRand(0, 16);
		}
		if (i >= 17)
		{
			i = 0;
		}
		prevRotationPos = BaseRotationPos;
		nextRotationPos = rotationPos[i];
	}
	Clrcle();
}

void Boss::Clrcle()
{
	if (nextRotationPos.x != BaseRotationPos.x || nextRotationPos.y != BaseRotationPos.y || nextRotationPos.z != BaseRotationPos.z)
	{
		if (moveTimer < moveTime)
		{
			moveTimer++;
			Fire();
		}
			model->material.constMapMaterial->color = { 0.0,0.0,1.f,1 };

		if (moveTimer >= moveTime)
		{
			moveTimer = 0;
			BaseRotationPos = nextRotationPos;
			prevRotationPos = BaseRotationPos;
		}
		BaseRotationPos.x = Lerp(prevRotationPos.x, nextRotationPos.x, moveTime, moveTimer);
		BaseRotationPos.y = Lerp(prevRotationPos.y, nextRotationPos.y, moveTime, moveTimer);
		BaseRotationPos.z = Lerp(prevRotationPos.z, nextRotationPos.z, moveTime, moveTimer);
	}
	else if(!(rotationPosMoveTimer >= maxRotationPosMoveTimer * 2 / 3))
	{
		model->material.constMapMaterial->color = { 1,1.f,1.f,1 };
	
	}
	if (nextRotationPos.x == BaseRotationPos.x && nextRotationPos.y == BaseRotationPos.y && nextRotationPos.z == BaseRotationPos.z)
	{
		rotationAngle += 0.015f;
	}

	rotationVec.vec = { 0,1,0 };
	Quaternion positionQ;
	positionQ = positionQ.SetRotationQuaternion(rotationVec, startPositionVec, rotationAngle);
	Vector3D PositionVec = { positionQ.x,positionQ.y,positionQ.z };
	PositionVec.V3Norm();
	Float3 tempPosition = { position.x,position.y,position.z };
	//position.x = PositionVec.vec.x * distance, position.y = PositionVec.vec.y * distance, position.z = PositionVec.vec.z * distance;
	position.x = PositionVec.vec.x * (distance - sinf(rotationAngle) * 10) + BaseRotationPos.x;
	position.y = PositionVec.vec.y * (distance - sinf(rotationAngle) * 10) + BaseRotationPos.y;
	position.z = PositionVec.vec.z * (distance - sinf(rotationAngle) * 10) + BaseRotationPos.z;
}

void Boss::Rotasion()
{
	Vector3D targetVec = targetVec.V3Get({ 0,0,1 }, { playerPtr->position.x, playerPtr->position.y,playerPtr->position.z });
	nowFrontVec = NORM_FRONT_VEC;
	Vector3D DefoVec = { 0,0,1 };
	Vector3D RotasionAxis = RotasionAxis.GetV3Cross(DefoVec, targetVec);
	RotasionAxis.V3Norm();
	float angle = DefoVec.GetV3Dot(targetVec);
	enemyQ.SetRota(RotasionAxis, angle);
	nowFrontVec = targetVec;
}

void Boss::Fire()
{
	bAttackTimer++;

	if (bAttackTimer > maxBAttackTime)
	{
		bAttackTimer = 0;
		for (int i = 0; i < 10; i++)
		{
			std::unique_ptr<EnemyBullet> bullet;
			bullet = std::make_unique<NoHomingEnemyBullet>();
			bullet->Fire({ position.x,position.y,position.z }, { (float)GetRand(-10000,10000) / (float)10000,(float)GetRand(-10000,10000) / (float)10000 ,(float)GetRand(-10000,10000) / (float)10000 }, nullptr);
			bullet->model = EnemyBModel;
			bullet->scale = { 6,6,6 };
			bullet->rotasion = rotasion;
			bullet->maxLifeTime = 60;
			bullet->speedOffSet = 50;
			bullet->mapTexture = EnemyBMapTex;
			bullets.push_back(std::move(bullet));
		}
		std::unique_ptr<EnemyBullet> bullet;
		bullet = std::make_unique<HomingEnemyBullet>();
		bullet->Fire({ position.x,position.y,position.z }, { (float)GetRand(-10000,10000) / (float)10000,(float)GetRand(-10000,10000) / (float)10000 ,(float)GetRand(-10000,10000) / (float)10000 }, playerPtr);
		bullet->model = EnemyBModel;
		bullet->scale = { 6,6,6 };
		bullet->rotasion = rotasion;
		bullet->maxLifeTime = 180;
		bullet->speedOffSet = 50;
		bullet->mapTexture = EnemyBMapTex;
		bullets.push_back(std::move(bullet));
	}
}

void Boss::GFire()
{
	gbAttackTimer++;

	if (gbAttackTimer > maxBAttackTime * 6)
	{
		gbAttackTimer = 0;
		for (int i = 0; i < 10; i++)
		{
			std::unique_ptr<EnemyBullet> bullet;
			bullet = std::make_unique<NoHomingEnemyBullet>();
			bullet->Fire({ position.x,position.y,position.z }, { (float)GetRand(-10000,10000) / (float)10000,(float)GetRand(-10000,10000) / (float)10000 ,(float)GetRand(-10000,10000) / (float)10000 }, nullptr);
			bullet->model = EnemyBModel;
			bullet->scale = { 6,6,6 };
			bullet->rotasion = rotasion;
			bullet->maxLifeTime = 60;
			bullet->speedOffSet = 50;
			bullet->mapTexture = EnemyBMapTex;
			bullets.push_back(std::move(bullet));
		}
		std::unique_ptr<EnemyBullet> bullet;
		bullet = std::make_unique<HomingEnemyBullet>();
		bullet->Fire({ position.x,position.y,position.z }, { (float)GetRand(-10000,10000) / (float)10000,(float)GetRand(-10000,10000) / (float)10000 ,(float)GetRand(-10000,10000) / (float)10000 }, playerPtr);
		bullet->model = EnemyBModel;
		bullet->scale = { 6,6,6 };
		bullet->rotasion = rotasion;
		bullet->maxLifeTime = 120;
		bullet->speedOffSet = 10;
		bullet->mapTexture = EnemyBMapTex;
		bullets.push_back(std::move(bullet));
	}
}

void Boss::Attack()
{
	

	PopEnemy();
	FireDrone();
	if (hp <= maxhp / 6)
	{
		GFire();
	}

}

void Boss::Deth(int damege)
{

	if (!imotalFlag)
	{
		hp -= damege;
		imotalTimer = imotalTime;
	}
	hitCount++;
	if (hp <= 0)
	{
		dethFlag = true;
		sound->PlaySoundWave(DethSound);
	}

}

void Boss::Init(Player* target, MCB::Float3 position,
	MCB::Model* model, MCB::Model* DroneModel, MCB::Model* EnemyModel, MCB::Model* EnemyBModel, 
	MCB::Texture* bossmap, MCB::Texture* dronemap, MCB::Texture* enemymap, MCB::Texture* enemyBmap)
{

	playerPtr = target;
	this->position.x = position.x;
	this->position.y = position.y;
	this->position.z = position.z;
	this->model = model;
	this->DroneModel = DroneModel;
	this->EnemyModel = EnemyModel;
	this->EnemyBModel = EnemyBModel;
	scale = { 40,40,40 };
	moveTime = 120;
	EnemyMapTex = enemymap;
	EnemyBMapTex = enemyBmap;
	bossMapTex = bossmap;
	droneMapTex = dronemap;
	enemyPopTimer = 160;
	enemyPopTime = 200;
	enemysPopNum = 5;
	dromeAttacktimer = 299;
	dromeAttacktime = 300;
	fireDroneIsyorFront = false;
	droneAttacked = false;
	imotalTimer = 0;
	imotalFlag = false;
	imotalTime = 20;
	drones.clear();
	hitCount = 0;
	countResetTimer = 0;
	countResetMaxTime = 150;
	rotationPosMoveTimer = 0;
	maxRotationPosMoveTimer = 300 + moveTime;
	rotasionSpeed = 0.025f;
	r = 60;
	enemys.enemys.clear();
	BaseRotationPos = rotationPos[0];
	BossQ.SetRota({ 0,1,0 }, ConvertRadius(180));
	rightVec.vec = { 1,0,0 };
	UpVec.vec = { 0,1,0 };
	rotationVec.vec = { 0,1,0 };
	rotationAngle = 0.0f;
	startPositionVec.vec = { position.x, position.y,position.z};
	distance = 1500;
	SetRota = false;
	moveTimer = 0;
	nextRotationPos = BaseRotationPos;
	prevRotationPos = BaseRotationPos;
	attackTime = 0;
	attackResponceTime = 20;
	maxhp = 700;
	hp = maxhp;
	dethFlag = false;
	nowFrontVec.vec = { 0,0,1 };
	bullets.clear();
	bAttackTimer = 0;
	gbAttackTimer = 0;
}

