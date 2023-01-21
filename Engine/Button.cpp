#include "Button.h"

#include "GUI.h"
#include "MathHelper.h"

using namespace DirectX;

Button::Button()
{
	x = 0; y = 0;
	width = 0; height = 0;
	action = 0;
	color = Color(0.0f, 0.0f, 0.0f);
	focus = Color(0.0f, 0.0f, 0.0f);

	mBrush = 0;
}

Button::Button(Button& obj)
{
	x = obj.x;
	y = obj.y;
	width = obj.width;
	height = obj.height;
	action = obj.action;
	color = obj.color;
	focus = obj.focus;

	D2D1_COLOR_F c;
	c.r = color.r;
	c.g = color.g;
	c.b = color.b;
	c.a = color.a;

	FOG_TRACE(GUI::RenderTarget()->CreateSolidColorBrush(c, &mBrush));
}

Button::~Button()
{
	SAFE_RELEASE(mBrush);
}

void Button::Draw()
{
	static D2D1_RECT_F rect;
	rect.left = (float)x;
	rect.top = (float)y;
	rect.right = (float)x + width;
	rect.bottom = (float)y + height;

	static D2D1_COLOR_F c;
	if (mFocus)
	{
		c.r = focus.r;
		c.g = focus.g;
		c.b = focus.b;
		c.a = focus.a;
	}
	else
	{
		c.r = color.r;
		c.g = color.g;
		c.b = color.b;
		c.a = color.a;
	}

	mBrush->SetColor(c);
	GUI::RenderTarget()->FillRectangle(rect, mBrush);
}

void Button::Action()
{
	if (action) action();
}