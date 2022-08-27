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
    SpriteInit();
    InitRand();
    Skydorm.Init();
    Skydorm.model = skydomeModel.get();
    Skydorm.scale = { 60,60,60 };
    ground.Init();
    ground.model = testGroundModel.get();
    ground.position = { 0,-1600,0 };
    ground.scale = { 120,120,120 };
    player.Init();
    boss.Object3d::Init();
    rockOnleticle.Init();
    rockOnleticle.model = planeModel.get();
    rockOnleticle.scale = {60,60,60};
    TitleSceneInit();
    GameSceneInit();
    ClearSceneInit();
    OverSceneInit();

    //soundManager.PlaySoundWave(testSound, loopFlag);
    light = Light::LightCreate();
    light->SetLightColor({ 0.7,0.7,0.7 });
    Object3d::SetLight(light);


}

void MCB::Scene::Object3DInit()
{
    player.PlayerInit(playerModel.get(), testBoxModel.get(), testBoxModel.get(), rayModel.get(), bombModel.get(),rockOnRayModel.get());
    boss.Init(&player, { (player.nowFrontVec * boss.distance).vec },
        bossModel.get(), enemyModel.get(), enemyModel.get(), testSphereModel.get(),
        mapEnemyTexture.get(), mapEnemyTexture.get(), mapEnemyTexture.get(), mapEnemyBTexture.get());

    exps.clear();
}

#pragma endregion 通常変数の初期化

#pragma region 各種リソースの読み込みと初期化
void MCB::Scene::LoadModel()
{

	skydomeModel = make_shared<Model>("skydome");
    testBoxModel = make_shared<Model>("Box");
    testSphereModel = make_shared<Model>("sphere",true);
    testGroundModel = make_shared<Model>("ground",true);
    rayModel = make_shared<Model>("Ray");
    rayModel->material.constMapMaterial->color.w = 0.8f;
    rockOnRayModel = make_shared<Model>("Ray");
    rockOnRayModel->material.constMapMaterial->color.w = 0.2f;
    playerModel = make_shared<Model>("player");
    bombModel = make_shared<Model>("bomb");
    enemyModel = make_shared<Model>("enemy");
    bossModel = make_shared<Model>("boss");
    planeModel = make_shared<Model>("plane");
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
    mapPlayerBTexture->CreateTexture(L"Resources\\testplayerB.png");
    mapEnemyTexture = make_shared<Texture>();
    mapEnemyTexture->CreateTexture(L"Resources\\testenemy.png");
    mapEnemyBTexture = make_shared<Texture>();
    mapEnemyBTexture->CreateTexture(L"Resources\\testEnemyB.png");
    titleName = make_shared<Texture>();
    titleName->CreateTexture(L"Resources\\TitleName.png");
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
    boss.sprite = boss.sprite.CreateSprite();
    titleNameSprite = titleNameSprite.CreateSprite();

}
void MCB::Scene::ChengeScene()
{
    if (nowScene != nextScene)
    {
        nowScene = nextScene;
    }
}
void MCB::Scene::GameSceneInit()
{
    Object3DInit();
    exps.clear();
}
void MCB::Scene::TitleSceneInit()
{
    player.PlayerInit(playerModel.get(), testBoxModel.get(), testBoxModel.get(), rayModel.get(), bombModel.get(), rockOnRayModel.get());
    titlePos = { (float)dxWindow->window_width / 2, (float)dxWindow->window_height / 2 };
    titleSize = { (float)160 * 6,(float)64 * 6 };
    titleTimer = 0;
    titleMaxTime = 120;
    nowTutorial = None;
    player.Level = 0;
    nowSPAttackTuto = None;
    enemys.enemys.clear();;
    exps.clear();
    SPAttackTutoEnd = false;
    titleMove = false;
}
void MCB::Scene::ClearSceneInit()
{
    player.PlayerInit(playerModel.get(), testBoxModel.get(), testBoxModel.get(), rayModel.get(), bombModel.get(), rockOnRayModel.get());
}
void MCB::Scene::OverSceneInit()
{
    player.PlayerInit(playerModel.get(), testBoxModel.get(), testBoxModel.get(), rayModel.get(), bombModel.get(), rockOnRayModel.get());
}
#pragma endregion 各種リソースの読み込みと初期化

