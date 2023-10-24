#pragma once

#include "Core.h"

#include <d3d11.h>

template <typename T>
class FOG_API ConstantBuffer;

class FOG_API Application;
class FOG_API PipelineState;
class FOG_API Model;

class FOG_API ShadowMap
{
	friend class Application;
	friend class PipelineState;

public:
	static int GetResolution();
	static void SetResolution(int resolution);

private:
	static void Setup();
	static void Shotdown();
	static void UpdateTexture();

	static void Clear();
	static void UpdateBuffer(Model& mesh);
	static ID3D11Buffer* const* GetBuffer();

	static ID3D11ShaderResourceView* const* GetSRV();
	static ID3D11DepthStencilView* GetDSV();

private:
	static ID3D11DepthStencilView* mDepthStencilView;
	static ID3D11ShaderResourceView* mShaderResourceView;

	struct ShadowBuffer;
	static ConstantBuffer<ShadowBuffer> mShadowBuffer;

	static int mResolution;
};