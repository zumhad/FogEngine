#pragma once

#include "Core.h"
#include "DeviceInput.h"

namespace Module
{
	class Keyboard;
	class Mouse;
}


class FOG_API Input
{
public:
	static void SetKeyboard(Module::Keyboard* keyboard);
	static void SetMouse(Module::Mouse* keyboard);

	static bool IsKeyDown(short key);
	static bool IsKeyUp(short key);
	static bool IsKeyPress(short key);

	static bool IsMouseDown(short mouse);
	static bool IsMouseUp(short mouse);
	static bool IsMousePress(short mouse);

	static short GetCursorX();
	static short GetCursorY();

	static float GetMouseAxis(MouseAxis ma);

private:
	static Module::Keyboard* mKeyboard;
	static Module::Mouse* mMouse;
};

