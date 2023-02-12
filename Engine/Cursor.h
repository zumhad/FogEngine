#pragma once

#include "Core.h"

#include "Vector3.h"

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
	static Vector3 GetDirection();

private:
	static bool mVisible;
};

