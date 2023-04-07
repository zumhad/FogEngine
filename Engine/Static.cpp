#include "Static.h"

#include "GUI.h"
#include "Application.h"

Static::Static()
{
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
	alignm = obj.alignm;

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
	if (mParent)
	{
		if (alignm.horizontal == HorizontalAlignm::ALIGNM_LEFT)
		{
			mRect.left = (float)mParent->mRect.left + x;
			mRect.right = (float)mParent->mRect.left + x + width;
		}
		if (alignm.horizontal == HorizontalAlignm::ALIGNM_RIGHT)
		{
			mRect.left = (float)mParent->mRect.right - width + x;
			mRect.right = (float)mParent->mRect.right + x;
		}
		if (alignm.horizontal == HorizontalAlignm::ALIGNM_CENTER_H)
		{
			mRect.left = (float)mParent->mRect.left + (mParent->mRect.right - mParent->mRect.left - width) * 0.5f + x;
			mRect.right = (float)mParent->mRect.left + (mParent->mRect.right - mParent->mRect.left + width) * 0.5f + x;
		}

		if (alignm.vertical == VerticalAlignm::ALIGNM_TOP)
		{
			mRect.top = (float)mParent->mRect.top + y;
			mRect.bottom = (float)mParent->mRect.top + y + height;
		}
		if (alignm.vertical == VerticalAlignm::ALIGNM_BOTTOM)
		{
			mRect.top = (float)mParent->mRect.bottom - height + y;
			mRect.bottom = (float)mParent->mRect.bottom + y;
		}
		if (alignm.vertical == VerticalAlignm::ALIGNM_CENTER_V)
		{
			mRect.top = (float)mParent->mRect.top + (mParent->mRect.bottom - mParent->mRect.top - height) * 0.5f + y;
			mRect.bottom = (float)mParent->mRect.top + (mParent->mRect.bottom - mParent->mRect.top + height) * 0.5f + y;
		}
	}
	else
	{
		if (alignm.horizontal == HorizontalAlignm::ALIGNM_LEFT)
		{
			mRect.left = (float)x;
			mRect.right = (float)x + width;
		}
		if (alignm.horizontal == HorizontalAlignm::ALIGNM_RIGHT)
		{
			mRect.left = (float)Application::GetEditorWidth() - width + x;
			mRect.right = (float)Application::GetEditorWidth() + x;
		}
		if (alignm.horizontal == HorizontalAlignm::ALIGNM_CENTER_H)
		{
			mRect.left = (float)(Application::GetEditorWidth() - width) * 0.5f + x;
			mRect.right = (float)(Application::GetEditorWidth() + width) * 0.5f + x;
		}

		if (alignm.vertical == VerticalAlignm::ALIGNM_TOP)
		{
			mRect.top = (float)y;
			mRect.bottom = (float)y + height;
		}
		if (alignm.vertical == VerticalAlignm::ALIGNM_BOTTOM)
		{
			mRect.top = (float)Application::GetEditorHeight() - height + y;
			mRect.bottom = (float)Application::GetEditorHeight() + y;
		}
		if (alignm.vertical == VerticalAlignm::ALIGNM_CENTER_V)
		{
			mRect.top = (float)(Application::GetEditorHeight() - height) * 0.5f + y;
			mRect.bottom = (float)(Application::GetEditorHeight() + height) * 0.5f + y;
		}
	}

	static D2D1_COLOR_F c;
	c.r = color.r;
	c.g = color.g;
	c.b = color.b;
	c.a = color.a;

	mBrush->SetColor(c);
	GUI::RenderTarget()->FillRectangle(mRect, mBrush);
}