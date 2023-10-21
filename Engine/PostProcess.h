#pragma once

#include "Core.h"

#include <d3d11.h>

template <typename T>
class FOG_API ConstantBuffer;

class FOG_API Application;
class FOG_API PipelineState;

class FOG_API PostProcess
{
	friend class Application;
	friend class PipelineState;

private:
	static void Setup();
	static void Shotdown();

	static void UpdateBuffer();
	static ID3D11Buffer* const* GetBuffer();

private:
	struct PostProcessBuffer;
	static ConstantBuffer<PostProcessBuffer> mPostProcessBuffer;
};