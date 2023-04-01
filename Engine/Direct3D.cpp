#pragma warning(disable : 6387)

#include "Direct3D.h"

#include "Trace.h"
#include "Definitions.h"
#include "PathHelper.h"
#include "ObjectManager.h"
#include "Application.h"
#include "Camera.h"
#include "CustomFile.h"
#include "GUI.h"
#include "Shader.h"
#include "Mesh.h"
#include "FrustumCulling.h"
#include "Cursor.h"
#include "DepthMap.h"
#include "SelectMap.h"

#include <d3dcompiler.h>
#include <directxcolors.h>
#include <dxgidebug.h>
#include <d2d1_3.h>
#include <dwrite.h>

using namespace DirectX;

ID3D11Device* Direct3D::mDevice = 0;
ID3D11DeviceContext* Direct3D::mDeviceContext = 0;
ID3D11DeviceContext1* Direct3D::mDeviceContext1 = 0;
IDXGISwapChain* Direct3D::mSwapChain = 0;
ID3D11RenderTargetView* Direct3D::mRenderTargetView = 0;
ID3D11RasterizerState* Direct3D::mRasterizerState = 0;

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

	FOG_TRACE(mSwapChain->ResizeBuffers(0, width, height, DXGI_FORMAT_UNKNOWN, DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING));

	ID3D11Texture2D* backBuffer = 0;
	FOG_TRACE(mSwapChain->GetBuffer(0, IID_PPV_ARGS(&backBuffer)));

	FOG_TRACE(mDevice->CreateRenderTargetView(backBuffer, 0, &mRenderTargetView));
	SAFE_RELEASE(backBuffer);
}

void Direct3D::Initialize()
{
	UINT d3d11DeviceFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
	UINT dxgiFactoryFlags = 0;

#ifdef _DEBUG
	dxgiFactoryFlags |= DXGI_CREATE_FACTORY_DEBUG;
	d3d11DeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
	};

	IDXGIDevice* dxgiDevice = 0;
	FOG_TRACE(D3D11CreateDevice(0, D3D_DRIVER_TYPE_HARDWARE, 0, d3d11DeviceFlags, featureLevels, ARRAYSIZE(featureLevels), D3D11_SDK_VERSION, &mDevice, 0, &mDeviceContext));
	FOG_TRACE(mDeviceContext->QueryInterface(IID_PPV_ARGS(&mDeviceContext1)));
	FOG_TRACE(mDevice->QueryInterface(IID_PPV_ARGS(&dxgiDevice)));
	SAFE_RELEASE(dxgiDevice);

	IDXGIFactory2* dxgiFactory = 0;
	FOG_TRACE(CreateDXGIFactory2(dxgiFactoryFlags, IID_PPV_ARGS(&dxgiFactory)));

	DXGI_SWAP_CHAIN_DESC desc{};
	desc.BufferDesc.RefreshRate.Numerator = 0;
	desc.BufferDesc.RefreshRate.Numerator = 0;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	desc.BufferCount = 2;
	desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING;
	desc.Windowed = true;
	desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.OutputWindow = Application::GetHWND();

	FOG_TRACE(dxgiFactory->CreateSwapChain(mDevice, &desc, &mSwapChain));

	FOG_TRACE(dxgiFactory->MakeWindowAssociation(Application::GetHWND(), DXGI_MWA_NO_ALT_ENTER));
	SAFE_RELEASE(dxgiFactory);

	Resize();

	D3D11_RASTERIZER_DESC rd = {};
	rd.ScissorEnable = false;
	rd.FillMode = D3D11_FILL_SOLID;
	rd.CullMode = D3D11_CULL_BACK;
	FOG_TRACE(mDevice->CreateRasterizerState(&rd, &mRasterizerState));

	mDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
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
	ObjectManager::Draw();
}

void Direct3D::Present()
{
	FOG_TRACE(mSwapChain->Present(0, DXGI_PRESENT_ALLOW_TEARING));
}

void Direct3D::Shotdown()
{
	SAFE_RELEASE(mRasterizerState);
	SAFE_RELEASE(mSwapChain);
	SAFE_RELEASE(mDeviceContext);
	SAFE_RELEASE(mDeviceContext1);
	SAFE_RELEASE(mRenderTargetView);

#ifdef _DEBUG
	ID3D11Debug* d3dDebug;
	FOG_TRACE(mDevice->QueryInterface(IID_PPV_ARGS(&d3dDebug)));
	FOG_TRACE(d3dDebug->ReportLiveDeviceObjects(D3D11_RLDO_IGNORE_INTERNAL));
	SAFE_RELEASE(d3dDebug);
#endif
	SAFE_RELEASE(mDevice);

#ifdef _DEBUG
	typedef HRESULT(__stdcall* fPtr)(const IID&, void**);
	HMODULE hDll = GetModuleHandle(L"dxgidebug.dll");
	FOG_ASSERT(hDll);
	fPtr DXGIGetDebugInterface = (fPtr)GetProcAddress(hDll, "DXGIGetDebugInterface");
	IDXGIDebug* pDxgiDebug;
	DXGIGetDebugInterface(IID_PPV_ARGS(&pDxgiDebug));
	pDxgiDebug->ReportLiveObjects(DXGI_DEBUG_ALL, DXGI_DEBUG_RLO_IGNORE_INTERNAL);
	SAFE_RELEASE(pDxgiDebug);
#endif
}


ID3D11ShaderResourceView* const* Direct3D::NullSRV()
{
	static ID3D11ShaderResourceView* nullSRV = 0;
	return &nullSRV;
}

ID3D11RenderTargetView* const* Direct3D::RTV()
{
	return &mRenderTargetView;
}