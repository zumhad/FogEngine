#include "Button.h"

#include "GUI.h"
#include "MathHelper.h"
#include "Application.h"

using namespace DirectX;

Button::Button()
{
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
	alignm = obj.alignm;

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
	if (alignm.horizontal == HorizontalAlignm::ALIGNM_LEFT)
	{
		rect.left = (float)x;
		rect.right = (float)x + width;
	}
	if (alignm.horizontal == HorizontalAlignm::ALIGNM_RIGHT)
	{
		rect.left = (float)Application::GetEditorWidth() - x - width;
		rect.right = (float)Application::GetEditorWidth() - x;
	}
	if (alignm.vertical == VerticalAlignm::ALIGNM_TOP)
	{
		rect.top = (float)y;
		rect.bottom = (float)y + height;
	}
	if (alignm.vertical == VerticalAlignm::ALIGNM_BOTTOM)
	{
		rect.top = (float)Application::GetEditorHeight() - y - height;
		rect.bottom = (float)Application::GetEditorHeight() - y;
	}

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

	//IDWriteTextLayout* lay;

	//GUI::RenderTarget()->DrawTextLayout()
}

void Button::Action()
{
	if (action) action();
}