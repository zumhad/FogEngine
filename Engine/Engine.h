#pragma once

#define FOG_LOAD_DLL
#define GAME_APP

#include "Definitions.h"

#include "CustomTypes.h"
#include "CustomString.h"
#include "Input.h"
#include "Application.h"
#include "ObjectManager.h"

#define CREATE_APPLICATION()\
int32 wmain()\
{\
	Edit::CheckDebug();\
	Editor* app = new Editor;\
	int32 exitCode = app->Run();\
	SAFE_DELETE(app);\
	return exitCode;\
}