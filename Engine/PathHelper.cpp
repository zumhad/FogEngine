#include "PathHelper.h"

#include "Application.h"
#include "CustomString.h"

String PathHelper::GetAssetsPath()
{
    String str = Application::GetPath();
    str += L"Engine\\Assets\\";

    return str;
}

String PathHelper::GetEnginePath()
{
    String str = Application::GetPath();
    str += L"Engine\\";

    return str;
}

String PathHelper::GetProjectPath()
{
    String str = Application::GetPath();
    str += L"Project\\";

    return str;
}

String PathHelper::GetEditorPath()
{
    String str = Application::GetPath();
    str += L"Editor\\";

    return str;
}