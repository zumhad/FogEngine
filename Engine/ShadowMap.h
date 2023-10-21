#pragma once

#pragma warning(push)
#pragma warning(disable: 4251)

#include "Core.h"

#include "ConstantBuffer.h"
#include "Matrix.h"
#include "Mesh.h"

class Application;
class PipelineState;

class FOG_API ShadowMap
{
	friend class Application;
	friend class PipelineState;

public:
	static int GetResolution();
	static void SetResolution(int resolution);

private:
	static void Setup();
	static void Shotdown();
	static void UpdateTexture();

	static void Clear();
	static void UpdateBuffer(Mesh& mesh);
	static ID3D11Buffer* const* GetBuffer();

	static ID3D11ShaderResourceView* const* GetSRV();
	static ID3D11DepthStencilView* GetDSV();

private:
	static ID3D11DepthStencilView* mDepthStencilView;
	static ID3D11ShaderResourceView* mShaderResourceView;

	struct ShadowBuffer
	{
		Matrix worldViewProj;
	};
	static ConstantBuffer<ShadowBuffer> mShadowBuffer;

	static int mResolution;
};

#pragma warning(pop)