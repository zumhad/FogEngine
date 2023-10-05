#pragma once

#include "Core.h"

#include <d3d11_4.h>
#include <DirectXMath.h>

class FOG_API Direct3D
{
public:
	static void Setup();
	static void Shotdown();
	static void Initialize();

	static void DrawGame();
	static void DrawEngine();
	static void DrawEditor();
	static void Present();

	static void Resize();
	
	static ID3D11ShaderResourceView* const* NullSRV();
	static ID3D11RenderTargetView* const* GetRTV();

	static IDXGISwapChain* SwapChain() { return mSwapChain1; }
	static ID3D11Device* Device() { return mDevice; }
	static ID3D11DeviceContext* DeviceContext() { return mDeviceContext; }

private:
	static ID3D11Device* mDevice;
	static ID3D11DeviceContext* mDeviceContext;
	static ID3D11DeviceContext1* mDeviceContext1;
	static IDXGISwapChain1* mSwapChain1;
	static ID3D11RenderTargetView* mRenderTargetView;
};