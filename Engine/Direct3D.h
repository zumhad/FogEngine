#pragma once

#include "Core.h"

#include "Camera.h"

#include <d3d11_4.h>
#include <directxmath.h>

namespace Module
{
    class FOG_API Direct3D
    {
    public:
        Direct3D(HWND hwnd);
        ~Direct3D();


        void DrawGame();
        void Present();
        void DrawScene();
        void DrawEngine();
        void DrawEditor();
        void UpdateViewMatrix(Matrix4 camera);

        void ResizeScene();
        void ResizeEditor();
        void ResizeEngine();
        void ResizeGame();

    private:
        void Initialize(HWND hwnd);
        void RenderGame();
        void RenderEditor();
        void UpdateGame();

    private:
        ID3D11Device* mDevice = 0;
        ID3D11Device1* mDevice1 = 0;
        ID3D11DeviceContext* mDeviceContext = 0;
        ID3D11DeviceContext1* mDeviceContext1 = 0;
        IDXGISwapChain* mSwapChain = 0;
        IDXGISwapChain1* mSwapChain1 = 0;
        ID3D11RenderTargetView* mRenderTargetView = 0;
        ID3D11VertexShader* mVertexShader = 0;
        ID3D11PixelShader* mPixelShader = 0;
        ID3D11InputLayout* mVertexLayout = 0;
        ID3D11Buffer* mVertexBuffer = 0;
        ID3D11Buffer* mIndexBuffer = 0;
        ID3D11Buffer* mConstantBuffer = 0;
        ID3D11RasterizerState* mRasterizerState = 0;

        D3D11_VIEWPORT mGameViewport = { 0 };
        D3D11_VIEWPORT mSceneViewport = { 0 };
        D3D11_VIEWPORT mEditorViewport = { 0 };

        RECT mSceneRect = { 0 };
        float mEditorColor[4] = { 0 };
        float mSceneColor[4] = { 0 };
        float mGameColor[4] = { 0 };

        Matrix4 mWorld;
        Matrix4 mView;
        Matrix4 mProjection;
    };

    struct SimpleVertex
    {
        DirectX::XMFLOAT3 pos;
    };

    struct ConstantBuffer
    {
        //Matrix4 mWorld;
        //Vector4 mColor;
        Matrix4 mWorldViewProj;
    };

}




