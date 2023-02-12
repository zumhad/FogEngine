#pragma once

#include "Core.h"

enum class TypeControl
{
	Control,
	Button,
	Static
};

enum HorizontalAlignm
{
	ALIGNM_LEFT,
	ALIGNM_RIGHT
};

enum VerticalAlignm
{
	ALIGNM_TOP,
	ALIGNM_BOTTOM
};

class GUI;

class FOG_API Control
{
	friend class GUI;

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
};