void MCB::Scene::Update()
{
 
    switch (nowScene)
    {
    case Title:
        TitleSceneUpdate();
        break;
    case Game:
        GameSceneUpdate();
        break;
    case Clear:
        ClearSceneUpdate();
        break;
    case GameOver:
        OverSceneUpdate();
        break;
    default:
        nowScene = Title;
        TitleSceneInit();
        TitleSceneUpdate();
        break;
    }


    
    light->Updata();
    //行列変換
    MatrixUpdate();
}

void MCB::Scene::GameSceneDraw()
{
    Skydorm.Draw();
    ground.Draw();
    player.AllDraw();
    boss.AllDraw();
    enemys.Draw();
    if (player.GetTarget())rockOnleticle.Draw();
    for (std::unique_ptr<Drone>& d : testD)
    {
        d->Draw();
    }
    for (std::unique_ptr<Exp>& exp : exps) { exp->ExpDraw(); }
    //スプライト
    Sprite::SpriteCommonBeginDraw(*spritePipelinePtr);

    mapBack.SpriteDraw(mapBack, *mapBackTexture.get(), mapPosition.x, mapPosition.y, mapSize, mapSize);
    mapPlayer.rotation = player.rotasion.y;

    mapPlayer.SpriteDraw(mapPlayer, *mapPlayerTexture.get(),mapPosition.x, mapPosition.y
                            , 20, 20);
    for (std::unique_ptr<PlayerBullet>& bullet : player.bullets)
    {
        MiniMapDraw(bullet->sprite, { bullet->position.x,bullet->position.z }, { player.position.x,player.position.z }, mapPlayerBTexture.get());
    }
    for (std::unique_ptr<Enemy>& enemy : boss.enemys.enemys)
    {
        MiniMapDraw(enemy->sprite, { enemy->position.x,enemy->position.z }, { player.position.x,player.position.z }, enemy->mapTexture);
    }
    for (std::unique_ptr<Enemy>& enemy : boss.enemys.enemys)
    {
        for (std::unique_ptr<EnemyBullet>& bullet : enemy->bullets)
        {
            MiniMapDraw(bullet->sprite, { bullet->position.x,bullet->position.z }, { player.position.x,player.position.z }, bullet->mapTexture);
        }
    }
    for (std::unique_ptr<Drone>& d : boss.drones)
    {
        MiniMapDraw(d->sprite, { d->position.x,d->position.z }, { player.position.x, player.position.z }, d->mapTexture);
    }
    MiniMapDraw(boss.sprite, { boss.BaseRotationPos.x,boss.BaseRotationPos.z }, { player.position.x, player.position.z }, boss.bossMapTex,20);
    MiniMapDraw(boss.sprite, { boss.nextRotationPos.x,boss.nextRotationPos.z }, { player.position.x, player.position.z }, boss.bossMapTex,20);
    MiniMapDraw(boss.sprite, { boss.position.x,boss.position.z }, { player.position.x, player.position.z }, boss.bossMapTex,40);



    debugText.Print(20, 200,2, "score:%d BossHp:%d",player.score,boss.hp);
    debugText.Print(20, 400,2, "bossBaseRotaMoveTimer:%d time:%d hp:%d",boss.rotationPosMoveTimer,boss.maxRotationPosMoveTimer,boss.hp);
    //if(exps.size() > 0) debugText.Print(20, 40, 2, "positin:%f,%f,%f", 
    //                    exps.begin()->get()->position.x, exps.begin()->get()->position.y,
    //                    exps.begin()->get()->position.z);

    debugText.Print(20, 0, 2, "exp:float->%f int->%d,NextLevelExp:%d,Level:%d,hp:%d",player.exp, (int)player.exp / 1,
                    player.nextLevelExp,player.Level,player.GetHp());
    debugText.Print(20, 80, 2, "homingMissileCount:%d laserCount:%d bombCount;%d", player.homingMissileCount,
        player.laserCount,player.bombCount);
    debugText.AllDraw();
}

