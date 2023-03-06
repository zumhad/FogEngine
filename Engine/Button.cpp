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
	GUI::RenderTarget()->FillRectangle(mRect, mBrush);
}

void Button::Action()
{
	if (action) action();
}