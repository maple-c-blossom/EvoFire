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
		float fps = 0;

		DWORD prev_time = timeGetTime();	// �O��̎���
		int frame_count = 0;		// �t���[���J�E���g
		DWORD now_time = timeGetTime();		// ����̃t���[���̎���
		int nowFps = 0;
	public:
		void Initialize(unsigned int fps);
		void FPSFixed();

		int GetFPS();

	};
}