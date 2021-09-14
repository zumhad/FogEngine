#include "Time.h"

#include "Properties.h"

namespace Module
{
	float Time::mDeltaTime = 0.0f;

	Time::Time()
		: mSecondsPerCount(0.0f), mBaseTime(0), mStopTime(0),
		mPausedTime(0), mPrevTime(0), mCurrTime(0), mStopped(false)
	{
		long long countsPerSec;
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
		if (Singlton.fpsMax <= 0) return true;

		static const float maxPeriod = 1.0f / Singlton.fpsMax;
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
}


