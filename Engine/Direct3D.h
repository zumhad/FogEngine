#pragma once

#include "Core.h"
#include "MathHelper.h"
#include "Light.h"
#include "CustomFile.h"
#include "PathHelper.h"
#include "Properties.h"
#include "LightHelper.h"

#include <d3d11_4.h>
#include <directxmath.h>

class FOG_API Direct3D
{
public:
	static void Setup();
	static void Shotdown();

	static void DrawGame();
	static void Present();
	static void DrawScene();
	static void DrawEngine();
	static void DrawEditor();

	static void ResizeScene();
	static void ResizeEditor();
	static void ResizeEngine();
	static void ResizeGame();

	static void InitializeShader();

	static ID3D11Device* Device() { return mDevice; }
	static ID3D11DeviceContext* DeviceContext() { return mDeviceContext; }

private:
	static void Initialize();
	static void RenderGame();

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
	static ID3D11Buffer* mVertexBuffer;
	static ID3D11Buffer* mIndexBuffer;
	static ID3D11Buffer* mPerFrameBuffer;
	static ID3D11Buffer* mPerObjectBuffer;
	static ID3D11RasterizerState* mRasterizerState;
	static ID3D11DepthStencilView* mDepthStencilView;

	static D3D11_VIEWPORT mGameViewport;
	static D3D11_VIEWPORT mSceneViewport;
	static D3D11_VIEWPORT mEditorViewport;

	static RECT mSceneRect;
	static float mEditorColor[4];
	static float mSceneColor[4];
	static float mGameColor[4];
};


class ShaderInclude : public ID3DInclude
{
	HRESULT Open(D3D_INCLUDE_TYPE, LPCSTR pFileName, LPCVOID, LPCVOID* ppData, UINT* pBytes) override
	{
		String includePath;
		PathHelper::GetAssetsPath(includePath);
		includePath += String(pFileName);

		if (File::Exists(includePath) == false)
			return E_FAIL;

		File file(includePath, FileOpenMode::Read);
		*pBytes = UINT(file.Size());
		UINT* data = reinterpret_cast<UINT*>(std::malloc(*pBytes));
		file.Read(data, *pBytes);
		*ppData = data;

		return S_OK;
	}

	HRESULT Close(LPCVOID pData) override
	{
		std::free(const_cast<void*>(pData));
		return S_OK;
	}
};

struct PerFrameBuffer
{
	DirectionalLightBuffer directionalLight[MAX_DIRECTIONAL_LIGHT];
	PointLightBuffer pointLight[MAX_POINT_LIGHT];
	DirectX::XMFLOAT3 cameraPosW;
	int directionalCount;
	int pointCount;
	float pad1;
	float pad2;
	float pad3;
};


struct PerObjectBuffer
{
	DirectX::XMMATRIX world;
	DirectX::XMMATRIX worldInvTranspose;
	DirectX::XMMATRIX worldViewProj;
	Material material;
};