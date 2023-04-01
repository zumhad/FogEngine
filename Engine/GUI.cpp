#include "GUI.h"

#include "Application.h"
#include "Trace.h"
#include "Shader.h"
#include "Camera.h"
#include "Cursor.h"
#include "PathHelper.h"
#include "Input.h"
#include "Button.h"
#include "Static.h"
#include "Timer.h"
#include "Text.h"

#include <d2d1_3.h>
#include <vector>

using namespace DirectX;
using namespace D2D1;
using namespace std;

GUI::Data* GUI::mData = 0;

struct GUI::Data
{
public:
	Data();
	~Data();

public:
	vector<Control*> v;
	int size;
	Control* focusControl;

	ID2D1Factory3* factory;
	ID2D1RenderTarget* renderTarget;
}; 

GUI::Data::Data() : size(0), renderTarget(0), factory(0), focusControl(0)
{
	D2D1_FACTORY_OPTIONS factoryOptions{};

#ifdef _DEBUG
	factoryOptions.debugLevel = D2D1_DEBUG_LEVEL_INFORMATION;
#endif

	FOG_TRACE(D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, __uuidof(ID2D1Factory3), &factoryOptions, (void**)&factory));
}

ID2D1RenderTarget* GUI::RenderTarget()
{
	return mData->renderTarget;
}

void GUI::Setup()
{
	mData = new Data;

	Resize();
}

void GUI::Release()
{
	SAFE_RELEASE(mData->renderTarget);
}

void GUI::Resize()
{
	IDXGISurface* surface;
	FOG_TRACE(Direct3D::SwapChain()->GetBuffer(0, IID_PPV_ARGS(&surface)));

	D2D1_RENDER_TARGET_PROPERTIES properties{};
	properties.type = D2D1_RENDER_TARGET_TYPE_DEFAULT;
	properties.pixelFormat.alphaMode = D2D1_ALPHA_MODE_PREMULTIPLIED;
	properties.pixelFormat.format = DXGI_FORMAT_UNKNOWN;

	FOG_TRACE(mData->factory->CreateDxgiSurfaceRenderTarget(surface, &properties, &mData->renderTarget));
	SAFE_RELEASE(surface);
}

Control& GUI::Get(int id)
{ 
	if (id >= mData->size || id < 0)
		Application::Close();

	return *(mData->v[id]);
}

void GUI::Draw()
{
	static D2D1_COLOR_F color{};

	Color c = Application::GetEditorColor();
	color.r = c.r;
	color.g = c.g;
	color.b = c.b;
	color.a = c.a;

	mData->renderTarget->BeginDraw();
	mData->renderTarget->Clear(color);

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
				text.Draw();
				break;
			}
		}
	}

	FOG_TRACE(mData->renderTarget->EndDraw());
}

void GUI::Update()
{
	if (mData->focusControl)
	{
		mData->focusControl->SetFocus(false);
		mData->focusControl = 0;
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
				mData->focusControl = &control;
				b.SetFocus(true);
				break;
			}
			else
				b.SetFocus(false);
		}
	}

	if (!Input::Down(MOUSE_LEFT)) return;

	if (mData->focusControl)
	{
		Control& control = *(mData->focusControl);
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
	return mData->focusControl;
}

int GUI::Size()
{ 
	return mData->size; 
}

void GUI::Shotdown()
{
	SAFE_DELETE(mData);
}

GUI::Data::~Data()
{
	SAFE_RELEASE(renderTarget);
	SAFE_RELEASE(factory);

	for (int i = 0; i < size; i++)
	{
		TypeControl type = v[i]->GetType();

		if (type == TypeControl::Control) delete (Control*)v[i];
		if (type == TypeControl::Button) delete (Button*)v[i];
		if (type == TypeControl::Static) delete (Static*)v[i];
		if (type == TypeControl::Text) delete (Text*)v[i];
	}
}

template<typename T>
int GUI::Add(T& control)
{
	T* t = new T(control);

	mData->v.push_back(t);
	int id = mData->size++;

	return id;
}

template<typename T>
int GUI::AddChild(int parent, T& child)
{
	Control* c = &Get(parent);

	//c->mChild = new T
	c->mChild = new T(child);

	c->mChild->mParent = c;

	mData->v.push_back(c->mChild);
	int id = mData->size++;

	return id;
}