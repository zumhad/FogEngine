#pragma once

#include "Core.h"

#include "Control.h"
#include "Color.h"

class FOG_API GUI;

class FOG_API Static : public Control
{
	friend class GUI;

private:
	void Draw();
};

