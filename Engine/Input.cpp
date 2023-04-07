#include "Input.h"

#include "Application.h"
#include "Timer.h"

IDirectInput8A* Input::mInput;
IDirectInputDevice8A* Input::mMouse;
IDirectInputDevice8A* Input::mKeyboard;

DIMOUSESTATE2 Input::mMouseState;
unsigned char Input::mKeyboardState[KeyInput::COUNT_KEY];

bool Input::mButtons[2][KeyInput::COUNT_KEY];
float Input::mHoldDuration[KeyInput::COUNT_KEY];
float Input::mAnalogs[MouseInput::COUNT_MOUSE];
bool Input::mVisible;

void Input::Setup()
{
	mInput = 0;
	mMouse = 0;
	mKeyboard = 0;

	memset(&mMouseState, 0, sizeof(DIMOUSESTATE2));
	memset(&mKeyboardState, 0, sizeof(mKeyboardState));
	memset(&mHoldDuration, 0, sizeof(mHoldDuration));

	memset(mButtons[1], 0, sizeof(mButtons[1]));
	memset(mButtons[0], 0, sizeof(mButtons[0]));
	memset(mAnalogs, 0, sizeof(mAnalogs));

	FOG_TRACE(DirectInput8Create(GetModuleHandle(0), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&mInput, 0));

	FOG_TRACE(mInput->CreateDevice(GUID_SysMouse, &mMouse, 0));
	FOG_TRACE(mMouse->SetDataFormat(&c_dfDIMouse2));
	//s_Mouse->SetCooperativeLevel(ApplicationEngine::GetHWND(), DISCL_FOREGROUND | DISCL_EXCLUSIVE);

	FOG_TRACE(mInput->CreateDevice(GUID_SysKeyboard, &mKeyboard, nullptr));
	FOG_TRACE(mKeyboard->SetDataFormat(&c_dfDIKeyboard));
	//mKeyboard->SetCooperativeLevel(ApplicationEngine::GetHWND(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);

	DIPROPDWORD dipdw{};
	dipdw.diph.dwSize = sizeof(DIPROPDWORD);
	dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER);
	dipdw.diph.dwObj = 0;
	dipdw.diph.dwHow = DIPH_DEVICE;
	dipdw.dwData = 10;
	mKeyboard->SetProperty(DIPROP_BUFFERSIZE, &dipdw.diph);
}


void Input::Update()
{
	memcpy(mButtons[1], mButtons[0], sizeof(mButtons[0]));
	memset(mButtons[0], 0, sizeof(mButtons[0]));
	memset(mAnalogs, 0, sizeof(mAnalogs));

	FOG_TRACE(mMouse->Acquire());
	FOG_TRACE(mMouse->GetDeviceState(sizeof(DIMOUSESTATE2), &mMouseState));
	FOG_TRACE(mKeyboard->Acquire());
	FOG_TRACE(mKeyboard->GetDeviceState(sizeof(mKeyboardState), mKeyboardState));

	if (Application::IsPaused()) return;

	for (int i = 0; i < 256; ++i)
	{
		mButtons[0][i] = (mKeyboardState[i] & 0x80) != 0;

		if (mKeyboardState[15] != 0 && mKeyboardState[128])
		{
			exit(0);
		}
	}

	for (int i = 0; i < 8; ++i)
	{
		if (mMouseState.rgbButtons[i] > 0) mButtons[0][MOUSE_LEFT + i] = true;
	}

	mAnalogs[MOUSE_X] = (float)mMouseState.lX * 0.0018f;
	mAnalogs[MOUSE_Y] = (float)mMouseState.lY * 0.0018f;

	if (mMouseState.lZ > 0)
		mAnalogs[MOUSE_SCROLL] = 1.0f;
	else if (mMouseState.lZ < 0)
		mAnalogs[MOUSE_SCROLL] = -1.0f;

	for (int i = 0; i < 256; ++i)
	{
		if (mButtons[0][i])
		{
			if (!mButtons[1][i])
				mHoldDuration[i] = 0.0f;
			else
				mHoldDuration[i] += Time::DeltaTime();
		}
	}
}

bool Input::Press(KeyInput di)
{
	return mButtons[0][di];
}

bool Input::Down(KeyInput di)
{
	return mButtons[0][di] && !mButtons[1][di];
}

bool Input::Up(KeyInput di)
{
	return !mButtons[0][di] && mButtons[1][di];
}

float Input::GetTimeAxis(KeyInput di)
{
	return mHoldDuration[di];
}

float Input::GetAxis(MouseInput ai)
{
	return mAnalogs[ai];
}