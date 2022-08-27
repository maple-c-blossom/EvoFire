#include "Boss.h"
#include "Util.h"

using namespace MCB;
using namespace std;

void Boss::Init(Player* target, MCB::Float3 position, MCB::Model* model, MCB::Model* bulletModel, int AttackType)
{
}

void Boss::AllMatrixUpdate(MCB::View& view, MCB::Projection& proj)
{
	MatrixUpdata(view, proj);
	enemys.UpdateMatrix(view, proj);
	for (std::unique_ptr<Drone>& d : drones)
	{
		d->MatrixUpdata(view, proj);
	}

}

void Boss::AllDraw()
{
	Draw();
	enemys.Draw();
	for (std::unique_ptr<Drone>& d : drones)
	{
		d->Draw();
	}
}

void Boss::Update()
{
	imotalTimer--;
	if (imotalTimer <= 0)
	{
		imotalFlag = false;
	}
	Move();
	Rotasion();
	Attack();
	for (std::unique_ptr<Drone>& d : drones)
	{
		d->Update();
	}
	drones.remove_if([](std::unique_ptr<Drone>& d) {return d->deleteFlag; });
	enemys.Update();


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
		}
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

void Boss::Attack()
{
	

	PopEnemy();
	FireDrone();


}

void Boss::Deth(int damege)
{

	if (!imotalFlag)
	{
		hp -= damege;
		imotalFlag = true;
	}
	hitCount++;
	if (hp <= 0)
	{
		dethFlag = true;
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
	imotalTime = 15;
	drones.clear();
	hitCount = 0;
	countResetTimer = 0;
	countResetMaxTime = 150;
	rotationPosMoveTimer = 0;
	maxRotationPosMoveTimer = 300 + moveTime;
	rotasionSpeed = 0.025f;
	r = 50;
	enemys.enemys.clear();
	BaseRotationPos = rotationPos[0];
	BossQ = { 0,0,0,1 };
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
	maxhp = 1000;
	hp = maxhp;
	dethFlag = false;
}

