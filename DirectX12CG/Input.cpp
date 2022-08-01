#include "Input.h"
#include <cassert>
#define DIRECINPUT_VERSION 0x0800 //DirectInput�̃o�[�W�����w��

using namespace MCB;

void Input::Init()
{
	Dx12* dx12 = Dx12::GetInstance();
	DxWindow* dxWindow = DxWindow::GetInstance();
	//���͌n������--------------
#pragma region ���͌n������


	dx12->result = DirectInput8Create(dxWindow->window.hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&dinput, nullptr);
	assert(SUCCEEDED(dx12->result));
	//�L�[�{�[�h�f�o�C�X�̐���-----------------
#pragma region �L�[�{�[�h�f�o�C�X�̐���


	dx12->result = dinput->CreateDevice(GUID_SysKeyboard, &devkeyboard, NULL);
	assert(SUCCEEDED(dx12->result));

#pragma endregion �L�[�{�[�h�f�o�C�X�̐���
	//--------------------------

	//���̓f�[�^�`���Z�b�g--------------------------------
	dx12->result = devkeyboard->SetDataFormat(&c_dfDIKeyboard);
	assert(SUCCEEDED(dx12->result));
	//---------------------------------

	//�r�����x������-------------------------------------------------------------
	dx12->result = devkeyboard->SetCooperativeLevel(dxWindow->hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	assert(SUCCEEDED(dx12->result));

#pragma endregion ���͌n������
	//----------------
}

void Input::UpDateInit()
{
	
	//�L�[�{�[�h������-------------------------------------
#pragma region �L�[�{�[�h������

		//�L�[�{�[�h���̎擾�J�n-----------------
	devkeyboard->Acquire();
	//----------------------------

	//�S�L�[�̓��͏�Ԃ��擾����---------------------------
	KeyInit();
	Dx12::GetInstance()->result = devkeyboard->GetDeviceState(sizeof(key), key);
	//----------------------------

#pragma endregion �L�[�{�[�h������
		//----------------------------------------

}

//Input::Input(HRESULT& result, WNDCLASSEX w, HWND hwnd)
//{
//	//���͌n������--------------
//#pragma region ���͌n������
//
//
//	result = DirectInput8Create(w.hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&dinput, nullptr);
//	assert(SUCCEEDED(result));
//	//�L�[�{�[�h�f�o�C�X�̐���-----------------
//#pragma region �L�[�{�[�h�f�o�C�X�̐���
//
//
//	result = dinput->CreateDevice(GUID_SysKeyboard, &devkeyboard, NULL);
//	assert(SUCCEEDED(result));
//
//#pragma endregion �L�[�{�[�h�f�o�C�X�̐���
//	//--------------------------
//
//	//���̓f�[�^�`���Z�b�g--------------------------------
//	result = devkeyboard->SetDataFormat(&c_dfDIKeyboard);
//	assert(SUCCEEDED(result));
//	//---------------------------------
//
//	//�r�����x������-------------------------------------------------------------
//	result = devkeyboard->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
//	assert(SUCCEEDED(result));
//
//#pragma endregion ���͌n������
//	//----------------
//}


Input* MCB::Input::GetInstance()
{
	static Input* instance = new Input;
	return instance;
}

void MCB::Input::DeleteInstace()
{
	delete Input::GetInstance();
}

Input* MCB::Input::GetInitInstance()
{
	static Input* instance = Input::GetInstance();
	instance->Init();
	return instance;
}

void Input::KeyInit()
{
	for (int i = 0; i < 256; i++)
	{
		oldkey[i] = key[i];
	}

}

bool Input::IsKeyDown(int keyNum)
{
		if (key[keyNum])
		{
			return true;
		}
		return false;
}

bool Input::IsKeyNDown(int keyNum)
{
	if (!key[keyNum])
	{
		return true;
	}
	return false;
}

bool Input::IsKeyPress(int keyNum)
{
	if (key[keyNum] && oldkey[keyNum])
	{
		return true;
	}
	return false;
}

bool Input::IsKeyTrigger(int keyNum)
{
	if (key[keyNum] && !oldkey[keyNum])
	{
		return true;
	}
	return false;
}

bool Input::IsKeyRelease(int keyNum)
{
	if (!key[keyNum] && oldkey[keyNum])
	{
		return true;
	}
	return false;
}

