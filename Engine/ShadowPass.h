#pragma once

#include "Core.h"

#include "CustomArray.h"
#include "Frustum.h"

#include <d3d11.h>

template <typename T>
class FOG_API ConstantBuffer;

template <typename T>
class FOG_API Array;

class FOG_API Application;
class FOG_API PipelineState;
class FOG_API Model;
class FOG_API LightMap;
class FOG_API VertexShader;
class FOG_API PixelShader;
class FOG_API InputLayout;

class FOG_API ShadowPass
{
	friend class Application;
	friend class PipelineState;
	friend class LightMap;

private:
	static void Setup();
	static void Shotdown();
	static void Bind();

	static int GetResolution();
	static void SetResolution(int resolution);
	static void SetSplit(int index, float split);
	static void SetBias(float bias);
	static float GetBias();
	static float GetBlend();
	static void SetBlend(float blend);

	static void Clear();
	static void UpdateBuffer0(int index);
	static void UpdateBuffer1(Model* model);
	static void UpdateCascade(Vector3 dir);
	static void UpdateTexture();
	static void CreateOffsetsAndScales(int index);
	static Matrix CreateSplits(Vector3 dir);
	static void CreateMatrix(Vector3 dir);
	static void UpdateViewport();

	static Matrix GetMatrix();
	static Matrix GetMatrix(int index);
	static Vector4 GetOffset(int index);
	static Vector4 GetScale(int index);
	static float GetSplit(int index);

	static ID3D11ShaderResourceView* const* GetDepthSRV();
	static ID3D11DepthStencilView* GetDepthDSV(int index);

private:
	static Array<ID3D11DepthStencilView*> mDepthDSV;
	static ID3D11ShaderResourceView* mDepthSRV;

	static VertexShader mVertexShader;
	static InputLayout mInputLayout;

	struct Buffer0;
	static ConstantBuffer<Buffer0> mBuffer0;

	struct Buffer1;
	static ConstantBuffer<Buffer1> mBuffer1;

	static struct
	{
		int resolution;
		Array<float> splits;
		Array<Vector4> offsets;
		Array<Vector4> scales;
		Array<Matrix> matrices;
		Matrix matrix;
		float bias;
		float blend;
	} mCascade;

	static Frustum mFrustum;
};

