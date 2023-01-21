#pragma once

#include "Core.h"

#include "Direct3D.h"
#include "Trace.h"
#include "Control.h"
#include "Color.h"
#include "Matrix.h"

#include <d2d1_3.h>

class GUI;

class FOG_API Button : public Control
{
	friend class GUI;

public:
	TypeControl GetType() override { return TypeControl::Button; }

	Button();
	Button(Button& obj);
	~Button();

private:
	void Action();
	void Draw();

public:
	Color color;
	Color focus;
	int x, y;
	int width, height;
	void (*action)();

private:
	ID2D1SolidColorBrush* mBrush;
};

