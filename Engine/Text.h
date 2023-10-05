#pragma once

#include "Core.h"

#include "Control.h"
#include "CustomString.h"
#include "Color.h"

class GUI;
class Application;
class Button;

class FOG_API Text : public Control
{
	friend class GUI;
	friend class Application;
	friend class Button;

public:
	TypeControl GetType() override { return TypeControl::Text; }

	Text();
	~Text();
	Text(Text& t);

private:
	static void Setup();
	void Draw();
	static void Shotdown();

public:
	String text;
	Color color;

private:
	struct Data;
	static Data* mData;

	ID2D1SolidColorBrush* mBrush;
};

