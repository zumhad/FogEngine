#pragma once

#include "Core.h"

#include <d3d11.h>

class FOG_API Application;
class FOG_API GUI;

class FOG_API Font
{
	friend class Application;
	friend class GUI;

private:
	static void Setup();
	static void Shotdown();
	
	static ID3D11ShaderResourceView* const* GetSRV();


private:
	static ID3D11ShaderResourceView* mTexture;
};

