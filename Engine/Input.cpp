#include "Input.h"

#include "EditHelper.h"
#include "Trace.h"
#include "Devices.h"
#include "Properties.h"


Module::Keyboard* Input::mKeyboard = 0;
Module::Mouse* Input::mMouse = 0;

namespace Module
{
	Keyboard::Keyboard()
	{
		Input::SetKeyboard(this);
	}

	Mouse::Mouse()
	{
		Input::SetMouse(this);

		SetEnabled(Singlton.cursorShow);
		SetState(Singlton.cursorShow);
		SetCapture(Singlton.isGame);
		Initialize();
	}

	void Keyboard::KeyDown(uint32 key)
	{
		mKeysPress[key] = true;
		mKeysDown[key] = true;
		mIsKeyDown = true;
	}

	void Mouse::KeyDown(int16 mouse)
	{
		mKeysPress[mouse] = true;
		mKeysDown[mouse] = true;
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

	void Mouse::KeyUp(int16 mouse)
	{
		if (mKeysPress[mouse])
		{
			mKeysPress[mouse] = false;
			mKeysUp[mouse] = true;
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

	void Mouse::ResetKeys()
	{
		ZeroMemory(mKeysDown, 2);
		ZeroMemory(mKeysUp, 2);
	}


	void Mouse::ResetKeysPress()
	{
		for (int16 i = 0; i < 2; i++)
			KeyUp(i);
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



	bool Mouse::IsMousePress(uint32 key)
	{
		return mKeysPress[key];
	}

	bool Mouse::IsMouseDown(uint32 key)
	{
		return mKeysDown[key];
	}

	bool Mouse::IsMouseUp(uint32 key)
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
		mMouseDevice->Acquire();
		mMouseDevice->GetDeviceState(sizeof(DIMOUSESTATE2), &mMouseState);

		SetZeroAxis();

		if (Edit::IsAppPaused())
		{
			mAxis[MOUSE_X] = 0;
			mAxis[MOUSE_Y] = 0;
		}
		else
		{
			mAxis[MOUSE_X] = (float)mMouseState.lX;
			mAxis[MOUSE_Y] = (float)mMouseState.lY;
		}


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

bool Input::IsMouseDown(uint32 mouse)
{
	return mMouse->IsMouseDown(mouse);
}

bool Input::IsMouseUp(uint32 mouse)
{
	return mMouse->IsMouseUp(mouse);
}

bool Input::IsMousePress(uint32 mouse)
{
	return mMouse->IsMousePress(mouse);
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

short Input::GetCursorX()
{ 
	return mMouse->GetX(); 
}

short Input::GetCursorY()
{
	return mMouse->GetY();
}