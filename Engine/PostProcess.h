#pragma once

#pragma warning(push)
#pragma warning(disable: 4251)

#include "Core.h"

#include "ConstantBuffer.h"

class Application;
class PipelineState;

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
	struct PostProcessBuffer
	{
		float invWidth;
		float invHeight; float pad[2];
	};
	static ConstantBuffer<PostProcessBuffer> mPostProcessBuffer;
};

#pragma warning(pop)