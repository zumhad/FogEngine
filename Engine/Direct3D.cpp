#include "Direct3D.h"

#include "Properties.h"
#include "Trace.h"
#include "Definitions.h"
#include "PathHelper.h"
#include "ObjectManager.h"

#include <d3dcompiler.h>
#include <directxcolors.h>
#include <dxgidebug.h>

using namespace DirectX;

namespace Module
{
    ID3D11Device* Direct3D::mDevice = 0;
    ID3D11DeviceContext* Direct3D::mDeviceContext = 0;

    Direct3D::Direct3D(HWND hwnd)
    {
        Initialize(hwnd);

        mEditorColor[0] = GetRValue(Singlton.editor.color) / 255.0f;
        mEditorColor[1] = GetGValue(Singlton.editor.color) / 255.0f;
        mEditorColor[2] = GetBValue(Singlton.editor.color) / 255.0f;
        mEditorColor[3] = 1.0f;

        mSceneColor[0] = GetRValue(Singlton.scene.color) / 255.0f;
        mSceneColor[1] = GetGValue(Singlton.scene.color) / 255.0f;
        mSceneColor[2] = GetBValue(Singlton.scene.color) / 255.0f;
        mSceneColor[3] = 1.0f;

        mGameColor[0] = GetRValue(Singlton.game.color) / 255.0f;
        mGameColor[1] = GetGValue(Singlton.game.color) / 255.0f;
        mGameColor[2] = GetBValue(Singlton.game.color) / 255.0f;
        mGameColor[3] = 1.0f;
    }

    void CompileShaderFromFile(const WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut)
    {
        DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;

#ifdef _DEBUG
        dwShaderFlags |= D3DCOMPILE_DEBUG;
        dwShaderFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

        ID3DBlob* pErrorBlob = nullptr;
        FOG_TRACE(D3DCompileFromFile(szFileName, nullptr, nullptr, szEntryPoint, szShaderModel, dwShaderFlags, 0, ppBlobOut, &pErrorBlob));

        SAFE_RELEASE(pErrorBlob);
    }


    void Direct3D::ResizeScene()
    {
        short left = Singlton.scene.x;
        short top = Singlton.scene.y;
        short right = left + Singlton.scene.width;
        short bottom = top + Singlton.scene.height;
        mSceneRect = { left, top, right, bottom };

        mSceneViewport.MinDepth = 0.0f;
        mSceneViewport.MaxDepth = 1.0f;
        mSceneViewport.TopLeftX = Singlton.scene.x;
        mSceneViewport.TopLeftY = Singlton.scene.y;
        mSceneViewport.Width = Singlton.scene.width;
        mSceneViewport.Height = Singlton.scene.height;
    }

