#include "Direct3D.h"

#include "PathHelper.h"
#include "Trace.h"
#include "Time.h"
#include "ObjectManager.h"

namespace Module
{
    Direct3D::Direct3D(HWND hwnd, Rect editorRect, COLORREF editorColor, Rect sceneRect, COLORREF sceneColor, CAMERACLASS* camera)
    {
        mIsGame = false;

        mHwnd = hwnd;
        mRect_Editor = { editorRect.left, editorRect.top, editorRect.right, editorRect.bottom };
        mRect_Scene = { sceneRect.left, sceneRect.top, sceneRect.right, sceneRect.bottom };

        mColor_Editor[0] = GetRValue(editorColor) / 255.0f;
        mColor_Editor[1] = GetGValue(editorColor) / 255.0f;
        mColor_Editor[2] = GetBValue(editorColor) / 255.0f;
        mColor_Editor[3] = 1.0f;

        mColor_Scene[0] = GetRValue(sceneColor) / 255.0f;
        mColor_Scene[1] = GetGValue(sceneColor) / 255.0f;
        mColor_Scene[2] = GetBValue(sceneColor) / 255.0f;
        mColor_Scene[3] = 1.0f;

        Initialize(camera);
    }

    Direct3D::Direct3D(HWND hwnd, Rect gameRect, COLORREF gameColor, CAMERACLASS* camera)
    {
        mIsGame = true;

        mHwnd = hwnd;
        mRect_Game = { gameRect.left, gameRect.top, gameRect.right, gameRect.bottom };

        mColor_Game[0] = GetRValue(gameColor) / 255.0f;
        mColor_Game[1] = GetGValue(gameColor) / 255.0f;
        mColor_Game[2] = GetBValue(gameColor) / 255.0f;
        mColor_Game[3] = 1.0f;

        Initialize(camera);
    }


    Direct3D::~Direct3D()
    {
        SAFE_RELEASE(mVertexShader);
        SAFE_RELEASE(mPixelShader);
        SAFE_RELEASE(mConstantBuffer);

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
        mDevice->QueryInterface(IID_PPV_ARGS(&d3dDebug));
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


    void Direct3D::ResizeApp(uint16 width, uint16 height)
    {
        if (mIsGame)
            mRect_Game = { 0, 0, width, height };
        else
            mRect_Editor = { 0, 0, width, height };


        SAFE_RELEASE(mRenderTargetView);
        SAFE_RELEASE(mDepthStencilView);
        SAFE_RELEASE(mDepthStencil);

        FOG_TRACE(mSwapChain->ResizeBuffers(2, width, height, DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING));
        ID3D11Texture2D* backBuffer;
        FOG_TRACE(mSwapChain->GetBuffer(0, IID_PPV_ARGS(&backBuffer)));
        FOG_TRACE(mDevice->CreateRenderTargetView(backBuffer, 0, &mRenderTargetView));
        SAFE_RELEASE(backBuffer);

        // Create depth stencil texture
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
        FOG_TRACE(mDevice->CreateTexture2D(&descDepth, nullptr, &mDepthStencil));

        // Create the depth stencil view
        D3D11_DEPTH_STENCIL_VIEW_DESC descDSV = {};
        descDSV.Format = descDepth.Format;
        descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
        descDSV.Texture2D.MipSlice = 0;
        FOG_TRACE(mDevice->CreateDepthStencilView(mDepthStencil, 0, &mDepthStencilView));

        mDeviceContext->OMSetRenderTargets(1, &mRenderTargetView, mDepthStencilView);

        if (mIsGame)
        {
            D3D11_VIEWPORT vp;
            vp.TopLeftX = 0;
            vp.TopLeftY = 0;
            vp.Width = width;
            vp.Height = height;
            vp.MinDepth = 0.0f;
            vp.MaxDepth = 1.0f;
            mDeviceContext->RSSetViewports(1, &vp);
        }

    }

    void Direct3D::ResizeScene(uint16 left, uint16 top, uint16 right, uint16 bottom)
    {
        mRect_Scene = { left, top, right, bottom };

        D3D11_VIEWPORT vp;
        vp.TopLeftX = left;
        vp.TopLeftY = top;
        vp.Width = float(right - left);
        vp.Height = float(bottom - top);
        vp.MinDepth = 0.0f;
        vp.MaxDepth = 1.0f;

        mDeviceContext->RSSetViewports(1, &vp);
    }


    void Direct3D::Initialize(CAMERACLASS* camera)
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
            D3D_FEATURE_LEVEL_11_0
        };

