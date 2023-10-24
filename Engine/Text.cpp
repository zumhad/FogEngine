#include "Text.h"

#include "Timer.h"
#include "GUI.h"
#include "Application.h"
#include "Utility.h"
#include "Vector2.h"
#include "Direct3D.h"
#include "Font.h"

struct Text::TextBuffer
{
	Color color;
};

Text::Text() : Control()
{
	text = L"";
	size = 32;
	color = Color(1.0f, 1.0f, 1.0f);

	mVertexSize = 0;
	mVertexBuffer = 0;
	mText = L"";
}

Text::Text(const Text& t) : Control(t)
{
	text = t.text;
	size = t.size;
}

Text::Text(Text&& t) noexcept : Text(t)
{
	mVertexBuffer = 0;
	mTextBuffer.Create();
}

Text::~Text()
{
	SAFE_RELEASE(mVertexBuffer);
	mTextBuffer.Release();
}

void Text::Draw()
{
	if (text == L"") return;

	Update();

	static TextBuffer buffer;
	buffer.color = color;
	mTextBuffer.Bind(buffer);

	Direct3D::DeviceContext()->PSSetConstantBuffers(1, 1, mTextBuffer.Get());
	Direct3D::DeviceContext()->RSSetScissorRects(1, &mRect);

	Font::Bind();

	static UINT stride = mVertexSize;
	static UINT offset = 0;
	Direct3D::DeviceContext()->IASetVertexBuffers(0, 1, &mVertexBuffer, &stride, &offset);
	Direct3D::DeviceContext()->Draw(4 * mText.Size(), 0);
}

void Text::Update()
{
	mText = text;
	width = mText.Size() * size;
	height = size;

	struct Vertex
	{
		Vector2 pos;
		Vector2 uv;
	};

	static Array<Vertex> vertex;
	vertex.Resize(4 * mText.Size());
	mVertexSize = sizeof(Vertex);

	int xOffset, yOffset;

	if (mParent)
	{
		D3D11_RECT rect = mParent->mRect;

		if (alignm.horizontal == ALIGNM_LEFT)
		{
			mRect.left = x + rect.left;
			mRect.right = mRect.left + width;
		}
		if (alignm.horizontal == ALIGNM_CENTER_H)
		{
			mRect.left = rect.left + int((rect.right - rect.left - width) * 0.5f);
			mRect.right = mRect.left + width;
		}
		if (alignm.horizontal == ALIGNM_RIGHT)
		{
			mRect.left = x + rect.right - width;
			mRect.right = mRect.left + width;
		}

		if (alignm.vertical == ALIGNM_TOP)
		{
			mRect.top = y + rect.top;
			mRect.bottom = mRect.top + height;
		}
		if (alignm.vertical == ALIGNM_CENTER_V)
		{
			mRect.top = rect.top + int((rect.bottom - rect.top - height) * 0.5f);
			mRect.bottom = mRect.top + height;
		}
		if (alignm.vertical == ALIGNM_BOTTOM)
		{
			mRect.top = y + rect.bottom - height;
			mRect.bottom = mRect.top + height;
		}

		xOffset = mRect.left;
		yOffset = mRect.top;

		if (mRect.left < rect.left) mRect.left = rect.left;
		if (mRect.right > rect.right) mRect.right = rect.right;
		if (mRect.top < rect.top) mRect.top = rect.top;
		if (mRect.bottom > rect.bottom) mRect.bottom = rect.bottom;
	}
	else
	{
		if (alignm.horizontal == ALIGNM_LEFT)
		{
			mRect.left = x;
			mRect.right = mRect.left + width;
		}
		if (alignm.horizontal == ALIGNM_CENTER_H)
		{
			mRect.left = x + int((Application::GetEditorWidth() - width) * 0.5f);
			mRect.right = mRect.left + width;
		}
		if (alignm.horizontal == ALIGNM_RIGHT)
		{
			mRect.left = x + Application::GetEditorWidth() - width;
			mRect.right = mRect.left + width;
		}

		if (alignm.vertical == ALIGNM_TOP)
		{
			mRect.top = y;
			mRect.bottom = mRect.top + height;
		}
		if (alignm.vertical == ALIGNM_CENTER_V)
		{
			mRect.top = y + int((Application::GetEditorHeight() - height) * 0.5f);
			mRect.bottom = mRect.top + height;
		}
		if (alignm.vertical == ALIGNM_BOTTOM)
		{
			mRect.top = y + Application::GetEditorHeight() - height;
			mRect.bottom = mRect.top + height;
		}

		xOffset = mRect.left;
		yOffset = mRect.top;

		if (mRect.left < 0) mRect.left = 0;
		if (mRect.right > Application::GetEditorWidth()) mRect.right = Application::GetEditorWidth();
		if (mRect.top < 0) mRect.top = 0;
		if (mRect.bottom > Application::GetEditorHeight()) mRect.bottom = Application::GetEditorHeight();
	}

	int textSize = mText.Size();
	for (int i = 0; i < textSize; i++)
	{
		float xPos = (float)i * size + xOffset;
		float yPos = (float)yOffset;
		float uvWidth = 32.0f / 3008.0f;

		static Vertex v;

		v.pos = Vector2(xPos, yPos);
		v.uv = Vector2((text[i] - L'!') * uvWidth, 0.0f);
		vertex[i * 4 + 0] = v;

		v.pos = Vector2(xPos + size, yPos);
		v.uv = Vector2((text[i] - L'!' + 1) * uvWidth, 0.0f);
		vertex[i * 4 + 1] = v;

		v.pos = Vector2(xPos, yPos + size);
		v.uv = Vector2((text[i] - L'!') * uvWidth, 1.0f);
		vertex[i * 4 + 2] = v;

		v.pos = Vector2(xPos + size, yPos + size);
		v.uv = Vector2((text[i] - L'!' + 1) * uvWidth, 1.0f);
		vertex[i * 4 + 3] = v;
	}

	SAFE_RELEASE(mVertexBuffer);

	D3D11_BUFFER_DESC bd = {};
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = (UINT)sizeof(vertex[0]) * (UINT)vertex.Size();
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	D3D11_SUBRESOURCE_DATA InitData = {};
	InitData.pSysMem = vertex.Data();
	FOG_TRACE(Direct3D::Device()->CreateBuffer(&bd, &InitData, &mVertexBuffer));
}