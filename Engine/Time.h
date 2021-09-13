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

		int64 mBaseTime;
		int64 mPausedTime;
		int64 mStopTime;
		int64 mPrevTime;
		int64 mCurrTime;

		bool mStopped;
	};
}