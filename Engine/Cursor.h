#pragma once

#include "Core.h"

enum CursorAxis
{
	CURSOR_X,
	CURSOR_Y
};

class FOG_API Cursor
{
public:
	static int GetPosition(CursorAxis axis);
	static void SetPosition(int x, int y);
	static void SetVisible(bool var);
	static bool GetVisible();

private:
	static bool mVisible;
};

