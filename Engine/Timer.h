#pragma once

#include "Core.h"

class FOG_API Application;

class FOG_API Time
{
	friend class Application;

public:
	static float DeltaTime();
	static int GetFPS();

private:
	static void Setup();

	static float TotalTime();
	static void Reset();
	static void Start();
	static void Stop();
	static void Tick();
	static bool LockFPS();

private:
	static float mSecondsPerCount;
	static float mDeltaTime;
	static int mFPS;

	static long long mBaseTime;
	static long long mPausedTime;
	static long long mStopTime;
	static long long mPrevTime;
	static long long mCurrTime;

	static bool mStopped;
};