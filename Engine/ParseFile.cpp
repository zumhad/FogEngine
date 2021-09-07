#include "ParseFile.h"

#include "EditHelper.h"

#include <stdio.h>
#include <shellapi.h>


const WCHAR* EngineProperties[]
{
	L"scene.rect.left",
	L"scene.rect.top",
	L"scene.rect.right",
	L"scene.rect.bottom",
	L"scene.color.r",
	L"scene.color.g",
	L"scene.color.b",
	L"cursor.enabled"
};

PropertiesType Properties::GetType(const WCHAR* str)
{
	if (CString::IsFindCh(str, L'.')) return PROP_FLOAT;
	if (CString::IsFind(str, L"false") || CString::IsFind(str, L"true")) return PROP_BOOL;
	if (CString::CountCh(str, L'\"') == 2) return PROP_STRING;
	if (CString::IsInt(str)) return PROP_INT;

	return PROP_ERROR;
}



Parse::Parse()
{
	WCHAR src[256];
	if (!IsEmptyProject(src))
		ParseProject(src);
}

bool Parse::IsEmptyProject(WCHAR* src)
{
	int argc;
	const WCHAR* line = GetCommandLine();
	WCHAR** argv = CommandLineToArgvW(line, &argc);

	if (argc == 1) return true; //is open .exe

	CString::Strcpy(src, argv[1]);
	WCHAR buff[10];
	uint16 srcLen = CString::Strlen(src);
	uint16 extLen = CString::Strlen(DOT_EXT);
	return !CString::Equal(CString::Strcpy(buff, src, srcLen - extLen, srcLen - 1), DOT_EXT);
}

void Parse::ParseProject(WCHAR* src)
{
	FILE* file;
	WCHAR arr[MAX_PATH];
	WCHAR prop[MAX_PATH];
	WCHAR cmd[MAX_PATH];

	file = _wfopen(src, L"r"); //open file
	if (file == 0) return;

	uint16 index;
	while (fgetws(arr, MAX_PATH, file)) //get line in file
	{
		if (CString::Strlen(arr) == 0) continue;

		if (CString::IsFindCh(arr, L'='))
		{
			index = CString::FindCh(arr, L'=');

			CString::Strcpy(prop, arr, 0, index - 1);
			CString::Strcpy(cmd, arr, index + 1, CString::Strlen(arr) - 1);
			ParseProperties(prop, cmd);
		}
	}
	fclose(file); //close file
}

void Parse::ParseProperties(WCHAR* prop, WCHAR* cmd)
{
	uint16 propCount = _countof(EngineProperties);

	uint16 index = 0;
	while (index < propCount)
	{
		if (CString::Equal(prop, EngineProperties[index])) break;
		index++;
	}

	if (index == propCount) return;

	PropertiesType type = Properties::GetType(cmd);
	switch (type)
	{
	case PROP_INT:
	{
		int32 var = CString::Atoi(cmd);

		switch (index)
		{
			case IP_SCENE_RECT_RIGHT: Edit::SetSceneRectRight((uint16)var); return;
			case IP_SCENE_RECT_LEFT: Edit::SetSceneRectLeft((uint16)var); return;
			case IP_SCENE_RECT_TOP: Edit::SetSceneRectTop((uint16)var); return;
			case IP_SCENE_RECT_BOTTOM: Edit::SetSceneRectBottom((uint16)var); return;
			case IP_SCENE_COLOR_R: Edit::SetSceneColorR(var); return;
			case IP_SCENE_COLOR_G: Edit::SetSceneColorG(var); return;
			case IP_SCENE_COLOR_B: Edit::SetSceneColorB(var); return;
		}
		return;
	}

	case PROP_FLOAT:
	{
		return;
	}

	case PROP_BOOL:
	{
		bool var = CString::Atob(cmd);

		switch (index)
		{
			case IP_CURSOR_ENABLED: Edit::SetCursorEnabled(var); return;
		}
		return;
	}

	case PROP_STRING:
	{
		return;
	}
	}
}