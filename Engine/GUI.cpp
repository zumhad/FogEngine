#include "GUI.h"

#include "Application.h"
#include "Shader.h"
#include "Camera.h"
#include "Cursor.h"
#include "PathHelper.h"
#include "Input.h"
#include "Timer.h"
#include "Text.h"
#include "Utility.h"
#include "Direct3D.h"
#include "DepthStencilState.h"
#include "SamplerState.h"
#include "PixelShader.h"
#include "VertexShader.h"
#include "InputLayout.h"

Array<Control*> GUI::mArr;
int GUI::mSize = 0;
Control* GUI::mNull = 0;

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

bool GUI::IsEnable(Control& control)
{
	bool enable = false;

	Control* ptr = &control;
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
	UpdateViewport();

	Direct3D::DeviceContext()->ClearRenderTargetView(*Direct3D::GetRTV(), Application::GetEditorColor());

	Direct3D::DeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	//Direct3D::DeviceContext()->RSSetState(mRasterizerState.Get());
	Direct3D::DeviceContext()->PSSetSamplers(0, 1, mSamplerState.Get());
	Direct3D::DeviceContext()->OMSetRenderTargets(1, Direct3D::GetRTV(), 0);
	Direct3D::DeviceContext()->OMSetDepthStencilState(mDepthStencilState.Get(), 0);

	Direct3D::DeviceContext()->IASetInputLayout(mInputLayout.Get());
	Direct3D::DeviceContext()->VSSetShader(mVertexShader.Get(), 0, 0);
	Direct3D::DeviceContext()->PSSetShader(mPixelShader.Get(), 0, 0);

	UpdateBuffer();

	int size = Size();
	for (int i = 0; i < size; i++)
	{
		if (IsEnable(*mArr[i]))
			mArr[i]->Draw();
	}

	Direct3D::DeviceContext()->PSSetShaderResources(0, 1, Direct3D::NullSRV());
}

void GUI::UpdateBuffer()
{
	static FrameBuffer buffer{};
	buffer.invWidth = 1.0f / Application::GetEditorWidth();
	buffer.invHeight = 1.0f / Application::GetEditorHeight();

	mFrameBuffer.Bind(buffer);
	Direct3D::DeviceContext()->VSSetConstantBuffers(0, 1, mFrameBuffer.Get());
}

void GUI::UpdateViewport()
{
	int width = Application::GetEditorWidth();
	int height = Application::GetEditorHeight();

	static D3D11_VIEWPORT viewport{};
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;
	viewport.Width = (FLOAT)width;
	viewport.Height = (FLOAT)height;

	static D3D11_RECT rect{};
	rect.left = 0;
	rect.top = 0;
	rect.right = width;
	rect.bottom = height;

	Direct3D::DeviceContext()->RSSetViewports(1, &viewport);
	Direct3D::DeviceContext()->RSSetScissorRects(1, &rect);
}

void GUI::Update()
{
	int size = Size();
	for (int i = 0; i < size; i++)
	{
		Control& control = *(mArr[i]);

		if (!IsEnable(control)) continue;

		int x = Cursor::GetPosition(CURSOR_X);
		int y = Cursor::GetPosition(CURSOR_Y);

		bool hover = (x >= control.mRect.left) && (x <= control.mRect.right) && (y >= control.mRect.top) && (y <= control.mRect.bottom);

		if (hover)
		{
			if (!control.mHover)
			{
				if (control.event.hoverOn)
					control.event.hoverOn(control);

				control.mHover = true;
			}

			if (Input::Down(MOUSE_LEFT))
			{
				control.mClick = true;
			}

			if (Input::Up(MOUSE_LEFT))
			{
				if (control.mClick)
				{
					if (control.event.leftClick)
						control.event.leftClick(control);

					if((control.event.focusOn))
						control.event.focusOn(control);

					control.mClick = false;
					control.mFocus = true;
				}
			}
		}
	    else
		{
			if (control.mHover)
			{
				if(control.event.hoverOff)
					control.event.hoverOff(control);

				control.mHover = false;
			}

			if (Input::Down(MOUSE_LEFT) || Input::Down(MOUSE_RIGHT))
			{
				if (control.mFocus)
				{
					if ((control.event.focusOff))
						control.event.focusOff(control);

					control.mFocus = false;
				}
			}

			if (Input::Up(MOUSE_LEFT))
			{
				if (control.mClick)
				{
					control.mClick = false;
				}
			}
		}

		if (Input::Down(KEY_RETURN))
		{
			if (control.mFocus)
			{
				if ((control.event.focusOff))
					control.event.focusOff(control);

				control.mFocus = false;
			}
		}

		if (Input::Press(MOUSE_LEFT))
		{
			if (control.mClick)
			{
				if (control.event.leftPress)
					control.event.leftPress(control);
			}
		}

		if (control.mFocus)
		{
			if (control.event.focus)
				control.event.focus(control);
		}
	}
}

bool GUI::IsEvent()
{
	int size = Size();
	for (int i = 0; i < size; i++)
	{
		Control& control = *(mArr[i]);

		int x = Cursor::GetPosition(CURSOR_X);
		int y = Cursor::GetPosition(CURSOR_Y);

		bool hover = (x >= control.mRect.left) && (x <= control.mRect.right) && (y >= control.mRect.top) && (y <= control.mRect.bottom);

		if (hover)
		{
			if (control.event.hoverOn) return true;
			if (control.event.hoverOff) return true;
			if (control.event.leftClick) return true;
			if (control.event.rightClick) return true;
			if (control.event.rightPress) return true;
			if (control.event.leftPress) return true;
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
		delete mArr[i];
	}
}