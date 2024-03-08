#pragma once

#include "Core.h"

template <typename T>
class FOG_API ConstantBuffer;

class FOG_API Model;
class FOG_API VertexShader;
class FOG_API PixelShader;
class FOG_API InputLayout;
class FOG_API PipelineState;
class FOG_API Application;

class FOG_API SkyboxPass
{
	friend class PipelineState;
	friend class Application;

private:
	static void Setup();
	static void Shotdown();
	static void Bind();

private:
	static void UpdateBuffer0();

private:
	static Model* mModel;
	static VertexShader mVertexShader;
	static PixelShader mPixelShader;
	static InputLayout mInputLayout;

	struct Buffer0;
	static ConstantBuffer<Buffer0> mBuffer0;
};

