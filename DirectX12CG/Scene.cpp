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
    soundManager.PlaySoundWave(titleBGM, true);

}

void MCB::Scene::Object3DInit()
{
    player.PlayerInit(playerModel.get(), playerBulletModel.get(), playerBulletModel.get(), rayModel.get(), bombModel.get(),rockOnRayModel.get());
    boss.Init(&player, { (player.nowFrontVec * boss.distance).vec },
        bossModel.get(), enemyModel.get(), enemyModel.get(), testSphereModel.get(),
        mapBossTexture.get(), mapEnemyTexture.get(), mapEnemyTexture.get(), mapEnemyBTexture.get());

    exps.clear();
}

#pragma endregion 通常変数の初期化

#pragma region 各種リソースの読み込みと初期化
void MCB::Scene::LoadModel()
{

	skydomeModel = make_shared<Model>("skydome");
    testBoxModel = make_shared<Model>("Box");
    testSphereModel = make_shared<Model>("sphere",true);
    testSphereModel->material.constMapMaterial->color = { 1.0,0.6,0.6,1 };
    playerBulletModel = make_shared<Model>("sphere",true);
    playerBulletModel->material.constMapMaterial->color = { 0.3,0.8,0.8,1 };
    enemyExpModel = make_shared<Model>("Box");
    enemyExpModel->material.constMapMaterial->color = { 0.3,0.9,0.9,0.6 };
    playerExpModel = make_shared<Model>("Box");
    playerExpModel->material.constMapMaterial->color = { 1.0,0.3,0.3,0.4 };

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
    mapBossTexture = make_shared<Texture>();
    mapBossTexture->CreateTexture(L"Resources\\UI\\bossMap.png");
    titleName = make_shared<Texture>();
    titleName->CreateTexture(L"Resources\\TitleName.png");
    moveText = make_shared<Texture>();
    moveText->CreateTexture(L"Resources\\Tutorial\\MoveText.png");
    moveTuto = make_shared<Texture>();
    moveTuto->CreateTexture(L"Resources\\Tutorial\\MoveTuto.png");
    RockOnText = make_shared<Texture>();
    RockOnText->CreateTexture(L"Resources\\Tutorial\\RockOntuto.png");
    RotaTuto = make_shared<Texture>();
    RotaTuto->CreateTexture(L"Resources\\Tutorial\\RotaTuto.png");
    attackText = make_shared<Texture>();
    attackText->CreateTexture(L"Resources\\Tutorial\\AttackText.png");
    attackTuto = make_shared<Texture>();
    attackTuto->CreateTexture(L"Resources\\Tutorial\\AttackTuto.png");
    expTuto = make_shared<Texture>();
    expTuto->CreateTexture(L"Resources\\Tutorial\\ExpText.png");
    expText = make_shared<Texture>();
    expText->CreateTexture(L"Resources\\UI\\Exp.png");
    GazeSpace = make_shared<Texture>();
    GazeSpace->CreateTexture(L"Resources\\UI\\gazeSpace.png");
    Gaze = make_shared<Texture>();
    Gaze->CreateTexture(L"Resources\\UI\\Gaze.png");
    level = make_shared<Texture>();
    level->CreateTexture(L"Resources\\UI\\Level.png");
    homingMissileTex = make_shared<Texture>();
    homingMissileTex->CreateTexture(L"Resources\\UI\\HomingMissileTex.png");
    laserTex = make_shared<Texture>();
    laserTex->CreateTexture(L"Resources\\UI\\LaserTex.png");
    bombTex = make_shared<Texture>();
    bombTex->CreateTexture(L"Resources\\UI\\bombTex.png");

    homingMissileTuto = make_shared<Texture>();
    homingMissileTuto->CreateTexture(L"Resources\\Tutorial\\HomingMissileTuto.png");
    laserTuto = make_shared<Texture>();
    laserTuto->CreateTexture(L"Resources\\Tutorial\\LaserTuto.png");
    bombTuto = make_shared<Texture>();
    bombTuto->CreateTexture(L"Resources\\Tutorial\\BombTuto.png");

    homingMissileTutoC = make_shared<Texture>();
    homingMissileTutoC->CreateTexture(L"Resources\\Tutorial\\HomingMissileTutoC.png");
    laserTutoC = make_shared<Texture>();
    laserTutoC->CreateTexture(L"Resources\\Tutorial\\LaserTutoC.png");
    bombTutoC = make_shared<Texture>();
    bombTutoC->CreateTexture(L"Resources\\Tutorial\\BombTutoC.png");

    hpGazeSpace = make_shared<Texture>();
    hpGazeSpace->CreateTexture(L"Resources\\UI\\gazeSpace.png");
    hpGaze = make_shared<Texture>();
    hpGaze->CreateTexture(L"Resources\\UI\\Gaze.png");

    BossGazeSpace = make_shared<Texture>();
    BossGazeSpace->CreateTexture(L"Resources\\UI\\bossHpGazeSpace.png");
    BossGaze = make_shared<Texture>();
    BossGaze->CreateTexture(L"Resources\\UI\\Gaze.png");

    hpTex = make_shared<Texture>();
    hpTex->CreateTexture(L"Resources\\UI\\HpText.png");

    pHpTex = make_shared<Texture>();
    pHpTex->CreateTexture(L"Resources\\UI\\HpText.png");

    ScoreTex = make_shared<Texture>();
    ScoreTex->CreateTexture(L"Resources\\UI\\ScoreText.png");

    HiScoreTex = make_shared<Texture>();
    HiScoreTex->CreateTexture(L"Resources\\UI\\HiScoreText.png");

    bombText = make_shared<Texture>();
    bombText->CreateTexture(L"Resources\\Tutorial\\BombText.png");

    homingMissileText = make_shared<Texture>();
    homingMissileText->CreateTexture(L"Resources\\Tutorial\\HomingMisileText.png");

    laserText = make_shared<Texture>();
    laserText->CreateTexture(L"Resources\\Tutorial\\LaserText.png");

    endText = make_shared<Texture>();
    endText->CreateTexture(L"Resources\\Tutorial\\EndText.png");

    StartTexture = make_shared<Texture>();
    StartTexture->CreateTexture(L"Resources\\Tutorial\\Start.png");
    

    attensyonTexture = make_shared<Texture>();
    attensyonTexture->CreateTexture(L"Resources\\Tutorial\\Atte.png");

    noTexture = make_shared<Texture>();
    noTexture->CreateNoTextureFileIsTexture();

    GameClearTexture = make_shared<Texture>();
    GameClearTexture->CreateTexture(L"Resources\\UI\\GameClearText.png");

    GameOverTexture = make_shared<Texture>();
    GameOverTexture->CreateTexture(L"Resources\\UI\\GameOverText.png");

    GameSelectTexture = make_shared<Texture>();
    GameSelectTexture->CreateTexture(L"Resources\\UI\\GameSelect.png");

    TitleSelectTexture = make_shared<Texture>();
    TitleSelectTexture->CreateTexture(L"Resources\\UI\\TitleSelect.png");

    tutorialSelectTexture = make_shared<Texture>();
    tutorialSelectTexture->CreateTexture(L"Resources\\UI\\tutorialSelect.png");

    tutorialSkipTexture = make_shared<Texture>();
    tutorialSkipTexture->CreateTexture(L"Resources\\UI\\tutorialSkip.png");

    SelectTutoTexture = make_shared<Texture>();
    SelectTutoTexture->CreateTexture(L"Resources\\Tutorial\\SelectTuto.png");
}

