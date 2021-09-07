#pragma once

#include "Core.h"

#include "DeviceInput.h"

#include <Xinput.h>
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

namespace Module
{
	class Keyboard
	{
	public:
		Keyboard();

		void KeyDown(uint32 key);
		void KeyUp(uint32 key);
		void ResetKeys();
		void ResetKeysPress();

		bool IsKeyDown(uint32 key);
		bool IsKeyUp(uint32 key);
		bool IsKeyPress(uint32 key);

	private:
		bool mKeysPress[256] = {};
		bool mKeysUp[256] = {};
		bool mKeysDown[256] = {};

		bool mIsKeyDown = false;
		bool mIsKeyUp = false;
	};

	class Mouse
	{
	public:
		Mouse(bool cursor, bool isGame);
		void Initialize();
		void Update();
		void SetZeroAxis();

		void SetPos(uint16 x, uint16 y) { mPosX = x; mPosY = y; }
		uint16 GetX() { return mPosX; }
		uint16 GetY() { return mPosY; }

		void SetCapture(bool var) { mCapture = var; }
		void SetEnabled(bool var) { mCursorEnabled = var; }
		void SetState(bool var) { mCursorState = var; }
		bool GetCapture() { return mCapture; }
		bool GetEnabled() { return mCursorEnabled; }
		bool GetState() { return mCursorState; }

		float GetMouseAxis(MouseAxis ma);

	private:
		void SetZeroInput();

	private:
		IDirectInput8A* mDirectInput;
		IDirectInputDevice8A* mMouseDevice;
		DIMOUSESTATE2 mMouseState;

		float mAxis[2];

		uint16 mPosX, mPosY;

		bool mCapture = false;
		bool mCursorEnabled;
		bool mCursorState;
	};
}
