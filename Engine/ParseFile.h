#pragma once

#include "Core.h"

extern const WCHAR* EngineProperties[];

enum PropertiesType
{
	PROP_INT,
	PROP_FLOAT,
	PROP_BOOL,
	PROP_STRING,
	PROP_ERROR
};

class Properties
{
public:
	static PropertiesType GetType(const WCHAR* str);
};

enum IndexEngineProperties
{
	IP_SCENE_RECT_LEFT,
	IP_SCENE_RECT_TOP,
	IP_SCENE_RECT_RIGHT,
	IP_SCENE_RECT_BOTTOM,
	IP_SCENE_COLOR_R,
	IP_SCENE_COLOR_G,
	IP_SCENE_COLOR_B,
	IP_CURSOR_ENABLED
};

class Parse
{
public:
	Parse();

private:
	bool IsEmptyProject(WCHAR* src);
	void ParseProject(WCHAR* src);
	void ParseProperties(WCHAR* prop, WCHAR* cmd);
};