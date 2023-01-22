#pragma once

#include "Core.h"

#include "Control.h"
#include "Color.h"

#include <d2d1_3.h>

class GUI;

class FOG_API Static : public Control
{
	friend class GUI;

public:
	TypeControl GetType() { return TypeControl::Static; }

	Static();
	Static(Static& obj);
	~Static();

private:
	void Draw();

public:
	Color color;
	int x, y;
	int width, height;

private:
	ID2D1SolidColorBrush* mBrush;
};

