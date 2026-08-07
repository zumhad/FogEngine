#pragma once

#include "Core.h"

template <typename T>
class FOG_API ConstantBuffer;

class FOG_API VertexShader;
class FOG_API PixelShader;
class FOG_API InputLayout;
class FOG_API Application;
class FOG_API PipelineState;
class FOG_API Model;

class FOG_API DebugPass
{
	friend class Application;
	friend class PipelineState;

private:
	static void Setup();
	static void Shotdown();
	static void Bind();

private:
	static void Clear();
	static void UpdateBuffer0();
	static void UpdateBuffer1(Model* model);

private:
	static VertexShader mVertexShader;
	static PixelShader mPixelShader;
	static InputLayout mInputLayout;

	struct Buffer0;
	static ConstantBuffer<Buffer0> mBuffer0;

	struct Buffer1;
	static ConstantBuffer<Buffer1> mBuffer1;
};

