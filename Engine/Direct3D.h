#pragma once

#include "Core.h"

#include <d3d11_4.h>
#include <DirectXMath.h>

class Application;
class GUI;
class Mesh;
class ObjectManager;
class Texture;
class DepthMap;
class TextureMap;
class SelectMap;
class Picking;
class PassMap;

template <typename T>
class ConstantBuffer;

class FOG_API Direct3D
{
	friend class Application;
	friend class GUI;
	friend class Mesh;
	friend class ObjectManager;
	friend class Texture;
	friend class DepthMap;
	friend class TextureMap;
	friend class SelectMap;
	friend class Picking;
	friend class PassMap;

	template <typename T>
	friend class ConstantBuffer;

private:
	static void Setup();
	static void Shotdown();
	static void Initialize();

	static void DrawGame();
	static void DrawEngine();
	static void DrawEditor();
	static void Present();

	static void Resize();
	
	static ID3D11RenderTargetView* const* RTV();
	static ID3D11ShaderResourceView* const* NullSRV();

	static IDXGISwapChain* SwapChain() { return mSwapChain; }
	static ID3D11Device* Device() { return mDevice; }
	static ID3D11DeviceContext* DeviceContext() { return mDeviceContext; }

private:
	static ID3D11Device* mDevice;
	static ID3D11DeviceContext* mDeviceContext;
	static ID3D11DeviceContext1* mDeviceContext1;
	static IDXGISwapChain* mSwapChain;
	static ID3D11RenderTargetView* mRenderTargetView;
	static ID3D11RasterizerState* mRasterizerState;
};