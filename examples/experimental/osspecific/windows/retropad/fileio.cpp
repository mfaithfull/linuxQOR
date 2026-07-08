// Text file load/save helpers with simple BOM detection for retropad.
#include "fileio.h"
#include <commdlg.h>
#include <strsafe.h>
#include <stdlib.h>

static TextEncoding DetectEncoding(const BYTE *data, DWORD size) {
    if (size >= 2 && data[0] == 0xFF && data[1] == 0xFE) {
        return ENC_UTF16LE;
    }
    if (size >= 2 && data[0] == 0xFE && data[1] == 0xFF) {
        return ENC_UTF16BE;
    }
    if (size >= 3 && data[0] == 0xEF && data[1] == 0xBB && data[2] == 0xBF) {
        return ENC_UTF8;
    }
    // Assume UTF-8 if it converts cleanly, else ANSI
    int wlen = MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, (LPCSTR)data, size, NULL, 0);
    return (wlen > 0) ? ENC_UTF8 : ENC_ANSI;
}

static BOOL DecodeToWide(const BYTE *data, DWORD size, TextEncoding encoding, TCHAR **outText, size_t *outLength) 
{
    int chars = 0;
    TCHAR *buffer = NULL;

    switch (encoding) 
    {
    case ENC_UTF16LE: {
        if (size < 2) return FALSE;
        DWORD byteOffset = (data[0] == 0xFF && data[1] == 0xFE) ? 2 : 0;
        DWORD wcharCount = (size - byteOffset) / 2;
        buffer = (TCHAR *)HeapAlloc(GetProcessHeap(), 0, (wcharCount + 1) * sizeof(TCHAR));
        if (!buffer) return FALSE;
        CopyMemory(buffer, data + byteOffset, wcharCount * sizeof(TCHAR));
        buffer[wcharCount] = L'\0';
        chars = (int)wcharCount;
        break;
    }
    case ENC_UTF16BE: {
        if (size < 2) return FALSE;
        DWORD byteOffset = (data[0] == 0xFE && data[1] == 0xFF) ? 2 : 0;
        DWORD wcharCount = (size - byteOffset) / 2;
        buffer = (TCHAR *)HeapAlloc(GetProcessHeap(), 0, (wcharCount + 1) * sizeof(TCHAR));
        if (!buffer) return FALSE;
        for (DWORD i = 0; i < wcharCount; ++i) {
            buffer[i] = (TCHAR)((data[byteOffset + i * 2] << 8) | data[byteOffset + i * 2 + 1]);
        }
        buffer[wcharCount] = L'\0';
        chars = (int)wcharCount;
        break;
    }
    case ENC_UTF8: {
        DWORD byteOffset = (size >= 3 && data[0] == 0xEF && data[1] == 0xBB && data[2] == 0xBF) ? 3 : 0;
        chars = MultiByteToWideChar(CP_UTF8, 0, (LPCSTR)(data + byteOffset), size - byteOffset, NULL, 0);
        if (chars <= 0) return FALSE;
        buffer = (TCHAR *)HeapAlloc(GetProcessHeap(), 0, (chars + 1) * sizeof(TCHAR));
        if (!buffer) return FALSE;
        //MultiByteToWideChar(CP_UTF8, 0, (LPCSTR)(data + byteOffset), size - byteOffset, buffer, chars);
        buffer[chars] = TEXT('\0');
        break;
    }
    case ENC_ANSI:
    default: {
        chars = MultiByteToWideChar(CP_ACP, 0, (LPCSTR)data, size, NULL, 0);
        if (chars <= 0) return FALSE;
        buffer = (TCHAR *)HeapAlloc(GetProcessHeap(), 0, (chars + 1) * sizeof(TCHAR));
        if (!buffer) return FALSE;
        //MultiByteToWideChar(CP_ACP, 0, (LPCSTR)data, size, buffer, chars);
        buffer[chars] = TEXT('\0');
        break;
    }
    }

    *outText = buffer;
    if (outLength) {
        *outLength = (size_t)chars;
    }
    return TRUE;
}

