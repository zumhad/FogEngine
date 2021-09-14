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

		void KeyDown(short key);
		void KeyUp(short key);
		void ResetKeys();
		void ResetKeysPress();

		bool IsKeyDown(short key);
		bool IsKeyUp(short key);
		bool IsKeyPress(short key);

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
		Mouse();
		void Initialize();
		void Update();
		void SetZeroAxis();

		void KeyDown(short mouse);
		void KeyUp(short mouse);
		void ResetKeys();
		void ResetKeysPress();

		bool IsMouseDown(short key);
		bool IsMouseUp(short key);
		bool IsMousePress(short key);

		void SetPos(short x, short y) { mPosX = x; mPosY = y; }
		short GetX() { return mPosX; }
		short GetY() { return mPosY; }

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

		bool mKeysPress[2] = {};
		bool mKeysUp[2] = {};
		bool mKeysDown[2] = {};

		short mPosX, mPosY;

		bool mCapture = false;
		bool mCursorEnabled;
		bool mCursorState;
	};
}
