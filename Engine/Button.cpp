#pragma warning (disable : 26495)

#include "Button.h"

#include "Timer.h"
#include "GUI.h"
#include "Application.h"
#include "Utility.h"
#include "Vector2.h"
#include "Direct3D.h"
#include "Font.h"

struct Button::TextBuffer
{
	Color color;
};

Button::Button()
{
	event = {};
	text.alignm = { ALIGNM_LEFT, ALIGNM_TOP };
	rect.alignm = { ALIGNM_LEFT, ALIGNM_TOP };
	text.color = Color(1.0f, 1.0f, 1.0f);
	rect.color = Color(0.0f, 0.0f, 0.0f);
	text.x = 0; text.y = 0;
	text.text = L"";
	text.size = 16;
	rect.x = 0; rect.y = 0;
	rect.width = 0; rect.height = 0;
	enable = true;
	data = 0;

	mNeedUpdateText = false;
	mVertexSize = 0;
	mFocus = false;
	mClick = false;
	mHover = false;
	mID = 0;
	mParent = 0;
	mRect = {};
	mStaticText = {};
	mStaticRect = {};
	mStaticWindow = {};
}

Button::Button(const Button& button) : Button()
{
	text = button.text;
	rect = button.rect;
	event = button.event;
	enable = button.enable;
	data = button.data;
}

Button::Button(Button&& button) noexcept : Button(button)
{
	mTextBuffer.Create();
}

Button::~Button()
{
	mVertexBuffer.Release();
	mTextBuffer.Release();
}

int Button::BinarySearch(const Array<Button*>& arr, int i)
{
	int size = arr.Size();
	int left = 0;
	int right = size - 1;

	while (left <= right)
	{
		int m = left + (right - left) / 2;

		if (arr[m]->mID == i)
			return m;

		if (arr[m]->mID < i)
			left = m + 1;
		else
			right = m - 1;
	}

	return -1;
}

Button* Button::GetParent()
{
	return mParent;
}

Button* Button::GetChildWithID(int i)
{
	int res = BinarySearch(mChild, i);

	if (res == -1) return 0;

	return mChild[res];
}

Button* Button::GetChildWithNumber(int i)
{
	if (i > mChild.Size() - 1 || i < 0) return 0;

	return mChild[i];
}

int Button::GetID()
{
	return mID;
}

int Button::GetChildCount()
{
	return mChild.Size();
}

int Button::GetChildNumber()
{
	if (!mParent) return -1;

	int res = BinarySearch(mParent->mChild, mID);

	return res;
}

void Button::Draw()
{
	NeedUpdateText();

	DrawRect();

	if (text.text == L"")
	{
		mNeedUpdateText = false;
		return;
	}

	UpdateText();
	DrawText();

	mNeedUpdateText = false;
}

