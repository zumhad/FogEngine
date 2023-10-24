#pragma once

#pragma warning(push)
#pragma warning(disable : 4251)

#include "Core.h"

#include "CustomArray.h"
#include "Control.h"
#include "ConstantBuffer.h"

#include <d3d11.h>

class FOG_API DepthStencilState;
class FOG_API SamplerState;
class FOG_API VertexShader;
class FOG_API PixelShader;
class FOG_API InputLayout;
class FOG_API Application;
class FOG_API Direct3D;
class FOG_API Static;
class FOG_API Text;

class FOG_API GUI
{
	friend class Application;
	friend class Direct3D;
	friend class Control;
	friend class Static;
	friend class Text;

public:
	template<typename T>
	static int Add(T& control);

	template<typename T>
	static int AddChild(int parent, T& child);

	template <class T>
	static T* Get(int id);

	template <class T>
	static T* Get(Control& control);

	template <class T>
	static T* Get(Control* control);

	template <class T>
	static T* GetParent(int id);

	template <class T>
	static T* GetParent(Control& control);

	template <class T>
	static T* GetParent(Control* control);

	static int Size();

private:
	static void UpdateBuffer();
	static void UpdateViewport();
	static void Shotdown();
	static void Setup();
	static void Draw();
	static void Update();
	static bool IsEvent();
	static bool IsEnable(Control& control);

private:
	static Array<Control*> mArr;
	static int mSize;
	static Control* mNull;

	static DepthStencilState mDepthStencilState;
	static SamplerState mSamplerState;
	static VertexShader mVertexShader;
	static PixelShader mPixelShader;
	static InputLayout mInputLayout;

	struct FrameBuffer
	{
		float invWidth;
		float invHeight; float pad[2];
	};
	static ConstantBuffer<FrameBuffer> mFrameBuffer;
};

template <class T>
T* GUI::GetParent(int id)
{
	for (int i = 0; i < mArr.Size(); i++)
	{
		if (mArr[i]->mID == id)
		{
			return (T*)(mArr[i]->mParent);
		}
	}

	return 0;
}

template <class T>
T* GUI::GetParent(Control& control)
{
	return (T*)(control.mParent);
}

template <class T>
T* GUI::GetParent(Control* control)
{
	return (T*)(control->mParent);
}

template <class T>
T* GUI::Get(int id)
{
	for (int i = 0; i < mArr.Size(); i++)
	{
		if (mArr[i]->mID == id)
		{
			return (T*)(mArr[i]);
		}
	}

	return 0;
}

template <class T>
T* GUI::Get(Control& control)
{
	return (T*)(&control);
}

template <class T>
T* GUI::Get(Control* control)
{
	return (T*)(control);
}

template<typename T>
int GUI::Add(T& control)
{
	T* t = new T(std::move(control));
	t->mID = mSize;

	mArr.Add(t);

	return mSize++;
}

template<typename T>
int GUI::AddChild(int parent, T& child)
{
	Control* p = Get<Control>(parent);
	Control* c = new T(std::move(child));

	p->mChild.Add(c);
	c->mParent = p;
	c->mID = mSize;

	mArr.Add(c);

	return mSize++;
}

#pragma warning(pop)