#pragma once

#pragma warning(push)
#pragma warning(disable: 4251)

#include "Core.h"

#include "Control.h"
#include "CustomString.h"
#include "Color.h"
#include "ConstantBuffer.h"

#include <d3d11.h>

class GUI;
class Application;
class Button;

class FOG_API Text : public Control
{
	friend class GUI;
	friend class Application;
	friend class Button;

public:
	Text();
	~Text();

private:
	void Draw();
	void Update();

public:
	String text;
	Color color;
	int size;

private:
	ID3D11Buffer* mVertexBuffer;
	String mText;
	int mVertexSize;

	struct TextBuffer
	{
		Color color;
	};
	ConstantBuffer<TextBuffer> mTextBuffer;
};

#pragma warning(pop)