void MCB::Scene::TitleSceneDraw()
{
    Skydorm.Draw();
    ground.Draw();
    player.AllDraw();
    enemys.Draw();
    if (player.GetTarget())rockOnleticle.Draw();
    for (std::unique_ptr<Exp>& exp : exps) { exp->ExpDraw(); }
    //スプライト
    Sprite::SpriteCommonBeginDraw(*spritePipelinePtr);

    mapBack.SpriteDraw(mapBack, *mapBackTexture.get(), mapPosition.x, mapPosition.y, mapSize, mapSize);
    mapPlayer.rotation = player.rotasion.y;

    mapPlayer.SpriteDraw(mapPlayer, *mapPlayerTexture.get(), mapPosition.x, mapPosition.y
        , 20, 20);
    for (std::unique_ptr<PlayerBullet>& bullet : player.bullets)
    {
        MiniMapDraw(bullet->sprite, { bullet->position.x,bullet->position.z }, { player.position.x,player.position.z }, mapPlayerBTexture.get());
    }
    for (std::unique_ptr<Enemy>& enemy : enemys.enemys)
    {
        MiniMapDraw(enemy->sprite, { enemy->position.x,enemy->position.z }, { player.position.x,player.position.z }, enemy->mapTexture);
    }
    for (std::unique_ptr<Enemy>& enemy :enemys.enemys)
    {
        for (std::unique_ptr<EnemyBullet>& bullet : enemy->bullets)
        {
            MiniMapDraw(bullet->sprite, { bullet->position.x,bullet->position.z }, { player.position.x,player.position.z }, bullet->mapTexture);
        }
    }
    titleNameSprite.SpriteDraw(titleNameSprite, *titleName, titlePos.x, titlePos.y, titleSize.x , titleSize.y);

    debugText.Print(20, 200, 2, "TitleScene HiScore:%d Score:%d",player.HiScore, player.score);
    debugText.Print(20, 0, 2, "exp:float->%f int->%d,NextLevelExp:%d,Level:%d,hp:%d", player.exp, (int)player.exp / 1,
        player.nextLevelExp, player.Level, player.GetHp());
    debugText.Print(20, 80, 2, "homingMissileCount:%d laserCount:%d bombCount;%d", player.homingMissileCount,
        player.laserCount, player.bombCount);
    debugText.AllDraw();
}

void MCB::Scene::ClearSceneDraw()
{    //スプライト
    Sprite::SpriteCommonBeginDraw(*spritePipelinePtr);
    debugText.Print(20, 200, 2," ClearScene");
    debugText.AllDraw();
}

void MCB::Scene::OverSceneDraw()
{    //スプライト
    Sprite::SpriteCommonBeginDraw(*spritePipelinePtr);
    debugText.Print(20, 200, 2, "OverScene");
    debugText.AllDraw();
}

