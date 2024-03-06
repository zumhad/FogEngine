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

class FOG_API ShadowMap
{
	friend class Application;
	friend class PipelineState;
	friend class LightMap;

private:
	static void Setup();
	static void Shotdown();
	static void UpdateTexture();

	static int GetResolution();
	static void SetResolution(int resolution);
	static void SetSplit(int index, float split);

	static void Clear(int index);
	static void UpdateBuffer(Model& mesh, int index);
	static ID3D11Buffer* const* GetBuffer();
	static void UpdateCascade(Vector3 dir);

	static Matrix GetMatrix();
	static Matrix GetMatrix(int index);
	static Vector4 GetOffset(int index);
	static Vector4 GetScale(int index);
	static float GetSplit(int index);

	static void CreateOffsetsAndScales(int index);
	static Matrix CreateSplits(Vector3 dir);
	static void CreateMatrix(Vector3 dir);

	static ID3D11ShaderResourceView* const* GetSRV();
	static ID3D11DepthStencilView* GetDSV(int index);

private:
	static Array<ID3D11DepthStencilView*> mDepthStencilView;
	static ID3D11ShaderResourceView* mShaderResourceView;

	struct ShadowBuffer;
	static ConstantBuffer<ShadowBuffer> mShadowBuffer;

	static struct
	{
		int resolution;
		Array<float> splits;
		Array<Vector4> offsets;
		Array<Vector4> scales;
		Array<Matrix> matrices;
		Matrix matrix;
	} mCascade;

	static Frustum mFrustum;
};

