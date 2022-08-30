#pragma once
#include "IScene.h"
#define _USE_MATH_DEFINES
#include <cmath>


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

#pragma region ゲーム系.h include

#include "RayObject.h"
#include "SphereObj.h"
#include "Player.h"
#include "EnemyManager.h"
#include "exp.h"
#include "Drone.h"
#include "Boss.h"
#pragma endregion ゲーム系.h include

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

		#pragma region 変換行列
		//変換行列
		View matView;
		Projection matProjection;
#pragma endregion 変換行列
		
		#pragma region 各種リソース
		//3Dモデル
		#pragma region 3Dモデル
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
		std::shared_ptr<Model> enemyExpModel;
		std::shared_ptr<Model> playerExpModel;
		std::shared_ptr<Model> playerBulletModel;
#pragma endregion 3Dモデル

		//テクスチャ
		#pragma region テクスチャ
		std::shared_ptr <Texture> debugTextTexture;
		std::shared_ptr <Texture> mapEnemyTexture;
		std::shared_ptr <Texture> mapEnemyBTexture;
		std::shared_ptr <Texture> mapPlayerTexture;
		std::shared_ptr <Texture> mapPlayerBTexture;
		std::shared_ptr <Texture> mapBackTexture;
		std::shared_ptr <Texture> mapBossTexture;
		std::shared_ptr <Texture> rockOnreticle;
		std::shared_ptr <Texture> titleName;
		std::shared_ptr <Texture> moveText;
		std::shared_ptr <Texture> moveTuto;
		std::shared_ptr <Texture> RockOnText;
		std::shared_ptr <Texture> RotaTuto;
		std::shared_ptr <Texture> attackText;
		std::shared_ptr <Texture> attackTuto;
		std::shared_ptr <Texture> expTuto;
		std::shared_ptr <Texture> expText;
		std::shared_ptr <Texture> Gaze;
		std::shared_ptr <Texture> GazeSpace;
		std::shared_ptr <Texture> level;
		std::shared_ptr <Texture> homingMissileTex;
		std::shared_ptr <Texture> laserTex;
		std::shared_ptr <Texture> bombTex;
		std::shared_ptr <Texture> hpTex;
		std::shared_ptr <Texture> pHpTex;

		std::shared_ptr <Texture> homingMissileTuto;
		std::shared_ptr <Texture> laserTuto;
		std::shared_ptr <Texture> bombTuto;
		std::shared_ptr <Texture> homingMissileTutoC;
		std::shared_ptr <Texture> laserTutoC;
		std::shared_ptr <Texture> bombTutoC;

		std::shared_ptr <Texture> hpGazeSpace;
		std::shared_ptr <Texture> hpGaze;

		std::shared_ptr <Texture> BossGazeSpace;
		std::shared_ptr <Texture> BossGaze;

		std::shared_ptr <Texture> ScoreTex;
		std::shared_ptr <Texture> HiScoreTex;

		std::shared_ptr <Texture> bombText;
		std::shared_ptr <Texture> homingMissileText;
		std::shared_ptr <Texture> laserText;
		std::shared_ptr <Texture> endText;

		std::shared_ptr <Texture> noTexture;
		std::shared_ptr <Texture> StartTexture;

		std::shared_ptr <Texture> attensyonTexture;

		std::shared_ptr <Texture> GameClearTexture;
		std::shared_ptr <Texture> GameOverTexture;

		std::shared_ptr <Texture> GameSelectTexture;
		std::shared_ptr <Texture> TitleSelectTexture;
		std::shared_ptr <Texture> tutorialSelectTexture;
		std::shared_ptr <Texture> tutorialSkipTexture;
		std::shared_ptr <Texture> SelectTutoTexture;
#pragma endregion テクスチャ

		//サウンド
		#pragma region サウンド
		int rockOnreticleNum = 0;
		int volume = 255;

		int selectSound;
		int titleBGM;
		int enterSound;
		int gameBGM;
#pragma endregion サウンド

#pragma endregion 各種リソース

		#pragma region 3Dオブジェクト

		Object3d Skydorm;
		Object3d ground;
		Object3d rockOnleticle;
		Player player;
		EnemyManager enemys;
		std::list<std::unique_ptr<Exp>> exps;