BOOL LoadTextFile(HWND owner, LPCTSTR path, TCHAR **textOut, size_t *lengthOut, TextEncoding *encodingOut) 
{
    *textOut = NULL;
    if (lengthOut) *lengthOut = 0;
    if (encodingOut) *encodingOut = ENC_UTF8;

    HANDLE file = CreateFile(path, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (file == INVALID_HANDLE_VALUE) {
        MessageBox(owner, TEXT("Unable to open file."), TEXT("retropad"), MB_ICONERROR);
        return FALSE;
    }

    LARGE_INTEGER size = {0};
    if (!GetFileSizeEx(file, &size) || size.QuadPart > (LONGLONG)UINT_MAX) {
        CloseHandle(file);
        MessageBox(owner, TEXT("Unsupported file size."), TEXT("retropad"), MB_ICONERROR);
        return FALSE;
    }

    DWORD bytes = (DWORD)size.QuadPart;
    BYTE *buffer = (BYTE *)HeapAlloc(GetProcessHeap(), 0, bytes + 3);
    if (!buffer) {
        CloseHandle(file);
        MessageBox(owner, TEXT("Out of memory."), TEXT("retropad"), MB_ICONERROR);
        return FALSE;
    }

    DWORD read = 0;
    BOOL ok = ReadFile(file, buffer, bytes, &read, NULL);
    CloseHandle(file);
    if (!ok) {
        HeapFree(GetProcessHeap(), 0, buffer);
        MessageBox(owner, TEXT("Failed reading file."), TEXT("retropad"), MB_ICONERROR);
        return FALSE;
    }

    if (read == 0) {
        TCHAR *empty = (TCHAR *)HeapAlloc(GetProcessHeap(), 0, sizeof(TCHAR));
        if (!empty) {
            HeapFree(GetProcessHeap(), 0, buffer);
            return FALSE;
        }
        empty[0] = TEXT('\0');
        *textOut = empty;
        if (lengthOut) *lengthOut = 0;
        if (encodingOut) *encodingOut = ENC_UTF8;
        HeapFree(GetProcessHeap(), 0, buffer);
        return TRUE;
    }

    TextEncoding enc = DetectEncoding(buffer, read);
    TCHAR *text = NULL;
    size_t len = 0;
    if (!DecodeToWide(buffer, read, enc, &text, &len)) {
        HeapFree(GetProcessHeap(), 0, buffer);
        MessageBox(owner, TEXT("Unable to decode file."), TEXT("retropad"), MB_ICONERROR);
        return FALSE;
    }

    HeapFree(GetProcessHeap(), 0, buffer);
    *textOut = text;
    if (lengthOut) *lengthOut = len;
    if (encodingOut) *encodingOut = enc;
    return TRUE;
}

static BOOL WriteUTF8WithBOM(HANDLE file, const WCHAR *text, size_t length) {
    static const BYTE bom[] = {0xEF, 0xBB, 0xBF};
    DWORD written = 0;
    if (!WriteFile(file, bom, sizeof(bom), &written, NULL)) {
        return FALSE;
    }
    int bytes = WideCharToMultiByte(CP_UTF8, 0, text, (int)length, NULL, 0, NULL, NULL);
    if (bytes <= 0) return FALSE;
    BYTE *buffer = (BYTE *)HeapAlloc(GetProcessHeap(), 0, bytes);
    if (!buffer) return FALSE;
    WideCharToMultiByte(CP_UTF8, 0, text, (int)length, (LPSTR)buffer, bytes, NULL, NULL);
    BOOL ok = WriteFile(file, buffer, bytes, &written, NULL);
    HeapFree(GetProcessHeap(), 0, buffer);
    return ok;
}

static BOOL WriteUTF16LE(HANDLE file, const WCHAR *text, size_t length) {
    static const BYTE bom[] = {0xFF, 0xFE};
    DWORD written = 0;
    if (!WriteFile(file, bom, sizeof(bom), &written, NULL)) {
        return FALSE;
    }
    return WriteFile(file, text, (DWORD)(length * sizeof(WCHAR)), &written, NULL);
}

static BOOL WriteANSI(HANDLE file, const TCHAR *text, size_t length) 
{
    /*
    int bytes = WideCharToMultiByte(CP_ACP, 0, text, (int)length, NULL, 0, NULL, NULL);
    if (bytes <= 0) return FALSE;
    BYTE *buffer = (BYTE *)HeapAlloc(GetProcessHeap(), 0, bytes);
    if (!buffer) return FALSE;
    WideCharToMultiByte(CP_ACP, 0, text, (int)length, (LPSTR)buffer, bytes, NULL, NULL);
    DWORD written = 0;
    BOOL ok = WriteFile(file, buffer, bytes, &written, NULL);
    HeapFree(GetProcessHeap(), 0, buffer);
    */
    return TRUE;//ok;
}

BOOL SaveTextFile(HWND owner, LPCTSTR path, LPCTSTR text, size_t length, TextEncoding encoding) 
{
    /*
    HANDLE file = CreateFile(path, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (file == INVALID_HANDLE_VALUE) {
        MessageBox(owner, TEXT("Unable to create file."), TEXT("retropad"), MB_ICONERROR);
        return FALSE;
    }

    BOOL ok = FALSE;
    switch (encoding) {
    case ENC_UTF16LE:
        ok = WriteUTF16LE(file, text, length);
        break;
    case ENC_ANSI:
        ok = WriteANSI(file, text, length);
        break;
    case ENC_UTF16BE:
        // Saving as UTF-16BE is uncommon; fall back to UTF-8 with BOM to preserve readability
        ok = WriteUTF8WithBOM(file, text, length);
        encoding = ENC_UTF8;
        break;
    case ENC_UTF8:
    default:
        ok = WriteUTF8WithBOM(file, text, length);
        break;
    }

    CloseHandle(file);
    if (!ok) {
        MessageBox(owner, TEXT("Failed writing file."), TEXT("retropad"), MB_ICONERROR);
    }
    return ok;
    */
   return TRUE;
}

BOOL OpenFileDialog(HWND owner, TCHAR *pathOut, DWORD pathLen) {
    pathOut[0] = L'\0';
    OPENFILENAME ofn = {0};
    TCHAR filter[] = TEXT("Text Files (*.txt)\0*.txt\0All Files (*.*)\0*.*\0\0");
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = owner;
    ofn.lpstrFilter = filter;
    ofn.lpstrFile = pathOut;
    ofn.nMaxFile = pathLen;
    ofn.Flags = OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_PATHMUSTEXIST;
    ofn.lpstrDefExt = TEXT("txt");
    return GetOpenFileName(&ofn);
}

BOOL SaveFileDialog(HWND owner, TCHAR *pathOut, DWORD pathLen) 
{
    OPENFILENAME ofn = {0};
    TCHAR filter[] = TEXT("Text Files (*.txt)\0*.txt\0All Files (*.*)\0*.*\0\0");
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = owner;
    ofn.lpstrFilter = filter;
    ofn.lpstrFile = pathOut;
    ofn.nMaxFile = pathLen;
    ofn.Flags = OFN_OVERWRITEPROMPT | OFN_PATHMUSTEXIST;
    ofn.lpstrDefExt = TEXT("txt");
    if (pathOut[0] == TEXT('\0')) 
    {
        StringCchCopy(pathOut, pathLen, TEXT("*.txt"));
    }
    return GetSaveFileName(&ofn);
}