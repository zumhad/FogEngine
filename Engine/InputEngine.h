#pragma once

#include "Core.h"

#include "Input.h"

class InputEngine : public Input
{
public:
	static void Setup();
	static void Update();
};