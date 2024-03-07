#pragma once

#include "Core.h"

#include <d3d11.h>

template <typename T>
class FOG_API ConstantBuffer;

class FOG_API Application;
class FOG_API PipelineState;
class FOG_API DirectionLight;
class FOG_API PointLight;
class FOG_API OutlineMap;
class FOG_API PrePass;

class FOG_API LightMap
{
	friend class Application;
	friend class PipelineState;
	friend class OutlineMap;
	friend class PrePass;

private:
	static void Setup();
	static void Shotdown();

	static void UpdateBuffer(DirectionLight& dir);
	static int UpdateBuffer(PointLight& point);
	static void UpdateBuffer();
	static void Clear();

	static ID3D11ShaderResourceView* const* GetSRV();
	static ID3D11RenderTargetView* const* GetRTV();
	static ID3D11Buffer* const* GetBuffer();

private:
	static ID3D11RenderTargetView* mRenderTargetView;
	static ID3D11ShaderResourceView* mShaderResourceView;

	struct FOG_API LightBuffer;
	static ConstantBuffer<LightBuffer> mLightBuffer;
	static LightBuffer mBuffer;
};