#pragma once
#include "IScene.h"
#define _USE_MATH_DEFINES
#include <cmath>


#pragma region �W��.h include

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

#pragma endregion �W��.h include

#pragma region ����.h include
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
#pragma endregion ����.h include

#pragma region �Q�[���n.h include

#include "RayObject.h"
#include "SphereObj.h"
#include "Player.h"
#include "EnemyManager.h"
#include "exp.h"
#include "Drone.h"
#include "Boss.h"
#pragma endregion �Q�[���n.h include

#pragma region pragma comment

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")

#pragma endregion pragma comment

namespace MCB
{
	class Scene :public IScene
	{
	private:
		enum NowScene
		{
			Title,
			Game,
			Clear,
			GameOver
		};

		enum Tutotial
		{
			None = 0,
			Move = 1,
			RockOn = 2,
			Attack = 3,
			ExpTuto = 4,
			SPAttack = 5,
			End = 6,
		};

		enum SPAttackTutorial
		{
			HomingMisilleTuto = 1,
			LaserTuto = 2,
			BombTuto = 3
		};
		#pragma region DirectX��b�@�\�Q
		DxWindow* dxWindow = DxWindow::GetInstance();
		//DirectX�N���X����
		Dx12* dx = Dx12::GetInstance();
		//input�N���X����
		Input* input = Input::GetInstance();

		ShaderResource* descriptor = ShaderResource::GetInstance();

		Depth* depth;
		//���[�g�p�����[�^
		RootParameter* rootparamsPtr;

		PipelineRootSignature* obj3dPipelinePtr;

		PipelineRootSignature* spritePipelinePtr;

		Light* light = nullptr;

		//�`��O�����ƕ`��㏈��
		MCB::Draw draw;

		//�T�E���h�}�l�[�W���[
		SoundManager soundManager;
		//�N���A�J���[
		float clearColor[4] = { 0.0f,0.0f, 0.0f,0.0f }; // ���ۂ��F
#pragma endregion DirectX��b�@�\�Q

		#pragma region �ϊ��s��
		//�ϊ��s��
		View matView;
		Projection matProjection;
#pragma endregion �ϊ��s��
		
		#pragma region �e�탊�\�[�X
		//3D���f��
		#pragma region 3D���f��
		std::shared_ptr<Model> skydomeModel;
		std::shared_ptr<Model> testBoxModel;
		std::shared_ptr<Model> testSphereModel;
		std::shared_ptr<Model> testGroundModel;
		std::shared_ptr<Model> rayModel;
		std::shared_ptr<Model> bombModel;
		std::shared_ptr<Model> rockOnRayModel;
		std::shared_ptr<Model> playerModel;
		std::shared_ptr<Model> enemyModel;
		std::shared_ptr<Model> bossModel;
		std::shared_ptr<Model> planeModel;
#pragma endregion 3D���f��

		//�e�N�X�`��
		#pragma region �e�N�X�`��
		std::shared_ptr <Texture> debugTextTexture;
		std::shared_ptr <Texture> mapEnemyTexture;
		std::shared_ptr <Texture> mapEnemyBTexture;
		std::shared_ptr <Texture> mapPlayerTexture;
		std::shared_ptr <Texture> mapPlayerBTexture;
		std::shared_ptr <Texture> mapBackTexture;
		std::shared_ptr <Texture> rockOnreticle;
		std::shared_ptr <Texture> titleName;
#pragma endregion �e�N�X�`��

		//�T�E���h
		#pragma region �T�E���h
		int rockOnreticleNum = 0;
		int volume = 255;
#pragma endregion �T�E���h

#pragma endregion �e�탊�\�[�X

		#pragma region 3D�I�u�W�F�N�g

		Object3d Skydorm;
		Object3d ground;
		Object3d rockOnleticle;
		Player player;
		EnemyManager enemys;
		std::list<std::unique_ptr<Exp>> exps;

#pragma endregion 3D�I�u�W�F�N�g

		#pragma region �X�v���C�g

		DebugText debugText;
		Sprite test;
		Sprite mapBack;
		Sprite mapPlayer;
		Sprite titleNameSprite;
#pragma endregion �X�v���C�g
		
		#pragma region �ʏ�ϐ�
		float mapSize = 300;
		MCB::Float2 mapPosition = { (mapSize /2.0f), (float)dxWindow->window_height - (mapSize / 2.0f) };
		float mapOffSet = 5;
		std::list<std::unique_ptr<Drone>> testD;
		Boss boss;
		bool degug = false;
		int nowScene = Title;
		int nextScene = Title;

		MCB::Float2 titlePos = { (float)dxWindow->window_width / 2, (float)dxWindow->window_height / 2 };
		MCB::Float2 titleSize = { 160 * 6,64 * 6 };
		int titleTimer = 0;
		int titleMaxTime = 300;
		bool titleMove = false;
		bool SPAttackTutoEnd = false;
		int nowTutorial = None;
		int nowSPAttackTuto = None;
#pragma endregion �ʏ�ϐ�
		
	public:
		Scene(RootParameter* root,Depth* depth, PipelineRootSignature* pipeline, PipelineRootSignature* pipeline1);
		~Scene();
		void Initialize() override;
		FPS* fps = nullptr;

		//�e�������n�֐��Q--------------------
		void LoadModel()  override;
		void LoadTexture()  override;
		void LoadSound()  override;
		void Object3DInit()  override;
		void SpriteInit()  override;
		//---------------
		void ChengeScene() override;
		void GameSceneInit();
		void TitleSceneInit();
		void ClearSceneInit();
		void OverSceneInit();

		void MatrixUpdate();

		void GameSceneUpdate();
		void TitleSceneUpdate();
		void ClearSceneUpdate();
		void OverSceneUpdate();
		void Update() override;

		void GameSceneDraw();
		void TitleSceneDraw();
		void ClearSceneDraw();
		void OverSceneDraw();
		void Draw() override;
		void Sporn(Float3 enemyPos, float expPoint);

		void DeleteExp();

		void MiniMapDraw(Sprite sprite, Float2 objectPos, Float2 playerPos, Texture* maptex, float SpriteSize = 15);
		void CheckAllColision();

		void TutorialCheckAllColision();
	};

}

