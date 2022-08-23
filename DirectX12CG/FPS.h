#pragma once
#include <Windows.h>
#include <tchar.h>

#include <mmsystem.h>
#pragma comment(lib,"winmm.lib")


namespace MCB
{
	class FPS
	{
		float MIN_FREAM_TIME = 1.0f / 60;
		float frameTime = 0;
		LARGE_INTEGER timeStart;
		LARGE_INTEGER timeEnd;
		LARGE_INTEGER timeFreq;

		DWORD prevTime = timeGetTime();	// �O��̎���
		int frameCount = 0;		// �t���[���J�E���g
		DWORD nowTime = timeGetTime();		// ����̃t���[���̎���
		int nowFps = 0;
	public:
		float fps = 0;
		void Initialize(unsigned int fps);
		void FPSFixed();

		float GetFPS();

	};
}