#pragma once

#include "Core.h"

#include "Button.h"

#include <vector>

class FOG_API GUI
{
public:
	template<typename T>
	static void Add(T& b);

	static Control& Get(int index);
	static int Size();

	static void Shotdown();
	static void Setup();
	static void Draw();
	static bool Click();
	static bool ClickTest();

private:
	struct Data;
	static Data* mData;
};

struct ConstantBuffer
{
	DirectX::XMFLOAT4X4 worldViewProj;
	DirectX::XMFLOAT4 material;
};

template FOG_API void GUI::Add<Control>(Control&);
template FOG_API void GUI::Add<Button>(Button&);