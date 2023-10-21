#pragma once

#include "Core.h"

#include "InputHelper.h"

class Application;

class FOG_API Input
{
    friend class Application;

public:
    static bool Press(KeyInput di);
    static bool Down(KeyInput di);
    static bool Up(KeyInput di);
    static float GetTimeAxis(KeyInput di);
    static float GetAxis(MouseInput ai);
    static char GetNumber();

private:
    static void Setup();
    static void Update();
    static void UpdateChar(char ch);
    static void UpdateChar();

private:
	static IDirectInput8A* mInput;
	static IDirectInputDevice8A* mMouse;
    static IDirectInputDevice8A* mKeyboard;

	static DIMOUSESTATE2 mMouseState;
    static unsigned char mKeyboardState[KeyInput::COUNT_KEY];

    static char mChar;
    static bool mButtons[2][KeyInput::COUNT_KEY];
    static float mHoldDuration[KeyInput::COUNT_KEY];
    static float mAnalogs[MouseInput::COUNT_MOUSE];
    static bool mVisible;
};