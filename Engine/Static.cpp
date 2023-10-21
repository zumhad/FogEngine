#include "Static.h"

#include "GUI.h"
#include "Application.h"
#include "Utility.h"
#include "Direct3D.h"

Static::Static()
{
	color = Color(0, 0, 0);
}

Static::~Static()
{
}

void Static::Draw()
{
	if (mParent)
	{
		D3D11_RECT rect = mParent->mRect;

		if (alignm.horizontal == ALIGNM_LEFT)
		{
			mRect.left = x + rect.left;
			mRect.right = mRect.left + width;
		}
		if (alignm.horizontal == ALIGNM_CENTER_H)
		{
			mRect.left = rect.left + int((rect.right - rect.left - width) * 0.5f);
			mRect.right = mRect.left + width;
		}
		if (alignm.horizontal == ALIGNM_RIGHT)
		{
			mRect.left = x + rect.right - width;
			mRect.right = mRect.left + width;
		}

		if (alignm.vertical == ALIGNM_TOP)
		{
			mRect.top = y + rect.top;
			mRect.bottom = mRect.top + height;
		}
		if (alignm.vertical == ALIGNM_CENTER_V)
		{
			mRect.top = rect.top + int((rect.bottom - rect.top - height) * 0.5f);
			mRect.bottom = mRect.top + height;
		}
		if (alignm.vertical == ALIGNM_BOTTOM)
		{
			mRect.top = y + rect.bottom - height;
			mRect.bottom = mRect.top + height;
		}
	}
	else
	{
		if (alignm.horizontal == ALIGNM_LEFT)
		{
			mRect.left = x;
			mRect.right = mRect.left + width;
		}
		if (alignm.horizontal == ALIGNM_CENTER_H)
		{
			mRect.left = x + int((Application::GetEditorWidth() - width) * 0.5f);
			mRect.right = mRect.left + width;
		}
		if (alignm.horizontal == ALIGNM_RIGHT)
		{
			mRect.left = x + Application::GetEditorWidth() - width;
			mRect.right = mRect.left + width;
		}

		if (alignm.vertical == ALIGNM_TOP)
		{
			mRect.top = y;
			mRect.bottom = mRect.top + height;
		}
		if (alignm.vertical == ALIGNM_CENTER_V)
		{
			mRect.top = y + int((Application::GetEditorHeight() - height) * 0.5f);
			mRect.bottom = mRect.top + height;
		}
		if (alignm.vertical == ALIGNM_BOTTOM)
		{
			mRect.top = y + Application::GetEditorHeight() - height;
			mRect.bottom = mRect.top + height;
		}
	}

	Direct3D::DeviceContext()->ClearView(*Direct3D::GetRTV(), color, &mRect, 1);
}