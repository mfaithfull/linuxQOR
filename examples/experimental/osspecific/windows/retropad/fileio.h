// File loading and saving helpers for retropad
#pragma once

#include "src/configuration/configuration.h"

#include "src/platform/os/windows/api_layer/kernel/kernel32.h"

typedef enum TextEncoding {
    ENC_UTF8 = 1,
    ENC_UTF16LE = 2,
    ENC_UTF16BE = 3,
    ENC_ANSI = 4
} TextEncoding;

typedef struct FileResult {
    TCHAR path[MAX_PATH];
    TextEncoding encoding;
} FileResult;

BOOL OpenFileDialog(HWND owner, TCHAR *pathOut, DWORD pathLen);
BOOL SaveFileDialog(HWND owner, TCHAR *pathOut, DWORD pathLen);

BOOL LoadTextFile(HWND owner, LPCTSTR path, TCHAR **textOut, size_t *lengthOut, TextEncoding *encodingOut);
BOOL SaveTextFile(HWND owner, LPCTSTR path, LPCTSTR text, size_t length, TextEncoding encoding);