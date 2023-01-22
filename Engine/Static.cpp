#include "Static.h"

#include "Trace.h"
#include "GUI.h"

Static::Static()
{
	x = 0; y = 0;
	width = 0; height = 0;
	color = Color(0, 0, 0);

	mBrush = 0;
}

Static::Static(Static& obj)
{
	x = obj.x;
	y = obj.y;
	width = obj.width;
	height = obj.height;
	color = obj.color;

	D2D1_COLOR_F c;
	c.r = color.r;
	c.g = color.g;
	c.b = color.b;
	c.a = color.a;

	FOG_TRACE(GUI::RenderTarget()->CreateSolidColorBrush(c, &mBrush));
}

Static::~Static()
{
	SAFE_RELEASE(mBrush);
}

void Static::Draw()
{
	static D2D1_RECT_F rect;
	rect.left = (float)x;
	rect.top = (float)y;
	rect.right = (float)x + width;
	rect.bottom = (float)y + height;

	static D2D1_COLOR_F c;
	c.r = color.r;
	c.g = color.g;
	c.b = color.b;
	c.a = color.a;

	mBrush->SetColor(c);
	GUI::RenderTarget()->FillRectangle(rect, mBrush);
}