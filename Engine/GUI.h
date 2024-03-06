#pragma once

#pragma warning(push)
#pragma warning(disable : 4251)

#include "Core.h"

#include "CustomArray.h"
#include "ConstantBuffer.h"

#include <d3d11.h>

class FOG_API DepthStencilState;
class FOG_API SamplerState;
class FOG_API VertexShader;
class FOG_API PixelShader;
class FOG_API InputLayout;
class FOG_API Application;
class FOG_API Direct3D;
class FOG_API Button;

class FOG_API GUI
{
	friend class Application;
	friend class Direct3D;

public:
	static int Add(Button& button);
	static int AddChild(int parent, Button& child);
	static Button* GetWithID(int i);
	static Button* GetWithNumber(int i);
	static void DeleteWithID(int i);
	static int Size();

private:
	static void UpdateBuffer();
	static void Shotdown();
	static void Setup();
	static void Draw();
	static void Update();
	static bool IsEvent();
	static bool IsEnable(Button* button);
	static int BinarySearch(Array<Button*>& arr, int i);

private:
	static Array<Button*> mArr;
	static int mSize;

	static DepthStencilState mDepthStencilState;
	static SamplerState mSamplerState;
	static VertexShader mVertexShader;
	static PixelShader mPixelShader;
	static InputLayout mInputLayout;

	struct FrameBuffer;
	static ConstantBuffer<FrameBuffer> mFrameBuffer;
};

#pragma warning(pop)