#pragma endregion 3Dオブジェクト

		#pragma region スプライト

		DebugText debugText;
		Sprite test;
		Sprite mapBack;
		Sprite mapPlayer;
		
		Sprite titleNameSprite;
		Sprite moveTextSprite;
		Sprite moveTutoSprite;
		Sprite RockOnTextSprite;
		Sprite RotaTutoSprite;
		Sprite attackTextSprite;
		Sprite attackTutoSprite;
		Sprite expTutoSprite;
		Sprite expTextSprite;
		Sprite GazeSprite;
		Sprite GazeSpaceSprite;
		Sprite levelSprite;
		Sprite hommingMissileSprite;
		Sprite laserSprite;
		Sprite bombSprite;
		Sprite hpTextSprite;
		Sprite pHpTextSprite;

		Sprite homingMissileTutoSprite;
		Sprite laserTutoSprite;
		Sprite bombTutoSprite;
		Sprite homingMissileTutoCSprite;
		Sprite laserTutoCSprite;
		Sprite bombTutoCSprite;

		Sprite hpGazeSpaceSprite;
		Sprite bosshpGazeSpaceSprite;
		Sprite bosshpGazeSprite;
		Sprite hpGazeSprite;

		Sprite ScoreSprite;
		Sprite HiScoreSprite;

		Sprite bombTextSprite;
		Sprite homingMissileTextSprite;
		Sprite laserTextSprite;
		Sprite endTextSprite;

		Sprite SceneChengeSprite;
		Sprite StartSprite;

		Sprite GameOverSprite;
		Sprite GameClearSprite;

		Sprite BossMarkSprite;
		Sprite Attensyon;

		Sprite GameSelectSprite;
		Sprite TitleSelectSprite;
		Sprite tutorialSelectSprite;
		Sprite tutorialSkipSprite;
		Sprite SelectTutoSprite;
#pragma endregion スプライト
		
		#pragma region 通常変数
		float mapSize = 300;
		MCB::Float2 mapPosition = { (mapSize /2.0f) + 10, (float)dxWindow->window_height - (mapSize / 2.0f)- 10 };
		float mapOffSet = 5;
		std::list<std::unique_ptr<Drone>> testD;
		Boss boss;
		bool degug = false;
		int nowScene = Title;
		int nextScene = Title;

		MCB::Float2 titlePos = { (float)dxWindow->window_width / 2, (float)dxWindow->window_height / 2-100 };
		MCB::Float2 titleSize = { 160 * 6,64 * 6 };
		int titleTimer = 0;
		int titleMaxTime = 300;
		bool titleMove = false;

		MCB::Float2 moveTutoPos = { (float)dxWindow->window_width / 2, 32 * 3 + 70 };
		MCB::Float2 moveTutoSize = { 288 * 2, 64 * 2 };
		int moveTutoTimer = 0;
		int moveTutoMaxTime = 30;
		bool moveTutoFlag = false;

		MCB::Float2 rotaTutoPos = { (float)dxWindow->window_width / 2, 64 * 3 + 70 };
		MCB::Float2 rotaTutoSize = { 112 * 3, 64 * 3 };
		int rotaTutoTimer = 0;
		int rotaTutoMaxTime = 30;
		bool rotaTutoFlag = false;

		MCB::Float2 attackTutoPos = { (float)dxWindow->window_width / 2, 64 * 3 + 70 };
		MCB::Float2 attackTutoSize = { 173 * 1.25, 64 * 2 };
		int attackTutoTimer = 0;
		int attackTutoMaxTime = 30;
		bool attackTutoFlag = false;

		MCB::Float2 gazePos = { mapPosition.x - mapSize / 2, (mapPosition.y - mapSize / 2 - 90) };
		MCB::Float2 gazeSize = { 32 / 2, 64 / 2 };

		int GameTimer = 0;
		int GameMaxTime = 120;
		float StartPos = -1000;
		bool GameStartFlag = false;

		bool SPAttackTutoEnd = false;
		int nowTutorial = None;
		int nowSPAttackTuto = None;

		int sinTimer = -1;

		int isChengeSceneTimer = 0;
		int isChengeSceneTime = 120;
		bool isChengeScene = false;
		int prevScene = None;

		int SelectScene = Title;
		bool TutorialSkip = false;

		int SelectTimer = 0;
#pragma endregion 通常変数
		
	public:
		Scene(RootParameter* root,Depth* depth, PipelineRootSignature* pipeline, PipelineRootSignature* pipeline1);
		~Scene();
		void Initialize() override;
		FPS* fps = nullptr;

		//各初期化系関数群--------------------
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
		void Sporn(Float3 enemyPos, float expPoint, bool isplayer = false);

		void DeleteExp();

		void MiniMapDraw(Sprite sprite, Float2 objectPos, Float2 playerPos, Texture* maptex, float SpriteSize = 20,bool flag = false);
		void CheckAllColision();

		void TutorialCheckAllColision();
	};

}

