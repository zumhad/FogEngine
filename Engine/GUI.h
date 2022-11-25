#pragma once

#include "Core.h"
#include "Button.h"

class FOG_API GUI
{
public:
	template<typename T>
	static void Add(T& b);

	static Control& Get(int index);
	static int Size() { return mSize; }

	static void Shotdown();
	static void Setup();
	static void Draw();
	static void Click(int x, int y);
	static bool ClickTest(int x, int y);

private:
	static Array<Control*> mArr;
	static int mSize;

	static ID3D11VertexShader* mVertexShader;
	static ID3D11PixelShader* mPixelShader;
	static ID3D11InputLayout* mVertexLayout;
	static ID3D11Buffer* mBuffer;
};

struct ConstantBuffer
{
	DirectX::XMFLOAT4X4 worldViewProj;
	DirectX::XMFLOAT4 material;
};

template<typename T>
void GUI::Add(T& b)
{
	T* t = new T(b);

	mArr.Add(t);
	mSize++;
}