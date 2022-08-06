#include "FPS.h"

using namespace MCB;



void MCB::FPS::Initialize(unsigned int FPS)
{
	MIN_FREAM_TIME = 1.0f / FPS;
	QueryPerformanceCounter(&timeStart);
	QueryPerformanceFrequency(&timeFreq);
}

void MCB::FPS::FPSFixed() 
{
	QueryPerformanceCounter(&timeEnd);
	frameTime = static_cast<float>(timeEnd.QuadPart - timeStart.QuadPart) / static_cast<float>(timeFreq.QuadPart);

	if (frameTime < MIN_FREAM_TIME) {
		// ミリ秒に変換
		DWORD sleepTime = static_cast<DWORD>((MIN_FREAM_TIME - frameTime) * 1000);

		timeBeginPeriod(1);
		Sleep(sleepTime);
		timeEndPeriod(1);

		return;
	}

	timeStart = timeEnd; // 入れ替え
}

int FPS::GetFPS()
{
	now_time = timeGetTime();		// 今回のフレームの時間

	frame_count++;	// フレーム数をカウントする

	// 経過時間が１秒を超えたらカウントと時間をリセット
	if (now_time - prev_time >= 1000)
	{
		prev_time = timeGetTime();
		nowFps = frame_count;
		frame_count = 0;
	}
	return nowFps;
}