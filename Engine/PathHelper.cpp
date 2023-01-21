#include "PathHelper.h"

#include "CustomString.h"
#include "Application.h"

void PathHelper::GetAssetsPath(String& dest)
{
    dest = Application::GetPath();
    dest += L"Engine\\Assets\\";
}

void PathHelper::GetEnginePath(String& dest)
{
    dest = Application::GetPath();
    dest += L"Engine\\";
}

void PathHelper::GetProjectPath(String& dest)
{
    dest = Application::GetPath();
    dest += L"Project\\";
}

void PathHelper::GetEditorPath(String& dest)
{
    dest = Application::GetPath();
    dest += L"Editor\\";
}