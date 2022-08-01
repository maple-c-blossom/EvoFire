#pragma once
#include <dinput.h>
#include "Dx12.h"
#include "DxWindow.h"
#define DIRECINPUT_VERSION 0x0800 //DirectInput�̃o�[�W�����w��

namespace MCB
{
    class Input
    {
    private:
        IDirectInputDevice8* devkeyboard = nullptr;

        IDirectInput8* dinput = nullptr;

        Input() {};
        Input(const Input& input) {};
        ~Input() {};
        Input& operator= (const Input & Input){};

    public:

        static Input* GetInstance();
        static void DeleteInstace();
        static Input* GetInitInstance();

        //�L�[������----------------------
        BYTE key[256] = {};
        BYTE oldkey[256] = {};
        //-----------------------

        //Input(HRESULT& result, WNDCLASSEX w, HWND hwnd);

        //�L�[�{�[�h������-------------------------------
        void KeyInit();
        //---------------------

        //���͊֐�-----------------------------------
#pragma region ���͊֐�
//�����Ă�����
        bool IsKeyDown(int keyNum);
        //�����Ă��Ȃ����
        bool IsKeyNDown(int keyNum);
        //���������
        bool IsKeyPress(int keyNum);
        //�������u��
        bool IsKeyTrigger(int keyNum);
        //�������u��
        bool IsKeyRelease(int keyNum);
#pragma endregion ���͊֐�
        //--------------------

        void Init();

        void UpDateInit();
    };
}


