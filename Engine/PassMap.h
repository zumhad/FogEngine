#pragma once

#include "Core.h"

#include "ConstantBuffer.h"
#include "Mesh.h"
#include "Matrix.h"

#include <DirectXMath.h>

class Application;
class ObjectManager;
class BufferManager;

class FOG_API PassMap
{
	friend class Application;
	friend class ObjectManager;
	friend class BufferManager;

private:
	static void Setup();
	static void Shotdown();

	static void BindSRV();
	static void BindRTV();
	static void Bind(Mesh& mesh);
	static void UnbindRTV();

	static void UpdateShader();

private:
	static ID3D11VertexShader* mVertexShaderPrePass;
	static ID3D11PixelShader* mPixelShaderPrePass;
	static ID3D11InputLayout* mInputLayoutPrePass;

	static ID3D11VertexShader* mVertexShaderPass;
	static ID3D11PixelShader* mPixelShaderPass;
	static ID3D11InputLayout* mInputLayoutPass;

	struct PrePassBuffer
	{
		Matrix worldViewProj;
		Matrix world;
	};
	static ConstantBuffer<PrePassBuffer> mPrePassBuffer;

	struct PassBuffer
	{
		int width;
		int height; float pad[2];
	};
	static ConstantBuffer<PassBuffer> mPassBuffer;
};

