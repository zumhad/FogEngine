#include "Direct3D.h"

#include "Properties.h"
#include "Trace.h"
#include "Definitions.h"
#include "PathHelper.h"
#include "ObjectManager.h"
#include "ApplicationEngine.h"
#include "CameraEngine.h"
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
IDXGISwapChain1* Direct3D::mSwapChain1 = 0;
ID3D11RenderTargetView* Direct3D::mRenderTargetView = 0;
ID3D11RasterizerState* Direct3D::mRasterizerState = 0;
ID3D11DepthStencilView* Direct3D::mDepthStencilView = 0;
ID3D11DepthStencilState* Direct3D::mDepthDisabledStencilState = 0;
ID3D11DepthStencilState* Direct3D::mDepthStencilState = 0;

D3D11_VIEWPORT Direct3D::mGameViewport = { 0 };
D3D11_VIEWPORT Direct3D::mSceneViewport = { 0 };
D3D11_VIEWPORT Direct3D::mEditorViewport = { 0 };

RECT Direct3D::mSceneRect = { 0 };
float Direct3D::mEditorColor[4] = { 0 };
float Direct3D::mSceneColor[4] = { 0 };
float Direct3D::mGameColor[4] = { 0 };


ID2D1RenderTarget* md2dRenderTarget;
ID2D1Factory3* md2dFactory;

IDWriteFactory* mdWriteFactory;
//IDWriteTextFormat* md2dTextFormat;
ID2D1SolidColorBrush* md2dBlackBrush;

void Direct3D::Setup()
{
	Initialize();

	mEditorColor[0] = Singlton.editor.color.r;
	mEditorColor[1] = Singlton.editor.color.g;
	mEditorColor[2] = Singlton.editor.color.b;
	mEditorColor[3] = Singlton.editor.color.a;

	mSceneColor[0] = Singlton.scene.color.r;
	mSceneColor[1] = Singlton.scene.color.g;
	mSceneColor[2] = Singlton.scene.color.b;
	mSceneColor[3] = Singlton.scene.color.a;

	mGameColor[0] = Singlton.game.color.r;
	mGameColor[1] = Singlton.game.color.g;
	mGameColor[2] = Singlton.game.color.b;
	mGameColor[3] = Singlton.game.color.a;
}

void Direct3D::ResizeScene()
{
	int left = Singlton.scene.x;
	int top = Singlton.scene.y;
	int right = left + Singlton.scene.width;
	int bottom = top + Singlton.scene.height;
	mSceneRect = { left, top, right, bottom };

	mSceneViewport.MinDepth = 0.0f;
	mSceneViewport.MaxDepth = 1.0f;
	mSceneViewport.TopLeftX = (float)Singlton.scene.x;
	mSceneViewport.TopLeftY = (float)Singlton.scene.y;
	mSceneViewport.Width = (float)Singlton.scene.width;
	mSceneViewport.Height = (float)Singlton.scene.height;
}

void Direct3D::ResizeEditor()
{
	SAFE_RELEASE(mRenderTargetView);
	SAFE_RELEASE(mDepthStencilView);
	SAFE_RELEASE(md2dRenderTarget);

	int width = Singlton.editor.width;
	int height = Singlton.editor.height;

	FOG_TRACE(mSwapChain->ResizeBuffers(0, width, height, DXGI_FORMAT_UNKNOWN, DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING));

	ID3D11Texture2D* pBackBuffer2 = 0;
	FOG_TRACE(mSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer2)));
	FOG_TRACE(mDevice->CreateRenderTargetView(pBackBuffer2, 0, &mRenderTargetView));

	D2D1_RENDER_TARGET_PROPERTIES properties{};
	properties.type = D2D1_RENDER_TARGET_TYPE_DEFAULT;
	properties.pixelFormat.alphaMode = D2D1_ALPHA_MODE_PREMULTIPLIED;
	properties.pixelFormat.format = DXGI_FORMAT_UNKNOWN;

	IDXGISurface* surface;
	FOG_TRACE(pBackBuffer2->QueryInterface(&surface));
	FOG_TRACE(md2dFactory->CreateDxgiSurfaceRenderTarget(surface, &properties, &md2dRenderTarget));

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
	mEditorViewport.Width = (float)Singlton.editor.width;
	mEditorViewport.Height = (float)Singlton.editor.height;

	SAFE_RELEASE(surface);
	SAFE_RELEASE(pBackBuffer2);
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

	int width = Singlton.resolution.width;
	int height = Singlton.resolution.height;

	FOG_TRACE(mSwapChain->ResizeBuffers(0, width, height, DXGI_FORMAT_UNKNOWN, DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING));

	ID3D11Texture2D* pBackBuffer2 = 0;
	FOG_TRACE(mSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer2)));
	FOG_TRACE(mDevice->CreateRenderTargetView(pBackBuffer2, 0, &mRenderTargetView));

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

	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV = {};
	descDSV.Format = descDepth.Format;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0;
	FOG_TRACE(mDevice->CreateDepthStencilView(depthStencil, 0, &mDepthStencilView));
	SAFE_RELEASE(depthStencil);

	mGameViewport.MinDepth = 0.0f;
	mGameViewport.MaxDepth = 1.0f;
	mGameViewport.TopLeftX = 0.0f;
	mGameViewport.TopLeftY = 0.0f;
	mGameViewport.Width = (float)Singlton.resolution.width;
	mGameViewport.Height = (float)Singlton.resolution.height;

	SAFE_RELEASE(pBackBuffer2);
}

