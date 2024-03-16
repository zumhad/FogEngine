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
class FOG_API DirectionLight;
class FOG_API PointLight;

class FOG_API PrePass
{
	friend class PipelineState;
	friend class Application;
	friend class Picking;
	friend class DirectionLight;
	friend class PointLight;

private:
	static void Setup();
	static void Bind();
	static void Shotdown();

private:
	static ID3D11DepthStencilView* const GetDepthDSV();

	static ID3D11RenderTargetView* const* GetColorRTV();
	static ID3D11RenderTargetView* const* GetNormalLightingRTV();
	static ID3D11RenderTargetView* const* GetIDRTV();
	static ID3D11RenderTargetView* const* GetPositionMaterialRTV();

	static ID3D11ShaderResourceView* const* GetDepthSRV();
	static ID3D11ShaderResourceView* const* GetColorSRV();
	static ID3D11ShaderResourceView* const* GetNormalLightingSRV();
	static ID3D11ShaderResourceView* const* GetIDSRV();
	static ID3D11ShaderResourceView* const* GetPositionMaterialSRV();

private:
	static void UpdateViewport();
	static void Clear();
	static void UpdateBuffer0();
	static void UpdateBuffer1(Model* model);
	static void UpdateBuffer2(Model* model);

private:
	static ID3D11RenderTargetView* mColorRTV;
	static ID3D11ShaderResourceView* mColorSRV;

	static ID3D11RenderTargetView* mNormalLightingRTV;
	static ID3D11ShaderResourceView* mNormalLightingSRV;

	static ID3D11RenderTargetView* mPositionMaterialRTV;
	static ID3D11ShaderResourceView* mPositionMaterialSRV;

	static ID3D11RenderTargetView* mIDRTV;
	static ID3D11ShaderResourceView* mIDSRV;

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

