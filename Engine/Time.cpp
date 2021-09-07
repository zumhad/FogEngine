#include "Time.h"


float Time::mDeltaTime = 0.0f;

Time::Time(int16 maxFps)
	: mSecondsPerCount(0.0f), mBaseTime(0), mStopTime(0),
	mPausedTime(0), mPrevTime(0), mCurrTime(0), mStopped(false), mMaxFps(maxFps)
{
	int64 countsPerSec;
	QueryPerformanceFrequency((LARGE_INTEGER*)&countsPerSec);

	mSecondsPerCount = 1.0f / (float)countsPerSec;
}

float Time::TotalTime()const
{
	if (mStopped)
	{
		return (float)(((mStopTime - mPausedTime) - mBaseTime) * mSecondsPerCount);
	}
	else
	{
		return (float)(((mCurrTime - mPausedTime) - mBaseTime) * mSecondsPerCount);
	}
}


float Time::DeltaTime()
{
	return mDeltaTime;
}

void Time::Reset()
{
	int64 currTime;
	QueryPerformanceCounter((LARGE_INTEGER*)&currTime);

	mCurrTime = currTime;
	mBaseTime = currTime;
	mPrevTime = currTime;
	mStopTime = 0;
	mStopped = false;
}

void Time::Start()
{
	if (mStopped)
	{
		int64 startTime;
		QueryPerformanceCounter((LARGE_INTEGER*)&startTime);

		mPausedTime += (startTime - mStopTime);

		mPrevTime = startTime;
		mStopTime = 0;
		mStopped = false;
	}
}

void Time::Stop()
{
	if (!mStopped)
	{
		int64 currTime;
		QueryPerformanceCounter((LARGE_INTEGER*)&currTime);

		mStopTime = currTime;
		mStopped = true;
	}
}

bool Time::LockFPS()
{
	if (mMaxFps <= 0) return true;

	static const float maxPeriod = 1.0f / mMaxFps;
	static float elipsedTime = 0.0f;

	int64 currTime;
	QueryPerformanceCounter((LARGE_INTEGER*)&currTime);
	float time = (currTime - mBaseTime) * mSecondsPerCount;

	bool res = (time - elipsedTime >= maxPeriod);
	if (res)
		elipsedTime += maxPeriod;

	return res;
}

void Time::Tick()
{
	if (mStopped)
	{
		mDeltaTime = 0.0;
		return;
	}

	//static int64 currTime;
	QueryPerformanceCounter((LARGE_INTEGER*)&mCurrTime);
	//mCurrTime = currTime;


	mDeltaTime = (mCurrTime - mPrevTime) * mSecondsPerCount;

	mPrevTime = mCurrTime;


	if (mDeltaTime < 0.0)
	{
		mDeltaTime = 0.0;
	}
}
