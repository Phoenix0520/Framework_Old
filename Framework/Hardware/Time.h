#pragma once

#include "Framework/Framework.h"
#include "Base/Singleton.h"


class Timer
{
public:
	Timer(float startTime, bool freezed) : leftTime(startTime), stopped(freezed) {}
	~Timer() {}

public:
	bool Tick();

	bool IsStopped()	{ return stopped; }
	void Pause()		{ stopped = true; }
	void Resume()		{ stopped = false; }

private:
	bool stopped;
	float leftTime;

};

class Time : public Singleton<Time>
{
private:
	Time();
	Time(const Time& other) {}
	~Time() {}

public:
	friend Singleton<Time>;

public:
	virtual void Update() final override;
	
	void Start();
	void Stop();

	inline float GetGameTime()		const { return runningTime; }	// 현재 시간 출력
	inline float GetDeltaTime()		const { return stopped ? 0.0f : elapsedTime; }	// 델타 시간 (틱당 시간) 출력
	inline float GetFramePerSec()	const { return stopped ? 0.0f : frameCountPerSec; }
	inline float GetLockFPS()		const { return FPSLock; }

	inline void  SetLockFPS(float maxFPS) { FPSLock = maxFPS;}

public: // 타이머 함수
	bool CreateTimer(string timerName, float startTime, bool freezed);
	bool DestroyTimer(string timerName);
	bool StartTimer(string timerName);
	bool PauseTimer(string timerName);

private:
	bool stopped = false;			// 시간 멈춤

	INT64 tickPerSec		= 0;	// 초당 틱
	INT64 curTime			= 0;	// 현재 측정된 시간
	INT64 lastTime			= 0;	// 마지막으로 측정된 시간
	INT64 lastFPSUpdate		= 0;	// 마지막으로 업데이트 한 시간
	INT64 FPSUpdateInterval = 10;	// 업데이트 간격 틱
	// 단위 : ms(밀리 초)

	float runningTime		= 0.0f;		// 게임 실행후 흐른 시간
	float elapsedTime		= 0.0f;		// 틱당 시간 (second/frame | 이전 프레임 과의 시간 차이)
	UINT  frameCount		= 0;		// 프레임 수
	float frameCountPerSec	= 0;		// 초당 프레임 수 (FPS)

	float framePerSec		= 0.0f;		// 현재 초당 프레임
	float FPSLock			= 0.0f;		// 초당 프레임 제한값 (0 일시 제한 없음)
	float FPSElapsedTime	= 0.0f;		// FPS 틱당 시간
	// 단위 : sec(초)

	chrono::system_clock::time_point startTime;	// 크로노 기반 시간 측정시 사용할 시작시간

	map<string, Timer*> timerMap;	// 타이머

};