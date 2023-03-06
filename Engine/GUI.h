#pragma once

#include "Core.h"

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
	struct Data;
	static Data* mData;
};

template FOG_API int GUI::Add<Control>(Control&);
template FOG_API int GUI::Add<Button>(Button&);
template FOG_API int GUI::Add<Static>(Static&);
template FOG_API int GUI::Add<Text>(Text&);

template FOG_API int GUI::AddChild<Control>(int, Control&);
template FOG_API int GUI::AddChild<Button>(int, Button&);
template FOG_API int GUI::AddChild<Static>(int, Static&);
template FOG_API int GUI::AddChild<Text>(int, Text&);