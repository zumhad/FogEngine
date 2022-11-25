#pragma once

#include "Core.h"
#include "Light.h"
#include "CustomFile.h"
#include "PathHelper.h"
#include "Properties.h"
#include "LightHelper.h"

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

	static void InitializeShader();

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
	static ID3D11VertexShader* mVertexShader;
	static ID3D11PixelShader* mPixelShader;
	static ID3D11InputLayout* mVertexLayout;
	static ID3D11Buffer* mPerFrameBuffer;
	static ID3D11Buffer* mPerObjectBuffer;
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

struct PerFrameBuffer
{
	DirectionalLightBuffer directionalLight[MAX_DIRECTIONAL_LIGHT];
	PointLightBuffer pointLight[MAX_POINT_LIGHT];
	DirectX::XMFLOAT3 cameraPosW;
	int directionalCount;
	int pointCount;
	float pad[3];
};


struct PerObjectBuffer
{
	DirectX::XMFLOAT4X4 world;
	DirectX::XMFLOAT4X4 worldInvTranspose;
	DirectX::XMFLOAT4X4 worldViewProj;
	Material material;
};