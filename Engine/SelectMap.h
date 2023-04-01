#pragma once

#include "Core.h"

#include "ConstantBuffer.h"

#include <d3d11.h>

class Direct3D;
class Application;
class ObjectManager;
class BufferManager;
class Picking;

class FOG_API SelectMap
{
	friend class Direct3D;
	friend class Application;
	friend class ObjectManager;
	friend class BufferManager;
	friend class Picking;

public:
	static bool GetEnable();
	static bool GetDraw();

private:
	static void Setup();
	static void Shotdown();

	static void Bind(Mesh& mesh);
	static void BindSRV();
	static void BindRTV();
	static void UnbindRTV();

	static void SetEnable(bool enable);
	static void SetDraw(bool draw);

	static ID3D11ShaderResourceView* GetSRV();
	static ID3D11RenderTargetView* GetRTV();

private:
	static ID3D11RenderTargetView* mRenderTargetView;
	static ID3D11ShaderResourceView* mShaderResourceView;

	struct SelectBuffer
	{
		unsigned int id; float pad[3];
	};

	static ConstantBuffer<SelectBuffer> mSelectBuffer;

	static bool mEnable;
	static bool mDraw;
};

