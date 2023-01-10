#pragma once

#include "Core.h"

#include "Direct3D.h"
#include "Trace.h"
#include "Control.h"
#include "Color.h"
#include "Matrix.h"

struct VertexBuf
{
	DirectX::XMFLOAT3 pos;
};

class FOG_API Button : public Control
{
public:
	TypeControl GetType() override { return TypeControl::Button; }

	Button();
	Button(Button& obj);
	~Button();

	Matrix GetWorldMatrix();
	void Bind();
	void Action();

public:
	Color color;
	int x, y;
	int width, height;
	void (*action)();

private:
	ID3D11Buffer* mVertexBuffer;
	ID3D11Buffer* mIndexBuffer;
};

