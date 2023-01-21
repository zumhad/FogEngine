#include "Cursor.h"

#include "Application.h"
#include "CustomString.h"

bool Cursor::mVisible = true;

int Cursor::GetPosition(CursorAxis axis)
{
	POINT p;
	GetCursorPos(&p);
	ScreenToClient(Application::GetHWND(), &p);

	return (axis ? p.y : p.x);
}

void Cursor::SetPosition(int x, int y)
{
	POINT p = { x, y };
	ClientToScreen(Application::GetHWND(), &p);

	SetCursorPos(p.x, p.y);
}

void Cursor::SetVisible(bool var)
{
	if (var)
		while (ShowCursor(var) < 0);
	else
		while (ShowCursor(var) >= 0);

	mVisible = var;
}

bool Cursor::GetVisible()
{
	return mVisible;
}