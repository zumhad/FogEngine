#pragma once

#include "Core.h"
#include "MathHelper.h"

#include <windows.h>

typedef unsigned char 		uint8;
typedef unsigned short int	uint16;
typedef unsigned int		uint32;
typedef unsigned long long	uint64;

typedef	signed char			int8;
typedef signed short int	int16;
typedef signed int	 		int32;
typedef signed long long	int64;

typedef char				CHAR;
typedef wchar_t				WCHAR;

typedef struct _Rect
{
	int16 left;
	int16 top;
	int16 right;
	int16 bottom;
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

