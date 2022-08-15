#include "Scene.h"

using namespace MCB;
using namespace DirectX;
using namespace std;

MCB::Scene::~Scene()
{
    soundManager.ReleasexAudio2();
    soundManager.AllDeleteSound();
    delete light;
}

#pragma region 通常変数の初期化と3Dオブジェクトの初期化
void MCB::Scene::Initialize()
{
    matView.CreateMatrixView(XMFLOAT3(0.0f, 0.0f, -10.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f));
    matProjection.CreateMatrixProjection(XMConvertToRadians(45.0f), (float)dxWindow->window_width / dxWindow->window_height, 0.1f, 100000.0f);
    LoadTexture();
    LoadModel();
    LoadSound();
    Object3DInit();
    SpriteInit();
    InitRand();
    //soundManager.PlaySoundWave(testSound, loopFlag);
    light = Light::LightCreate();
    light->SetLightColor({ 1,1,1 });
    Object3d::SetLight(light);


}

void MCB::Scene::Object3DInit()
{
    Skydorm.Init();
    Skydorm.model = skydomeModel.get();
    Skydorm.scale = { 60,60,60 };

    player.Init();

    player.PlayerInit(testBoxModel.get(), testBoxModel.get(), testBoxModel.get(), testBoxModel.get(), testSphereModel.get());

}

#pragma endregion 通常変数の初期化

#pragma region 各種リソースの読み込みと初期化
void MCB::Scene::LoadModel()
{

	skydomeModel = make_shared<Model>("skydome");
    testBoxModel = make_shared<Model>("Box");
    testSphereModel = make_shared<Model>("sphere");
}

void MCB::Scene::LoadTexture()
{
    debugTextTexture = make_shared<Texture>();
    debugTextTexture->CreateTexture(L"Resources\\debugfont.png");
}

void MCB::Scene::LoadSound()
{
}

void MCB::Scene::SpriteInit()
{
    debugText.Init(debugTextTexture.get());
    

}
#pragma endregion 各種リソースの読み込みと初期化

void MCB::Scene::Update()
{
    if (input->IsKeyTrigger(DIK_P))
    {
        //enemys.enemyPop(&player, { (float)GetRand(-500,500),(float)GetRand(-100,100),(float)GetRand(-20,20) }, testBoxModel.get(), testSphereModel.get());
        enemys.enemyPop(&player, { (float)GetRand(-500,500),0,(float)GetRand(-100,100) }, testBoxModel.get(), testSphereModel.get(),EnemyManager::Turret,Enemy::Homing);
    }

    if (input->IsKeyTrigger(DIK_L))
    {
        enemys.enemyPop(&player, { (float)GetRand(-500,500),0,(float)GetRand(-100,100) }, testBoxModel.get(), testSphereModel.get(), EnemyManager::Turret,Enemy::NoHoming);
    }


    player.Update();
    enemys.Update();
    //if (enemys.enemys.size() > 0)
    //{
    //    player.SetTarget(enemys.enemys.begin()->get());
    //    player.SetHomingTarget(enemys.enemys.begin()->get());
    //}

    if (input->IsKeyTrigger(DIK_O))
    {
        player.homingMissileCount++;
    }

    if (input->IsKeyTrigger(DIK_I))
    {
        player.laserCount++;
    }

    if (input->IsKeyTrigger(DIK_U))
    {
        player.bombCount++;
    }

    for (std::unique_ptr<Exp>& exp : exps)
    {
        exp->Update();
    }
    exps.remove_if([](std::unique_ptr<Exp>& exp) {return exp->deleteFlag; });

    CheckAllColision();
    light->Updata();
    //行列変換
    MatrixUpdate();
}

void MCB::Scene::Draw()
{
    draw.PreDraw(*depth, *obj3dPipelinePtr, clearColor);
    //3Dオブジェクト
    Skydorm.Draw();
    player.AllDraw();
    enemys.Draw();
    for (std::unique_ptr<Exp>& exp : exps) { exp->ExpDraw(); }
    //スプライト
    Sprite::SpriteCommonBeginDraw(*spritePipelinePtr);
    debugText.Print(20, 20,2, "fps:%f",fps->GetFPS());
    if(exps.size() > 0) debugText.Print(20, 40, 2, "positin:%f,%f,%f", 
                        exps.begin()->get()->position.x, exps.begin()->get()->position.y,
                        exps.begin()->get()->position.z);

    debugText.Print(20, 60, 2, "exp:float->%f int->%d,NextLevelExp:%d,Level:%d",player.exp, (int)player.exp / 1,
                    player.nextLevelExp,player.Level);
    debugText.Print(20, 80, 2, "homingMissileCount:%d laserCount:%d bombCount;%d", player.homingMissileCount,
        player.laserCount,player.bombCount);
    debugText.AllDraw();
    draw.PostDraw();
}

