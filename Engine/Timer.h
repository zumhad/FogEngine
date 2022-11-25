#pragma once

#include "Core.h"

class FOG_API Time
{
public:
	static float DeltaTime();

protected:
	static float mSecondsPerCount;
	static float mDeltaTime;

	static long long mBaseTime;
	static long long mPausedTime;
	static long long mStopTime;
	static long long mPrevTime;
	static long long mCurrTime;

	static bool mStopped;
};