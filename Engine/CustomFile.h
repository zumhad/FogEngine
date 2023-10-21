#pragma once

#include "Core.h"

#include "CustomString.h"

#include <windows.h>

enum class FileOpenMode
{
    Read,
    Write
};

class FOG_API File
{
public:
    File(String name, FileOpenMode mode);
    ~File();

    void Open(String name, FileOpenMode mode);
    void Close();
    void Read(void* data, int size);
    int Size();
    void Write(const void* data, size_t size);

    static bool Exists(String name);

private:
    HANDLE mFile;
};