void MCB::Scene::CheckAllColision()
{
    float mintargetDist = player.targetRay.range;
    //ロックオン
    for (std::unique_ptr<Enemy>& enemy : enemys.enemys)
    {
        float nowtargetdist = 0.0f;
        if (CalcRaySphere(player.targetRay, { enemy->position.x,enemy->position.y,
            enemy->position.z }, enemy->r, &nowtargetdist))
        {
            if (nowtargetdist < mintargetDist && (player.GetTarget() == nullptr || input->IsKeyTrigger(DIK_R)))
            {
                player.SetTarget(enemy.get());
                player.SetHomingTarget(enemy.get());
                mintargetDist = nowtargetdist;
            }
            
        }
    }


    //敵と自弾の当たり判定
    for (std::unique_ptr<PlayerBullet>& bullet : player.bullets)
    {
        for (std::unique_ptr<Enemy>& enemy : enemys.enemys)
        {
            if (CalcSphere({ bullet.get()->position.x,bullet.get()->position.y,bullet.get()->position.z }, bullet.get()->r,
                { enemy->position.x,enemy->position.y,enemy->position.z }, enemy->r))
            {
                bullet->BulletHit();
                enemy->Deth(bullet.get()->damage);
                if (enemy->deleteFlag)
                {
                    for (int i = 0; i < 20; i++)
                    {
                        Sporn({ enemy->position.x,enemy->position.y,enemy->position.z }, enemy->expPoint);
                        DeleteExp();
                    }
                    player.SetTarget(nullptr);
                    player.SetHomingTarget(nullptr);
                }
                continue;
            }

            if (CalcSphere({ bullet.get()->position.x,bullet.get()->position.y,bullet.get()->position.z }, bullet.get()->slerpStopR,
                { enemy->position.x,enemy->position.y,enemy->position.z }, enemy->r))
            {
                bullet->SlerpHit();
            }


        }
    }

    //敵とホーミングミサイルの当たり判定
    for (std::unique_ptr<HomingMissile>& missile : player.homingMissile)
    {
        for (std::unique_ptr<Enemy>& enemy : enemys.enemys)
        {
            for (int i = 0; i < 3; i++)
            {
                if (missile->deleteFlag[i]) continue;

                if (CalcSphere({ missile.get()->homingMissiles[i].position.x,
                    missile.get()->homingMissiles[i].position.y,
                    missile.get()->homingMissiles[i].position.z }, missile.get()->r,
                    { enemy->position.x,enemy->position.y,enemy->position.z }, enemy->r) && !enemy->deleteFlag)
                {
                   
                    missile->BulletHit(i);
                    enemy->Deth(missile.get()->damage);
                    if (enemy->deleteFlag)
                    {
                        for (int i = 0; i < 20; i++)
                        {
                            Sporn({ enemy->position.x,enemy->position.y,enemy->position.z }, enemy->expPoint);
                            DeleteExp();
                        }
                        player.SetHomingTarget(nullptr);
                        player.SetTarget(nullptr);
                    }
                    continue;
                }

                if (CalcSphere({ missile.get()->homingMissiles[i].position.x,
                    missile.get()->homingMissiles[i].position.y,
                    missile.get()->homingMissiles[i].position.z }, missile.get()->slerpStopR,
                    { enemy->position.x,enemy->position.y,enemy->position.z }, enemy->r))
                {
                    missile->SetSlerp(i,false);
                    missile->SetMaxSpeed(missile->r + enemy.get()->r - 5,i);
                }
                else if (CalcSphere({ missile.get()->homingMissiles[i].position.x,
                    missile.get()->homingMissiles[i].position.y,
                    missile.get()->homingMissiles[i].position.z }, missile.get()->slerpStopR * 10,
                    { enemy->position.x,enemy->position.y,enemy->position.z }, enemy->r))
                {
                    missile->SetSlerp(i, true);
                    missile->SetMaxSpeed(missile->maxDefaultSpeed,i);
                }
                else if (CalcSphere({ missile.get()->homingMissiles[i].position.x,
                    missile.get()->homingMissiles[i].position.y,
                    missile.get()->homingMissiles[i].position.z }, missile.get()->slerpStopR + missile->speed[i],
                    { enemy->position.x,enemy->position.y,enemy->position.z }, enemy->r))
                {
                    missile->SetMaxSpeed(missile->r + enemy.get()->r - 5, i);
                }
                else
                {
                    missile->SetMaxSpeed(missile->maxDefaultSpeed, i);
                }

            }


        }
    }

    //敵とレーザーの当たり判定
    for (std::unique_ptr<Laser>& laser : player.lasers)
    {
        for (std::unique_ptr<Enemy>& enemy : enemys.enemys)
        {
            if (CalcRaySphere(laser->laser, { enemy->position.x,enemy->position.y,enemy->position.z }, enemy->r))
            {
                enemy->Deth(laser.get()->damage);
                if (enemy->deleteFlag)
                {
                    for (int i = 0; i < 20; i++)
                    {
                        Sporn({ enemy->position.x,enemy->position.y,enemy->position.z }, enemy->expPoint);
                        DeleteExp();
                    }
                    player.SetTarget(nullptr);
                    player.SetHomingTarget(nullptr);
                }
                continue;
            }
          
        }
    }


    for (std::unique_ptr<Bomb>& bomb : player.bombs)
    {

        for (std::unique_ptr<Enemy>& enemy : enemys.enemys)
        {
            if (CalcSphere({ bomb.get()->position.x,
                   bomb.get()->position.y,
                    bomb.get()->position.z }, bomb.get()->r,
                { enemy->position.x,enemy->position.y,enemy->position.z }, enemy->r) && !enemy->deleteFlag)
            {
                enemy->Deth(bomb.get()->damage);
                if (enemy->deleteFlag)
                {
                    for (int i = 0; i < 20; i++)
                    {
                        Sporn({ enemy->position.x,enemy->position.y,enemy->position.z }, enemy->expPoint);
                        DeleteExp();
                    }
                    player.SetHomingTarget(nullptr);
                    player.SetTarget(nullptr);
                }
            }
        }
    }
    //敵弾とプレイヤーの当たり判定
    for (std::unique_ptr<Enemy>& enemy : enemys.enemys)
    {
        for (std::unique_ptr<EnemyBullet>& bullet : enemy->bullets)
        {
            if (CalcSphere({ bullet->position.x,bullet->position.y,bullet->position.z }, bullet->r,
                { player.position.x,player.position.y,player.position.z }, player.r))
            {
                bullet->BulletHit();
                player.EnemyBulletHit(bullet->damage);
                continue;
            }

            if (CalcSphere({ bullet->position.x,bullet->position.y,bullet->position.z }, bullet->slerpStopR,
                { player.position.x,player.position.y,player.position.z }, player.r))
            {
                bullet->SlerpHit();
            }
        }
    }

    //経験値とプレイヤーの当たり判定
    for (std::unique_ptr<Exp>& exp : exps)
    {
        if (CalcSphere({ exp->position.x,exp->position.y,exp->position.z }, exp->rudius,
            { player.position.x,player.position.y,player.position.z }, player.r))
        {
            exp->GetExp();
            player.GetExp(exp->expPoint);
            continue;
        }

        if (CalcSphere({ exp->position.x,exp->position.y,exp->position.z }, exp->slerpStopR,
            { player.position.x,player.position.y,player.position.z }, player.r))
        {
            exp->StopSlerp();
        }

        if (CalcSphere({ exp->position.x,exp->position.y,exp->position.z }, exp->approachR,
            { player.position.x,player.position.y,player.position.z }, player.r))
        {
            exp->ExpApproach();
        }
    }


    if (player.GetTarget() == nullptr)
    {
        for (std::unique_ptr<PlayerBullet>& bullet : player.bullets)
        {
            bullet.get()->SetTarget(nullptr);
        }
    }

    if (player.GetHomingTarget() == nullptr)
    {
        for (std::unique_ptr<HomingMissile>& missile : player.homingMissile)
        {
            missile.get()->SetTarget(nullptr);
        }
    }

    
}

