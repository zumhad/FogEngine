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
ID3D11DepthStencilView* Direct3D::mDepthStencilView = 0;
ID3D11DepthStencilState* Direct3D::mDepthDisabledStencilState = 0;
ID3D11DepthStencilState* Direct3D::mDepthStencilState = 0;

D3D11_VIEWPORT Direct3D::mGameViewport = { 0 };
D3D11_VIEWPORT Direct3D::mSceneViewport = { 0 };
D3D11_VIEWPORT Direct3D::mEditorViewport = { 0 };

RECT Direct3D::mSceneRect = { 0 };

void Direct3D::Setup()
{
	Initialize();
}

void Direct3D::ResizeScene()
{
	int left = Application::GetSceneX();
	int top = Application::GetSceneY();
	int right = left + Application::GetSceneWidth();
	int bottom = top + Application::GetSceneHeight();
	mSceneRect = { left, top, right, bottom };

	mSceneViewport.MinDepth = 0.0f;
	mSceneViewport.MaxDepth = 1.0f;
	mSceneViewport.TopLeftX = (float)left;
	mSceneViewport.TopLeftY = (float)top;
	mSceneViewport.Width = (float)right - left;
	mSceneViewport.Height = (float)bottom - top;
}

void Direct3D::ResizeEditor()
{
	SAFE_RELEASE(mRenderTargetView);
	SAFE_RELEASE(mDepthStencilView);
	GUI::Release();

	int width = Application::GetEditorWidth();
	int height = Application::GetEditorHeight();

	FOG_TRACE(mSwapChain->ResizeBuffers(0, width, height, DXGI_FORMAT_UNKNOWN, DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING));

	ID3D11Texture2D* backBuffer = 0;
	FOG_TRACE(mSwapChain->GetBuffer(0, IID_PPV_ARGS(&backBuffer)));
	FOG_TRACE(mDevice->CreateRenderTargetView(backBuffer, 0, &mRenderTargetView));
	SAFE_RELEASE(backBuffer);

	GUI::Resize();

	D3D11_TEXTURE2D_DESC descDepth{};
	descDepth.Width = width;
	descDepth.Height = height;
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	ID3D11Texture2D* depthStencil = 0;
	FOG_TRACE(mDevice->CreateTexture2D(&descDepth, 0, &depthStencil));

	// Create the depth stencil view
	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV = {};
	descDSV.Format = descDepth.Format;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0;
	FOG_TRACE(mDevice->CreateDepthStencilView(depthStencil, 0, &mDepthStencilView));
	SAFE_RELEASE(depthStencil);

	mEditorViewport.MinDepth = 0.0f;
	mEditorViewport.MaxDepth = 1.0f;
	mEditorViewport.TopLeftX = 0.0f;
	mEditorViewport.TopLeftY = 0.0f;
	mEditorViewport.Width = (float)width;
	mEditorViewport.Height = (float)height;
}

void Direct3D::ResizeEngine()
{
	ResizeEditor();
	ResizeScene();
}

void Direct3D::ResizeGame()
{
	SAFE_RELEASE(mRenderTargetView);
	SAFE_RELEASE(mDepthStencilView);

	int width = Application::GetGameWidth();
	int height = Application::GetGameHeight();

	FOG_TRACE(mSwapChain->ResizeBuffers(0, width, height, DXGI_FORMAT_UNKNOWN, DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING));

	ID3D11Texture2D* backBuffer = 0;
	FOG_TRACE(mSwapChain->GetBuffer(0, IID_PPV_ARGS(&backBuffer)));

	D3D11_RENDER_TARGET_VIEW_DESC descRTV{};
	descRTV.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	descRTV.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;

	FOG_TRACE(mDevice->CreateRenderTargetView(backBuffer, &descRTV, &mRenderTargetView));
	SAFE_RELEASE(backBuffer);

	D3D11_TEXTURE2D_DESC descDepth = {};
	descDepth.Width = width;
	descDepth.Height = height;
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;
	ID3D11Texture2D* depthStencil = 0;
	FOG_TRACE(mDevice->CreateTexture2D(&descDepth, 0, &depthStencil));

	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV{};
	descDSV.Format = descDepth.Format;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;

	FOG_TRACE(mDevice->CreateDepthStencilView(depthStencil, 0, &mDepthStencilView));
	SAFE_RELEASE(depthStencil);

	mGameViewport.MinDepth = 0.0f;
	mGameViewport.MaxDepth = 1.0f;
	mGameViewport.TopLeftX = 0.0f;
	mGameViewport.TopLeftY = 0.0f;
	mGameViewport.Width = (float)width;
	mGameViewport.Height = (float)height;
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


	if (Application::IsGame())
		ResizeGame();
	else
		ResizeEngine();


	D3D11_RASTERIZER_DESC rd = {};
	rd.ScissorEnable = false;
	rd.FillMode = D3D11_FILL_SOLID;
	rd.CullMode = D3D11_CULL_BACK;
	FOG_TRACE(mDevice->CreateRasterizerState(&rd, &mRasterizerState));

	D3D11_DEPTH_STENCIL_DESC dsd = {};
	dsd.DepthEnable = false;
	dsd.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dsd.DepthFunc = D3D11_COMPARISON_LESS;
	dsd.StencilEnable = true;
	dsd.StencilReadMask = 0xFF;
	dsd.StencilWriteMask = 0xFF;
	dsd.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	dsd.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	dsd.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	dsd.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	dsd.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	dsd.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	dsd.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	dsd.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	FOG_TRACE(mDevice->CreateDepthStencilState(&dsd, &mDepthDisabledStencilState));
	dsd.DepthEnable = true;
	FOG_TRACE(mDevice->CreateDepthStencilState(&dsd, &mDepthStencilState));

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
	const UINT count = 0;
	mDeviceContext->OMSetRenderTargetsAndUnorderedAccessViews(1, &mRenderTargetView, mDepthStencilView, 1, 1, ObjectManager::GetUAV(), &count);
	mDeviceContext->ClearDepthStencilView(mDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);



	static FLOAT color[4]{};

	if (Application::IsGame())
	{
		Color c = Application::GetGameColor();
		color[0] = c.r;
		color[1] = c.g;
		color[2] = c.b;
		color[3] = c.a;

		mDeviceContext->RSSetViewports(1, &mGameViewport);
		mDeviceContext->ClearRenderTargetView(mRenderTargetView, color);
	}
	else
	{
		Color c = Application::GetSceneColor();
		color[0] = c.r;
		color[1] = c.g;
		color[2] = c.b;
		color[3] = c.a;

		mDeviceContext->RSSetViewports(1, &mSceneViewport);
		mDeviceContext1->ClearView(mRenderTargetView, color, &mSceneRect, 1);
	}

	ObjectManager::Draw();
	ObjectManager::Pick();
}

void Direct3D::Present()
{
	FOG_TRACE(mSwapChain->Present(0, DXGI_PRESENT_ALLOW_TEARING));
}


void Direct3D::Shotdown()
{
	SAFE_RELEASE(mRasterizerState);
	SAFE_RELEASE(mDepthStencilView);
	SAFE_RELEASE(mDepthStencilState);
	SAFE_RELEASE(mDepthDisabledStencilState);
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

