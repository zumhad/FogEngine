#pragma once

#include "Core.h"

#include <d3d11.h>

template <typename T>
class FOG_API ConstantBuffer;

template <typename T>
class FOG_API Array;

class FOG_API PipelineState;
class FOG_API Application;
class FOG_API PixelShader;
class FOG_API VertexShader;
class FOG_API InputLayout;
class FOG_API Model;
class FOG_API Picking;

class FOG_API PrePass
{
	friend class PipelineState;
	friend class Application;
	friend class Picking;

private:
	static void Setup();
	static void Bind();
	static void Shotdown();

private:
	static ID3D11DepthStencilView* const GetDepthDSV();
	static ID3D11RenderTargetView* const* GetColorRTV();
	static ID3D11RenderTargetView* const* GetNormalLightRTV();
	static ID3D11RenderTargetView* const* GetRangeMaterialRTV();
	static ID3D11RenderTargetView* const* GetPositionIDRTV();

	static ID3D11ShaderResourceView* const* GetDepthSRV();
	static ID3D11ShaderResourceView* const* GetColorSRV();
	static ID3D11ShaderResourceView* const* GetNormalLightSRV();
	static ID3D11ShaderResourceView* const* GetRangeMaterialSRV();
	static ID3D11ShaderResourceView* const* GetPositionIDSRV();

private:
	static void Clear();
	static void UpdateBuffer0();
	static void UpdateBuffer1(Model* model);
	static void UpdateBuffer2(Model* model);

private:
	static ID3D11RenderTargetView* mColorRTV;
	static ID3D11ShaderResourceView* mColorSRV;

	static ID3D11RenderTargetView* mNormalLightRTV;
	static ID3D11ShaderResourceView* mNormalLightSRV;

	static ID3D11RenderTargetView* mPositionIDRTV;
	static ID3D11ShaderResourceView* mPositionIDSRV;

	static ID3D11RenderTargetView* mRangeMaterialRTV;
	static ID3D11ShaderResourceView* mRangeMaterialSRV;

	static ID3D11DepthStencilView* mDepthDSV;
	static ID3D11ShaderResourceView* mDepthSRV;

	static VertexShader mVertexShader;
	static PixelShader mPixelShader;
	static InputLayout mInputLayout;

	struct Buffer0;
	static ConstantBuffer<Buffer0> mBuffer0;

	struct Buffer1;
	static ConstantBuffer<Buffer1> mBuffer1;

	struct Buffer2;
	static ConstantBuffer<Buffer2> mBuffer2;

	static Array<ID3D11RenderTargetView*> mRTV;
};

