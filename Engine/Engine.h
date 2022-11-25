#pragma once

#define FOG_LOAD_DLL

#include "Definitions.h"
#include "Application.h"
#include "Input.h"
#include "Time.h"
#include "Camera.h"

#include "GUI.h"
#include "CustomArray.h"
#include "ObjectManager.h"
#include "Cube.h"
#include "Light.h"
#include "Plane.h"
#include "Model.h"


#define CREATE_APPLICATION(x)\
int wmain()\
{\
	ApplicationEngine* app = new ApplicationEngine;\
	app->CheckDebug();\
	app->DefaultProp();\
	x();\
	int exitCode = app->Run();\
	SAFE_DELETE(app);\
	return exitCode;\
}