void MCB::Scene::LoadSound()
{
    selectSound = soundManager.LoadWaveSound("Resources\\sound\\select01.wav");
    titleBGM = soundManager.LoadWaveSound("Resources\\sound\\titleBGM.wav");
    gameBGM = soundManager.LoadWaveSound("Resources\\sound\\GameBGM.wav");
    enterSound = soundManager.LoadWaveSound("Resources\\sound\\Enter.wav");
    soundManager.SetVolume(5, titleBGM);
    soundManager.SetVolume(5, gameBGM);
    soundManager.SetVolume(5, selectSound);
    soundManager.SetVolume(20, enterSound);
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
    moveTextSprite = moveTextSprite.CreateSprite();
    moveTutoSprite = moveTutoSprite.CreateSprite();
    RockOnTextSprite = RockOnTextSprite.CreateSprite();
    RotaTutoSprite = RotaTutoSprite.CreateSprite();
    attackTextSprite = attackTextSprite.CreateSprite();
    attackTutoSprite = attackTutoSprite.CreateSprite();
    expTutoSprite = expTutoSprite.CreateSprite();
    expTextSprite = expTextSprite.CreateSprite();
    GazeSprite = GazeSprite.CreateSprite();
    GazeSprite.anchorPoint = { 0,0 };
    GazeSpaceSprite = GazeSpaceSprite.CreateSprite();
    GazeSpaceSprite.anchorPoint = { 0,0 };
    levelSprite = levelSprite.CreateSprite();
    hommingMissileSprite = hommingMissileSprite.CreateSprite();
    laserSprite = laserSprite.CreateSprite();
    bombSprite = bombSprite.CreateSprite();
    homingMissileTutoSprite = homingMissileTutoSprite.CreateSprite();
    laserTutoSprite = laserTutoSprite.CreateSprite();
    bombTutoSprite = bombTutoSprite.CreateSprite();
    homingMissileTutoCSprite = homingMissileTutoCSprite.CreateSprite();
    laserTutoCSprite = laserTutoCSprite.CreateSprite();
    bombTutoCSprite = bombTutoCSprite.CreateSprite();
    BossMarkSprite = BossMarkSprite.CreateSprite();

    bosshpGazeSprite = bosshpGazeSprite.CreateSprite();
    bosshpGazeSprite.anchorPoint = { 0,0 };
    bosshpGazeSprite.color = { 1,0,0,0.3f };

    bosshpGazeSpaceSprite = bosshpGazeSpaceSprite.CreateSprite();
    bosshpGazeSpaceSprite.anchorPoint = { 0,0 };
    bosshpGazeSpaceSprite.color = { 1,1,1,1 };

    hpGazeSprite = hpGazeSprite.CreateSprite();
    hpGazeSprite.anchorPoint = { 0,0 };
    hpGazeSprite.color = { 0.5,0.8,0.5,0.4f };
    hpGazeSpaceSprite = hpGazeSpaceSprite.CreateSprite();
    hpGazeSpaceSprite.anchorPoint = { 0,0 };

    hpTextSprite = hpTextSprite.CreateSprite();
    pHpTextSprite = pHpTextSprite.CreateSprite();

    ScoreSprite = ScoreSprite.CreateSprite();
    ScoreSprite.anchorPoint = { 0,0 };
    HiScoreSprite = HiScoreSprite.CreateSprite();
    HiScoreSprite.anchorPoint = { 0,0 };

    bombTextSprite  = bombTextSprite.CreateSprite();
    homingMissileTextSprite = homingMissileTextSprite.CreateSprite();
    laserTextSprite = laserTextSprite.CreateSprite();
    endTextSprite = endTextSprite.CreateSprite();
    StartSprite = StartSprite.CreateSprite();
    Attensyon = Attensyon.CreateSprite();

    GameOverSprite = GameOverSprite.CreateSprite();
    GameClearSprite = GameClearSprite.CreateSprite();

    SceneChengeSprite = SceneChengeSprite.CreateSprite();
    SceneChengeSprite.color = { 0,0,0,0 };
    SceneChengeSprite.anchorPoint = { 0,0 };

    GameSelectSprite = GameSelectSprite.CreateSprite();
    TitleSelectSprite = TitleSelectSprite.CreateSprite();
    tutorialSelectSprite = tutorialSelectSprite.CreateSprite();
    tutorialSkipSprite = tutorialSkipSprite.CreateSprite();
    SelectTutoSprite = SelectTutoSprite.CreateSprite();
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
    GameTimer = 0;
    GameMaxTime = 120;
    StartPos = -3000;
    boss.position.y = StartPos;
    GameStartFlag = false;
    moveTutoSize.x = InOutQuad(288 * 2, 288 * 1, 1, 1);
    moveTutoSize.y = InOutQuad(64 * 2, 64 * 1, 1, 1);
    moveTutoPos.x = InOutQuad((float)dxWindow->window_width / 2, (float)dxWindow->window_width - 288 / 2 - 12, 1, 1);
    moveTutoPos.y = InOutQuad(32 * 3 + 70, 64 / 2 + 64, 1, 1);
    rotaTutoSize.x = InOutQuad(112 * 3, 112 * 1, 1, 1);
    rotaTutoSize.y = InOutQuad(64 * 2, 64 * 1, 1, 1);
    rotaTutoPos.x = InOutQuad((float)dxWindow->window_width / 2, (float)dxWindow->window_width - 112 / 2 - 12, 1, 1);
    rotaTutoPos.y = InOutQuad(32 * 3 + 70, 64 / 2 + 64 * 2 + 10, 1, 1);
    attackTutoSize.x = InOutQuad(176 * 1.25, 176 * 1, 1, 1);
    attackTutoSize.y = InOutQuad(64 * 3, 64 * 1, 1, 1);
    attackTutoPos.x = InOutQuad((float)dxWindow->window_width / 2, (float)dxWindow->window_width - 176 / 2 + 32, 1, 1);
    attackTutoPos.y = InOutQuad(32 * 3 + 90, 64 / 2 + 64 * 3 + 10 * 2, 1, 1);


}
void MCB::Scene::TitleSceneInit()
{
    player.PlayerInit(playerModel.get(), playerBulletModel.get(), playerBulletModel.get(), rayModel.get(), bombModel.get(), rockOnRayModel.get());
    titlePos = { (float)dxWindow->window_width / 2, (float)dxWindow->window_height / 2-100 };
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
    moveTutoPos = { (float)dxWindow->window_width / 2, 32 * 3 + 70 };
    moveTutoSize = { 288 * 2, 64 * 2 };
    moveTutoTimer = 0;
    moveTutoMaxTime = 60;
    moveTutoFlag = false;

    rotaTutoPos = { (float)dxWindow->window_width / 2, 32 * 3 + 70 };
    rotaTutoSize = { 112 * 3, 64 * 2 };
    rotaTutoTimer = 0;
    rotaTutoMaxTime = 60;
    rotaTutoFlag = false;

    attackTutoPos = { (float)dxWindow->window_width / 2, 32 * 3 + 120 };
    attackTutoSize = { 112 * 3, 64 * 2 };
    attackTutoTimer = 0;
    attackTutoMaxTime = 60;
    attackTutoFlag = false;
    sinTimer = -1;
    TutorialSkip = false;
    SelectTimer = 0;
}
void MCB::Scene::ClearSceneInit()
{
    //player.PlayerInit(playerModel.get(), testBoxModel.get(), testBoxModel.get(), rayModel.get(), bombModel.get(), rockOnRayModel.get());
    SelectScene = Title;
    SelectTimer = 0;
}
void MCB::Scene::OverSceneInit()
{
    //player.PlayerInit(playerModel.get(), testBoxModel.get(), testBoxModel.get(), rayModel.get(), bombModel.get(), rockOnRayModel.get());
    SelectScene = Title;
    SelectTimer = 0;
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
                            , 30, 30);
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
    //MiniMapDraw(boss.sprite, { boss.BaseRotationPos.x,boss.BaseRotationPos.z }, { player.position.x, player.position.z }, boss.bossMapTex,20);
    //MiniMapDraw(boss.sprite, { boss.nextRotationPos.x,boss.nextRotationPos.z }, { player.position.x, player.position.z }, boss.bossMapTex,20);
    MiniMapDraw(boss.sprite, { boss.position.x,boss.position.z }, { player.position.x, player.position.z }, boss.bossMapTex,40,true);

