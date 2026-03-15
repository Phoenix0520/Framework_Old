#include "Time.h"

bool Timer::Tick()
{
	if (leftTime >= DELTA)
	{
		leftTime -= DELTA;
		return true;
	}
	
	return false;
}

Time::Time()
{
	FPSLock = STARTLOCKFPS;

	startTime = chrono::system_clock::now();
	QueryPerformanceFrequency((LARGE_INTEGER*)&tickPerSec);

	FPSUpdateInterval = tickPerSec >> 1;
}

void Time::Update()
{
	// 위는 타이머가 멈추어도 업데이트 할 내용

	if (stopped)
	{
		// 멈출때만 업데이트 할 내용
		return;
	}

	// 아래는 타이머가 멈추면 멈출 내용

	chrono::duration<double> sec = chrono::system_clock::now() - startTime;

	elapsedTime = (float)sec.count();

	if (FPSLock != 0.0f)
	{
		while (elapsedTime < 1.0f / FPSLock)
		{
			sec = chrono::system_clock::now() - startTime;
			elapsedTime = (float)sec.count();
		}
	}

	startTime = chrono::system_clock::now();
	frameCount++;
	runningTime += elapsedTime;

	FPSElapsedTime += elapsedTime;

	if (FPSElapsedTime > 1.0f)
	{
		framePerSec = FPSElapsedTime / frameCount;
		frameCountPerSec = (float)frameCount;
		FPSElapsedTime = 0.0f;
		frameCount = 0;
	}

	for (auto timer : timerMap)					// 모든 타이머를 실행한다
	{
		if (!timer.second->IsStopped())			// 일시 정지 상태가 아니라면
		{
			if (timer.second->Tick())			// 시간 진행
			{

			}
			else if (TIMERAUTODESTROY)			// 자동 타이머 삭제가 켜져있다면
			{
				DestroyTimer(timer.first);		// 자동으로 삭제한다
			}
		}
	}
}

void Time::Start()
{
	QueryPerformanceCounter((LARGE_INTEGER*)&lastTime);	// 현재 시간 저장 후
	stopped = false;									// 시간 재개
}

void Time::Stop()
{
	INT64 stopTime = 0;
	QueryPerformanceCounter((LARGE_INTEGER*)&stopTime);					// 현재 시간 저장후

	runningTime += (float)(stopTime - lastTime) / (float)tickPerSec;	// 흐른 시간에 반영한다
	stopped = true;														// 시간 일시정지
}

bool Time::CreateTimer(string timerName, float startTime, bool freezed)
{
	for (auto timer : timerMap)
	{
		if (timer.first.compare(timerName))
		{
			return false;
		}
	}

	Timer* timer = new Timer(startTime, freezed);

	timerMap.insert(make_pair(timerName, timer));

	return true;
}

bool Time::DestroyTimer(string timerName)
{
	for (auto timer : timerMap)
	{
		if (timer.first.compare(timerName))
		{
			timerMap.erase(timer.first);
			return true;
		}
	}

	return false;
}

bool Time::StartTimer(string timerName)
{
	for (auto timer : timerMap)
	{
		if (timer.first.compare(timerName))
		{
			if (timer.second->IsStopped())
			{
				timer.second->Resume();
				return true;

			}
			else
			{
				return false;
			}
		}
	}

	return false;
}

bool Time::PauseTimer(string timerName)
{
	for (auto timer : timerMap)
	{
		if (timer.first.compare(timerName))
		{
			if (!timer.second->IsStopped())
			{
				timer.second->Pause();
				return true;

			}
			else
			{
				
				return false;
			}
		}
	}

	return false;
}

