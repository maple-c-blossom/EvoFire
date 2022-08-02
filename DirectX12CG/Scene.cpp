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
    matProjection.CreateMatrixProjection(XMConvertToRadians(45.0f), (float)dxWindow->window_width / dxWindow->window_height, 0.1f, 4000.0f);
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
    Skydorm.scale = { 100,100,100 };

    player.Init();
    player.model = testBoxModel.get();
    player.scale = { 4,4,4 };
}

#pragma endregion �ʏ�ϐ��̏�����

#pragma region �e�탊�\�[�X�̓ǂݍ��݂Ə�����
void MCB::Scene::LoadModel()
{

	skydomeModel = make_shared<Model>("skydome");
    testBoxModel = make_shared<Model>("Box");
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
#pragma endregion �e�탊�\�[�X�̓ǂݍ��݂Ə�����

void MCB::Scene::Update()
{

    player.Update();


    light->Updata();
    //�s��ϊ�
    MatrixUpdate();
}

void MCB::Scene::Draw()
{
    draw.PreDraw(*depth, *obj3dPipelinePtr, clearColor);
    //3D�I�u�W�F�N�g
    Skydorm.Draw();
    player.Draw();
    //�X�v���C�g
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
}

MCB::Scene::Scene(RootParameter* root, Depth* depthptr, PipelineRootSignature* pipeline, PipelineRootSignature* pipeline1)
{
	rootparamsPtr = root;
	obj3dPipelinePtr = pipeline;
	spritePipelinePtr = pipeline1;
    depth = depthptr;
}

