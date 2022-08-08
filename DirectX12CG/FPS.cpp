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

float FPS::GetFPS()
{
	//nowTime = timeGetTime();		// 今回のフレームの時間

	//frameCount++;	// フレーム数をカウントする

	//// 経過時間が１秒を超えたらカウントと時間をリセット
	//if (nowTime - prevTime >= 1000)
	//{
	//	prevTime = timeGetTime();
	//	nowFps = frameCount;
	//	frameCount = 0;
	//}
	//return nowFps;

	if (frameTime > 0.0) { // 経過時間が0より大きい(こうしないと下の計算でゼロ除算になると思われ)
		fps = (fps * 0.99f) + (0.01f / frameTime); // 平均fpsを計算
	}
	return fps;
}