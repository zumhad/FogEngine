#pragma once

#include "Core.h"

#include <d3d11.h>

class FOG_API Font
{
public:
	static void Setup();
	static void Shotdown();
	static void Bind();

private:
	static ID3D11ShaderResourceView* mTexture;
};

