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
    matProjection.CreateMatrixProjection(XMConvertToRadians(45.0f), (float)dxWindow->window_width / dxWindow->window_height, 0.1f, 10000.0f);
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
    Skydorm.scale = { 30,30,30 };

    player.Init();
    player.model = testBoxModel.get();
    player.bulletModel = testSphereModel.get();
    player.scale = { 4,4,4 };
    player.PlayerInit();

    testEnemy.Init();
    testEnemy.model = testBoxModel.get();
    testEnemy.scale = { 6,6,6 };
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
	debugTextTexture.CreateTexture(L"Resources\\debugfont.png");
}

void MCB::Scene::LoadSound()
{
}

void MCB::Scene::SpriteInit()
{
    debugText.Init(&debugTextTexture);

}
#pragma endregion 各種リソースの読み込みと初期化

void MCB::Scene::Update()
{
    if (input->IsKeyTrigger(DIK_P))
    {
        enemys.enemyPop(&player, { (float)GetRand(-20,20),(float)GetRand(-20,20),(float)GetRand(-20,20) }, testBoxModel.get(), testSphereModel.get());
        player.SetTarget(enemys.enemys.begin()->get());
    }
    player.Update();
    enemys.Update();
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
    player.Draw();
    for (std::unique_ptr<PlayerBullet>& bullet : player.bullets) { bullet->Draw(); }
    enemys.Draw();
    //スプライト
    Sprite::SpriteCommonBeginDraw(*spritePipelinePtr);
    debugText.AllDraw();
    draw.PostDraw();
}

void MCB::Scene::CheckAllColision()
{

    //敵と自弾の当たり判定
    for (std::unique_ptr<PlayerBullet>& bullet : player.bullets)
    {
        for (std::unique_ptr<Enemy>& enemy : enemys.enemys)
        {
            if (CalcSphere({ bullet.get()->position.x,bullet.get()->position.y,bullet.get()->position.z }, bullet.get()->r,
                { enemy->position.x,enemy->position.y,enemy->position.z }, enemy->r))
            {
                bullet->BulletHit();
                enemy->Deth();
                player.SetTarget(nullptr);
                continue;
            }

            if (CalcSphere({ bullet.get()->position.x,bullet.get()->position.y,bullet.get()->position.z }, bullet.get()->slerpStopR,
                { testEnemy.position.x,testEnemy.position.y,testEnemy.position.z }, testEnemyR))
            {
                bullet->SlerpHit();
            }
        }
    }

    for (std::unique_ptr<Enemy>& enemy : enemys.enemys)
    {
        for (std::unique_ptr<EnemyBullet>& bullet : enemy->bullets)
        {
            if (CalcSphere({ bullet->position.x,bullet->position.y,bullet->position.z }, bullet->r,
                { player.position.x,player.position.y,player.position.z }, player.r))
            {
                bullet->BulletHit();
                continue;
            }

            if (CalcSphere({ bullet->position.x,bullet->position.y,bullet->position.z }, bullet->slerpStopR,
                { player.position.x,player.position.y,player.position.z }, player.r))
            {
                bullet->SlerpHit();
            }
        }
    }
}

void MCB::Scene::MatrixUpdate()
{
    matProjection.UpdataMatrixProjection();
    matView.FollowingFor3DObject(player.position,player.nowFrontVec,{100,50,100},player.UpVec);
    matView.UpDateMatrixView();
    Skydorm.MatrixUpdata(matView, matProjection);
    player.MatrixUpdata(matView, matProjection,player.playerQ);
    enemys.UpdateMatrix(matView, matProjection);
    for (std::unique_ptr<PlayerBullet>& bullet : player.bullets)
    {
        bullet->MatrixUpdata(matView, matProjection);
    }
}

MCB::Scene::Scene(RootParameter* root, Depth* depthptr, PipelineRootSignature* pipeline, PipelineRootSignature* pipeline1)
{
	rootparamsPtr = root;
	obj3dPipelinePtr = pipeline;
	spritePipelinePtr = pipeline1;
    depth = depthptr;
}

