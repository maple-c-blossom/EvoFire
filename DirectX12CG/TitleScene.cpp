#include "TitleScene.h"

MCB::TitleScene::TitleScene(RootParameter* root, Depth* depth, PipelineRootSignature* pipeline, PipelineRootSignature* pipeline1)
{
	rootparamsPtr = root;
	obj3dPipelinePtr = pipeline;
	spritePipelinePtr = pipeline1;
	this->depth = depth;
}

void MCB::TitleScene::Initialize()
{
}

void MCB::TitleScene::LoadModel()
{
}

void MCB::TitleScene::LoadTexture()
{
}

void MCB::TitleScene::LoadSound()
{
}

void MCB::TitleScene::Object3DInit()
{
}

void MCB::TitleScene::SpriteInit()
{

}

void MCB::TitleScene::Update()
{

}

void MCB::TitleScene::Draw()
{

}

void MCB::TitleScene::ChengeScene()
{
}