void MCB::Scene::MatrixUpdate()
{
    matProjection.UpdataMatrixProjection();
    matView.FollowingFor3DObject(player.position,player.nowFrontVec,{100,50,100},player.UpVec);
    matView.UpDateMatrixView();
    Skydorm.MatrixUpdata(matView, matProjection);
    enemys.UpdateMatrix(matView, matProjection);
    player.AllMatrixUpdate(matView, matProjection);
    for (std::unique_ptr<Exp>& exp : exps)
    {
        exp->MatrixUpdata(matView, matProjection);
    }



}

MCB::Scene::Scene(RootParameter* root, Depth* depthptr, PipelineRootSignature* pipeline, PipelineRootSignature* pipeline1)
{
	rootparamsPtr = root;
	obj3dPipelinePtr = pipeline;
	spritePipelinePtr = pipeline1;
    depth = depthptr;
}

void MCB::Scene::Sporn(Float3 enemyPos,float expPoint)
{
    std::unique_ptr<Exp> exp = make_unique<Exp>();
    exp->Init();
    exp->model = testBoxModel.get();
    exp->scale = { 2,2,2 };
    exp->ExpInit(GetRand(0, 10),
        { enemyPos.x,enemyPos.y,enemyPos.z },
        { (float)GetRand(-1000, 1000) / 1000.0f,(float)GetRand(-1000,1000) / 1000.0f,
        (float)GetRand(-1000,1000) / 1000.0f }, &player);
    exp->expPoint = expPoint;
    exps.push_back(std::move(exp));

}

void MCB::Scene::DeleteExp()
{
    for (std::unique_ptr<Exp>& exp : exps)
    {
        if (exps.size() <= 180)
        {
            break;
        }
        exps.remove(exp);
    }
}