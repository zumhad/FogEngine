#pragma once

#include "Core.h"

#include <d3d11.h>

class Texture
{
public:
	Texture();
	~Texture();

private:
	void Bind();

private:
	ID3D11ShaderResourceView* mTexture = 0;
	ID3D11SamplerState* mSamplerState = 0;
};

