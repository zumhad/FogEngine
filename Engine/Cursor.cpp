#include "Cursor.h"

#include "Application.h"
#include "CustomString.h"
#include "Vector4.h"
#include "Matrix.h"
#include "Camera.h"
#include "Vector3.h"

using namespace DirectX;

bool Cursor::mVisible = true;
int Cursor::mX = 0;
int Cursor::mY = 0;

void Cursor::Update()
{
	if (Application::IsPaused()) return;

	POINT p;
	GetCursorPos(&p);
	ScreenToClient(Application::GetHWND(), &p);

	mX = p.x;
	mY = p.y;
}

int Cursor::GetPosition(CursorAxis axis)
{
	if (axis == CURSOR_X)
		return mX;
	else
		return mY;
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

Vector3 Cursor::GetDirection()
{
    float xCursor = (float)GetPosition(CURSOR_X);
    float yCursor = (float)GetPosition(CURSOR_Y);

	float widthWindow;
	float heightWindow;
	float xWindow;
	float yWindow;

	if (Application::IsGame())
	{
		widthWindow = (float)Application::GetGameWidth();
		heightWindow = (float)Application::GetGameHeight();
		xWindow = 0.0f;
		yWindow = 0.0f;
	}
	else
	{
		widthWindow = (float)Application::GetSceneWidth();
		heightWindow = (float)Application::GetSceneHeight();
		xWindow = (float)Application::GetSceneX();
		yWindow = (float)Application::GetSceneY();
	}

	Matrix proj = Camera::GetProjMatrix();
	Matrix view = Camera::GetViewMatrix();
	Matrix world = Matrix();

	Vector3 v1 = Vector3(xCursor, yCursor, 1.0f);
	Vector3 ray1 = XMVector3Unproject(v1, xWindow, yWindow, widthWindow, heightWindow, 0.0f, 1.0f, proj, view, world);

	Vector3 v2 = Vector3(xCursor, yCursor, 0.0f);
	Vector3 ray2 = XMVector3Unproject(v2, xWindow, yWindow, widthWindow, heightWindow, 0.0f, 1.0f, proj, view, world);

    return Vector3::Normalize(ray2 - ray1);
}