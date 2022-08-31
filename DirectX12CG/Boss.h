#pragma once
#include "Object3d.h"
#include "EnemyBullet.h"
#include "Drone.h"
#include "EnemyManager.h"
#include "Player.h"
#include "NoHomingEnemyBullet.h"
#include "HomingEnemyBullet.h"
class Boss :public Enemy
{
private:
	enum Status
	{
		Turret,
		Rush,
		Circumference
	};

	enum AttaxkType
	{
		NoHoming,
		Homing
	};
	int attackTime = 0;
	int attackResponceTime = 20;
public:
	MCB::SoundManager* sound;
	int maxhp = 500;
	int hp = maxhp;
	bool dethFlag = false;
	MCB::Model* DroneModel = nullptr;
	MCB::Model* EnemyModel = nullptr;
	MCB::Model* EnemyBModel = nullptr;
	MCB::Texture* EnemyMapTex = nullptr;
	MCB::Texture* EnemyBMapTex = nullptr;
	MCB::Texture* bossMapTex = nullptr;
	MCB::Texture* droneMapTex = nullptr;
	std::list<std::unique_ptr<EnemyBullet>> bullets;
	Player* playerPtr = nullptr;
	int enemyPopTimer = 0;
	int enemyPopTime = 60;
	int enemysPopNum = 5;
	int dromeAttacktimer = 0;
	int dromeAttacktime = 300;
	bool fireDroneIsyorFront = false;
	bool droneAttacked = false;
	int imotalTimer = 0;
	bool imotalFlag = false;
	int imotalTime = 60;
	std::list<std::unique_ptr<Drone>> drones;
	int DethSound;

	int popTimer = 0;
	int popTime = 60;
	bool isPop = false;

	int bAttackTimer = 0;
	int gbAttackTimer = 0;
	const int maxBAttackTime = 5;


	EnemyManager enemys;
	int hitCount = 0;
	int countResetTimer = 0;
	int countResetMaxTime = 200;
	int rotationPosMoveTimer =  0;
	int maxRotationPosMoveTimer = 600;
	float rotasionSpeed = 0.025f;
	float r = 10;
	MCB::Float3 nextRotationPos;
	MCB::Float3 prevRotationPos;
	MCB::Float3 rotationPos[17] = 
	{
		{0,0,0},
		{2500,0,2500},
		{-2500,0,-2500},
		{2500,0,-2500},
		{-2500,0,2500},
		{1500,0,1500},
		{-1500,0,-1500},
		{1500,0,-1500},
		{-1500,0,1500},
		{2500,1500,2500},
		{-2500,1500,-2500},
		{2500,1500,-2500},
		{-2500,1500,2500},
		{1500,-1500,1500},
		{-1500,-1500,-1500},
		{1500,-1500,-1500},
		{-1500,-1500,1500}
	};
	MCB::Float3 BaseRotationPos = rotationPos[0];
	MCB::Quaternion BossQ = { 0,0,0,1 };
	MCB::Vector3D rightVec = {1,0,0};
	MCB::Vector3D UpVec = {0,1,0};
	MCB::Vector3D rotationVec = {0,1,0};
	float rotationAngle = 0.0f;
	MCB::Vector3D startPositionVec;
	float distance = 1500;
	bool SetRota = false;
	int moveTimer = 0;
	int moveTime = 120;

	void Init(Player* target,MCB::Float3 position,MCB::Model* model,MCB::Model* bulletModel,int AttackType = NoHoming) override;

	void AllMatrixUpdate(MCB::View& view, MCB::Projection& proj);
	void AllDraw();
	void Update() override;
	void FireDrone();
	void PopEnemy();
	void Move();
	void Clrcle();
	void Rotasion();
	void Fire();
	void GFire();
	void Attack();
	void Deth(int damege);
	void Init(Player* target, MCB::Float3 position, 
			MCB::Model* model,MCB::Model* DroneModel, MCB::Model* EnemyModel, MCB::Model* EnemyBModel,
			MCB::Texture* bossmap,MCB::Texture* dronemap, MCB::Texture* enemymap, MCB::Texture* enemyBmap);
};

