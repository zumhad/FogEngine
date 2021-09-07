#include "Input.h"

#include "EditHelper.h"
#include "Trace.h"
#include "Devices.h"


Module::Keyboard* Input::mKeyboard = 0;
Module::Mouse* Input::mMouse = 0;

namespace Module
{
	Keyboard::Keyboard()
	{
		Input::SetKeyboard(this);
	}

	Mouse::Mouse(bool cursor, bool isGame)
	{
		Input::SetMouse(this);

		SetEnabled(cursor);
		SetState(cursor);
		SetCapture(isGame);
		Initialize();
	}

	void Keyboard::KeyDown(uint32 key)
	{
		mKeysPress[key] = true;
		mKeysDown[key] = true;
		mIsKeyDown = true;
	}

	void Keyboard::KeyUp(uint32 key)
	{
		if (mKeysPress[key])
		{
			mKeysPress[key] = false;
			mKeysUp[key] = true;
			mIsKeyUp = true;
		}
	}

	void Keyboard::ResetKeys()
	{
		if (mIsKeyDown)
		{
			ZeroMemory(mKeysDown, 256);
			mIsKeyDown = false;
		}

		if (mIsKeyUp)
		{
			ZeroMemory(mKeysUp, 256);
			mIsKeyUp = false;
		}

	}

	void Keyboard::ResetKeysPress()
	{
		for (uint16 i = 0; i < 256; i++)
		{
			KeyUp(i);
		}
	}

	bool Keyboard::IsKeyPress(uint32 key)
	{
		return mKeysPress[key];
	}

	bool Keyboard::IsKeyDown(uint32 key)
	{
		return mKeysDown[key];
	}

	bool Keyboard::IsKeyUp(uint32 key)
	{
		return mKeysUp[key];
	}



	void Mouse::Initialize()
	{
		SetZeroAxis();

		FOG_TRACE(DirectInput8Create(GetModuleHandle(0), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&this->mDirectInput, 0));

		FOG_TRACE(mDirectInput->CreateDevice(GUID_SysMouse, &mMouseDevice, nullptr));
		FOG_TRACE(mMouseDevice->SetDataFormat(&c_dfDIMouse2));
		//FOG_TRACE(mMouseDevice->SetCooperativeLevel(Edit::GetHWND(), DISCL_FOREGROUND | DISCL_EXCLUSIVE)); //off cursor
	}

	void Mouse::Update()
	{
		if (Edit::IsAppPaused())
		{
			SetZeroInput();
		}
		else
		{
			mMouseDevice->Acquire();
			mMouseDevice->GetDeviceState(sizeof(DIMOUSESTATE2), &mMouseState);
		}

		SetZeroAxis();

		mAxis[MOUSE_X] = (float)mMouseState.lX;
		mAxis[MOUSE_Y] = (float)mMouseState.lY;

		/*if (mMouseState.lZ > 0)
			mAxis[GameInput::MouseScroll] = 1.0f;
		else if (s_MouseState.lZ < 0)
			s_Analogs[GameInput::MouseScroll] = -1.0f;*/
	}

	float Mouse::GetMouseAxis(MouseAxis ma)
	{
		if (mCapture)
			return mAxis[ma];
		else
			return 0.0f;
	}

	void Mouse::SetZeroAxis()
	{
		ZeroMemory(mAxis, sizeof(mAxis));
	}

	void Mouse::SetZeroInput()
	{
		ZeroMemory(&mMouseState, sizeof(mMouseState));
	}
}



bool Input::IsKeyPress(uint32 key)
{
	return mKeyboard->IsKeyPress(key);
}

bool Input::IsKeyDown(uint32 key)
{
	return mKeyboard->IsKeyDown(key);
}

bool Input::IsKeyUp(uint32 key)
{
	return mKeyboard->IsKeyUp(key);
}

void Input::SetKeyboard(Module::Keyboard* keyboard)
{
	mKeyboard = keyboard;
}

float Input::GetMouseAxis(MouseAxis ma)
{
	return mMouse->GetMouseAxis(ma);
}

void Input::SetMouse(Module::Mouse* mouse)
{
	mMouse = mouse;
}