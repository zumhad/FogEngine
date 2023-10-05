#pragma once

#pragma warning(push)
#pragma warning(disable: 4251)

#include "Core.h"

#include "CustomArray.h"
#include "Control.h"

#include <d2d1_3.h>
#include <d3d11.h>

class Application;
class Direct3D;
class Control;
class Button;
class Static;
class Text;

class FOG_API GUI
{
	friend class Application;
	friend class Direct3D;
	friend class Control;
	friend class Button;
	friend class Static;
	friend class Text;

public:
	template<typename T>
	static int Add(T& control);

	template<typename T>
	static int AddChild(int parent, T& child);

	static Control& Get(int id);
	static int Size();

private:
	static void Shotdown();
	static void Setup();
	static void Resize();
	static void Release();
	static void Draw();
	static void Update();
	static bool IsFocus();
	static ID2D1RenderTarget* RenderTarget();

private:
	static Array<Control*> mArr;
	static int mSize;

	static Control* mFocusControl;

	static ID2D1Factory3* mFactory;
	static ID2D1RenderTarget* mRenderTarget;
};

template<typename T>
int GUI::Add(T& control)
{
	T* t = new T(control);

	mArr.Add(t);
	int id = mSize++;

	return id;
}

template<typename T>
int GUI::AddChild(int parent, T& child)
{
	Control* c = &Get(parent);
	c->mChild = new T(child);
	c->mChild->mParent = c;

	mArr.Add(c->mChild);
	int id = mSize++;

	return id;
}

#pragma warning(pop)