void Button::DrawRect()
{
	if (mParent)
	{
		D3D11_RECT r = mParent->mRect;

		if (rect.alignm.horizontal == ALIGNM_LEFT)
		{
			mRect.left = rect.x + r.left;
			mRect.right = mRect.left + rect.width;
		}
		if (rect.alignm.horizontal == ALIGNM_CENTER_H)
		{
			mRect.left = r.left + int((r.right - r.left - rect.width) * 0.5f);
			mRect.right = mRect.left + rect.width;
		}
		if (rect.alignm.horizontal == ALIGNM_RIGHT)
		{
			mRect.left = rect.x + r.right - rect.width;
			mRect.right = mRect.left + rect.width;
		}

		if (rect.alignm.vertical == ALIGNM_TOP)
		{
			mRect.top = rect.y + r.top;
			mRect.bottom = mRect.top + rect.height;
		}
		if (rect.alignm.vertical == ALIGNM_CENTER_V)
		{
			mRect.top = r.top + int((r.bottom - r.top - rect.height) * 0.5f);
			mRect.bottom = mRect.top + rect.height;
		}
		if (rect.alignm.vertical == ALIGNM_BOTTOM)
		{
			mRect.top = rect.y + r.bottom - rect.height;
			mRect.bottom = mRect.top + rect.height;
		}

		if (mRect.left < r.left) mRect.left = r.left;
		if (mRect.right > r.right) mRect.right = r.right;
		if (mRect.top < r.top) mRect.top = r.top;
		if (mRect.bottom > r.bottom) mRect.bottom = r.bottom;

		if (mRect.left > r.right) mRect.left = r.right;
		if (mRect.right < r.left) mRect.right = r.left;
		if (mRect.top > r.bottom) mRect.top = r.bottom;
		if (mRect.bottom < r.top) mRect.bottom = r.top;
	}
	else
	{
		if (rect.alignm.horizontal == ALIGNM_LEFT)
		{
			mRect.left = rect.x;
			mRect.right = mRect.left + rect.width;
		}
		if (rect.alignm.horizontal == ALIGNM_CENTER_H)
		{
			mRect.left = rect.x + int((Application::GetEditorWidth() - rect.width) * 0.5f);
			mRect.right = mRect.left + rect.width;
		}
		if (rect.alignm.horizontal == ALIGNM_RIGHT)
		{
			mRect.left = rect.x + Application::GetEditorWidth() - rect.width;
			mRect.right = mRect.left + rect.width;
		}

		if (rect.alignm.vertical == ALIGNM_TOP)
		{
			mRect.top = rect.y;
			mRect.bottom = mRect.top + rect.height;
		}
		if (rect.alignm.vertical == ALIGNM_CENTER_V)
		{
			mRect.top = rect.y + int((Application::GetEditorHeight() - rect.height) * 0.5f);
			mRect.bottom = mRect.top + rect.height;
		}
		if (rect.alignm.vertical == ALIGNM_BOTTOM)
		{
			mRect.top = rect.y + Application::GetEditorHeight() - rect.height;
			mRect.bottom = mRect.top + rect.height;
		}

		if (mRect.left < 0) mRect.left = 0;
		if (mRect.right > Application::GetEditorWidth()) mRect.right = Application::GetEditorWidth();
		if (mRect.top < 0) mRect.top = 0;
		if (mRect.bottom > Application::GetEditorHeight()) mRect.bottom = Application::GetEditorHeight();
	}

	if (mRect.right <= mRect.left)
	{
		mRect.right = mRect.left;
		return;
	}
	if (mRect.bottom <= mRect.top)
	{
		mRect.bottom = mRect.top;
		return;
	}

	Direct3D::DeviceContext()->ClearView(*Direct3D::GetRTV(), rect.color, &mRect, 1);
}

void Button::DrawText()
{
	Direct3D::DeviceContext()->RSSetScissorRects(1, &mRect);

	if (mStaticText.color != text.color)
	{
		mStaticText.color = text.color;

		static TextBuffer buffer;
		buffer.color = text.color;
		mTextBuffer.Bind(buffer);
	}

	Direct3D::DeviceContext()->PSSetConstantBuffers(1, 1, mTextBuffer.Get());

	static UINT stride = mVertexSize;
	static UINT offset = 0;
	Direct3D::DeviceContext()->IASetVertexBuffers(0, 1, mVertexBuffer.Get(), &stride, &offset);
	Direct3D::DeviceContext()->Draw(4 * text.text.Size(), 0);
}