void MCB::Scene::GameSceneUpdate()
{

    if (degug)
    {
        if (input->IsKeyTrigger(DIK_R))
        {
            Object3DInit();
        }

        if (input->IsKeyTrigger(DIK_P))
        {
            //enemys.enemyPop(&player, { (float)GetRand(-500,500),(float)GetRand(-100,100),(float)GetRand(-20,20) }, testBoxModel.get(), testSphereModel.get());
            enemys.enemyPop(&player, { (float)GetRand(-500,500),0,(float)GetRand(-100,100) }, enemyModel.get(), testSphereModel.get(), mapEnemyTexture.get(), mapEnemyBTexture.get(), EnemyManager::Turret, Enemy::Homing);
        }

        if (input->IsKeyTrigger(DIK_L))
        {
            enemys.enemyPop(&player, { (float)GetRand(-500,500),0,(float)GetRand(-100,100) }, enemyModel.get(), testSphereModel.get(), mapEnemyTexture.get(), mapEnemyBTexture.get(), EnemyManager::Turret, Enemy::NoHoming);
        }

        if (input->IsKeyTrigger(DIK_M))
        {
            if (enemys.enemys.size() > 0) enemys.enemyPop(&player, { (float)GetRand(-100,100),(float)GetRand(-100,100),(float)GetRand(-100,100) }, enemyModel.get(), testSphereModel.get(), mapEnemyTexture.get(), mapEnemyBTexture.get(), EnemyManager::Guard, Enemy::NoHoming, enemys.enemys.begin()->get());
        }

        if (input->IsKeyTrigger(DIK_K))
        {
            enemys.enemyPop(&player, { (float)GetRand(-100,100),(float)GetRand(-100,100),(float)GetRand(-100,100) }, enemyModel.get(), testSphereModel.get(), mapEnemyTexture.get(), mapEnemyBTexture.get(), EnemyManager::Rash);
        }

        if (input->IsKeyTrigger(DIK_N))
        {
            enemys.enemyPop(&player, { (float)GetRand(-100,100),(float)GetRand(-100,100),(float)GetRand(-100,100) }, enemyModel.get(), testSphereModel.get(), mapEnemyTexture.get(), mapEnemyBTexture.get(), EnemyManager::Circumference);
        }

        if (input->IsKeyTrigger(DIK_J))
        {
            if (enemys.enemys.size() > 0)
            {
                unique_ptr<Drone> d = make_unique<Drone>();
                d->BossPtr = enemys.enemys.begin()->get();
                d->Init(&player, { player.nowFrontVec.vec.x,player.nowFrontVec.vec.y,player.nowFrontVec.vec.z }, testBoxModel.get());
                d->mapTexture = mapEnemyTexture.get();
                testD.push_back(move(d));
            }
        }

        if (input->IsKeyTrigger(DIK_G))
        {
            testD.begin()->get()->Shot();
        }
        for (std::unique_ptr<Drone>& d : testD)
        {
            d->Update();
        }
        testD.remove_if([](std::unique_ptr<Drone>& d) {return d->deleteFlag; });
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


    }

    boss.Update();
    player.Update();
    enemys.Update();

    for (std::unique_ptr<Exp>& exp : exps)
    {
        exp->Update();
    }
    exps.remove_if([](std::unique_ptr<Exp>& exp) {return exp->deleteFlag; });

    if(boss.dethFlag)
    {
        nextScene = Clear;
        ClearSceneInit();
    }
    else if (player.dethFlag)
    {
        nextScene = GameOver;
        OverSceneInit();
    }

    CheckAllColision();
    if (player.GetTarget())rockOnleticle.position = player.GetTarget()->position;

}
void MCB::Scene::TitleSceneUpdate()
{
    
    switch (nowTutorial)
    {
    case None:
        if (input->IsKeyTrigger(DIK_SPACE))
        {
            titleMove = true;
        }
        if (titleMove)
        {
            titleTimer++;
            if (titleTimer >= 0)
            {
                titleSize.x = InOutQuad(160 * 6, 160 * 1, titleMaxTime, titleTimer);
                titleSize.y = InOutQuad(64 * 6, 64 * 1, titleMaxTime, titleTimer);
                titlePos.x = InOutQuad((float)dxWindow->window_width / 2, (float)dxWindow->window_width - 160 / 2, titleMaxTime, titleTimer);
                titlePos.y = InOutQuad((float)dxWindow->window_height / 2, 64 / 2, titleMaxTime, titleTimer);
            }
        }
        if (titleTimer >= titleMaxTime)
        {
            titleMove == false;
            nowTutorial = Move;
            player.Level = 0;
        }
        break;
    case Move:
        player.Update();
        if (player.prevPosition.x != player.position.x || player.prevPosition.y != player.position.y || player.prevPosition.z != player.position.z)
        {
            nowTutorial = RockOn;
        }
        break;
    case RockOn:
        player.Update();
        if (player.bullets.size() > 0) player.bullets.clear();
        if(enemys.enemys.size() < 1)enemys.enemyPop(&player, { (float)GetRand(-100,100),0,(float)GetRand(30,100) }, enemyModel.get(), testSphereModel.get(), mapEnemyTexture.get(), mapEnemyBTexture.get(), EnemyManager::Turret, Enemy::NoHoming);
        if (player.GetTarget() != nullptr) nowTutorial = Attack;
        break;
    case Attack:
        player.Update();
        if (enemys.enemys.size() < 1)enemys.enemyPop(&player, { (float)GetRand(-100,100),0,(float)GetRand(30,100) }, enemyModel.get(), testSphereModel.get(), mapEnemyTexture.get(), mapEnemyBTexture.get(), EnemyManager::Turret, Enemy::NoHoming);
        break;
    case ExpTuto:
        if (enemys.enemys.size() < 1)enemys.enemyPop(&player, { (float)GetRand(-100,100),0,(float)GetRand(30,100) }, enemyModel.get(), testSphereModel.get(), mapEnemyTexture.get(), mapEnemyBTexture.get(), EnemyManager::Turret, Enemy::NoHoming);
        player.Update();
        break;
    case SPAttack:
        if (enemys.enemys.size() < 1)enemys.enemyPop(&player, { (float)GetRand(-100,100),0,(float)GetRand(30,100) }, enemyModel.get(), testSphereModel.get(), mapEnemyTexture.get(), mapEnemyBTexture.get(), EnemyManager::Turret, Enemy::NoHoming);
        switch (nowSPAttackTuto)
        {
        case HomingMisilleTuto:
            if (player.homingMissileCount < 3) player.homingMissileCount = 3;
            break;
        case LaserTuto:
            if (player.homingMissile.size() > 0) player.homingMissile.clear();
            if (player.laserCount < 2) player.laserCount = 2;
            break;
        case BombTuto:
            if (player.bombCount < 1) player.bombCount = 1;
            if (player.bombs.size() < 1 && SPAttackTutoEnd)
            {
                nowSPAttackTuto = None;
                nowTutorial = End;
            }
            player.exp = 0;
            break;
        default:
            break;
        }
        player.Update();
        player.Level = 0;
        break;
    case End:
        if (enemys.enemys.size() < 1)enemys.enemyPop(&player, { (float)GetRand(-100,100),0,(float)GetRand(30,100) }, enemyModel.get(), testSphereModel.get(), mapEnemyTexture.get(), mapEnemyBTexture.get(), EnemyManager::Turret, Enemy::NoHoming);
        if (player.homingMissileCount < 3) player.homingMissileCount = 3;
        if (player.laserCount < 2) player.laserCount = 2;
        if (player.bombCount < 1) player.bombCount = 1;

        player.Update();
        for (std::unique_ptr<Exp>& exp : exps)
        {
            exp->Update();
        }
        exps.remove_if([](std::unique_ptr<Exp>& exp) {return exp->deleteFlag; });

        if (player.exp >= 10)
        {
            player.Level = 1;
        }

        if (player.Level >= 1)
        {
            nextScene = Game;
            enemys.enemys.clear();;
            exps.clear();
            GameSceneInit();
        }
        break;
    default:
        break;
    }

    TutorialCheckAllColision();
    for (std::unique_ptr<Exp>& exp : exps)
    {
        exp->Update();
    }
    exps.remove_if([](std::unique_ptr<Exp>& exp) {return exp->deleteFlag; });

    enemys.enemys.remove_if([](std::unique_ptr<Enemy>& enemys) {return enemys->deleteFlag; });

    if (player.GetTarget())rockOnleticle.position = player.GetTarget()->position;
}

