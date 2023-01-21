#pragma once

#include "Core.h"

#include <d2d1_3.h>
#include <d3d11.h>

class Control;
class Button;
class Application;
class Direct3D;

class FOG_API GUI
{
	friend class Direct3D;
	friend class Control;
	friend class Button;
	friend class Application;

public:
	template<typename T>
	static void Add(T& b);

	static Control& Get(int index);
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
	struct Data;
	static Data* mData;
};

template FOG_API void GUI::Add<Control>(Control&);
template FOG_API void GUI::Add<Button>(Button&);