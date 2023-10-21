#pragma once

#include "Core.h"

#include "CustomString.h"

enum class ImageType
{
	RGBA,
	A
};

class FOG_API Image
{
public:
	static unsigned char* Load(String name, int& width, int& height, ImageType type = ImageType::RGBA);
};

