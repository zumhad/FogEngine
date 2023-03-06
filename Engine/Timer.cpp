#include "Timer.h"

#include "Application.h"

float Time::mDeltaTime = 0.0f;
float Time::mSecondsPerCount = 0.0f;
long long Time::mBaseTime = 0;
long long Time::mStopTime = 0;
long long Time::mPausedTime = 0;
long long Time::mPrevTime = 0;
long long Time::mCurrTime = 0;
bool Time::mStopped = false;
int Time::mFPS = 0;

void Time::Setup()
{
	long long countsPerSec;
	QueryPerformanceFrequency((LARGE_INTEGER*)&countsPerSec);

	mSecondsPerCount = 1.0f / (float)countsPerSec;
}

float Time::TotalTime()
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
	long long currTime;
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
		long long startTime;
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
		long long currTime;
		QueryPerformanceCounter((LARGE_INTEGER*)&currTime);

		mStopTime = currTime;
		mStopped = true;
	}
}

bool Time::LockFPS()
{
	if (Application::GetFpsMax() <= 0) return true;

	const float maxPeriod = 1.0f / Application::GetFpsMax();
	static float elipsedTime = 0.0f;

	long long currTime;
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

	QueryPerformanceCounter((LARGE_INTEGER*)&mCurrTime);

	mDeltaTime = (mCurrTime - mPrevTime) * mSecondsPerCount;
	mPrevTime = mCurrTime;

	if (mDeltaTime < 0.0)
	{
		mDeltaTime = 0.0;
	}

	static float timeElapsed = 0.0f;
	static int fps = 0;

	fps++;
	timeElapsed += mDeltaTime;

	if (timeElapsed >= 1.0f)
	{
		mFPS = fps;

		fps = 0;
		timeElapsed = 0.0f;
	}
}

int Time::GetFPS()
{
	return mFPS;
}

