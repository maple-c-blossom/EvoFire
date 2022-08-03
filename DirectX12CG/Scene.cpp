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

    player.Update();

    if (input->IsMouseDown(input->LEFT))
    {
        player.position = { 0,0,0 };
    }
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
    
    //スプライト
    Sprite::SpriteCommonBeginDraw(*spritePipelinePtr);
    debugText.AllDraw();
    draw.PostDraw();
}

void MCB::Scene::MatrixUpdate()
{
    matProjection.UpdataMatrixProjection();
    matView.FollowingFor3DObject(player.position,player.nowFrontVec,{100,50,100},player.UpVec);
    matView.UpDateMatrixView();
    Skydorm.MatrixUpdata(matView, matProjection);
    player.MatrixUpdata(matView, matProjection,player.playerQ);
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

