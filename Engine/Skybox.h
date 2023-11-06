#pragma once

#include "Core.h"

template <typename T>
class FOG_API ConstantBuffer;

class FOG_API Model;
class FOG_API VertexShader;
class FOG_API PixelShader;
class FOG_API InputLayout;

class FOG_API Skybox
{
public:
	static void Setup();
	static void Shotdown();

	static void Bind();

private:
	static Model* mModel;
	static VertexShader mVertexShader;
	static PixelShader mPixelShader;
	static InputLayout mInputLayout;

	struct SkyboxBuffer;
	static ConstantBuffer<SkyboxBuffer> mSkyboxBuffer;
};

