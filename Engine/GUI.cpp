#include "GUI.h"

#include "Application.h"
#include "Shader.h"
#include "Camera.h"
#include "Cursor.h"
#include "PathHelper.h"
#include "Input.h"
#include "Timer.h"
#include "Utility.h"
#include "Direct3D.h"
#include "DepthStencilState.h"
#include "SamplerState.h"
#include "PixelShader.h"
#include "VertexShader.h"
#include "InputLayout.h"
#include "Button.h"
#include "Font.h"

struct GUI::FrameBuffer
{
	float invWidth;
	float invHeight; float pad[2];
};

Array<Button*> GUI::mArr;
int GUI::mSize = 0;

DepthStencilState GUI::mDepthStencilState;
SamplerState GUI::mSamplerState;
VertexShader GUI::mVertexShader;
PixelShader GUI::mPixelShader;
InputLayout GUI::mInputLayout;	
ConstantBuffer<GUI::FrameBuffer> GUI::mFrameBuffer;

void GUI::Setup()
{
	mDepthStencilState.Create(DepthStencilStateType::GUI);
	mSamplerState.Create(SamplerStateType::GUI);
	mFrameBuffer.Create();

	{
		mVertexShader.Create(L"Text.hlsl");
		mPixelShader.Create(L"Text.hlsl");

		Array<String> input;
		input.Add(L"TEXPOS");
		input.Add(L"TEXCOORD");
		mInputLayout.Create(mVertexShader.GetBlob(), input);
	}
}

int GUI::BinarySearch(Array<Button*>& arr, int i)
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

Button* GUI::GetWithID(int i)
{
	int res = BinarySearch(mArr, i);

	if (res == -1) return 0;

	return mArr[res];
}

Button* GUI::GetWithNumber(int i)
{
	if (i < 0 || i > mArr.Size() - 1) return 0;

	return mArr[i];
}

int GUI::Add(Button& control)
{
	Button* b = new Button(std::move(control));
	b->mID = mSize;

	mArr.Add(b);

	return mSize++;
}

int GUI::AddChild(int parent, Button& child)
{
	Button* p = GetWithID(parent);
	Button* c = new Button(std::move(child));

	p->mChild.Add(c);
	c->mParent = p;
	c->mID = mSize;

	mArr.Add(c);

	return mSize++;
}

bool GUI::IsEnable(Button* button)
{
	bool enable = false;

	Button* ptr = button;
	while (true)
	{
		if (!ptr->enable)
		{
			break;
		}
		else
		{
			if (ptr->mParent)
			{
				ptr = ptr->mParent;
				continue;
			}
			else
			{
				enable = ptr->enable;
				break;
			}
		}
	}

	return enable;
}

void GUI::Draw()
{
	static D3D11_VIEWPORT viewport{};
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;
	viewport.Width = (FLOAT)Application::GetEditorWidth();
	viewport.Height = (FLOAT)Application::GetEditorHeight();
	Direct3D::DeviceContext()->RSSetViewports(1, &viewport);

	Direct3D::DeviceContext()->ClearRenderTargetView(*Direct3D::GetRTV(), Application::GetEditorColor());

	Direct3D::DeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	//Direct3D::DeviceContext()->RSSetState(mRasterizerState.Get());
	Direct3D::DeviceContext()->PSSetSamplers(0, 1, mSamplerState.Get());
	Direct3D::DeviceContext()->OMSetRenderTargets(1, Direct3D::GetRTV(), 0);
	Direct3D::DeviceContext()->OMSetDepthStencilState(mDepthStencilState.Get(), 0);

	Direct3D::DeviceContext()->IASetInputLayout(mInputLayout.Get());
	Direct3D::DeviceContext()->VSSetShader(mVertexShader.Get(), 0, 0);
	Direct3D::DeviceContext()->PSSetShader(mPixelShader.Get(), 0, 0);

	Direct3D::DeviceContext()->PSSetShaderResources(0, 1, Font::GetSRV());

	UpdateBuffer();

	int size = Size();
	for (int i = 0; i < size; i++)
	{
		if (IsEnable(mArr[i]))
			mArr[i]->Draw();
	}

	Direct3D::DeviceContext()->PSSetShaderResources(0, 1, Direct3D::NullSRV());
}