#pragma region  UI
    moveTutoSprite.SpriteDraw(moveTutoSprite, *moveTuto, moveTutoPos.x, moveTutoPos.y, moveTutoSize.x, moveTutoSize.y);
    RotaTutoSprite.SpriteDraw(RotaTutoSprite, *RotaTuto, rotaTutoPos.x, rotaTutoPos.y, rotaTutoSize.x, rotaTutoSize.y);
    attackTutoSprite.SpriteDraw(attackTutoSprite, *attackTuto, attackTutoPos.x, attackTutoPos.y, attackTutoSize.x, attackTutoSize.y);

    GazeSpaceSprite.SpriteDraw(GazeSpaceSprite, *GazeSpace, gazePos.x, gazePos.y, gazeSize.x * 20, gazeSize.y);
    GazeSprite.SpriteDraw(GazeSprite, *Gaze, gazePos.x, gazePos.y, (gazeSize.x * 20) * player.exp / player.nextLevelExp, gazeSize.y);
    expTextSprite.SpriteDraw(expTextSprite, *expText, gazePos.x + gazeSize.x * 2, gazePos.y + gazeSize.y / 2);
    debugText.Print(gazePos.x + gazeSize.x * 4, gazePos.y + gazeSize.y / 2 - 32 / 2, 2, "%d / %d", player.exp, player.nextLevelExp);

    levelSprite.SpriteDraw(levelSprite, *level, gazePos.x + 32 / 2 + 20, gazePos.y - 32, 64, 32);
    debugText.Print(gazePos.x + 32 / 2 + 32 + 30, gazePos.y - 50, 2, "%d", player.Level);

    homingMissileTutoSprite.SpriteDraw(homingMissileTutoSprite, *homingMissileTuto, dxWindow->window_width / 2 - 158, dxWindow->window_height * 5 / 7 + 28);
    homingMissileTutoCSprite.SpriteDraw(homingMissileTutoCSprite, *homingMissileTutoC, dxWindow->window_width / 2 - 158, dxWindow->window_height * 7 / 8 + 64);
    hommingMissileSprite.SpriteDraw(hommingMissileSprite, *homingMissileTex, dxWindow->window_width / 2 - 158, dxWindow->window_height * 5 / 6, 128, 128);
    debugText.Print(dxWindow->window_width / 2 - 202, dxWindow->window_height * 5 / 6 + (128 * 1 / 8 - 16), 2, "%d / %d", player.homingMissileCount, player.maxHomingMissileCount);

    laserTutoSprite.SpriteDraw(laserTutoSprite, *laserTuto, dxWindow->window_width / 2, dxWindow->window_height * 5 / 7 + 28);
    laserTutoCSprite.SpriteDraw(laserTutoCSprite, *laserTutoC, dxWindow->window_width / 2, dxWindow->window_height * 7 / 8 + 64);
    laserSprite.SpriteDraw(laserSprite, *laserTex, dxWindow->window_width / 2, dxWindow->window_height * 5 / 6, 128, 128);
    debugText.Print(dxWindow->window_width / 2 - 44, dxWindow->window_height * 5 / 6 + (128 * 1 / 8 - 16), 2, "%d / %d", player.laserCount, player.maxLaserCount);

    bombTutoSprite.SpriteDraw(bombTutoSprite, *bombTuto, dxWindow->window_width / 2 + 158, dxWindow->window_height * 5 / 7 + 28);
    bombTutoCSprite.SpriteDraw(bombTutoCSprite, *bombTutoC, dxWindow->window_width / 2 + 158, dxWindow->window_height * 7 / 8 + 64);
    bombSprite.SpriteDraw(bombSprite, *bombTex, dxWindow->window_width / 2 + 158, dxWindow->window_height * 5 / 6, 128, 128);
    debugText.Print(dxWindow->window_width / 2 + 106, dxWindow->window_height * 5 / 6 + (128 * 1 / 8 - 16), 2, "%d / %d", player.bombCount, player.maxBombCount);

    bosshpGazeSpaceSprite.SpriteDraw(bosshpGazeSpaceSprite,*BossGazeSpace,dxWindow->window_width * 1/6 + 60,10,gazeSize.x * 60,gazeSize.y * 1.5);
    bosshpGazeSprite.SpriteDraw(bosshpGazeSprite,*BossGaze,dxWindow->window_width * 1/6 + 60,10,gazeSize.x * 60 * boss.hp / boss.maxhp,gazeSize.y * 1.5);
    debugText.Print((dxWindow->window_width * 1 / 6) + gazeSize.x * 20 + 60, 10 + gazeSize.y / 2 - 32 / 2, 2, "%d / %d", boss.hp, boss.maxhp);
    hpTextSprite.SpriteDraw(hpTextSprite,*hpTex, (dxWindow->window_width * 1 / 6) + gazeSize.x * 18, 32 + gazeSize.y / 2 - 32 / 2);
    BossMarkSprite.SpriteDraw(BossMarkSprite, *mapBossTexture, dxWindow->window_width * 1 / 6 + 60 - gazeSize.x / 2, 10 + (gazeSize.y * 1.5) / 2);

    hpGazeSpaceSprite.SpriteDraw(hpGazeSpaceSprite, *hpGazeSpace, mapPosition.x - mapSize / 2, mapPosition.y - mapSize / 2 - 40, gazeSize.x * 20, gazeSize.y);
    hpGazeSprite.SpriteDraw(hpGazeSprite, *hpGaze, mapPosition.x - mapSize / 2, mapPosition.y - mapSize / 2 - 40, (gazeSize.x * 20) * player.GetHp() / player.maxhp, gazeSize.y);
    pHpTextSprite.SpriteDraw(pHpTextSprite, *pHpTex, mapPosition.x - mapSize / 2 + 30, mapPosition.y - mapSize / 2 - 40 + gazeSize.y / 2);
    debugText.Print(mapPosition.x - mapSize / 2 + 60, mapPosition.y - mapSize / 2 - 40, 2, "%d / %d", player.hp, player.maxhp);

    ScoreSprite.SpriteDraw(ScoreSprite,*ScoreTex,20,20);
    debugText.Print(100, 20, 2, "%d", player.score);


    HiScoreSprite.SpriteDraw(HiScoreSprite, *HiScoreTex, 20, 60);
    debugText.Print(120, 60, 2, "%d", player.HiScore);
