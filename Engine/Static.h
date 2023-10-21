#pragma once

#include "Core.h"

#include "Control.h"
#include "Color.h"

class GUI;

class FOG_API Static : public Control
{
	friend class GUI;

public:
	Static();
	~Static();

private:
	void Draw();

public:
	Color color;
};

