#pragma once

#include "Core.h"

#include "CustomString.h"

#include <d3d11.h>

class FOG_API Texture
{
public:
	Texture();

	void Create(String name);
	void Bind();
	void Release();

private:
	ID3D11ShaderResourceView* mTexture = 0;
};