void Button::NeedUpdateText()
{
	if (mStaticText.alignm.horizontal != text.alignm.horizontal)
	{
		mStaticText.alignm.horizontal = text.alignm.horizontal;
		mNeedUpdateText = true;
	}
	if (mStaticText.alignm.vertical != text.alignm.vertical)
	{
		mStaticText.alignm.vertical = text.alignm.vertical;
		mNeedUpdateText = true;
	}
	if (mStaticText.x != text.x)
	{
		mStaticText.x = text.x;
		mNeedUpdateText = true;
	}
	if (mStaticText.y != text.y)
	{
		mStaticText.y = text.y;
		mNeedUpdateText = true;
	}
	if (mStaticText.text != text.text)
	{
		mStaticText.text = text.text;
		mNeedUpdateText = true;
	}
	if (mStaticText.size != text.size)
	{
		mStaticText.size = text.size;
		mNeedUpdateText = true;
	}
	if (mStaticRect.x != rect.x)
	{
		mStaticRect.x = rect.x;
		mNeedUpdateText = true;
	}
	if (mStaticRect.y != rect.y)
	{
		mStaticRect.y = rect.y;
		mNeedUpdateText = true;
	}
	if (mStaticRect.width != rect.width)
	{
		mStaticRect.width = rect.width;
		mNeedUpdateText = true;
	}
	if (mStaticRect.height != rect.height)
	{
		mStaticRect.height = rect.height;
		mNeedUpdateText = true;
	}
	if (mStaticRect.alignm.horizontal != rect.alignm.horizontal)
	{
		mStaticRect.alignm.horizontal = rect.alignm.horizontal;
		mNeedUpdateText = true;
	}
	if (mStaticRect.alignm.vertical != rect.alignm.vertical)
	{
		mStaticRect.alignm.vertical = rect.alignm.vertical;
		mNeedUpdateText = true;
	}

	int width, height;

	if (Application::IsGame())
	{
		width = Application::GetGameWidth();
		height = Application::GetGameHeight();
	}
	else
	{
		width = Application::GetEditorWidth();
		height = Application::GetEditorHeight();
	}

	if (mStaticWindow.width != width)
	{
		mStaticWindow.width = width;
		mNeedUpdateText = true;
	}
	if (mStaticWindow.height != height)
	{
		mStaticWindow.height = height;
		mNeedUpdateText = true;
	}

	if (mNeedUpdateText)
	{
		int size = GetChildCount();
		for (int i = 0; i < size; i++)
		{
			mChild[i]->mNeedUpdateText = mNeedUpdateText;
		}
	}
}

void Button::UpdateText()
{
	if (!mNeedUpdateText) return;

	struct Vertex
	{
		Vector2 pos;
		Vector2 uv;
	};

	int textSize = text.text.Size();
	int width = textSize * text.size;
	int height = text.size;
	float xOffset = 0.0f, yOffset = 0.0f;

	if (text.alignm.horizontal == ALIGNM_LEFT)
	{
		xOffset = (float)mRect.left + text.x;
	}
	if (text.alignm.horizontal == ALIGNM_CENTER_H)
	{
		xOffset = (float)mRect.left + (mRect.right - mRect.left - width) * 0.5f + text.x;
	}
	if (text.alignm.horizontal == ALIGNM_RIGHT)
	{
		xOffset = (float)mRect.right - width + text.x;
	}

	if (text.alignm.vertical == ALIGNM_TOP)
	{
		yOffset = (float)mRect.top + text.y;
	}
	if (text.alignm.vertical == ALIGNM_CENTER_V)
	{
		yOffset = (float)mRect.top + (mRect.bottom - mRect.top - height) * 0.5f + text.y;
	}
	if (text.alignm.vertical == ALIGNM_BOTTOM)
	{
		yOffset = (float)mRect.bottom - height + text.y;
	}

	static Array<Vertex> vertex;
	vertex.Resize(4 * textSize);
	mVertexSize = sizeof(Vertex);

	for (int i = 0; i < textSize; i++)
	{
		float xPos = i * text.size + xOffset;
		float yPos = yOffset;
		float uvWidth = 32.0f / 3008.0f;

		static Vertex v;

		v.pos = Vector2(xPos, yPos);
		v.uv = Vector2((text.text[i] - L'!') * uvWidth, 0.0f);
		vertex[i * 4 + 0] = v;

		v.pos = Vector2(xPos + text.size, yPos);
		v.uv = Vector2((text.text[i] - L'!' + 1) * uvWidth, 0.0f);
		vertex[i * 4 + 1] = v;

		v.pos = Vector2(xPos, yPos + text.size);
		v.uv = Vector2((text.text[i] - L'!') * uvWidth, 1.0f);
		vertex[i * 4 + 2] = v;

		v.pos = Vector2(xPos + text.size, yPos + text.size);
		v.uv = Vector2((text.text[i] - L'!' + 1) * uvWidth, 1.0f);
		vertex[i * 4 + 3] = v;
	}

	mVertexBuffer.Release();
	mVertexBuffer.Create(vertex);
}