void GUI::UpdateBuffer()
{
	static FrameBuffer buffer{};

	float invWidth = 1.0f / Application::GetEditorWidth();
	float invHeight = 1.0f / Application::GetEditorHeight();

	if (buffer.invWidth != invWidth || buffer.invHeight != invHeight)
	{
		buffer.invWidth = invWidth;
		buffer.invHeight = invHeight;
		mFrameBuffer.Bind(buffer);
	}

	Direct3D::DeviceContext()->VSSetConstantBuffers(0, 1, mFrameBuffer.Get());
}

void GUI::Update()
{
	bool findControl = false;

	int size = Size();
	for (int i = size - 1; i >= 0; i--)
	{
		Button* button = mArr[i];

		if (!IsEnable(button)) continue;

		int x = Cursor::GetPosition(CURSOR_X);
		int y = Cursor::GetPosition(CURSOR_Y);

		bool hover = (x >= button->mRect.left) && (x <= button->mRect.right) && (y >= button->mRect.top) && (y <= button->mRect.bottom);

		if (hover && !findControl)
		{
			findControl = true;

			if (!button->mHover)
			{
				if (button->event.hoverOn)
					button->event.hoverOn(*button);

				button->mHover = true;
			}

			if (button->event.hover)
				button->event.hover(*button);

			if (Input::Down(MOUSE_LEFT))
			{
				button->mClick = true;
			}

			if (Input::Up(MOUSE_LEFT))
			{
				if (button->mClick)
				{
					if (button->event.leftClick)
						button->event.leftClick(*button);

					if((button->event.focusOn))
						button->event.focusOn(*button);

					if (button->event.leftUp)
						button->event.leftUp(*button);

					button->mClick = false;
					button->mFocus = true;
				}
			}

			if (Input::GetAxis(MOUSE_SCROLL))
			{
				if (button->event.scroll)
					button->event.scroll(*button);
			}
		}
	    else
		{
			if (button->mHover)
			{
				if(button->event.hoverOff)
					button->event.hoverOff(*button);

				button->mHover = false;
			}

			if (Input::Down(MOUSE_LEFT) || Input::Down(MOUSE_RIGHT))
			{
				if (button->mFocus)
				{
					if ((button->event.focusOff))
						button->event.focusOff(*button);

					button->mFocus = false;
				}
			}

			if (Input::Up(MOUSE_LEFT))
			{
				if (button->mClick)
				{
					if (button->event.leftUp)
						button->event.leftUp(*button);

					button->mClick = false;
				}
			}
		}

		if (Input::Down(KEY_RETURN))
		{
			if (button->mFocus)
			{
				if ((button->event.focusOff))
					button->event.focusOff(*button);

				button->mFocus = false;
			}
		}

		if (Input::Press(MOUSE_LEFT))
		{
			if (button->mClick)
			{
				if (button->event.leftPress)
					button->event.leftPress(*button);
			}
		}

		if (button->mFocus)
		{
			if (button->event.focus)
				button->event.focus(*button);
		}
	}
}

bool GUI::IsEvent()
{
	int size = Size();
	for (int i = 0; i < size; i++)
	{
		Button* button = mArr[i];

		int x = Cursor::GetPosition(CURSOR_X);
		int y = Cursor::GetPosition(CURSOR_Y);

		bool hover = (x >= button->mRect.left) && (x <= button->mRect.right) && (y >= button->mRect.top) && (y <= button->mRect.bottom);

		if (hover)
		{
			if (button->event.hoverOn) return true;
			if (button->event.hover) return true;
			if (button->event.hoverOff) return true;
			if (button->event.leftClick) return true;
			if (button->event.rightClick) return true;
			if (button->event.rightPress) return true;
			if (button->event.leftPress) return true;
			if (button->event.focus) return true;
			if (button->event.focusOn) return true;
			if (button->event.focusOff) return true;
		}
	}

	return false;
}

int GUI::Size()
{ 
	return mArr.Size(); 
}

void GUI::Shotdown()
{
	mDepthStencilState.Release();
	mSamplerState.Release();
	mVertexShader.Release();
	mPixelShader.Release();
	mInputLayout.Release();
	mFrameBuffer.Release();

	int size = mArr.Size();
	for (int i = 0; i < size; i++)
	{
		SAFE_DELETE(mArr[i]);
	}
}