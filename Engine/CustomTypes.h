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
	Color(int r, int g, int b) { Set(r, g, b); }
	Color(COLORREF c) { mColor = c; }
	Color(int c) { Set(c, c, c); }

	void Set(int r, int g, int b)
	{
		mColor = RGB(Math::Clamp(float(r), 0.0f, 255.0f), Math::Clamp(float(g), 0.0f, 255.0f), Math::Clamp(float(b), 0.0f, 255.0f));
	}

	operator COLORREF() { return mColor; }

private:
	COLORREF mColor = RGB(0, 0, 0);
};

/*class FOG_API ColorRGBA
{
public:
	ColorRGBA() {}
	ColorRGBA(int r, int g, int b, int a) { Set(r, g, b, a); }
	ColorRGBA(COLORREF c) { mColor = c; }
	ColorRGBA(int c) { Set(c, c, c, c); }

	void Set(int r, int g, int b, int a)
	{
		mColor = RGBA(Math::Clamp((float)r, 0.0f, 255.0f), Math::Clamp((float)g, 0.0f, 255.0f), Math::Clamp((float)b, 0.0f, 255.0f), Math::Clamp((float)a, 0.0f, 255.0f));
	}

	operator COLORREF() { return mColor; }

private:
	COLORREF mColor = RGBA(0, 0, 0, 0);
};*/

