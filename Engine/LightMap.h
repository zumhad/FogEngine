#pragma once

#pragma warning(push)
#pragma warning(disable: 4251)

#include "Core.h"

#include "ConstantBuffer.h"
#include "Color.h"
#include "Vector3.h"
#include "Light.h"
#include "Matrix.h"

class Application;
class PipelineState;

class FOG_API LightMap
{
	friend class Application;
	friend class PipelineState;

private:
	static void Setup();
	static void Shotdown();

	static void UpdateBuffer(DirectionalLight& dir);
	static void UpdateBuffer(PointLight& point);
	static void UpdateBuffer();
	static void Clear();

	static ID3D11ShaderResourceView* const* GetSRV();
	static ID3D11RenderTargetView* const* GetRTV();
	static ID3D11Buffer* const* GetBuffer();

private:
	static ID3D11RenderTargetView* mRenderTargetView;
	static ID3D11ShaderResourceView* mShaderResourceView;

	struct LightBuffer
	{
		struct
		{
			Color color;
			Vector3 dir;
			float power;
		} dirLight[16];
		struct
		{
			Color color;
			Vector3 position;
			float radius;
			float power; float pad[3];
		} pointLight[16];
		Vector3 cameraPos;
		int dirCount;
		int pointCount;
		int width;
		int height;
		float texelSize;
		Matrix shadowTransform;
	};
	static ConstantBuffer<LightBuffer> mLightBuffer;
	static LightBuffer mBuffer;
};

#pragma warning(pop)