void Direct3D::Initialize()
{
	UINT d3d11DeviceFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
	UINT dxgiFactoryFlags = 0;
	D2D1_FACTORY_OPTIONS d2dFactoryOptions{};

#ifdef _DEBUG
	dxgiFactoryFlags |= DXGI_CREATE_FACTORY_DEBUG;
	d3d11DeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
	d2dFactoryOptions.debugLevel = D2D1_DEBUG_LEVEL_INFORMATION;
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
	desc.OutputWindow = ApplicationEngine::GetHWND();

	FOG_TRACE(dxgiFactory->CreateSwapChain(mDevice, &desc, &mSwapChain));

	FOG_TRACE(dxgiFactory->MakeWindowAssociation(ApplicationEngine::GetHWND(), DXGI_MWA_NO_ALT_ENTER));
	SAFE_RELEASE(dxgiFactory);

	// direct2d // direct write
	FOG_TRACE(D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, __uuidof(ID2D1Factory3), &d2dFactoryOptions, (void**)&md2dFactory));
	FOG_TRACE(DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), (IUnknown**)&mdWriteFactory));
	SAFE_RELEASE(dxgiDevice);


	if (Singlton.isGame)
		ResizeGame();
	else
		ResizeEngine();


	/*mdWriteFactory->CreateTextFormat(
		L"Impact",                // Font family name.
		NULL,                       // Font collection (NULL sets it to use the system font collection).
		DWRITE_FONT_WEIGHT_LIGHT,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		16.0f,
		L"en",
		&md2dTextFormat
	);*/
	
	//md2dTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
	//md2dTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);

	md2dRenderTarget->CreateSolidColorBrush(
		D2D1::ColorF(D2D1::ColorF::Gray, 1.0f),
		&md2dBlackBrush
	);

	//md2dDeviceContext->SetTextAntialiasMode(D2D1_TEXT_ANTIALIAS_MODE_GRAYSCALE);

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


void Direct3D::SetZBuffer(bool var)
{
	if (var)
		mDeviceContext->OMSetDepthStencilState(mDepthStencilState, 1);
	else
		mDeviceContext->OMSetDepthStencilState(mDepthDisabledStencilState, 1);
}


void Direct3D::DrawEngine()
{
	DrawEditor();
	DrawGame();
}

void Direct3D::DrawEditor()
{
	//GUI::Draw();

	md2dRenderTarget->BeginDraw();
	md2dRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::White, 1.0f));

	D2D1_RECT_F rect = { 0,0,1920,50 };
	md2dRenderTarget->FillRectangle(&rect, md2dBlackBrush);

	FOG_TRACE(md2dRenderTarget->EndDraw());
}


void Direct3D::DrawGame()
{
	mDeviceContext->OMSetRenderTargets(1, &mRenderTargetView, mDepthStencilView);
	mDeviceContext->ClearDepthStencilView(mDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	if (Singlton.isGame)
	{
		mDeviceContext->RSSetViewports(1, &mGameViewport);
		mDeviceContext->ClearRenderTargetView(mRenderTargetView, mGameColor);
	}
	else
	{
		mDeviceContext->RSSetViewports(1, &mSceneViewport);
		mDeviceContext1->ClearView(mRenderTargetView, mSceneColor, &mSceneRect, 1);
	}

	ObjectManager::Draw();
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
	SAFE_RELEASE(mSwapChain1);
	SAFE_RELEASE(mDeviceContext);
	SAFE_RELEASE(mDeviceContext1);
	SAFE_RELEASE(mRenderTargetView);
	SAFE_RELEASE(md2dRenderTarget);
	SAFE_RELEASE(md2dFactory);
	SAFE_RELEASE(mdWriteFactory);
	SAFE_RELEASE(md2dBlackBrush);

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