        D3D_FEATURE_LEVEL featureLevel;
        FOG_TRACE(D3D11CreateDevice(0, D3D_DRIVER_TYPE_HARDWARE, 0, createDeviceFlags, featureLevels, ARRAYSIZE(featureLevels), D3D11_SDK_VERSION, &mDevice, &featureLevel, &mDeviceContext));
        FOG_ASSERT(featureLevel >= D3D_FEATURE_LEVEL_11_0);


        // Obtain DXGI factory from device (since we used nullptr for pAdapter above)
        IDXGIFactory1* dxgiFactory = 0;
        {
            IDXGIDevice* dxgiDevice = 0;
            FOG_TRACE(mDevice->QueryInterface(IID_PPV_ARGS(&dxgiDevice)));

            IDXGIAdapter* adapter = 0;
            FOG_TRACE(dxgiDevice->GetAdapter(&adapter));

            FOG_TRACE(adapter->GetParent(IID_PPV_ARGS(&dxgiFactory)));
            adapter->Release();
            dxgiDevice->Release();
        }

        // Create swap chain
        IDXGIFactory2* dxgiFactory2 = 0;
        FOG_TRACE(dxgiFactory->QueryInterface(IID_PPV_ARGS(&dxgiFactory2)));

        FOG_TRACE(mDevice->QueryInterface(IID_PPV_ARGS(&mDevice1)));
        FOG_TRACE(mDeviceContext->QueryInterface(IID_PPV_ARGS(&mDeviceContext1)));

        DXGI_SWAP_CHAIN_DESC1 scd = {};
        if (mIsGame)
        {
            scd.Width = RECT_WIDTH(mRect_Game);
            scd.Height = RECT_HEIGHT(mRect_Game);
        }
        else
        {
            scd.Width = RECT_WIDTH(mRect_Editor);
            scd.Height = RECT_HEIGHT(mRect_Editor);
        }
        scd.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        scd.SampleDesc.Count = 1;
        scd.SampleDesc.Quality = 0;
        scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        scd.BufferCount = 2;
        scd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
        scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING;

        FOG_TRACE(dxgiFactory2->CreateSwapChainForHwnd(mDevice, mHwnd, &scd, 0, 0, &mSwapChain1));
        FOG_TRACE(mSwapChain1->QueryInterface(IID_PPV_ARGS(&mSwapChain)));
        dxgiFactory2->Release();

        if (mIsGame)
        {
            //FOG_TRACE(mSwapChain1->SetFullscreenState(true, 0));
            mFullScreen = true;
        }


        FOG_TRACE(dxgiFactory->MakeWindowAssociation(mHwnd, DXGI_MWA_NO_ALT_ENTER));
        dxgiFactory->Release();


        if (mIsGame)
            ResizeApp((uint16)RECT_WIDTH(mRect_Game), (uint16)RECT_HEIGHT(mRect_Game));
        else
        {
            ResizeApp((uint16)RECT_WIDTH(mRect_Editor), (uint16)RECT_HEIGHT(mRect_Editor));
            ResizeScene((uint16)mRect_Scene.left, (uint16)mRect_Scene.top, (uint16)mRect_Scene.right, (uint16)mRect_Scene.bottom);
        }


        LoadAssets();

