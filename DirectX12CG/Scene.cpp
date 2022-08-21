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

#pragma region �ʏ�ϐ��̏�������3D�I�u�W�F�N�g�̏�����
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

#pragma endregion �ʏ�ϐ��̏�����

#pragma region �e�탊�\�[�X�̓ǂݍ��݂Ə�����
void MCB::Scene::LoadModel()
{

	skydomeModel = make_shared<Model>("skydome");
    testBoxModel = make_shared<Model>("Box");
    testSphereModel = make_shared<Model>("sphere",true);
}

void MCB::Scene::LoadTexture()
{
    debugTextTexture = make_shared<Texture>();
    debugTextTexture->CreateTexture(L"Resources\\debugfont.png");
    mapBackTexture = make_shared<Texture>();
    mapBackTexture->CreateTexture(L"Resources\\testMapBack.png"); 
    mapPlayerTexture = make_shared<Texture>();
    mapPlayerTexture->CreateTexture(L"Resources\\reimu.png");
    mapPlayerBTexture = make_shared<Texture>();
    mapPlayerBTexture->CreateTexture(L"Resources\\reimu.png");
    mapEnemyTexture = make_shared<Texture>();
    mapEnemyTexture->CreateTexture(L"Resources\\reimu.png");
    mapEnemyBTexture = make_shared<Texture>();
    mapEnemyBTexture->CreateTexture(L"Resources\\reimu.png");
}

void MCB::Scene::LoadSound()
{
}

void MCB::Scene::SpriteInit()
{
    debugText.Init(debugTextTexture.get());
    mapBack = mapBack.CreateSprite();
    mapBack.InitMatProje();
    mapPlayer = mapPlayer.CreateSprite();
    mapPlayer.InitMatProje();
    

}
#pragma endregion �e�탊�\�[�X�̓ǂݍ��݂Ə�����

void MCB::Scene::Update()
{
    if (input->IsKeyTrigger(DIK_P))
    {
        //enemys.enemyPop(&player, { (float)GetRand(-500,500),(float)GetRand(-100,100),(float)GetRand(-20,20) }, testBoxModel.get(), testSphereModel.get());
        enemys.enemyPop(&player, { (float)GetRand(-500,500),0,(float)GetRand(-100,100) }, testBoxModel.get(), testSphereModel.get(),mapEnemyTexture.get(),mapEnemyBTexture.get(), EnemyManager::Turret, Enemy::Homing);
    }

    if (input->IsKeyTrigger(DIK_L))
    {
        enemys.enemyPop(&player, { (float)GetRand(-500,500),0,(float)GetRand(-100,100) }, testBoxModel.get(), testSphereModel.get(), mapEnemyTexture.get(), mapEnemyBTexture.get(), EnemyManager::Turret,Enemy::NoHoming);
    }

    if (input->IsKeyTrigger(DIK_M))
    {
       if(enemys.enemys.size() > 0) enemys.enemyPop(&player, {(float)GetRand(-100,100),(float)GetRand(-100,100),(float)GetRand(-100,100)}, testBoxModel.get(), testSphereModel.get(), mapEnemyTexture.get(), mapEnemyBTexture.get(), EnemyManager::Guard, Enemy::NoHoming, enemys.enemys.begin()->get());
    }

    if (input->IsKeyTrigger(DIK_K))
    {
        enemys.enemyPop(&player, { (float)GetRand(-100,100),(float)GetRand(-100,100),(float)GetRand(-100,100) }, testBoxModel.get(), testSphereModel.get(), mapEnemyTexture.get(), mapEnemyBTexture.get(), EnemyManager::Rash);
    }

    if (input->IsKeyTrigger(DIK_N))
    {
        enemys.enemyPop(&player, { (float)GetRand(-100,100),(float)GetRand(-100,100),(float)GetRand(-100,100) }, testBoxModel.get(), testSphereModel.get(), mapEnemyTexture.get(), mapEnemyBTexture.get(), EnemyManager::Circumference);
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
    //�s��ϊ�
    MatrixUpdate();
}

void MCB::Scene::Draw()
{
    draw.PreDraw(*depth, *obj3dPipelinePtr, clearColor);
    //3D�I�u�W�F�N�g
    Skydorm.Draw();
    player.AllDraw();
    enemys.Draw();
    for (std::unique_ptr<Exp>& exp : exps) { exp->ExpDraw(); }
    //�X�v���C�g
    Sprite::SpriteCommonBeginDraw(*spritePipelinePtr);

    mapBack.SpriteDraw(mapBack, *mapBackTexture.get(), mapPosition.x, mapPosition.y, mapSize, mapSize);
    mapPlayer.rotation = player.rotasion.y;
    mapPlayer.SpriteUpdate(mapPlayer);
    mapPlayer.SpriteDraw(mapPlayer, *mapPlayerTexture.get(),mapPosition.x, mapPosition.y
                            , 20, 20);
    for (std::unique_ptr<PlayerBullet>& bullet : player.bullets)
    {
        MiniMapDraw(bullet->sprite, { bullet->position.x,bullet->position.z }, { player.position.x,player.position.z }, mapPlayerBTexture.get());
    }
    for (std::unique_ptr<Enemy>& enemy : enemys.enemys)
    {
        MiniMapDraw(enemy->sprite, { enemy->position.x,enemy->position.z }, { player.position.x,player.position.z }, enemy->mapTexture);
    }
    for (std::unique_ptr<Enemy>& enemy : enemys.enemys)
    {
        for (std::unique_ptr<EnemyBullet>& bullet : enemy->bullets)
        {
            MiniMapDraw(bullet->sprite, { bullet->position.x,bullet->position.z }, { player.position.x,player.position.z }, bullet->mapTexture);
        }
    }


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

void MCB::Scene::MiniMapDraw(Sprite sprite ,Float2 objectPos, Float2 playerPos,Texture* maptex)
{
    float spriteSize = 20;
    Float2 spritePos = { (objectPos.x - playerPos.x) / mapOffSet + mapPosition.x,
                        mapPosition.y - (objectPos.y - playerPos.y) / mapOffSet };
    if (spritePos.x - spriteSize < 0 || spritePos.x + spriteSize > mapSize || spritePos.y - spriteSize < mapPosition.y - mapSize / 2 || spritePos.y + spriteSize > mapPosition.y + mapSize / 2)
    {
        return;
    }
   sprite.SpriteDraw(sprite, *maptex, spritePos.x, spritePos.y
        , spriteSize, spriteSize);
}

void MCB::Scene::CheckAllColision()
{
    float mintargetDist = player.targetRay.range;
    //���b�N�I��
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


    //�G�Ǝ��e�̓����蔻��
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

    //�G�ƃz�[�~���O�~�T�C���̓����蔻��
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

    //�G�ƃ��[�U�[�̓����蔻��
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
    //�G�e�ƃv���C���[�̓����蔻��
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

    //�o���l�ƃv���C���[�̓����蔻��
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
    float tempExp = 0;
    for (std::unique_ptr<Exp>& exp : exps)
    {
        if (exps.size() <= 180)
        {
            break;
        }
        else
        {
            tempExp += exp.get()->expPoint;
            exps.remove(exp);
        }
    }

    exps.begin()->get()->expPoint += tempExp;
}