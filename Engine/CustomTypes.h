#pragma once

#include "Core.h"
#include "MathHelper.h"

#include <windows.h>

typedef char				CHAR;
typedef wchar_t				WCHAR;

typedef struct _Rect
{
	short left;
	short top;
	short right;
	short bottom;
} Rect;

class FOG_API Color
{
public:
	Color() {}

	Color(short r, short g, short b)
	{
		mColor = RGB(Math::Clamp(r, 0.0f, 255.0f), Math::Clamp(g, 0.0f, 255.0f), Math::Clamp(b, 0.0f, 255.0f));
	}

	Color(COLORREF c) { mColor = c; }

	Color(int c)
	{
		mColor = RGB(Math::Clamp((float)c, 0.0f, 255.0f), Math::Clamp((float)c, 0.0f, 255.0f), Math::Clamp((float)c, 0.0f, 255.0f));
	}

	void Set(short r, short g, short b)
	{
		mColor = RGB(Math::Clamp(r, 0.0f, 255.0f), Math::Clamp(g, 0.0f, 255.0f), Math::Clamp(b, 0.0f, 255.0f));
	}

	operator COLORREF() { return mColor; }

private:
	COLORREF mColor = RGB(0, 0, 0);
};

