#pragma once

#include "Core.h"

class FOG_API String;

class FOG_API Image
{
public:
	enum class Type
	{
		RGBA,
		A,
		HDR
	};

	static unsigned char* Load(const String& name, int& width, int& height, Type type = Type::RGBA);
};

