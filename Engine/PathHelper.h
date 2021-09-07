#pragma once

#include "Core.h"

class PathHelper
{
public:
    static void GetAssetsPath(WCHAR* dest);
    static void GetAssetsFullPath(WCHAR* dest, const WCHAR* asset);

private:
    static const WCHAR* mAppName;
};

