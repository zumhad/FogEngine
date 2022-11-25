#pragma once

#include "Core.h"

#include "Direct3D.h"
#include "Trace.h"
#include "Control.h"

struct VertexBuf
{
	DirectX::XMFLOAT3 pos;
};

class FOG_API Button : public Control
{
public:
	TypeControl GetType() override { return TypeControl::Button; }

	Button() {}
	Button(Button& obj);
	~Button();

	DirectX::XMMATRIX GetWorldMatrix();
	void Bind();
	void Action();

public:
	DirectX::SimpleMath::Color color;
	int x = 0, y = 0;
	int width = 0, height = 0;
	void (*action)() = 0;

private:
	ID3D11Buffer* mVertexBuffer = 0;
	ID3D11Buffer* mIndexBuffer = 0;
};

