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

		DWORD prev_time = timeGetTime();	// 前回の時間
		int frame_count = 0;		// フレームカウント
		DWORD now_time = timeGetTime();		// 今回のフレームの時間
		int nowFps = 0;
	public:
		void Initialize(unsigned int fps);
		void FPSFixed();

		int GetFPS();

	};
}