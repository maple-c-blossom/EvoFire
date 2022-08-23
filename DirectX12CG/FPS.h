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

		DWORD prevTime = timeGetTime();	// 前回の時間
		int frameCount = 0;		// フレームカウント
		DWORD nowTime = timeGetTime();		// 今回のフレームの時間
		int nowFps = 0;
	public:
		float fps = 0;
		void Initialize(unsigned int fps);
		void FPSFixed();

		float GetFPS();

	};
}