    void Direct3D::ResizeEditor()
    {
        SAFE_RELEASE(mRenderTargetView);
        SAFE_RELEASE(mDepthStencilView);

        short width = Singlton.editor.width;
        short height = Singlton.editor.height;

        FOG_TRACE(mSwapChain->ResizeBuffers(2, width, height, DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING));
        ID3D11Texture2D* pBackBuffer2 = 0;
        FOG_TRACE(mSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer2)));
        FOG_ASSERT(pBackBuffer2);
        FOG_TRACE(mDevice->CreateRenderTargetView(pBackBuffer2, nullptr, &mRenderTargetView));
        SAFE_RELEASE(pBackBuffer2);

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
        FOG_TRACE(mDevice->CreateTexture2D(&descDepth, nullptr, &depthStencil));

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
        mEditorViewport.Width = Singlton.editor.width;
        mEditorViewport.Height = Singlton.editor.height;
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

        short width = Singlton.resolution.width;
        short height = Singlton.resolution.height;

        FOG_TRACE(mSwapChain->ResizeBuffers(2, width+1, height+1, DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING));
        ID3D11Texture2D* pBackBuffer2 = {};
        FOG_TRACE(mSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer2)));
        FOG_ASSERT(pBackBuffer2);
        FOG_TRACE(mDevice->CreateRenderTargetView(pBackBuffer2, nullptr, &mRenderTargetView));
        SAFE_RELEASE(pBackBuffer2);

        D3D11_TEXTURE2D_DESC descDepth = {};
        descDepth.Width = width+1;
        descDepth.Height = height+1;
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
        FOG_TRACE(mDevice->CreateTexture2D(&descDepth, nullptr, &depthStencil));

        //EDIT
        // Create the depth stencil view
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
        mGameViewport.Width = Singlton.resolution.width;
        mGameViewport.Height = Singlton.resolution.height;
    }

    void Direct3D::Initialize(HWND hwnd)
    {
        UINT createDeviceFlags = 0;

#ifdef _DEBUG
        createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

        D3D_FEATURE_LEVEL featureLevels[] =
        {
            D3D_FEATURE_LEVEL_12_1,
            D3D_FEATURE_LEVEL_12_0,
            D3D_FEATURE_LEVEL_11_1,
            D3D_FEATURE_LEVEL_11_0,
            D3D_FEATURE_LEVEL_10_1,
            D3D_FEATURE_LEVEL_10_0,
        };

        FOG_TRACE(D3D11CreateDevice(0, D3D_DRIVER_TYPE_HARDWARE, 0, createDeviceFlags, featureLevels, ARRAYSIZE(featureLevels), D3D11_SDK_VERSION, &mDevice, 0, &mDeviceContext));
        IDXGIFactory1* dxgiFactory = 0;
        IDXGIDevice* dxgiDevice = 0;
        FOG_TRACE(mDevice->QueryInterface(IID_PPV_ARGS(&dxgiDevice)));
        IDXGIAdapter* adapter = 0;
        FOG_TRACE(dxgiDevice->GetAdapter(&adapter));
        SAFE_RELEASE(dxgiDevice);
        FOG_TRACE(adapter->GetParent(IID_PPV_ARGS(&dxgiFactory)));
        SAFE_RELEASE(adapter);


        // Create swap chain
        IDXGIFactory2* dxgiFactory2 = 0;
        FOG_TRACE(dxgiFactory->QueryInterface(IID_PPV_ARGS(&dxgiFactory2)));

        FOG_TRACE(mDevice->QueryInterface(IID_PPV_ARGS(&mDevice1)));
        FOG_TRACE(mDeviceContext->QueryInterface(IID_PPV_ARGS(&mDeviceContext1)));

        DXGI_SWAP_CHAIN_DESC1 scd = { 0 };
        scd.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        scd.SampleDesc.Count = 1;
        scd.SampleDesc.Quality = 0;
        scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        scd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
        scd.BufferCount = 2;
        scd.Scaling = DXGI_SCALING_STRETCH;
        scd.AlphaMode = DXGI_ALPHA_MODE_IGNORE;
        scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING;

        DXGI_SWAP_CHAIN_FULLSCREEN_DESC fsSwapChainDesc = {};
        fsSwapChainDesc.Windowed = true;

        FOG_TRACE(dxgiFactory2->CreateSwapChainForHwnd(mDevice, hwnd, &scd, &fsSwapChainDesc, 0, &mSwapChain1));
        FOG_TRACE(mSwapChain1->QueryInterface(IID_PPV_ARGS(&mSwapChain)));
        SAFE_RELEASE(dxgiFactory2);

        FOG_TRACE(dxgiFactory->MakeWindowAssociation(hwnd, DXGI_MWA_NO_ALT_ENTER));
        SAFE_RELEASE(dxgiFactory);


        if (Singlton.isGame)
            ResizeGame();
        else
            ResizeEngine();




        D3D11_RASTERIZER_DESC rd = {};
        rd.ScissorEnable = false;
        rd.FillMode = D3D11_FILL_SOLID;
        rd.CullMode = D3D11_CULL_BACK;
        FOG_TRACE(mDevice->CreateRasterizerState(&rd, &mRasterizerState));


        WCHAR shaderPath[MAX_PATH];
        PathHelper::GetAssetsFullPath(shaderPath, L"shader.hlsl");

        // Vertex Shader
        ID3DBlob* pVSBlob = nullptr;
        CompileShaderFromFile(shaderPath, "VS", "vs_5_0", &pVSBlob);
        FOG_TRACE(mDevice->CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), 0, &mVertexShader));



        // Input Layout
        D3D11_INPUT_ELEMENT_DESC layout[] =
        {
            { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        };
        FOG_TRACE(mDevice->CreateInputLayout(layout, ARRAYSIZE(layout), pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), &mVertexLayout));
        SAFE_RELEASE(pVSBlob);
        mDeviceContext->IASetInputLayout(mVertexLayout);


        // Pixel Shader
        ID3DBlob* pPSBlob = nullptr;
        CompileShaderFromFile(shaderPath, "PS", "ps_5_0", &pPSBlob);
        FOG_TRACE(mDevice->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), nullptr, &mPixelShader));
        SAFE_RELEASE(pPSBlob);


        D3D11_BUFFER_DESC bd = { 0 };
        bd.Usage = D3D11_USAGE_DEFAULT;
        bd.ByteWidth = sizeof(ConstantBuffer);
        bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        mDevice->CreateBuffer(&bd, nullptr, &mConstantBuffer);
        bd.ByteWidth = sizeof(ObjectBuffer);
        mDevice->CreateBuffer(&bd, nullptr, &mObjectBuffer);



        // Set primitive topology
        mDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);




        mWorld = XMMatrixIdentity();
        mProjection = XMMatrixPerspectiveFovLH(XMConvertToRadians(Singlton.camera.fov), 16.0f / 9.0f, Singlton.camera.nearZ, Singlton.camera.farZ);
    }

    void Direct3D::UpdateViewMatrix(Matrix4 camera)
    {
        mView = camera;
    }


    void Direct3D::DrawEngine()
    {
        DrawEditor();
        mDeviceContext->RSSetViewports(1, &mSceneViewport);
        RenderGame();
    }

    void Direct3D::DrawEditor()
    {
        mDeviceContext->RSSetViewports(1, &mEditorViewport);
        mDeviceContext->OMSetRenderTargets(1, &mRenderTargetView, mDepthStencilView);
        mDeviceContext->VSSetShader(mVertexShader, 0, 0);
        mDeviceContext->PSSetShader(mPixelShader, 0, 0);

        mDeviceContext->ClearRenderTargetView(mRenderTargetView, mEditorColor);
        mDeviceContext->ClearDepthStencilView(mDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
    }

    void Direct3D::DrawGame()
    {
        mDeviceContext->RSSetViewports(1, &mGameViewport);
        mDeviceContext->OMSetRenderTargets(1, &mRenderTargetView, mDepthStencilView);
        mDeviceContext->VSSetShader(mVertexShader, 0, 0);
        mDeviceContext->PSSetShader(mPixelShader, 0, 0);

        RenderGame();
    }

    void Direct3D::RenderGame()
    {
        if (Singlton.isGame)
        {
            mDeviceContext->ClearRenderTargetView(mRenderTargetView, mGameColor);
        }
        else
        {
            mDeviceContext1->ClearView(mRenderTargetView, mSceneColor, &mSceneRect, 1);
        }
        mDeviceContext->ClearDepthStencilView(mDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

        for (int i = 0; i < ObjectManager::Size(); i++)
        {
            Object* obj = ObjectManager::Get(i);

            if (obj->GetType() == tCube)
            {
                static ConstantBuffer cb;
                cb.mWorldViewProj = mView * mProjection;
                cb.mWorld = ((Cube*)obj)->GetWorldMatrix();
                mDeviceContext->UpdateSubresource(mConstantBuffer, 0, 0, &cb, 0, 0);
                mDeviceContext->VSSetConstantBuffers(0, 1, &mConstantBuffer);

                static ObjectBuffer ob;
                ob.color = ((Cube*)obj)->mMaterial;
                mDeviceContext->UpdateSubresource(mObjectBuffer, 0, 0, &ob, 0, 0);
                mDeviceContext->PSSetConstantBuffers(1, 1, &mObjectBuffer);

                ((Cube*)obj)->Set();
                mDeviceContext->DrawIndexed(((Cube*)obj)->VertexCount(), 0, 0);
            }
        }
    }

    void Direct3D::DrawScene()
    {
        mDeviceContext->RSSetViewports(1, &mSceneViewport);
        mDeviceContext->OMSetRenderTargets(1, &mRenderTargetView, mDepthStencilView);
        mDeviceContext->VSSetShader(mVertexShader, 0, 0);
        mDeviceContext->PSSetShader(mPixelShader, 0, 0);

        RenderGame();
    }

    void Direct3D::Present()
    {
        FOG_TRACE(mSwapChain->Present(0, DXGI_PRESENT_ALLOW_TEARING));
    }


    Direct3D::~Direct3D()
    {
        SAFE_RELEASE(mVertexShader);
        SAFE_RELEASE(mPixelShader);
        SAFE_RELEASE(mConstantBuffer);
        SAFE_RELEASE(mRasterizerState);
        SAFE_RELEASE(mVertexLayout);
        SAFE_RELEASE(mVertexBuffer);
        SAFE_RELEASE(mIndexBuffer);
        SAFE_RELEASE(mObjectBuffer);
        SAFE_RELEASE(mDepthStencilView);

        if (mSwapChain)
        {
            mSwapChain->SetFullscreenState(false, 0);
        }
        SAFE_RELEASE(mSwapChain);
        SAFE_RELEASE(mSwapChain1);

        if (mDeviceContext)
        {
            mDeviceContext->ClearState();
            mDeviceContext->Flush();
        }
        SAFE_RELEASE(mDeviceContext);
        SAFE_RELEASE(mDeviceContext1);
        SAFE_RELEASE(mRenderTargetView);


#ifdef _DEBUG
        ID3D11Debug* d3dDebug;
        FOG_TRACE(mDevice->QueryInterface(IID_PPV_ARGS(&d3dDebug)));
        FOG_TRACE(d3dDebug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL));
        SAFE_RELEASE(d3dDebug);
#endif

        SAFE_RELEASE(mDevice);
        SAFE_RELEASE(mDevice1);

#ifdef _DEBUG
        typedef HRESULT(__stdcall* fPtr)(const IID&, void**);
        HMODULE hDll = GetModuleHandle(L"dxgidebug.dll");
        FOG_ASSERT(hDll);
        fPtr DXGIGetDebugInterface = (fPtr)GetProcAddress(hDll, "DXGIGetDebugInterface");
        IDXGIDebug* pDxgiDebug;
        DXGIGetDebugInterface(IID_PPV_ARGS(&pDxgiDebug));
        pDxgiDebug->ReportLiveObjects(DXGI_DEBUG_ALL, DXGI_DEBUG_RLO_ALL);
        SAFE_RELEASE(pDxgiDebug);
#endif
    }
}

