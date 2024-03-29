#define _CRTDBG_MAP_ALLOC 
#include <crtdbg.h>
#include <cstdlib>
#define _USE_MATH_DEFINES
#include <cmath>
#include "Scene.h"
#include "TitleScene.h"
#include "FPS.h"
using namespace MCB;
int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine,
    _In_ int nCmdShow)
{
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    {
    DxWindow* dxWindow = DxWindow::GetInitInstance();

    //デバック時のみ----------
#pragma region デバック時のみ
#ifdef _DEBUG
//デバックレイヤーをオンに
    Microsoft::WRL::ComPtr<ID3D12Debug1> debugController;
    Microsoft::WRL::ComPtr<ID3D12InfoQueue> infoQueue;
    if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
    {
        debugController->EnableDebugLayer();
        debugController->SetEnableGPUBasedValidation(TRUE);
    }

    //if (SUCCEEDED(Dx12::GetInstance()->device.Get()->QueryInterface(IID_PPV_ARGS(infoQueue.GetAddressOf()))))
    //{
    //    infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, true);
    //    infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, true);
    //}
#endif
#pragma endregion デバック時のみ
    //-------------
    Dx12::GetInitInstance();
    Input* input = Input::GetInitInstance();
    Depth depth;
    ShaderResource::GetInitInstance();
    //ルートパラメータの設定---------------------------
#pragma region ルートパラメータの設定
    RootParameter rootparams;
    rootparams.SetRootParam(D3D12_ROOT_PARAMETER_TYPE_CBV, 0, 0, D3D12_SHADER_VISIBILITY_ALL,  0);
    rootparams.SetRootParam(D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE, 0, 0, D3D12_SHADER_VISIBILITY_ALL, 1);
    rootparams.SetRootParam(D3D12_ROOT_PARAMETER_TYPE_CBV, 1, 0, D3D12_SHADER_VISIBILITY_ALL, 0);
    rootparams.SetRootParam(D3D12_ROOT_PARAMETER_TYPE_CBV, 2, 0, D3D12_SHADER_VISIBILITY_ALL, 0);
#pragma endregion ルートパラメータの設定
    //------------------------
    PipelineRootSignature obj3dPipeline = obj3dPipeline.Create3DObjectPipeline(depth, rootparams);
    PipelineRootSignature spritePipeline = spritePipeline.CreateSpritePipeline(depth, rootparams);
    Scene scene(&rootparams,&depth ,&obj3dPipeline, &spritePipeline);
    scene.Initialize();
    IScene* iscene = &scene;
    FPS fpsControll;
    fpsControll.Initialize(60);
    scene.fps = &fpsControll;

#pragma region ゲームループ
    while (true)
    {
        input->UpDateInit();
        dxWindow->messageUpdate();
        if (input->IsKeyDown(DIK_ESCAPE) || dxWindow->breakFlag) break;
        iscene->Update();
        iscene->Draw();
        iscene->ChengeScene();
        fpsControll.GetFPS();
        fpsControll.FPSFixed();
    }
#pragma endregion ゲームループ
    
    ShaderResource::DeleteInstace();
    DxWindow::DeleteInstance();
    Dx12::DeleteInstace();
    Input::DeleteInstace();
}   
    _CrtDumpMemoryLeaks();
	return 0;
}
