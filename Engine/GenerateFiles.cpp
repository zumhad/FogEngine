#include "Application.h"

#include "CustomString.h"
#include "ObjectManager.h"
#include "PathHelper.h"
#include "Camera.h"
#include "Light.h"
#include "Mesh.h"

#include <shellapi.h>
#include <stdio.h>

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