#include "GUI.h"

#include "Application.h"
#include "Shader.h"
#include "Camera.h"
#include "Cursor.h"
#include "PathHelper.h"
#include "Input.h"
#include "Button.h"
#include "Static.h"
#include "Timer.h"
#include "Text.h"
#include "Utility.h"
#include "Direct3D.h"

#include <d2d1_3.h>
#include <vector>

using namespace DirectX;
using namespace D2D1;

Array<Control*> GUI::mArr;
int GUI::mSize = 0;
Control* GUI::mFocusControl = 0;
ID2D1Factory3* GUI::mFactory = 0;
ID2D1RenderTarget* GUI::mRenderTarget = 0;

ID2D1RenderTarget* GUI::RenderTarget()
{
	return mRenderTarget;
}

void GUI::Setup()
{
	D2D1_FACTORY_OPTIONS factoryOptions{};

#ifdef _DEBUG
	factoryOptions.debugLevel = D2D1_DEBUG_LEVEL_INFORMATION;
#endif

	FOG_TRACE(D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, __uuidof(ID2D1Factory3), &factoryOptions, (void**)&mFactory));

	Resize();
}

void GUI::Release()
{
	SAFE_RELEASE(mRenderTarget);
}

void GUI::Resize()
{
	IDXGISurface* surface;
	FOG_TRACE(Direct3D::SwapChain()->GetBuffer(0, IID_PPV_ARGS(&surface)));

	D2D1_RENDER_TARGET_PROPERTIES properties{};
	properties.type = D2D1_RENDER_TARGET_TYPE_DEFAULT;
	properties.pixelFormat.alphaMode = D2D1_ALPHA_MODE_PREMULTIPLIED;
	properties.pixelFormat.format = DXGI_FORMAT_UNKNOWN;

	FOG_TRACE(mFactory->CreateDxgiSurfaceRenderTarget(surface, &properties, &mRenderTarget));
	SAFE_RELEASE(surface);
}

Control& GUI::Get(int id)
{ 
	if (id >= mSize || id < 0)
		Application::Close();

	return *(mArr[id]);
}

void GUI::Draw()
{
	static D2D1_COLOR_F color{};

	Color c = Application::GetEditorColor();
	color.r = c.r;
	color.g = c.g;
	color.b = c.b;
	color.a = c.a;

	mRenderTarget->BeginDraw();
	mRenderTarget->Clear(color);

	int size = Size();
	for (int i = 0; i < size; i++)
	{
		Control& control = Get(i);
		TypeControl type = control.GetType();

		switch (type)
		{
			case TypeControl::Button:
			{
				Button& button = (Button&)control;
				button.Draw();
				break;
			}

			case TypeControl::Static:
			{
				Static& st = (Static&)control;
				st.Draw();
				break;
			}

			case TypeControl::Text:
			{
				Text& text = (Text&)control;
				text.Draw(); // fix!!!
				break;
			}
		}
	}

	FOG_TRACE(mRenderTarget->EndDraw());
}

void GUI::Update()
{
	if (mFocusControl)
	{
		mFocusControl->SetFocus(false);
		mFocusControl = 0;
	}

	int size = Size();
	for (int i = 0; i < size; i++)
	{
		Control& control = Get(i);
		TypeControl type = control.GetType();

		if (type == TypeControl::Button)
		{
			Button& b = (Button&)control;

			int x = Cursor::GetPosition(CURSOR_X);
			int y = Cursor::GetPosition(CURSOR_Y);

			bool isFocus = (x >= b.mRect.left) && (x <= b.mRect.right) && (y >= b.mRect.top) && (y <= b.mRect.bottom);

			if (isFocus)
			{
				mFocusControl = &control;
				b.SetFocus(true);
				break;
			}
			else
				b.SetFocus(false);
		}
	}

	if (!Input::Down(MOUSE_LEFT)) return;

	if (mFocusControl)
	{
		Control& control = *(mFocusControl);
		Button& b = (Button&)control;
		TypeControl type = b.GetType();
		
		if (type == TypeControl::Button)
		{
			b.Action();
		}
	}
}

bool GUI::IsFocus()
{
	return mFocusControl;
}

int GUI::Size()
{ 
	return mSize; 
}

void GUI::Shotdown()
{
	SAFE_RELEASE(mRenderTarget);
	SAFE_RELEASE(mFactory);

	for (int i = 0; i < mSize; i++)
	{
		TypeControl type = mArr[i]->GetType();

		if (type == TypeControl::Control) delete (Control*)mArr[i];
		if (type == TypeControl::Button) delete (Button*)mArr[i];
		if (type == TypeControl::Static) delete (Static*)mArr[i];
		if (type == TypeControl::Text) delete (Text*)mArr[i];
	}
}