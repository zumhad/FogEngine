#include "Control.h"

#include "GUI.h"

Control::Control()
{
	alignm.horizontal = ALIGNM_LEFT;
	alignm.vertical = ALIGNM_TOP;

	x = 0, y = 0;
	width = 0, height = 0;

	mFocus = false;

	mChildCount = 0;
	mChild = 0;
	mParent = 0;
	mRect = {};
}