#pragma endregion UI
    //debugText.Print(20, 200,2, "score:%d BossHp:%d",player.score,boss.hp);
    //debugText.Print(20, 400,2, "bossBaseRotaMoveTimer:%d time:%d hp:%d",boss.rotationPosMoveTimer,boss.maxRotationPosMoveTimer,boss.hp);
    ////if(exps.size() > 0) debugText.Print(20, 40, 2, "positin:%f,%f,%f", 
    ////                    exps.begin()->get()->position.x, exps.begin()->get()->position.y,
    ////                    exps.begin()->get()->position.z);

    //debugText.Print(20, 0, 2, "exp:float->%f int->%d,NextLevelExp:%d,Level:%d,hp:%d",player.exp, (int)player.exp / 1,
    //                player.nextLevelExp,player.Level,player.GetHp());
    //debugText.Print(20, 80, 2, "homingMissileCount:%d laserCount:%d bombCount;%d", player.homingMissileCount,
    //    player.laserCount,player.bombCount);
    debugText.AllDraw();
    SceneChengeSprite.SpriteDraw(SceneChengeSprite, *noTexture, 0, 0, dxWindow->window_width, dxWindow->window_height);
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
    ScoreSprite.SpriteDraw(ScoreSprite, *ScoreTex, 20, 20);
    debugText.Print(100, 20, 2, "%d", player.score);
    HiScoreSprite.SpriteDraw(HiScoreSprite, *HiScoreTex, 20, 60);
    debugText.Print(120, 60, 2, "%d", player.HiScore);



    mapBack.SpriteDraw(mapBack, *mapBackTexture.get(), mapPosition.x, mapPosition.y, mapSize, mapSize);
    mapPlayer.rotation = player.rotasion.y;

    mapPlayer.SpriteDraw(mapPlayer, *mapPlayerTexture.get(), mapPosition.x, mapPosition.y
        , 30, 30);
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

    switch (nowTutorial)
    {
    case None:
        StartSprite.SpriteDraw(StartSprite,*StartTexture,(float)dxWindow->window_width / 2, (float)dxWindow->window_height / 2 + 300, 320 * 1.5, 32 * 1.5);
        Attensyon.SpriteDraw(Attensyon,*attensyonTexture,(float)dxWindow->window_width / 2, (float)dxWindow->window_height / 2 - 400, 1388 * 1.5, 64 * 1.5);
        if (TutorialSkip)
        {
            if (SelectTimer % 50 <= 25)tutorialSkipSprite.SpriteDraw(tutorialSkipSprite, *tutorialSkipTexture, (float)dxWindow->window_width * 2 / 3, (float)dxWindow->window_height / 2 + 400, 128 * 3, 32 * 3);
            
        }
        else
        {
            tutorialSkipSprite.SpriteDraw(tutorialSkipSprite, *tutorialSkipTexture, (float)dxWindow->window_width * 2 / 3, (float)dxWindow->window_height / 2 + 400, 128 * 3, 32 * 3);
        }
        if (!TutorialSkip)
        {
          if(SelectTimer % 50 <= 25)  tutorialSelectSprite.SpriteDraw(tutorialSelectSprite, *tutorialSelectTexture, (float)dxWindow->window_width * 1 / 3, (float)dxWindow->window_height / 2 + 400, 128 * 3, 32 * 3);
        }
        else
        {
            tutorialSelectSprite.SpriteDraw(tutorialSelectSprite, *tutorialSelectTexture, (float)dxWindow->window_width * 1 / 3, (float)dxWindow->window_height / 2 + 400, 128 * 3, 32 * 3);
        }
        SelectTutoSprite.SpriteDraw(SelectTutoSprite, *SelectTutoTexture, (float)dxWindow->window_width / 2, (float)dxWindow->window_height / 2 + 200, 144 * 2, 64 * 2);
        break;
    case Move:
        moveTextSprite.SpriteDraw(moveTextSprite,*moveText,dxWindow->window_width / 2, 64,288 * 2, 32 * 2);
        break;
    case RockOn:
        RockOnTextSprite.SpriteDraw(RockOnTextSprite, *RockOnText, dxWindow->window_width / 2, 64, 288 * 2, 32 * 2);

        break;
    case Attack:
        attackTextSprite.SpriteDraw(attackTextSprite, *attackText, dxWindow->window_width / 2, 64, 288 * 2, 64 * 2);
        break;
    case ExpTuto:
        expTutoSprite.SpriteDraw(expTutoSprite, *expTuto, dxWindow->window_width / 2, 64, 288 * 2, 64 * 2);

        break;
    case SPAttack:
    
        switch (nowSPAttackTuto)
        {
        case HomingMisilleTuto:
            homingMissileTextSprite.SpriteDraw(homingMissileTextSprite, *homingMissileText, dxWindow->window_width / 2, 64, 288 * 4, 64 * 4);
            break;
        case LaserTuto:
            laserTextSprite.SpriteDraw(laserTextSprite, *laserText, dxWindow->window_width / 2, 64, 288 * 4, 64 * 4);
            break;
        case BombTuto:
            bombTextSprite.SpriteDraw(bombTextSprite, *bombText, dxWindow->window_width / 2, 64, 288 * 4, 64 * 4);

            break;
        default:
            break;
        }

        break;
    case End:
       if(!TutorialSkip)  endTextSprite.SpriteDraw(endTextSprite, *endText, dxWindow->window_width / 2, 64, 288 * 2, 64 * 2);
        break;
    default:
        break;
    }


    if(nowTutorial >= Move)moveTutoSprite.SpriteDraw(moveTutoSprite, *moveTuto, moveTutoPos.x, moveTutoPos.y, moveTutoSize.x, moveTutoSize.y);
    if(nowTutorial >= RockOn)RotaTutoSprite.SpriteDraw(RotaTutoSprite, *RotaTuto, rotaTutoPos.x, rotaTutoPos.y, rotaTutoSize.x, rotaTutoSize.y);
    if(nowTutorial >= Attack)attackTutoSprite.SpriteDraw(attackTutoSprite, *attackTuto, attackTutoPos.x, attackTutoPos.y, attackTutoSize.x, attackTutoSize.y);
    if (nowTutorial >= ExpTuto)
    {
        GazeSpaceSprite.SpriteDraw(GazeSpaceSprite, *GazeSpace, gazePos.x, gazePos.y, gazeSize.x * 20, gazeSize.y);
        GazeSprite.SpriteDraw(GazeSprite, *Gaze, gazePos.x, gazePos.y, (gazeSize.x * 20) * player.exp / player.nextLevelExp, gazeSize.y);
        expTextSprite.SpriteDraw(expTextSprite, *expText, gazePos.x + gazeSize.x * 2, gazePos.y + gazeSize.y / 2);
        debugText.Print(gazePos.x + gazeSize.x * 4, gazePos.y + gazeSize.y / 2 - 32 / 2, 2, "%d / %d", player.exp,player.nextLevelExp);
        levelSprite.SpriteDraw(levelSprite, *level, gazePos.x + 32 / 2 + 20, gazePos.y - 32, 64, 32);
        debugText.Print(gazePos.x + 32 / 2 + 32 + 30, gazePos.y - 50, 2, "%d", player.Level);
         
    }
    if ((nowTutorial >= SPAttack && nowSPAttackTuto >= HomingMisilleTuto) || nowTutorial >= End)
    {
        homingMissileTutoSprite.SpriteDraw(homingMissileTutoSprite, *homingMissileTuto, dxWindow->window_width / 2 - 158, dxWindow->window_height * 5 / 7 + 28);
        homingMissileTutoCSprite.SpriteDraw(homingMissileTutoCSprite, *homingMissileTutoC, dxWindow->window_width / 2 - 158, dxWindow->window_height * 7 / 8 + 64);
        hommingMissileSprite.SpriteDraw(hommingMissileSprite, *homingMissileTex, dxWindow->window_width / 2 - 158, dxWindow->window_height * 5 / 6, 128, 128);
        debugText.Print(dxWindow->window_width / 2 - 202, dxWindow->window_height * 5 / 6 + (128 * 1/8 - 16), 2, "%d / %d", player.homingMissileCount, player.maxHomingMissileCount);
    }
    if ((nowTutorial >= SPAttack && nowSPAttackTuto >= LaserTuto) || nowTutorial >= End)
    {
        laserTutoSprite.SpriteDraw(laserTutoSprite, *laserTuto, dxWindow->window_width / 2, dxWindow->window_height * 5 / 7 + 28);
        laserTutoCSprite.SpriteDraw(laserTutoCSprite, *laserTutoC, dxWindow->window_width / 2, dxWindow->window_height * 7 / 8 + 64);
        laserSprite.SpriteDraw(laserSprite, *laserTex, dxWindow->window_width / 2, dxWindow->window_height * 5 / 6, 128, 128);
        debugText.Print(dxWindow->window_width / 2 - 44, dxWindow->window_height * 5 / 6 + (128 * 1 / 8 - 16), 2, "%d / %d", player.laserCount, player.maxLaserCount);
    }
    if ((nowTutorial >= SPAttack && nowSPAttackTuto >= BombTuto) || nowTutorial >= End)
    {
        bombTutoSprite.SpriteDraw(bombTutoSprite, *bombTuto, dxWindow->window_width / 2 + 158, dxWindow->window_height * 5 / 7 + 28);
        bombTutoCSprite.SpriteDraw(bombTutoCSprite, *bombTutoC, dxWindow->window_width / 2 + 158, dxWindow->window_height * 7 / 8 + 64);
        bombSprite.SpriteDraw(bombSprite, *bombTex, dxWindow->window_width / 2 + 158, dxWindow->window_height * 5 / 6, 128, 128);
        debugText.Print(dxWindow->window_width / 2 + 106, dxWindow->window_height * 5 / 6 + (128 * 1 / 8 - 16), 2, "%d / %d", player.bombCount, player.maxBombCount);
    }


    debugText.AllDraw();
    SceneChengeSprite.SpriteDraw(SceneChengeSprite, *noTexture, 0, 0, dxWindow->window_width, dxWindow->window_height);

    //debugText.Print(20, 200, 2, "TitleScene HiScore:%d Score:%d",player.HiScore, player.score);
    //debugText.Print(20, 0, 2, "exp:float->%f int->%d,NextLevelExp:%d,Level:%d,hp:%d", player.exp, (int)player.exp / 1,
    //    player.nextLevelExp, player.Level, player.GetHp());
    //debugText.Print(20, 80, 2, "homingMissileCount:%d laserCount:%d bombCount;%d", player.homingMissileCount,
    //    player.laserCount, player.bombCount);
}

