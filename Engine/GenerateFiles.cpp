#include "Application.h"

#include "ObjectManager.h"
#include "PathHelper.h"
#include "Properties.h"
#include "Camera.h"

#include "Cube.h"
#include "Plane.h"
#include "Light.h"
#include "Model.h"

#include <shellapi.h>

#pragma warning(disable : 6387)

String mainStr = L"\
#include \"Engine.h\"\n\
#include \"Main.h\"\n\
void Setting()\n\
{\
	Singlton.foo.start = MyStart;\n\
	Singlton.isGame = true;\n\
	Singlton.cursorShow = false;\n\
	Singlton.game.color = { 1, 1, 1 };\n\
}\n\
#include \"ApplicationEngine.h\"\n\
CREATE_APPLICATION(Setting)";

void wsystem(const String& cmd)
{
	STARTUPINFOW si;
	PROCESS_INFORMATION pi;

	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));

	String command = L"/c ";
	command += cmd;

	_flushall();

	if (CreateProcess(_wgetenv(L"COMSPEC"), command, NULL, NULL, FALSE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi))
	{
		WaitForSingleObject(pi.hProcess, INFINITE);
		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);
	}
}

void Application::SaveProject()
{
	String filePath;

	PathHelper::GetProjectPath(filePath);
	filePath += L"src";
	wsystem(L"mkdir " + filePath);
	filePath += L"\\Main.cpp";

	FILE* mainFile = 0;
	mainFile = _wfopen(filePath, L"w");
	if (!mainFile) Application::Exit();
	fwprintf(mainFile, mainStr);
	fclose(mainFile);

	PathHelper::GetEditorPath(filePath);
	filePath += L"Main.h";

	FILE* headerFile = 0;
	headerFile = _wfopen(filePath, L"w");
	if (!headerFile) Application::Exit();

	String h;
	h = L"\
void MyStart()\n\
{\n\
Cube cube;\n\
DirectionalLight dir;\n\
PointLight point;\n\
Model model;\n\
Plane plane;\n";


	for (int i = 0; i < ObjectManager::Size(); i++)
	{
		Object& obj = ObjectManager::Get(i);
		TypeObject type = obj.GetType();

		if (type == TypeObject::Cube)
		{
			Cube& cube = (Cube&)obj;
			h += L"cube.material.ambient = " + String::ToStr(cube.material.ambient) + L";\n";
			h += L"cube.material.diffuse = " + String::ToStr(cube.material.diffuse) + L";\n";
			h += L"cube.material.specular = " + String::ToStr(cube.material.specular) + L";\n";
			h += L"cube.position = " + String::ToStr(cube.position) + L";\n";
			h += L"cube.rotation = " + String::ToStr(cube.rotation) + L";\n";
			h += L"cube.scale = " + String::ToStr(cube.scale) + L";\n";
			h += L"cube.lighting = " + String::ToStr(cube.lighting) + L";\n";
			h += L"ObjectManager::Add(cube);\n";
			continue;
		}

		if (type == TypeObject::Model)
		{
			Model& model = (Model&)obj;
			h += L"model.name = L\"" + model.name + L"\";\n";
			h += L"model.material.ambient = " + String::ToStr(model.material.ambient) + L";\n";
			h += L"model.material.diffuse = " + String::ToStr(model.material.diffuse) + L";\n";
			h += L"model.material.specular = " + String::ToStr(model.material.specular) + L";\n";
			h += L"model.position = " + String::ToStr(model.position) + L";\n";
			h += L"model.rotation = " + String::ToStr(model.rotation) + L";\n";
			h += L"model.scale = " + String::ToStr(model.scale) + L";\n";
			h += L"model.lighting = " + String::ToStr(model.lighting) + L";\n";
			h += L"ObjectManager::Add(model);\n";
			continue;
		}

		if (type == TypeObject::PointLight)
		{
			PointLight& point = (PointLight&)obj;
			h += L"point.att = " + String::ToStr(point.att) + L";\n";
			h += L"point.ambient = " + String::ToStr(point.ambient) + L";\n";
			h += L"point.diffuse = " + String::ToStr(point.diffuse) + L";\n";
			h += L"point.position = " + String::ToStr(point.position) + L";\n";
			h += L"point.range = " + String::ToStr(point.range) + L";\n";
			h += L"point.specular = " + String::ToStr(point.specular) + L";\n";
			h += L"ObjectManager::Add(point);\n";
			continue;
		}

		if (type == TypeObject::DirectionalLight)
		{
			DirectionalLight& dir = (DirectionalLight&)obj;
			h += L"dir.ambient = " + String::ToStr(dir.ambient) + L";\n";
			h += L"dir.direction = " + String::ToStr(dir.direction) + L";\n";
			h += L"dir.diffuse = " + String::ToStr(dir.diffuse) + L";\n";
			h += L"dir.specular = " + String::ToStr(dir.specular) + L";\n";
			h += L"ObjectManager::Add(dir);\n";
			continue;
		}
		
		if (type == TypeObject::Plane)
		{
			Plane& plane = (Plane&)obj;
			h += L"plane.material.ambient = " + String::ToStr(plane.material.ambient) + L";\n";
			h += L"plane.material.diffuse = " + String::ToStr(plane.material.diffuse) + L";\n";
			h += L"plane.material.specular = " + String::ToStr(plane.material.specular) + L";\n";
			h += L"plane.position = " + String::ToStr(plane.position) + L";\n";
			h += L"plane.rotation = " + String::ToStr(plane.rotation) + L";\n";
			h += L"plane.scale = " + String::ToStr(plane.scale) + L";\n";
			h += L"plane.lighting = " + String::ToStr(plane.lighting) + L";\n";
			h += L"ObjectManager::Add(plane);\n";
			continue;
		}
	}

	h += L"Camera::SetPosition(" + String::ToStr(Camera::GetPosition()) + L");\n";
	h += L"Camera::SetRotation(" + String::ToStr(Camera::GetRotation()) + L");\n";

	h += L"}\n";
	fwprintf(headerFile, h);
	fclose(headerFile);

	String buildPath = L"mkdir ";
	buildPath += Singlton.path;
	buildPath += L"Project\\build";
	wsystem(buildPath);

	String copyPath = L"copy ";
	copyPath += Singlton.path;
	copyPath += L"Build\\Release\\Engine.dll ";
	copyPath += Singlton.path;
	copyPath += L"Project\\build";
	wsystem(copyPath);

	String path = L"cd ";
	path += Singlton.path;
	path += L"Project & BuildTool.exe";
	wsystem(path);
}