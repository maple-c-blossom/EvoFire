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
#include "Vertex.h"
#include <vector>


namespace MCB
{
    class Particle
    {
    public:


        //�萔�o�b�t�@�p�\����(�s��)------------------------
        typedef struct ConstBufferDataTransform
        {
            DirectX::XMMATRIX mat;
        };
        //---------------------------------

        //�s��p�萔�o�b�t�@
        Microsoft::WRL::ComPtr<ID3D12Resource> constBuffTranceform = nullptr;

        //�s��p�萔�o�b�t�@�}�b�v
        ConstBufferDataTransform* constMapTranceform = nullptr;

        //�A�t�B���ϊ����
        DirectX::XMFLOAT3 scale = { 1.0f,1.0f,1.0f };
        DirectX::XMFLOAT3 rotasion = { 0.0f,0.0f,0.0f };
        DirectX::XMFLOAT3 position = { 0.0f, 0.0f, 0.0f };

        //���[���h�s��
        WorldMatrix matWorld = {};

        Vector3D NORM_FRONT_VEC = {};
        Vector3D nowFrontVec = {};
        float frontAngle = 0;
        
        Vertex vert;

        Particle(Dx12& dx12);

        ~Particle();

        //�e�I�u�W�F�N�g�ւ̃|�C���^
        Particle* parent = nullptr;

        bool trackingFlag = false;

        void Init(Dx12& dx12);

        void Updata(View& view, Projection& projection, bool isBillBord = false);

        void Draw(Dx12& dx12);

        //void CreateModel(const char* fileName);
    };

}





