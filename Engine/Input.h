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

	static bool IsKeyDown(uint32 key);
	static bool IsKeyUp(uint32 key);
	static bool IsKeyPress(uint32 key);

	static bool IsMouseDown(uint32 mouse);
	static bool IsMouseUp(uint32 mouse);
	static bool IsMousePress(uint32 mouse);

	static short GetCursorX();
	static short GetCursorY();

	static float GetMouseAxis(MouseAxis ma);

private:
	static Module::Keyboard* mKeyboard;
	static Module::Mouse* mMouse;
};

