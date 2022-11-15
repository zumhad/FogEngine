#pragma once

#include "Core.h"

enum class TypeControl
{
	Control,
	Button,
	Count
};

class FOG_API Control
{
public:
	virtual TypeControl GetType() { return TypeControl::Control; }
};

