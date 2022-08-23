#pragma once
#include "IScene.h"

#pragma region 標準.h include

#include <d3dcompiler.h>
#include <Windows.h>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <vector>
#include <string>
#include <DirectXMath.h>
#include <wrl.h>
#include <cassert>
#include <memory>
#include <DirectXTex.h>
#include <array>

#pragma endregion 標準.h include

#pragma region 自作.h include
#include "Input.h"
#include "DxWindow.h"
#include "Dx12.h"
#include "View.h"
#include "Projection.h"
#include "WorldMatrix.h"
#include "Depth.h"
#include "Object3d.h"
#include "ObjectMaterial.h"
#include "RootParameter.h"
#include "Vertex.h"
#include "MCBMatrix.h"
#include "Util.h"
#include "Particle.h"
#include "Quaternion.h"
#include "Texture.h"
#include "PIpelineRootSignature.h"
#include "Draw.h"
#include "Sprite.h"
#include "DebugText.h"
#include "Sound.h"
#include "Collider.h"
#include "Light.h"
#include "FPS.h"
#pragma endregion 自作.h include

#pragma region pragma comment

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")

#pragma endregion pragma comment

namespace MCB
{
	class TitleScene :public IScene
	{
	private:
#pragma region DirectX基礎機能群
		DxWindow* dxWindow = DxWindow::GetInstance();
		//DirectXクラス生成
		Dx12* dx = Dx12::GetInstance();
		//inputクラス生成
		Input* input = Input::GetInstance();

		ShaderResource* descriptor = ShaderResource::GetInstance();

		Depth* depth;
		//ルートパラメータ
		RootParameter* rootparamsPtr;

		PipelineRootSignature* obj3dPipelinePtr;

		PipelineRootSignature* spritePipelinePtr;

		Light* light = nullptr;

		//描画前処理と描画後処理
		MCB::Draw draw;

		//サウンドマネージャー
		SoundManager soundManager;
		//クリアカラー
		float clearColor[4] = { 0.0f,0.0f, 0.0f,0.0f }; // 青っぽい色
#pragma endregion DirectX基礎機能群
	public:
		TitleScene(RootParameter* root, Depth* depth, PipelineRootSignature* pipeline, PipelineRootSignature* pipeline1);
		void Initialize() override;
		//各初期化系関数群--------------------
		void LoadModel()  override;
		void LoadTexture()  override;
		void LoadSound()  override;
		void Object3DInit()  override;
		void SpriteInit()  override;
		void Update() override;
		void Draw() override;
		void ChengeScene() override;
	};

}

