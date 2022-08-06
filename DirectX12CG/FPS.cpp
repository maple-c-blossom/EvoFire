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
		// �~���b�ɕϊ�
		DWORD sleepTime = static_cast<DWORD>((MIN_FREAM_TIME - frameTime) * 1000);

		timeBeginPeriod(1);
		Sleep(sleepTime);
		timeEndPeriod(1);

		return;
	}

	timeStart = timeEnd; // ����ւ�
}

int FPS::GetFPS()
{
	now_time = timeGetTime();		// ����̃t���[���̎���

	frame_count++;	// �t���[�������J�E���g����

	// �o�ߎ��Ԃ��P�b�𒴂�����J�E���g�Ǝ��Ԃ����Z�b�g
	if (now_time - prev_time >= 1000)
	{
		prev_time = timeGetTime();
		nowFps = frame_count;
		frame_count = 0;
	}
	return nowFps;
}