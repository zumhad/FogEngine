#include "Cursor.h"

#include "Application.h"
#include "CustomString.h"
#include "Vector4.h"
#include "Matrix.h"
#include "Camera.h"

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

Vector3 Cursor::GetDirection()
{
    float xPos = GetPosition(CURSOR_X);
    float yPos = GetPosition(CURSOR_Y);
    float width = Application::GetSceneWidth();
    float height = Application::GetSceneHeight();
    float xPosW = Application::GetSceneX();
    float yPosW = Application::GetSceneY();


    float x = (2.0f * (xPos- xPosW)) / width - 1.0f;
    float y = 1.0f - (2.0f * (yPos - yPosW)) / height;
    //float y = (2.0f * yPos) / height - 1.0f;

    float z = 1.0f;
    Vector3 ray_nds = Vector3(x, y, z);
    Vector4 ray_clip = Vector4(ray_nds.x, ray_nds.y, -1.0f, 1.0f);

    Vector4 ray_eye = Matrix::Invert(Camera::GetProjMatrix()) * ray_clip;

    ray_eye = Vector4(ray_eye.x, ray_eye.y, 1.0f, 0.0f);

    Vector4 inv_ray_wor = (Matrix::Invert(Camera::GetViewMatrix()) * ray_eye);
    Vector3 ray_wor = Vector3(inv_ray_wor.x, inv_ray_wor.y, inv_ray_wor.z);
    ray_wor = Vector3::Normalize(ray_wor);
    return ray_wor;

    
}