        mWorld = XMMatrixIdentity();
        mProjection = XMMatrixPerspectiveFovLH(XMConvertToRadians(camera->fov), 16.0f / 9.0f, camera->nearZ, camera->farZ);
    }

    void Direct3D::CompileShaderFromFile(const WCHAR* fileName, LPCSTR entryPoint, LPCSTR shaderModel, ID3DBlob** ppBlobOut)
    {
        DWORD shaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#ifdef _DEBUG
        shaderFlags |= D3DCOMPILE_DEBUG;
        shaderFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

        ID3DBlob* pErrorBlob = 0;
        FOG_TRACE(D3DCompileFromFile(fileName, 0, 0, entryPoint, shaderModel, shaderFlags, 0, ppBlobOut, &pErrorBlob));
        SAFE_RELEASE(pErrorBlob);
    }

    void Direct3D::LoadAssets()
    {
        // Get asset path
        WCHAR shaderPath[MAX_PATH];
        PathHelper::GetAssetsFullPath(shaderPath, L"shader.hlsl");


        // Compile the vertex shader
        ID3DBlob* pVSBlob = nullptr;
        CompileShaderFromFile(shaderPath, "VS", "vs_5_0", &pVSBlob);

        // Create the vertex shader
        mDevice->CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), nullptr, &mVertexShader);


        // Define the input layout
        D3D11_INPUT_ELEMENT_DESC layout[] =
        {
            { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
            { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
            { "DIRECTION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
        };
        UINT numElements = ARRAYSIZE(layout);

        // Create the input layout
        mDevice->CreateInputLayout(layout, numElements, pVSBlob->GetBufferPointer(),
            pVSBlob->GetBufferSize(), &mVertexLayout);

        // Set the input layout
        mDeviceContext->IASetInputLayout(mVertexLayout);

        // Compile the pixel shader
        ID3DBlob* pPSBlob = nullptr;
        CompileShaderFromFile(shaderPath, "PSL", "ps_5_0", &pPSBlob);
        mDevice->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), nullptr, &mPixelShaderLight);

        CompileShaderFromFile(shaderPath, "PS", "ps_5_0", &pPSBlob);
        mDevice->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), nullptr, &mPixelShader);

        // Create vertex buffer
        ID3D11Buffer* vertexBuffer = 0;
        Vertex vertices[] =
        {
            { XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f) },
            { XMFLOAT3(1.0f, 1.0f, -1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f) },
            { XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f) },
            { XMFLOAT3(-1.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f) },

            { XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT3(0.0f, -1.0f, 0.0f) },
            { XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT3(0.0f, -1.0f, 0.0f) },
            { XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT3(0.0f, -1.0f, 0.0f) },
            { XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT3(0.0f, -1.0f, 0.0f) },

            { XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT3(-1.0f, 0.0f, 0.0f) },
            { XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT3(-1.0f, 0.0f, 0.0f) },
            { XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT3(-1.0f, 0.0f, 0.0f) },
            { XMFLOAT3(-1.0f, 1.0f, 1.0f), XMFLOAT3(-1.0f, 0.0f, 0.0f) },

            { XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT3(1.0f, 0.0f, 0.0f) },
            { XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT3(1.0f, 0.0f, 0.0f) },
            { XMFLOAT3(1.0f, 1.0f, -1.0f), XMFLOAT3(1.0f, 0.0f, 0.0f) },
            { XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT3(1.0f, 0.0f, 0.0f) },

            { XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT3(0.0f, 0.0f, -1.0f) },
            { XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT3(0.0f, 0.0f, -1.0f) },
            { XMFLOAT3(1.0f, 1.0f, -1.0f), XMFLOAT3(0.0f, 0.0f, -1.0f) },
            { XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT3(0.0f, 0.0f, -1.0f) },

            { XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 1.0f) },
            { XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 1.0f) },
            { XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 1.0f) },
            { XMFLOAT3(-1.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 1.0f) }
        };
        D3D11_BUFFER_DESC bd = {};
        bd.Usage = D3D11_USAGE_DEFAULT;
        bd.ByteWidth = sizeof(Vertex) * 24;
        bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        bd.CPUAccessFlags = 0;

        D3D11_SUBRESOURCE_DATA InitData = {};
        InitData.pSysMem = vertices;
        mDevice->CreateBuffer(&bd, &InitData, &vertexBuffer); //init vertex buffer
        const UINT stride = sizeof(Vertex);
        const UINT offset = 0;
        mDeviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset); // set vertex buffer

        // Create index buffer
        WORD indices[] =
        {
            3,1,0,
            2,1,3,

            6,4,5,
            7,4,6,

            11,9,8,
            10,9,11,

            14,12,13,
            15,12,14,

            19,17,16,
            18,17,19,

            22,20,21,
            23,20,22
        };
        bd.Usage = D3D11_USAGE_DEFAULT;
        bd.ByteWidth = sizeof(WORD) * 36;
        bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
        bd.CPUAccessFlags = 0;
        InitData.pSysMem = indices;
        mDevice->CreateBuffer(&bd, &InitData, &mIndexBuffer);

        mDeviceContext->IASetIndexBuffer(mIndexBuffer, DXGI_FORMAT_R16_UINT, 0);

        // Set primitive topology
        mDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

        // Create the constant buffer
        bd.Usage = D3D11_USAGE_DEFAULT;
        bd.ByteWidth = sizeof(ConstantBuffer);
        bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        bd.CPUAccessFlags = 0;
        mDevice->CreateBuffer(&bd, nullptr, &mConstantBuffer);
    }

    void Direct3D::UpdateViewMatrix(Matrix4 camera)
    {
        mView = camera;
    }

    void Direct3D::ToggleFullscreen(bool state)
    {
        FOG_TRACE(mSwapChain->SetFullscreenState(state, 0));
    }

    void Direct3D::DrawEngine()
    {
        DrawEditor();
        DrawScene();
    }

    void Direct3D::Update(float dt)
    {
        dt;
        /*ConstantBuffer cb;


        mDeviceContext->UpdateSubresource(mConstantBuffer, 0, 0, &cb, 0, 0);*/
    }

    void Direct3D::DrawGame()
    {
        mDeviceContext->OMSetRenderTargets(1, &mRenderTargetView, 0);

        mDeviceContext->ClearRenderTargetView(mRenderTargetView, mColor_Game);
        mDeviceContext->ClearDepthStencilView(mDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

        Render();
    }

    void Direct3D::DrawScene()
    {
        mDeviceContext1->ClearView(mRenderTargetView, mColor_Scene, &mRect_Scene, 1);
        mDeviceContext->ClearDepthStencilView(mDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
        
        Render();
    }
    
    /*void Direct3D::SwapFrame()
    {
        mDeviceContext->OMSetRenderTargets(1, &mRenderTargetView, 0);
    }*/

    void Direct3D::DrawEditor()
    {
        mDeviceContext->ClearRenderTargetView(mRenderTargetView, mColor_Editor);
    }



    void Direct3D::Render()
    {
        ConstantBuffer cb;

        int len = ObjectManager::Length();
        for (int i = 0; i < len; i++)
        {
            Object* o = ObjectManager::Get(i);
            mWorld = o->GetWorldMatrix();
            

            cb.mWorldViewProj = XMMatrixTranspose(mWorld * mView * mProjection);
            cb.mColor = Vector4(GET_OBJECT(o, Cube)->mMaterial);
            mDeviceContext->UpdateSubresource(mConstantBuffer, 0, 0, &cb, 0, 0);

            mDeviceContext->VSSetShader(mVertexShader, 0, 0);
            mDeviceContext->VSSetConstantBuffers(0, 1, &mConstantBuffer);
            mDeviceContext->PSSetShader(mPixelShader, 0, 0);
            mDeviceContext->PSSetConstantBuffers(0, 1, &mConstantBuffer);

            mDeviceContext->DrawIndexed(36, 0, 0);
        }
    }

    void Direct3D::Present()
    {
        FOG_TRACE(mSwapChain->Present(0, DXGI_PRESENT_ALLOW_TEARING));
    }
}
