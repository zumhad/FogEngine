#include "CustomFile.h"

#include <cstdio>

File::File(String name, FileOpenMode mode) : mFile(INVALID_HANDLE_VALUE)
{
    Open(name, mode);
}

File::~File()
{
    Close();
}

void File::Open(String name, FileOpenMode mode)
{
    if (mode == FileOpenMode::Read)
    {
        mFile = CreateFile(name, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    }
    else
    {
        if (Exists(name))
            DeleteFile(name);

        mFile = CreateFile(name, GENERIC_WRITE, 0, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
    }
}

void File::Close()
{
    if (mFile == INVALID_HANDLE_VALUE)
        return;

    CloseHandle(mFile);

    mFile = INVALID_HANDLE_VALUE;
}

void File::Read(void* data, int size)
{
    DWORD bytesRead;
    bool result = ReadFile(mFile, data, static_cast<DWORD>(size), &bytesRead, NULL);

    FOG_ASSERT(result);
}

int File::Size()
{
    LARGE_INTEGER fileSize;
    GetFileSizeEx(mFile, &fileSize);

    return (int)fileSize.QuadPart;
}

void File::Write(const void* data, size_t size)
{
    DWORD bytesWritten = 0;
    WriteFile(mFile, data, static_cast<DWORD>(size), &bytesWritten, NULL);
}

bool File::Exists(String& name)
{
    if (FILE* file = _wfopen(name, L"r"))
    {
        fclose(file);
        return true;
    }

    return false;
}