void MCB::Scene::ClearSceneDraw()
{   
    Skydorm.Draw();
    ground.Draw();
    player.AllDraw();

    for (std::unique_ptr<Exp>& exp : exps) { exp->ExpDraw(); }
    //スプライト
    Sprite::SpriteCommonBeginDraw(*spritePipelinePtr);

    mapBack.SpriteDraw(mapBack, *mapBackTexture.get(), mapPosition.x, mapPosition.y, mapSize, mapSize);
    mapPlayer.rotation = player.rotasion.y;

    mapPlayer.SpriteDraw(mapPlayer, *mapPlayerTexture.get(), mapPosition.x, mapPosition.y
        , 30, 30);
    for (std::unique_ptr<PlayerBullet>& bullet : player.bullets)
    {
        MiniMapDraw(bullet->sprite, { bullet->position.x,bullet->position.z }, { player.position.x,player.position.z }, mapPlayerBTexture.get());
    }
    //MiniMapDraw(boss.sprite, { boss.BaseRotationPos.x,boss.BaseRotationPos.z }, { player.position.x, player.position.z }, boss.bossMapTex,20);
    //MiniMapDraw(boss.sprite, { boss.nextRotationPos.x,boss.nextRotationPos.z }, { player.position.x, player.position.z }, boss.bossMapTex,20);
    MiniMapDraw(boss.sprite, { boss.position.x,boss.position.z }, { player.position.x, player.position.z }, boss.bossMapTex, 40, true);
    GameClearSprite.SpriteDraw(GameClearSprite, *GameClearTexture, dxWindow->window_width / 2, dxWindow->window_height / 2 - 200, 128 * 3, 32 * 3);
    SelectTutoSprite.SpriteDraw(SelectTutoSprite, *SelectTutoTexture, (float)dxWindow->window_width / 2, (float)dxWindow->window_height / 2 + 200, 144 * 2, 64 * 2);
    StartSprite.SpriteDraw(StartSprite, *StartTexture, (float)dxWindow->window_width / 2, (float)dxWindow->window_height / 2 + 100, 320 * 1.5, 32 * 1.5);

    ScoreSprite.SpriteDraw(ScoreSprite, *ScoreTex, dxWindow->window_width / 2 - 168, dxWindow->window_height / 2 + 64 - 200, 64 * 3, 32 * 3);
    debugText.Print(dxWindow->window_width / 2 + 80, dxWindow->window_height / 2 + 80 - 200, 4, "%d", player.score);
    HiScoreSprite.SpriteDraw(HiScoreSprite, *HiScoreTex, dxWindow->window_width / 2 - 208, dxWindow->window_height / 2 + 164 - 200, 96 * 3, 32 * 3);
    debugText.Print(dxWindow->window_width / 2 + 80, dxWindow->window_height / 2 + 64 * 2 + 60 - 200, 4, "%d", player.HiScore);

    if (SelectScene == Title)
    {
        if (SelectTimer % 50 <= 25)TitleSelectSprite.SpriteDraw(TitleSelectSprite, *TitleSelectTexture, (float)dxWindow->window_width * 1 / 3, (float)dxWindow->window_height / 2 + 400, 128 * 3, 32 * 3);

    }
    else
    {
        TitleSelectSprite.SpriteDraw(TitleSelectSprite, *TitleSelectTexture, (float)dxWindow->window_width * 1 / 3, (float)dxWindow->window_height / 2 + 400, 128 * 3, 32 * 3);
    }
    if (SelectScene == Game)
    {
        if (SelectTimer % 50 <= 25)  GameSelectSprite.SpriteDraw(GameSelectSprite, *GameSelectTexture, (float)dxWindow->window_width * 2 / 3, (float)dxWindow->window_height / 2 + 400, 128 * 3, 32 * 3);
    }
    else
    {
        GameSelectSprite.SpriteDraw(GameSelectSprite, *GameSelectTexture, (float)dxWindow->window_width * 2 / 3, (float)dxWindow->window_height / 2 + 400, 128 * 3, 32 * 3);
    }

    debugText.AllDraw();
    SceneChengeSprite.SpriteDraw(SceneChengeSprite, *noTexture, 0, 0, dxWindow->window_width, dxWindow->window_height);
}

