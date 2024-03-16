#include "Direct3D.h"

#include "Definitions.h"
#include "PathHelper.h"
#include "ObjectManager.h"
#include "Application.h"
#include "Camera.h"
#include "CustomFile.h"
#include "GUI.h"
#include "Shader.h"
#include "Mesh.h"
#include "Cursor.h"
#include "PipelineState.h"
#include "Utility.h"

#include <dxgidebug.h>

ID3D11Device* Direct3D::mDevice = 0;
ID3D11DeviceContext* Direct3D::mDeviceContext = 0;
ID3D11DeviceContext1* Direct3D::mDeviceContext1 = 0;
IDXGISwapChain1* Direct3D::mSwapChain1 = 0;
ID3D11RenderTargetView* Direct3D::mRenderTargetView = 0;

void Direct3D::Setup()
{
	Initialize();
}

void Direct3D::Resize()
{
	SAFE_RELEASE(mRenderTargetView);

	UINT width, height;

	if (Application::IsGame())
	{
		width = (UINT)Application::GetGameWidth();
		height = (UINT)Application::GetGameHeight();
	}
	else
	{
		width = (UINT)Application::GetEditorWidth();
		height = (UINT)Application::GetEditorHeight();
	}

	FOG_TRACE(mSwapChain1->ResizeBuffers(0, width, height, DXGI_FORMAT_UNKNOWN, DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING));

	ID3D11Texture2D* texture = 0;
	{
		FOG_TRACE(mSwapChain1->GetBuffer(0, IID_PPV_ARGS(&texture)));
		FOG_TRACE(mDevice->CreateRenderTargetView(texture, 0, &mRenderTargetView));
	}
	SAFE_RELEASE(texture);
}

void Direct3D::Initialize()
{
	UINT flag = 0;

#ifdef _DEBUG
	flag |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
	};

	IDXGIDevice* dxgiDevice = 0;
	FOG_TRACE(D3D11CreateDevice(0, D3D_DRIVER_TYPE_HARDWARE, 0, flag, featureLevels, ARRAY_SIZE(featureLevels), D3D11_SDK_VERSION, &mDevice, 0, &mDeviceContext));
	FOG_TRACE(mDeviceContext->QueryInterface(IID_PPV_ARGS(&mDeviceContext1)));
	FOG_TRACE(mDevice->QueryInterface(IID_PPV_ARGS(&dxgiDevice)));

	IDXGIAdapter* dxgiAdapter = 0;
	FOG_TRACE(dxgiDevice->GetParent(IID_PPV_ARGS(&dxgiAdapter)));
	SAFE_RELEASE(dxgiDevice);

	IDXGIFactory* dxgiFactory = 0;
	dxgiAdapter->GetParent(IID_PPV_ARGS(&dxgiFactory));
	SAFE_RELEASE(dxgiAdapter);

	IDXGIFactory2* dxgiFactory2 = 0;
	dxgiFactory->QueryInterface(IID_PPV_ARGS(&dxgiFactory2));
	SAFE_RELEASE(dxgiFactory);

	DXGI_SWAP_CHAIN_DESC1 desc = {};
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	desc.BufferCount = 2;
	desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.AlphaMode = DXGI_ALPHA_MODE_IGNORE;
	desc.Scaling = DXGI_SCALING_STRETCH;

	FOG_TRACE(dxgiFactory2->CreateSwapChainForHwnd(mDevice, Application::GetHWND(), &desc, 0, 0, &mSwapChain1));

	FOG_TRACE(dxgiFactory2->MakeWindowAssociation(Application::GetHWND(), DXGI_MWA_NO_ALT_ENTER));
	SAFE_RELEASE(dxgiFactory2);

	Resize();
}

void Direct3D::DrawEngine()
{
	DrawEditor();
	DrawGame();
}

void Direct3D::DrawEditor()
{
	GUI::Draw();
}

void Direct3D::DrawGame()
{
	PipelineState::Bind();
}

void Direct3D::Present()
{
	FOG_TRACE(mSwapChain1->Present(0, DXGI_PRESENT_ALLOW_TEARING));
}

void Direct3D::Shotdown()
{
	SAFE_RELEASE(mRenderTargetView);
	SAFE_RELEASE(mSwapChain1);
	SAFE_RELEASE(mDeviceContext);
	SAFE_RELEASE(mDeviceContext1);
	SAFE_RELEASE(mDevice);

#ifdef _DEBUG
	IDXGIDebug1* dxgiDebug = 0;
	FOG_TRACE(DXGIGetDebugInterface1(0, IID_PPV_ARGS(&dxgiDebug)));
	FOG_TRACE(dxgiDebug->ReportLiveObjects(DXGI_DEBUG_ALL, DXGI_DEBUG_RLO_IGNORE_INTERNAL));
	SAFE_RELEASE(dxgiDebug);
#endif
}

ID3D11ShaderResourceView* const* Direct3D::NullSRV()
{
	static ID3D11ShaderResourceView* nullSRV = 0;
	return &nullSRV;
}

ID3D11RenderTargetView* const* Direct3D::GetRTV()
{
	return &mRenderTargetView;
}

ID3D11RenderTargetView* const* Direct3D::NullRTV()
{
	static ID3D11RenderTargetView* nullRTV = 0;
	return &nullRTV;
}

ID3D11UnorderedAccessView* const* Direct3D::NullUAV()
{
	static ID3D11UnorderedAccessView* nullUAV = 0;
	return &nullUAV;
}