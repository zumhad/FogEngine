#pragma once

#include "Core.h"

#include "InputHelper.h"

class FOG_API Input
{
public:
    static bool IsDown();
    static bool Press(KeyInput di);
    static bool Down(KeyInput di);
    static bool Up(KeyInput di);
    static float GetTimeAxis(KeyInput di);
    static float GetAxis(MouseInput ai);

protected:
	static IDirectInput8A* mInput;
	static IDirectInputDevice8A* mMouse;
    static IDirectInputDevice8A* mKeyboard;

	static DIMOUSESTATE2 mMouseState;
    static unsigned char mKeyboardState[KeyInput::COUNT_KEY];

    static bool mButtons[2][KeyInput::COUNT_KEY];
    static float mHoldDuration[KeyInput::COUNT_KEY];
    static float mAnalogs[MouseInput::COUNT_MOUSE];
    static bool mVisible;
};