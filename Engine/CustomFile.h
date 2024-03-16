#pragma once

#include "Core.h"

#include "CustomString.h"

#include <windows.h>



class FOG_API File
{
public:
    enum class Mode
    {
        Read,
        Write
    };

    File(const String& name, Mode mode);
    ~File();

    void Open(const String& name, Mode mode);
    void Close();
    void Read(void* data, int size);
    int Size();
    void Write(const void* data, size_t size);

    static bool Exists(const String& name);

private:
    HANDLE mFile;
};

