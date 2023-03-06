#pragma once

#include "Core.h"

#include <d2d1.h>

enum class TypeControl
{
	Control,
	Button,
	Static,
	Text
};

enum HorizontalAlignm
{
	ALIGNM_LEFT,
	ALIGNM_RIGHT,
	ALIGNM_CENTER_H
};

enum VerticalAlignm
{
	ALIGNM_TOP,
	ALIGNM_BOTTOM,
	ALIGNM_CENTER_V
};

class GUI;
class Text;
class Static;
class Button;

class FOG_API Control
{
	friend class GUI;
	friend class Text;
	friend class Static;
	friend class Button;

public:
	virtual TypeControl GetType() { return TypeControl::Control; }

	Control();

private:
	void SetFocus(bool focus) { mFocus = focus; }

public:
	struct
	{
		HorizontalAlignm horizontal;
		VerticalAlignm vertical;
	} alignm;

	int x, y;
	int width, height;

protected:
	bool mFocus;
	Control* mChild;
	Control* mParent;
	D2D1_RECT_F mRect;
};

