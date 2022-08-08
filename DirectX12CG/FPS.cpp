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

float FPS::GetFPS()
{
	//nowTime = timeGetTime();		// ����̃t���[���̎���

	//frameCount++;	// �t���[�������J�E���g����

	//// �o�ߎ��Ԃ��P�b�𒴂�����J�E���g�Ǝ��Ԃ����Z�b�g
	//if (nowTime - prevTime >= 1000)
	//{
	//	prevTime = timeGetTime();
	//	nowFps = frameCount;
	//	frameCount = 0;
	//}
	//return nowFps;

	if (frameTime > 0.0) { // �o�ߎ��Ԃ�0���傫��(�������Ȃ��Ɖ��̌v�Z�Ń[�����Z�ɂȂ�Ǝv���)
		fps = (fps * 0.99f) + (0.01f / frameTime); // ����fps���v�Z
	}
	return fps;
}