#include "Cursor.h"

#include "Application.h"
#include "CustomString.h"
#include "Vector4.h"
#include "Matrix.h"
#include "Camera.h"

#include <DirectXMath.h>

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
	if (axis == CursorAxis::CURSOR_X)
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

	XMMATRIX proj = Camera::GetProjMatrix();
	XMMATRIX view = Camera::GetViewMatrix();
	XMMATRIX world = XMMatrixIdentity();

	XMVECTOR v1 = XMVectorSet(xCursor, yCursor, 1.0f, 0.0f);
    XMVECTOR ray1 = XMVector3Unproject(v1, xWindow, yWindow, widthWindow, heightWindow, 0.0f, 1.0f, proj, view, world);

	XMVECTOR v2 = XMVectorSet(xCursor, yCursor, 0.0f, 0.0f);
	XMVECTOR ray2 = XMVector3Unproject(v2, xWindow, yWindow, widthWindow, heightWindow, 0.0f, 1.0f, proj, view, world);

    return Vector3::Normalize(ray2 - ray1);
}