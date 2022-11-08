#include "PathHelper.h"

#include "Properties.h"

void PathHelper::GetAssetsPath(String& dest)
{
    dest = Singlton.path;
    dest += L"Engine\\Assets\\";
}

void PathHelper::GetEnginePath(String& dest)
{
    dest = Singlton.path;
    dest += L"Engine\\";
}

void PathHelper::GetProjectPath(String& dest)
{
    dest = Singlton.path;
    dest += L"Project\\";
}

void PathHelper::GetEditorPath(String& dest)
{
    dest = Singlton.path;
    dest += L"Editor\\";
}