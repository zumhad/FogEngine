#include "Control.h"

#include "GUI.h"

Control::Control()
{
	alignm.horizontal = ALIGNM_LEFT;
	alignm.vertical = ALIGNM_TOP;

	x = 0, y = 0;
	width = 0, height = 0;
	enable = true;

	mFocus = false;
	mClick = false;
	mHover = false;
	mID = 0;
	mParent = 0;
	mRect = {};

	event = {};
}

Control::Control(const Control& c) : Control()
{
	x = c.x;
	y = c.y;
	width = c.width;
	height = c.height;
	enable = c.enable;
	event = c.event;
	alignm = c.alignm;
	color = c.color;
}