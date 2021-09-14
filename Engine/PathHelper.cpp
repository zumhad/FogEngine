#include "PathHelper.h"


void PathHelper::GetAssetsPath(WCHAR* dest)
{
    WCHAR buff[MAX_PATH];

    GetModuleFileName(0, buff, _countof(buff));
    CString::Strcpy(dest, buff, 0, CString::FindStr(buff, L"Build") - 1);

    CString::Strcat(dest, L"Engine\\Assets\\");
}

void PathHelper::GetAssetsFullPath(WCHAR* dest, const WCHAR* asset)
{
    GetAssetsPath(dest);
    CString::Strcat(dest, asset);
}