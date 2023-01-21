#pragma once

#include "Core.h"

#include <d3d11_4.h>
#include <DirectXMath.h>

class Application;
class GUI;
class Mesh;
class ObjectManager;
class Texture;

class FOG_API Direct3D
{
	friend class Application;
	friend class GUI;
	friend class Mesh;
	friend class ObjectManager;
	friend class Texture;

private:
	static void Setup();
	static void Shotdown();
	static void Initialize();

	static void DrawGame();
	static void DrawEngine();
	static void DrawEditor();
	static void Present();

	static void ResizeScene();
	static void ResizeEditor();
	static void ResizeEngine();
	static void ResizeGame();

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
	static ID3D11DepthStencilView* mDepthStencilView;
	static ID3D11DepthStencilState* mDepthStencilState;
	static ID3D11DepthStencilState* mDepthDisabledStencilState;

	static D3D11_VIEWPORT mGameViewport;
	static D3D11_VIEWPORT mSceneViewport;
	static D3D11_VIEWPORT mEditorViewport;

	static RECT mSceneRect;
};