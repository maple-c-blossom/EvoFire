#include "FPS.h"

using namespace MCB;



void MCB::FPS::Initialize(unsigned int FPS)
{
	MIN_FREAM_TIME = 1.0f / FPS;
	QueryPerformanceCounter(&timeStart);
}

void MCB::FPS::FPSFixed() 
{
	QueryPerformanceCounter(&timeEnd);
	frameTime = static_cast<float>(timeEnd.QuadPart - timeStart.QuadPart) / static_cast<float>(timeFreq.QuadPart);

	if (frameTime < MIN_FREAM_TIME) {
		// ƒ~ƒŠ•b‚É•ÏŠ·
		DWORD sleepTime = static_cast<DWORD>((MIN_FREAM_TIME - frameTime) * 1000);

		timeBeginPeriod(1);
		Sleep(sleepTime);
		timeEndPeriod(1);

		return;
	}

	timeStart = timeEnd; // “ü‚ê‘Ö‚¦
}

