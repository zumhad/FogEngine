#pragma once

#include "Core.h"

namespace Module
{
	class FOG_API Time
	{
	public:
		Time();
		float TotalTime()const;
		static float DeltaTime();

		void Reset(); // Call before message loop.
		void Start(); // Call when unpaused.
		void Stop();  // Call when paused.
		void Tick();  // Call every frame.
		bool LockFPS();

	private:
		float mSecondsPerCount;
		static float mDeltaTime;

		long long mBaseTime;
		long long mPausedTime;
		long long mStopTime;
		long long mPrevTime;
		long long mCurrTime;

		bool mStopped;
	};
}