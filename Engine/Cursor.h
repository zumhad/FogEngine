#pragma once

#include "Core.h"

enum CursorAxis
{
	CURSOR_X,
	CURSOR_Y
};

class FOG_API Application;
class FOG_API Vector3;

class FOG_API Cursor
{
	friend class Application;

public:
	static int GetPosition(CursorAxis axis);
	static void SetPosition(int x, int y);
	static void SetVisible(bool var);
	static bool GetVisible();
	static Vector3 GetDirection();

private:
	static void Update();

private:
	static bool mVisible;
	static int mX;
	static int mY;
};

