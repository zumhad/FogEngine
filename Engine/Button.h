#pragma once

#pragma warning(push)
#pragma warning(disable : 4251)

#include "Core.h"

#include "ConstantBuffer.h"
#include "Color.h"
#include "CustomString.h"
#include "CustomArray.h"

#include <d3d11.h>

#undef DrawText

enum HorizontalAlignm
{
	ALIGNM_LEFT,
	ALIGNM_RIGHT,
	ALIGNM_CENTER_H
};

enum VerticalAlignm
{
	ALIGNM_TOP,
	ALIGNM_BOTTOM,
	ALIGNM_CENTER_V
};

class FOG_API GUI;

class FOG_API Button
{
	friend class GUI;

public:
	Button();
	Button(const Button& button);
	Button(Button&& button) noexcept;
	~Button();

	Button* GetParent();
	Button* GetChildWithID(int i);
	Button* GetChildWithNumber(int i);
	int GetID();
	int GetChildCount();
	int GetChildNumber();

private:
	void NeedUpdateText();
	void Draw();
	void DrawRect();
	void DrawText();
	void UpdateText();
	int BinarySearch(Array<Button*>& arr, int i);

public:
	struct
	{
		void(*hoverOn)(Button&);
		void(*hover)(Button&);
		void(*hoverOff)(Button&);
		void(*leftClick)(Button&);
		void(*rightClick)(Button&);
		void(*leftPress)(Button&);
		void(*rightPress)(Button&);
		void(*leftUp)(Button&);
		void(*rightUp)(Button&);
		void(*leftDown)(Button&);
		void(*rightDown)(Button&);
		void(*focusOn)(Button&);
		void(*focus)(Button&);
		void(*focusOff)(Button&);
		void(*scroll)(Button&);
	} event;

	struct
	{
		struct
		{
			HorizontalAlignm horizontal;
			VerticalAlignm vertical;
		}alignm;

		Color color;
		int x, y;
		int width, height;
	}rect;

	struct
	{
		struct
		{
			HorizontalAlignm horizontal;
			VerticalAlignm vertical;
		}alignm;

		Color color;
		int x, y;
		String text;
		int size;
	}text;

	bool enable;
	int data;

private:
	ID3D11Buffer* mVertexBuffer;
	int mVertexSize;

	struct
	{
		struct
		{
			HorizontalAlignm horizontal;
			VerticalAlignm vertical;
		}alignm;

		Color color;
		int x, y;
		String text;
		int size;
	}mStaticText;

	struct
	{
		struct
		{
			HorizontalAlignm horizontal;
			VerticalAlignm vertical;
		}alignm;

		Color color;
		int x, y;
		int width, height;
	}mStaticRect;

	struct TextBuffer;
	ConstantBuffer<TextBuffer> mTextBuffer;

	Array<Button*> mChild;
	Button* mParent;

	D3D11_RECT mRect;
	int mID;
	bool mHover;
	bool mClick;
	bool mFocus;
	bool mNeedUpdateText;
};

#pragma warning(pop)