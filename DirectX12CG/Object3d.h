#pragma once
#include <d3dcompiler.h>
#include <Windows.h>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <DirectXMath.h>
#include <wrl.h>
#include "WorldMatrix.h"
#include "Dx12.h"
#include "View.h"
#include "Projection.h"
#include "Vector3D.h"
#include "Descriptor.h"
#include <vector>
#include "Model.h"
#include "Quaternion.h"
#include "Light.h"

namespace MCB
{
    class Object3d
    {
    public:

        //定数バッファ用構造体(行列)------------------------
        typedef struct ConstBufferDataTransform
        {
            //DirectX::XMMATRIX mat;
            DirectX::XMMATRIX viewproj;
            DirectX::XMMATRIX world;
            Float3 cameraPos;

        };
        //---------------------------------

        static Light* light;

        //行列用定数バッファ
        Microsoft::WRL::ComPtr<ID3D12Resource> constBuffTranceform = nullptr;

        //行列用定数バッファマップ
        ConstBufferDataTransform* constMapTranceform = nullptr;

        //アフィン変換情報
        DirectX::XMFLOAT3 scale = { 1.0f,1.0f,1.0f };
        DirectX::XMFLOAT3 rotasion = { 0.0f,0.0f,0.0f };
        DirectX::XMFLOAT3 position = { 0.0f, 0.0f, 0.0f };

        //ワールド行列
        WorldMatrix matWorld = {};

        const Vector3D NORM_FRONT_VEC = {0,0,1};
        Vector3D nowFrontVec = {0,0,1};
        float frontAngle = 0;

        Model* model = nullptr;


        Object3d();

        ~Object3d();

        //親オブジェクトへのポインタ
        Object3d* parent = nullptr;

        bool trackingFlag = false;

        void Init();

        void MatrixUpdata(View& view, Projection& projection, bool isBillBord = false);

        void MatrixUpdata(View& view, Projection& projection, Quaternion q, bool isBillBord = false);

        void Draw();

        void Draw(unsigned short int incremant);

        static void SetLight(Light* light);
        //void CreateModel(const char* fileName);
    };

}

