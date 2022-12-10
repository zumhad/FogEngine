#pragma once

#include "Core.h"

#include "Timer.h"

class FOG_API TimeEngine : public Time
{
public:
	static void Setup();

	static float TotalTime();
	static void Reset(); // Call before message loop.
	static void Start(); // Call when unpaused.
	static void Stop();  // Call when paused.
	static void Tick();  // Call every frame.
	static bool LockFPS();
};