void MCB::Scene::ClearSceneUpdate()
{
    if (input->IsKeyTrigger(DIK_SPACE))
    {
        nextScene = Title;
        GameSceneInit();
    }
}
void MCB::Scene::OverSceneUpdate()
{
    if (input->IsKeyTrigger(DIK_SPACE))
    {
        nextScene = Title;
        GameSceneInit();
    }
}
void MCB::Scene::Draw()
{
    draw.PreDraw(*depth, *obj3dPipelinePtr, clearColor);
    switch (nowScene)
    {
    case Title:
        TitleSceneDraw();
        break;
    case Game:
        GameSceneDraw();
        break;
    case Clear:
        ClearSceneDraw();
        break;
    case GameOver:
        OverSceneDraw();
        break;
    default:
        nowScene = Title;
        TitleSceneInit();
        TitleSceneUpdate();
        TitleSceneDraw();
        break;
    }
    draw.PostDraw();
}

void MCB::Scene::MiniMapDraw(Sprite sprite ,Float2 objectPos, Float2 playerPos,Texture* maptex,float SpriteSize)
{
    float spriteSize = SpriteSize;
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
    //エリア外侵入阻止
    if (!CalcSphere({ player.position.x,player.position.y,player.position.z }, player.r, { 0.f,0.f,0.f }, 5500))
    {
        player.position.x = player.prevPosition.x;
        player.position.y = player.prevPosition.y;
        player.position.z = player.prevPosition.z;
    }

    if (player.position.y < -1500)
    {
        player.position.y = player.prevPosition.y;
    }

    float mintargetDist = player.targetRay.range;
    {
        float nowtargetdist = 0.0f;
        //ロックオン
        for (std::unique_ptr<Enemy>& enemy : boss.enemys.enemys)
        {
            if (CalcRaySphere(player.targetRay, { enemy->position.x,enemy->position.y,
                enemy->position.z }, enemy->r, &nowtargetdist))
            {
                if (nowtargetdist < mintargetDist)
                {
                    player.SetTarget(enemy.get());
                    player.SetHomingTarget(enemy.get());
                    mintargetDist = nowtargetdist;
                }

            }

        }
        if (CalcRaySphere(player.targetRay, { boss.position.x,boss.position.y,
            boss.position.z }, boss.r, &nowtargetdist))
        {
            if (nowtargetdist < mintargetDist)
            {
                player.SetTarget(&boss);
                player.SetHomingTarget(&boss);
                mintargetDist = nowtargetdist;
            }

        }
    }

    //敵と自弾の当たり判定
    for (std::unique_ptr<PlayerBullet>& bullet : player.bullets)
    {
        for (std::unique_ptr<Enemy>& enemy : boss.enemys.enemys)
        {
            if (CalcSphere({ bullet.get()->position.x,bullet.get()->position.y,bullet.get()->position.z }, bullet.get()->r,
                { enemy->position.x,enemy->position.y,enemy->position.z }, enemy->r))
            {
                bullet->BulletHit();
                enemy->Deth(bullet.get()->damage);
                player.score += bullet.get()->damage * player.Level;
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

        if (CalcSphere({ bullet.get()->position.x,bullet.get()->position.y,bullet.get()->position.z }, bullet.get()->r,
            { boss.position.x,boss.position.y,boss.position.z }, boss.r))
        {
            bullet->BulletHit();
            boss.Deth(bullet.get()->damage);
            boss.imotalTimer = 0;
            player.GetExp(1.0f);
            player.score += bullet.get()->damage * player.Level;
            continue;
        }
        if (CalcSphere({ bullet.get()->position.x,bullet.get()->position.y,bullet.get()->position.z }, bullet.get()->slerpStopR,
            { boss.position.x, boss.position.y, boss.position.z }, boss.r))
        {
            bullet->SlerpHit();
        }

    }

    //敵とホーミングミサイルの当たり判定
    for (std::unique_ptr<HomingMissile>& missile : player.homingMissile)
    {
        for (std::unique_ptr<Enemy>& enemy : boss.enemys.enemys)
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
                    player.score += missile.get()->damage * player.Level;
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
        for (int i = 0; i < 3; i++)
        {
            if (missile->deleteFlag[i]) continue;

            if (CalcSphere({ missile.get()->homingMissiles[i].position.x,
                missile.get()->homingMissiles[i].position.y,
                missile.get()->homingMissiles[i].position.z }, missile.get()->r,
                { boss.position.x, boss.position.y, boss.position.z }, boss.r) && !boss.deleteFlag)
            {

                missile->BulletHit(i);
                boss.imotalTimer = 0;
                boss.Deth(missile.get()->damage);
                player.GetExp(1.0f);
                player.score += missile.get()->damage * player.Level;
            }

            if (CalcSphere({ missile.get()->homingMissiles[i].position.x,
               missile.get()->homingMissiles[i].position.y,
               missile.get()->homingMissiles[i].position.z }, missile.get()->slerpStopR,
                { boss.position.x, boss.position.y, boss.position.z }, boss.r))
            {
                missile->SetSlerp(i, false);
                missile->SetMaxSpeed(missile->r + boss.r - 5, i);
            }
            else if (CalcSphere({ missile.get()->homingMissiles[i].position.x,
                missile.get()->homingMissiles[i].position.y,
                missile.get()->homingMissiles[i].position.z }, missile.get()->slerpStopR * 10,
                { boss.position.x, boss.position.y, boss.position.z }, boss.r))
            {
                missile->SetSlerp(i, true);
                missile->SetMaxSpeed(missile->maxDefaultSpeed, i);
            }
            else if (CalcSphere({ missile.get()->homingMissiles[i].position.x,
                missile.get()->homingMissiles[i].position.y,
                missile.get()->homingMissiles[i].position.z }, missile.get()->slerpStopR + missile->speed[i],
                { boss.position.x, boss.position.y, boss.position.z }, boss.r))
            {
                missile->SetMaxSpeed(missile->r + boss.r - 5, i);
            }
            else
            {
                missile->SetMaxSpeed(missile->maxDefaultSpeed, i);
            }
        }
    }

    //敵とレーザーの当たり判定
    for (std::unique_ptr<Laser>& laser : player.lasers)
    {
        for (std::unique_ptr<Enemy>& enemy : boss.enemys.enemys)
        {
            if (CalcRaySphere(laser->laser, { enemy->position.x,enemy->position.y,enemy->position.z }, enemy->r))
            {
                enemy->Deth(laser.get()->damage);
                player.score += laser.get()->damage * player.Level;
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
        if (CalcRaySphere(laser->laser, {boss.position.x,boss.position.y,boss.position.z }, boss.r))
        {
            boss.Deth(laser.get()->damage);
            boss.imotalTimer = 10;
            player.GetExp(1.0f);
            player.score += laser.get()->damage * player.Level;
        }

    }


    for (std::unique_ptr<Bomb>& bomb : player.bombs)
    {

        for (std::unique_ptr<Enemy>& enemy : boss.enemys.enemys)
        {
            if (CalcSphere({ bomb.get()->position.x,
                   bomb.get()->position.y,
                    bomb.get()->position.z }, bomb.get()->r,
                { enemy->position.x,enemy->position.y,enemy->position.z }, enemy->r) && !enemy->deleteFlag)
            {
                enemy->Deth(bomb.get()->damage);
                player.score += bomb.get()->damage * player.Level;
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
        if (CalcSphere({ bomb.get()->position.x,
       bomb.get()->position.y,
        bomb.get()->position.z }, bomb.get()->r,
            { boss.position.x,boss.position.y,boss.position.z }, boss.r) && !boss.deleteFlag)
        {
            boss.Deth(bomb.get()->damage);
            boss.imotalTimer = 90;
            player.GetExp(1.0f);
            player.score += bomb.get()->damage * player.Level;
        }
    }
    //敵弾とプレイヤーの当たり判定
    for (std::unique_ptr<Enemy>& enemy : boss.enemys.enemys)
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


    for (std::unique_ptr<Drone>& d : boss.drones)
    {
        if (CalcSphere({ d->position.x,d->position.y,d->position.z }, d->r,
            { player.position.x,player.position.y,player.position.z }, player.r))
        {
            player.EnemyBulletHit(d->damage);
            continue;
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





void MCB::Scene::TutorialCheckAllColision()
{
    //エリア外侵入阻止
    if (!CalcSphere({ player.position.x,player.position.y,player.position.z }, player.r, { 0.f,0.f,0.f }, 5500))
    {
        player.position.x = player.prevPosition.x;
        player.position.y = player.prevPosition.y;
        player.position.z = player.prevPosition.z;
    }

    if (player.position.y < -1500)
    {
        player.position.y = player.prevPosition.y;
    }

    float mintargetDist = player.targetRay.range;
    {
        float nowtargetdist = 0.0f;
        //ロックオン
        for (std::unique_ptr<Enemy>& enemy : enemys.enemys)
        {
            if (CalcRaySphere(player.targetRay, { enemy->position.x,enemy->position.y,
                enemy->position.z }, enemy->r, &nowtargetdist))
            {
                if (nowtargetdist < mintargetDist)
                {
                    player.SetTarget(enemy.get());
                    player.SetHomingTarget(enemy.get());
                    mintargetDist = nowtargetdist;
                }

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
                        if (nowTutorial == Attack)
                        {
                            nowTutorial = ExpTuto;
                        }
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
                    nowSPAttackTuto = LaserTuto;
                    player.homingMissileCount = 0;
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
                    missile->SetSlerp(i, false);
                    missile->SetMaxSpeed(missile->r + enemy.get()->r - 5, i);
                }
                else if (CalcSphere({ missile.get()->homingMissiles[i].position.x,
                    missile.get()->homingMissiles[i].position.y,
                    missile.get()->homingMissiles[i].position.z }, missile.get()->slerpStopR * 10,
                    { enemy->position.x,enemy->position.y,enemy->position.z }, enemy->r))
                {
                    missile->SetSlerp(i, true);
                    missile->SetMaxSpeed(missile->maxDefaultSpeed, i);
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
                nowSPAttackTuto = BombTuto;
                player.laserCount = 0;
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
                break;
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
                SPAttackTutoEnd = true;

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

    if (nowTutorial >= ExpTuto)
    {
        //経験値とプレイヤーの当たり判定
        for (std::unique_ptr<Exp>& exp : exps)
        {
            if (CalcSphere({ exp->position.x,exp->position.y,exp->position.z }, exp->rudius,
                { player.position.x,player.position.y,player.position.z }, player.r))
            {
                exp->GetExp();
                player.GetExp(exp->expPoint);
                if (nowTutorial == ExpTuto)
                {
                    nowTutorial = SPAttack;
                    nowSPAttackTuto = HomingMisilleTuto;
                }
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
    if(nowScene  == Game || nowScene == Title) matView.FollowingFor3DObject(player.position,player.nowFrontVec,{100,50,100},player.UpVec);
    matView.UpDateMatrixView();

    switch (nowScene)
    {
    case Title:

        Skydorm.MatrixUpdata(matView, matProjection);
        ground.MatrixUpdata(matView, matProjection);
        enemys.UpdateMatrix(matView, matProjection);
        player.AllMatrixUpdate(matView, matProjection);
        for (std::unique_ptr<Exp>& exp : exps)
        {
            exp->MatrixUpdata(matView, matProjection);
        }
        if (player.GetTarget())rockOnleticle.MatrixUpdata(matView, matProjection, true);
        break;
    case Game:
        Skydorm.MatrixUpdata(matView, matProjection);
        ground.MatrixUpdata(matView, matProjection);
        enemys.UpdateMatrix(matView, matProjection);
        player.AllMatrixUpdate(matView, matProjection);
        for (std::unique_ptr<Exp>& exp : exps)
        {
            exp->MatrixUpdata(matView, matProjection);
        }
        for (std::unique_ptr<Drone>& d : testD)
        {
            d->MatrixUpdata(matView, matProjection);
        }

        boss.AllMatrixUpdate(matView, matProjection);
        if (player.GetTarget())rockOnleticle.MatrixUpdata(matView, matProjection,true);

        break;
    case Clear:



        break;
    case GameOver:


        break;
    default:
        nowScene = Title;
        nextScene = nowScene;

        break;
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