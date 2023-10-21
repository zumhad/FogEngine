#pragma once

#include "Core.h"

#include <d3d11.h>

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

class GUI;
class Static;
class Text;

class FOG_API Control
{
	friend class GUI;
	friend class Static;
	friend class Text;

private:
	virtual void Draw() = 0;
	virtual ~Control() {};

	Control();

public:
	struct
	{
		HorizontalAlignm horizontal;
		VerticalAlignm vertical;
	} alignm;

	struct
	{
		void(*hoverOn)(Control&);
		void(*hoverOff)(Control&);
		void(*leftClick)(Control&);
		void(*rightClick)(Control&);
		void(*leftPress)(Control&);
		void(*rightPress)(Control&);
		void(*focusOn)(Control&);
		void(*focus)(Control&);
		void(*focusOff)(Control&);
	} event;

	int x, y;
	int width, height;
	bool enable;

protected:
	Control* mChild;
	Control* mParent;
	D3D11_RECT mRect;
	int mID;
	bool mHover;
	bool mClick;
	bool mFocus;
};

