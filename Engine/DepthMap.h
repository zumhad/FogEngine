#pragma once

#include "Core.h"

#include "ConstantBuffer.h"
#include "CustomString.h"

#include <d3d11.h>

class Direct3D;
class Application;
class ObjectManager;
class BufferManager;

class FOG_API DepthMap
{
	friend class Direct3D;
	friend class Application;
	friend class ObjectManager;
	friend class BufferManager;

public:
	static bool GetEnable();
	static bool GetDraw();

private:
	static void Setup();
	static void Shotdown();

	static void Bind(Mesh& mesh);
	static void BindRTV();
	static void BindSRV();
	static void UnbindRTV();

	static void SetEnable(bool enable);
	static void SetDraw(bool draw);

	static ID3D11ShaderResourceView* GetSRV();
	static ID3D11DepthStencilView* GetDSV();

private:
	static ID3D11DepthStencilView* mDepthStencilView;
	static ID3D11ShaderResourceView* mShaderResourceView;
	static ID3D11DepthStencilState* mDepthStencilState;

	static bool mEnable;
	static bool mDraw;
};