#pragma once

#include "Core.h"

enum class TypeControl
{
	Control,
	Button
};

class GUI;

class FOG_API Control
{
	friend class GUI;

public:
	virtual TypeControl GetType() { return TypeControl::Control; }

private:
	void SetFocus(bool focus) { mFocus = focus; }

protected:
	bool mFocus = false;
};