void MCB::Scene::OverSceneDraw()
{   
    Skydorm.Draw();
    ground.Draw();
    boss.AllDraw();
    enemys.Draw();

    for (std::unique_ptr<Exp>& exp : exps) { exp->ExpDraw(); }
    //スプライト
    Sprite::SpriteCommonBeginDraw(*spritePipelinePtr);

    mapBack.SpriteDraw(mapBack, *mapBackTexture.get(), mapPosition.x, mapPosition.y, mapSize, mapSize);
    mapPlayer.rotation = player.rotasion.y;

    mapPlayer.SpriteDraw(mapPlayer, *mapPlayerTexture.get(), mapPosition.x, mapPosition.y
        , 30, 30);
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
    //MiniMapDraw(boss.sprite, { boss.BaseRotationPos.x,boss.BaseRotationPos.z }, { player.position.x, player.position.z }, boss.bossMapTex,20);
    //MiniMapDraw(boss.sprite, { boss.nextRotationPos.x,boss.nextRotationPos.z }, { player.position.x, player.position.z }, boss.bossMapTex,20);
    MiniMapDraw(boss.sprite, { boss.position.x,boss.position.z }, { player.position.x, player.position.z }, boss.bossMapTex, 40, true);
    //スプライト
    GameOverSprite.SpriteDraw(GameOverSprite, *GameOverTexture, dxWindow->window_width / 2, dxWindow->window_height / 2 - 200, 128 * 3, 32 * 3);
    SelectTutoSprite.SpriteDraw(SelectTutoSprite, *SelectTutoTexture, (float)dxWindow->window_width / 2, (float)dxWindow->window_height / 2 + 200, 144 * 2, 64 * 2);
    StartSprite.SpriteDraw(StartSprite, *StartTexture, (float)dxWindow->window_width / 2, (float)dxWindow->window_height / 2 + 100, 320 * 1.5, 32 * 1.5);

    ScoreSprite.SpriteDraw(ScoreSprite, *ScoreTex, dxWindow->window_width / 2 - 168, dxWindow->window_height / 2 + 64-200, 64 * 3, 32 * 3);
    debugText.Print(dxWindow->window_width / 2 + 80, dxWindow->window_height / 2 + 80-200, 4, "%d", player.score);
    HiScoreSprite.SpriteDraw(HiScoreSprite, *HiScoreTex, dxWindow->window_width / 2 - 208, dxWindow->window_height / 2 + 164-200, 96 * 3, 32 * 3);
    debugText.Print(dxWindow->window_width / 2 + 80, dxWindow->window_height / 2 + 64 * 2 + 60-200, 4, "%d", player.HiScore);

    if (SelectScene == Title)
    {
        if (SelectTimer % 50 <= 25)TitleSelectSprite.SpriteDraw(TitleSelectSprite, *TitleSelectTexture, (float)dxWindow->window_width * 1 / 3, (float)dxWindow->window_height / 2 + 400, 128 * 3, 32 * 3);

    }
    else
    {
        TitleSelectSprite.SpriteDraw(TitleSelectSprite, *TitleSelectTexture, (float)dxWindow->window_width * 1 / 3 , (float)dxWindow->window_height / 2 + 400, 128 * 3, 32 * 3);
    }
    if (SelectScene == Game)
    {
        if (SelectTimer % 50 <= 25)  GameSelectSprite.SpriteDraw(GameSelectSprite, *GameSelectTexture, (float)dxWindow->window_width * 2 / 3, (float)dxWindow->window_height / 2 + 400, 128 * 3, 32 * 3);
    }
    else
    {
        GameSelectSprite.SpriteDraw(GameSelectSprite, *GameSelectTexture, (float)dxWindow->window_width * 2 / 3, (float)dxWindow->window_height / 2 + 400,128*3,32*3);
    }

    debugText.AllDraw();
    SceneChengeSprite.SpriteDraw(SceneChengeSprite, *noTexture, 0, 0, dxWindow->window_width, dxWindow->window_height);


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

    if (!isChengeScene)
    {


        if (GameStartFlag)
        {


            boss.Update();
            player.Update();
            enemys.Update();

            for (std::unique_ptr<Exp>& exp : exps)
            {
                exp->Update();
            }
            exps.remove_if([](std::unique_ptr<Exp>& exp) {return exp->deleteFlag; });

            if (boss.dethFlag)
            {
                isChengeScene = true;
                isChengeSceneTimer = 0;
                soundManager.StopSoundWave(gameBGM);

            }
            else if (player.dethFlag)
            {
                isChengeScene = true;
                isChengeSceneTimer = 0;
                soundManager.StopSoundWave(gameBGM);
            }
            if (!player.dethFlag || !boss.dethFlag)
            {
                CheckAllColision();
            }
            if (player.GetTarget())rockOnleticle.position = player.GetTarget()->position;
        }

        if (!GameStartFlag)
        {
            if (GameTimer < GameMaxTime)
            {
                GameTimer++;
                boss.position.y = Lerp(StartPos, player.nowFrontVec.vec.y * boss.distance, GameMaxTime, GameTimer);

            }
            else if (GameTimer < GameMaxTime + 10)
            {
                GameTimer++;
            }
            else
            {
                GameStartFlag = true;
                Object3DInit();
                exps.clear();
            }
        }
    }

    if (isChengeScene)
    {
        if (isChengeSceneTimer < isChengeSceneTime)
        {
            isChengeSceneTimer++;
        }
        if (prevScene != Game)
        {
            SceneChengeSprite.color.w = Lerp(1, 0, isChengeSceneTime, isChengeSceneTimer);
            if (isChengeSceneTimer >= isChengeSceneTime)
            {
                prevScene = nowScene;
                enemys.enemys.clear();;
                exps.clear();
                GameSceneInit();
                isChengeSceneTimer = 0;
                isChengeScene = false;
                SceneChengeSprite.color.w = 0;
                soundManager.PlaySoundWave(gameBGM, true);
            }
        }
        else
        {
            SceneChengeSprite.color.w = Lerp(0, 1, isChengeSceneTime, isChengeSceneTimer);
            if (isChengeSceneTimer >= isChengeSceneTime)
            {
                if (boss.dethFlag)
                {
                    nextScene = Clear;
                    ClearSceneInit();
                    isChengeSceneTimer = 0;
                    for (int i = 0; i < 60; i++)
                    {
                        Sporn({ boss.position.x,boss.position.y,boss.position.z }, 0);
                        DeleteExp();
                    }
                }
                else if (player.dethFlag)
                {
                    nextScene = GameOver;
                    OverSceneInit();
                    isChengeSceneTimer = 0;
                    for (int i = 0; i < 40; i++)
                    {
                        Sporn({ player.position.x,player.position.y,player.position.z }, 0,true);
                        DeleteExp();
                    }
                }
                prevScene = nowScene;
                enemys.enemys.clear();
                isChengeSceneTimer = 0;
            }
        }
    }

}



