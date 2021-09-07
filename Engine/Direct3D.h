#pragma once

#include "Core.h"

#include "DirectXIncludes.h"
#include "Camera.h"
#include "Object.h"
#include "Properties.h"

namespace Module
{
	class Direct3D
	{
	public:
		Direct3D(HWND hwnd, Rect editorRect, COLORREF editorColor, Rect sceneRect, COLORREF sceneColor, CAMERACLASS* camera);
		Direct3D(HWND hwnd, Rect gameRect, COLORREF gameColor, CAMERACLASS* camera);
		~Direct3D();

		void UpdateViewMatrix(Matrix4 camera);
		void ResizeApp(uint16 width, uint16 height);
		void ResizeScene(uint16 left, uint16 top, uint16 right, uint16 bottom);
		void ToggleFullscreen(bool state);

		void DrawEngine();
		void DrawGame();
		void DrawScene();
		void DrawEditor();
		void Present();
		void Update(float dt);

	private:
		void Initialize(CAMERACLASS* camera);
		void LoadAssets();
		void CompileShaderFromFile(const WCHAR* fileName, LPCSTR entryPoint, LPCSTR shaderModel, ID3DBlob** ppBlobOut);
		void Render();

	public:
		ID3D11Device* mDevice = 0;
		ID3D11Device1* mDevice1 = 0;
		ID3D11DeviceContext* mDeviceContext = 0;
		ID3D11DeviceContext1* mDeviceContext1 = 0;
		IDXGISwapChain* mSwapChain = 0;
		IDXGISwapChain1* mSwapChain1 = 0;
		ID3D11RenderTargetView* mRenderTargetView = 0;
		ID3D11Texture2D* mDepthStencil = 0;
		ID3D11DepthStencilView* mDepthStencilView = 0;

		ID3D11InputLayout* mVertexLayout = 0;
		ID3D11VertexShader* mVertexShaderLight = 0;
		ID3D11VertexShader* mVertexShader = 0;
		ID3D11PixelShader* mPixelShaderLight = 0;
		ID3D11PixelShader* mPixelShader = 0;

		ID3D11Buffer* mConstantBuffer = 0;
		ID3D11Buffer* mVertexBuffer = 0;
		ID3D11Buffer* mIndexBuffer = 0;

		HWND mHwnd;

		D3D11_RECT mRect_Editor;
		D3D11_RECT mRect_Scene;
		D3D11_RECT mRect_Game;

		float mColor_Editor[4];
		float mColor_Scene[4];
		float mColor_Game[4];

		bool mIsGame;
		bool mFullScreen = false;

		Matrix4 mWorld;
		Matrix4 mView;
		Matrix4 mProjection;
	};
}



struct ConstantBuffer
{
	Matrix4 mWorld;
	Vector4 mColor;
	Matrix4 mWorldViewProj;
};

struct Vertex
{
	XMFLOAT3 pos;
	XMFLOAT3 normal;
};