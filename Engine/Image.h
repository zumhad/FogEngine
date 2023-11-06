#pragma once

#include "Core.h"

enum class ImageType
{
	RGBA,
	A,
	HDR
};

class FOG_API String;

class FOG_API Image
{
public:
	static unsigned char* Load(String name, int& width, int& height, ImageType type = ImageType::RGBA);
};