void MCB::Scene::TitleSceneUpdate()
{

    switch (nowTutorial)
    {
    case None:
        if (isChengeScene)
        {
            if (isChengeSceneTimer < isChengeSceneTime)
            {
                isChengeSceneTimer++;
            }
            if (prevScene != Title)
            {
                SceneChengeSprite.color.w = Lerp(1, 0, isChengeSceneTime, isChengeSceneTimer);
                if (isChengeSceneTimer >= isChengeSceneTime)
                {
                    prevScene = nowScene;
                    enemys.enemys.clear();;
                    exps.clear();
                    GameSceneInit();
                    isChengeSceneTimer = 0;
                    isChengeScene = false;
                    SceneChengeSprite.color.w = 0;
                    soundManager.PlaySoundWave(titleBGM, true);
                }
            }
        }
        else
        {
            if (input->IsKeyTrigger(DIK_A) || input->gamePad->IsButtonTrigger(GAMEPAD_LEFT))
            {
                TutorialSkip = false;
                soundManager.StopSoundWave(selectSound);
                soundManager.PlaySoundWave(selectSound);
            }
            else if(input->IsKeyTrigger(DIK_D) || input->gamePad->IsButtonTrigger(GAMEPAD_RIGHT))
            {
                TutorialSkip = true;
                soundManager.StopSoundWave(selectSound);
                soundManager.PlaySoundWave(selectSound);

            }

            if (SelectTimer > 256)
            {
                SelectTimer = 0;
            }

            SelectTimer++;

            if (sinTimer > 360) sinTimer = 0;
            sinTimer++;
            player.position.y = sinf(ConvertRadius(sinTimer)) * 20;
            player.rockOnlaser.position = { player.position.x, player.position.y,player.position.z };
            if (input->IsKeyTrigger(DIK_SPACE) || input->gamePad->IsButtonTrigger(GAMEPAD_X))
            {
                if (!TutorialSkip)
                {
                    titleMove = true;
                    moveTutoPos = { (float)dxWindow->window_width / 2, 32 * 3 + 70 };
                    moveTutoSize = { 288 * 2, 64 * 2 };
                    rotaTutoPos = { (float)dxWindow->window_width / 2, 32 * 3 + 70 };
                    rotaTutoSize = { 112 * 3, 64 * 2 };
                    attackTutoPos = { (float)dxWindow->window_width / 2, 32 * 3 + 120 };
                    attackTutoSize = { 112 * 3, 64 * 2 };


                }
                else
                {
                    nowTutorial = End;
                    isChengeScene = true;
                }
                soundManager.StopSoundWave(titleBGM); 
                soundManager.PlaySoundWave(enterSound);

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
                player.position.y = 0;
            }
        }
        break;
    case Move:
        player.Update();
        if (player.prevPosition.x != player.position.x || player.prevPosition.y != player.position.y || player.prevPosition.z != player.position.z)
        {
            moveTutoFlag = true;
        }
        if (moveTutoFlag)
        {
            moveTutoTimer++;
            if (moveTutoTimer >= 0)
            {
                moveTutoSize.x = InOutQuad(288 * 2, 288 * 1, moveTutoMaxTime, moveTutoTimer);
                moveTutoSize.y = InOutQuad(64 * 2, 64 * 1, moveTutoMaxTime, moveTutoTimer);
                moveTutoPos.x = InOutQuad((float)dxWindow->window_width / 2,   (float)dxWindow->window_width - 288 / 2 - 12, moveTutoMaxTime, moveTutoTimer);
                moveTutoPos.y = InOutQuad(32 * 3 + 70, 64 / 2 + 64, moveTutoMaxTime, moveTutoTimer);
            }
        }
        if (moveTutoTimer >= moveTutoMaxTime)
        {
            moveTutoFlag == false;
            nowTutorial = RockOn;
            player.Level = 0;
        }
        
        break;
    case RockOn:
        player.Update();
        if (player.bullets.size() > 0) player.bullets.clear();
        if(enemys.enemys.size() < 1)enemys.enemyPop(&player, { -player.nowFrontVec.vec.x + 100 + player.position.x ,0 , player.nowFrontVec.vec.z * -100 + player.position.z }, enemyModel.get(), testSphereModel.get(), mapEnemyTexture.get(), mapEnemyBTexture.get(), EnemyManager::Turret, Enemy::NoHoming);
       if (player.GetTarget() != nullptr)
        {
            rotaTutoFlag = true;
        }
        if (rotaTutoFlag)
        {
            rotaTutoTimer++;
            if (rotaTutoTimer >= 0)
            {
                rotaTutoSize.x = InOutQuad(112 * 3, 112 * 1, rotaTutoMaxTime, rotaTutoTimer);
                rotaTutoSize.y = InOutQuad(64 * 2, 64 * 1, rotaTutoMaxTime, rotaTutoTimer);
                rotaTutoPos.x = InOutQuad((float)dxWindow->window_width / 2, (float)dxWindow->window_width - 112 / 2 - 12, rotaTutoMaxTime, rotaTutoTimer);
                rotaTutoPos.y = InOutQuad(32 * 3 + 70, 64 / 2 + 64 * 2 + 10, rotaTutoMaxTime, rotaTutoTimer);
            }
        }
        if (rotaTutoTimer >= rotaTutoMaxTime)
        {
            rotaTutoFlag == false;
            nowTutorial = Attack;
            player.Level = 0;
        }
        break;
    case Attack:
        if (enemys.enemys.size() < 1)enemys.enemyPop(&player, { (float)GetRand(-100,100),0,(float)GetRand(30,100) }, enemyModel.get(), testSphereModel.get(), mapEnemyTexture.get(), mapEnemyBTexture.get(), EnemyManager::Turret, Enemy::NoHoming);
        player.Update();

        if (attackTutoFlag)
        {
            attackTutoTimer++;
            if (attackTutoTimer >= 0)
            {
                attackTutoSize.x = InOutQuad(176 * 1.25, 176 * 1, attackTutoMaxTime, attackTutoTimer);
                attackTutoSize.y = InOutQuad(64 * 3, 64 * 1, attackTutoMaxTime, attackTutoTimer);
                attackTutoPos.x = InOutQuad((float)dxWindow->window_width / 2, (float)dxWindow->window_width - 176 / 2 + 32, attackTutoMaxTime, attackTutoTimer);
                attackTutoPos.y = InOutQuad(32 * 3 + 90, 64 / 2 + 64 * 3 + 10 * 2, attackTutoMaxTime, attackTutoTimer);
            }
        }
        if (attackTutoTimer >= attackTutoMaxTime)
        {
            attackTutoFlag == false;
            nowTutorial = ExpTuto;
            player.Level = 0;
        }

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
            
            break;
        default:
            break;
        }
        player.exp = 0;
        player.Update();
        player.Level = 0;
        break;
    case End:
        if (!TutorialSkip)
        {
            if (enemys.enemys.size() < 1)enemys.enemyPop(&player, { (float)GetRand(-100,100),0,(float)GetRand(30,100) }, enemyModel.get(), testSphereModel.get(), mapEnemyTexture.get(), mapEnemyBTexture.get(), EnemyManager::Turret, Enemy::NoHoming);
            if (player.homingMissileCount < 3) player.homingMissileCount = 3;
            if (player.laserCount < 2) player.laserCount = 2;
            if (player.bombCount < 1) player.bombCount = 1;
            player.Update();
        }
        for (std::unique_ptr<Exp>& exp : exps)
        {
            exp->Update();
        }
        exps.remove_if([](std::unique_ptr<Exp>& exp) {return exp->deleteFlag; });

        if (player.exp >= 10 * 20)
        {
            player.Level = 1;
            isChengeScene = true;
            player.exp = 0;
        }
        if (isChengeScene)
        {
         
            if (isChengeSceneTimer < isChengeSceneTime)
            {
                isChengeSceneTimer++;
            }
            SceneChengeSprite.color.w = Lerp(0, 1, isChengeSceneTime, isChengeSceneTimer);
            if (isChengeSceneTimer >= isChengeSceneTime)
            {
                prevScene = nowScene;
                nextScene = Game;
                enemys.enemys.clear();;
                exps.clear();
                GameSceneInit();

                isChengeSceneTimer = 0;
                SceneChengeSprite.color.w = 1;
                nowTutorial = None;
            }
        }



        if (nowTutorial < Attack)
        {
            player.bullets.clear();
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
    if (isChengeScene)
    {
        if (isChengeSceneTimer < isChengeSceneTime)
        {
            isChengeSceneTimer++;
        }
        if (prevScene != Clear)
        {
            SceneChengeSprite.color.w = Lerp(1, 0, isChengeSceneTime, isChengeSceneTimer);
            if (isChengeSceneTimer >= isChengeSceneTime)
            {
                prevScene = nowScene;
                enemys.enemys.clear();;
                ClearSceneInit();
                isChengeSceneTimer = 0;
                isChengeScene = false;
                SceneChengeSprite.color.w = 0;
            }
        }
        else
        {
            SceneChengeSprite.color.w = Lerp(0, 1, isChengeSceneTime, isChengeSceneTimer);
            if (isChengeSceneTimer >= isChengeSceneTime)
            {
                nextScene = SelectScene;
                isChengeSceneTimer = 0;
                prevScene = nowScene;
                exps.clear();
                GameSceneInit();
                TitleSceneInit();
            }
        }
    }
    else
    {
        if (SelectTimer > 256)
        {
            SelectTimer = 0;
        }

        SelectTimer++;
        if (input->IsKeyTrigger(DIK_A) || input->gamePad->IsButtonTrigger(GAMEPAD_LEFT))
        {
            soundManager.StopSoundWave(selectSound);
            soundManager.PlaySoundWave(selectSound);
            SelectScene = Title;
        }
        else if (input->IsKeyTrigger(DIK_D) || input->gamePad->IsButtonTrigger(GAMEPAD_RIGHT))
        {
            soundManager.StopSoundWave(selectSound);
            soundManager.PlaySoundWave(selectSound);
            SelectScene = Game;
        }
        if (input->IsKeyTrigger(DIK_SPACE) || input->gamePad->IsButtonTrigger(GAMEPAD_X))
        {
            isChengeScene = true;
            isChengeSceneTimer = 0;
            soundManager.PlaySoundWave(enterSound);
        }
    }
        for (std::unique_ptr<Exp>& exp : exps)
        {
            exp->Update();
        }
}
void MCB::Scene::OverSceneUpdate()
{
    if (isChengeScene)
    {
        if (isChengeSceneTimer < isChengeSceneTime)
        {
            isChengeSceneTimer++;
        }
        if (prevScene != GameOver)
        {
            SceneChengeSprite.color.w = Lerp(1, 0, isChengeSceneTime, isChengeSceneTimer);
            if (isChengeSceneTimer >= isChengeSceneTime)
            {
                prevScene = nowScene;
                enemys.enemys.clear();;
                ClearSceneInit();
                isChengeSceneTimer = 0;
                isChengeScene = false;
                SceneChengeSprite.color.w = 0;
            }
        }
        else
        {
            SceneChengeSprite.color.w = Lerp(0, 1, isChengeSceneTime, isChengeSceneTimer);
            if (isChengeSceneTimer >= isChengeSceneTime)
            {
                nextScene = SelectScene;
                isChengeSceneTimer = 0;
                prevScene = nowScene;
                exps.clear();
                GameSceneInit();
                TitleSceneInit();
            }
        }
    }
    else
    {
        if (SelectTimer > 256)
        {
            SelectTimer = 0;
        }

        SelectTimer++;
        if (input->IsKeyTrigger(DIK_A) || input->gamePad->IsButtonTrigger(GAMEPAD_LEFT))
        {
            soundManager.StopSoundWave(selectSound);
            soundManager.PlaySoundWave(selectSound);
            SelectScene = Title;
        }
        else if (input->IsKeyTrigger(DIK_D) || input->gamePad->IsButtonTrigger(GAMEPAD_RIGHT))
        {
            soundManager.StopSoundWave(selectSound);
            soundManager.PlaySoundWave(selectSound);
            SelectScene = Game;
        }
        if (input->IsKeyTrigger(DIK_SPACE) || input->gamePad->IsButtonTrigger(GAMEPAD_X))
        {
            isChengeScene = true;
            isChengeSceneTimer = 0;
            soundManager.PlaySoundWave(enterSound);
        }
    }
        for (std::unique_ptr<Exp>& exp : exps)
        {
            exp->Update();
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

void MCB::Scene::MiniMapDraw(Sprite sprite ,Float2 objectPos, Float2 playerPos,Texture* maptex,float SpriteSize,bool flag)
{
    float spriteSize = SpriteSize;
    Float2 spritePos = { (objectPos.x - playerPos.x) / mapOffSet + mapPosition.x,
                        mapPosition.y - (objectPos.y - playerPos.y) / mapOffSet };
    if (!flag && (spritePos.x - spriteSize < 0 || spritePos.x + spriteSize > mapSize || spritePos.y - spriteSize < mapPosition.y - mapSize / 2 || spritePos.y + spriteSize > mapPosition.y + mapSize / 2))
    {
        return;
    }

    if (spritePos.x - spriteSize < 0 && flag)
    {
        spritePos.x = 0;
    }
    
    if (spritePos.x + spriteSize > mapSize && flag)
    {
        spritePos.x = mapSize;
    }

    if (spritePos.y - spriteSize < mapPosition.y - mapSize / 2)
    {
        spritePos.y = mapPosition.y - mapSize / 2;
    }

    if (spritePos.y + spriteSize > mapPosition.y + mapSize / 2)
    {
        spritePos.y = mapPosition.y + mapSize / 2;
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
            player.GetExp(1.0f);
            player.GetSPAttack();
            player.LevelUp();
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
                boss.Deth(missile.get()->damage);
                player.GetExp(1.0f);
                player.GetSPAttack();
                player.LevelUp();
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
            if (!boss.imotalFlag)
            {
                boss.imotalTimer = 10;
                boss.imotalFlag = true;
            }
            player.GetExp(1.0f);
            player.GetSPAttack();
            player.LevelUp();
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
                if (!boss.imotalFlag)
                {
                    boss.imotalTimer = 60;
                    boss.imotalFlag = true;
                }
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
            player.GetExp(1.0f);
            player.GetSPAttack();
            player.LevelUp();
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

    for (std::unique_ptr<Enemy>& enemy : boss.enemys.enemys)
    {
        if (CalcSphere({ enemy->position.x,enemy->position.y,enemy->position.z }, enemy->r,
            { player.position.x,player.position.y,player.position.z }, player.r))
        {
            player.EnemyBulletHit(1);
            enemy->deleteFlag = true;
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
        else
        {
            exp->sinFlag = true;
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
                            attackTutoFlag = true;
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
                player.exp += exp.get()->expPoint;
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
            else
            {
                exp->sinFlag = true;
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
    if(nowScene  == Game || (nowScene == Title && sinTimer <= 0) || nowTutorial != None) matView.FollowingFor3DObject(player.position,player.nowFrontVec,{100,50,100},player.UpVec);
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
        Skydorm.MatrixUpdata(matView, matProjection);
        ground.MatrixUpdata(matView, matProjection);
        player.AllMatrixUpdate(matView, matProjection);
        for (std::unique_ptr<Exp>& exp : exps)
        {
            exp->MatrixUpdata(matView, matProjection);
        }


        break;
    case GameOver:

        Skydorm.MatrixUpdata(matView, matProjection);
        ground.MatrixUpdata(matView, matProjection);
        for (std::unique_ptr<Exp>& exp : exps)
        {
            exp->MatrixUpdata(matView, matProjection);
        }
        boss.AllMatrixUpdate(matView, matProjection);
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

void MCB::Scene::Sporn(Float3 enemyPos,float expPoint,bool isplayer)
{
    std::unique_ptr<Exp> exp = make_unique<Exp>();
    exp->Init();
    if (isplayer)
    {
        exp->model = playerExpModel.get();
    }
    else
    {
        exp->model = enemyExpModel.get();
    }
    exp->scale = { 2,2,2 };
    exp->ExpInit(GetRand(2, 10),
        { enemyPos.x + GetRand(-10, 10),enemyPos.y + GetRand(-10, 10),enemyPos.z + GetRand(-10, 10) },
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