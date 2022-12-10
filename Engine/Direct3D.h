#pragma once

#include "Core.h"

#include <d3d11_4.h>
#include <DirectXMath.h>

class FOG_API Direct3D
{
public:
	static void Setup();
	static void Shotdown();

	static void DrawGame();
	static void DrawEngine();
	static void DrawEditor();
	static void Present();

	static void ResizeScene();
	static void ResizeEditor();
	static void ResizeEngine();
	static void ResizeGame();

	static void SetZBuffer(bool var);

	static ID3D11Device* Device() { return mDevice; }
	static ID3D11DeviceContext* DeviceContext() { return mDeviceContext; }

private:
	static void Initialize();

private:
	static ID3D11Device* mDevice;
	static ID3D11Device1* mDevice1;
	static ID3D11DeviceContext* mDeviceContext;
	static ID3D11DeviceContext1* mDeviceContext1;
	static IDXGISwapChain* mSwapChain;
	static IDXGISwapChain1* mSwapChain1;
	static ID3D11RenderTargetView* mRenderTargetView;
	static ID3D11RasterizerState* mRasterizerState;
	static ID3D11DepthStencilView* mDepthStencilView;
	static ID3D11DepthStencilState* mDepthStencilState;
	static ID3D11DepthStencilState* mDepthDisabledStencilState;

	static D3D11_VIEWPORT mGameViewport;
	static D3D11_VIEWPORT mSceneViewport;
	static D3D11_VIEWPORT mEditorViewport;

	static RECT mSceneRect;
	static float mEditorColor[4];
	static float mSceneColor[4];
	static float mGameColor[4];
};