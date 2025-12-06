#include "src/configuration/configuration.h"

#include <string>
#include <strsafe.h>
#include <tchar.h>
#include "model.h"
#include "src/platform/os/windows/common/structures.h"
#include "src/platform/os/windows/api_layer/kernel/kernel32.h"
#undef min

const stdstring APP_TITLE = TEXT("Retropad");
const stdstring UNTITLED_NAME = TEXT("Untitled");

void ModelState::SetCurrentPath(const stdstring& path)
{
    m_currentPath = path;
}

stdstring& ModelState::GetCurrentPath()
{
    return m_currentPath;
}

size_t ModelState::GetCurrentPathLength()
{
    return m_currentPath.size();
}

void ModelState::SetModified()
{
    m_modified = true;
}

void ModelState::ClearModified()
{
    m_modified = false;
}

bool ModelState::GetModified()
{
    return m_modified;
}

void ModelState::SetEncoding(TextEncoding enc)
{
    m_encoding = enc;
}

TextEncoding ModelState::GetEncoding()
{
    return m_encoding;
}

void ModelState::SetFindFlags(unsigned int flags)
{
    m_findFlags = flags;
}

unsigned int ModelState::GetFindFlags()
{
    return m_findFlags;
}

void ModelState::SetFindText(const stdstring& find)
{
    m_findText = find;
}

stdstring& ModelState::GetFindText()
{
    return m_findText;
}

size_t ModelState::GetFindTextLength()
{
    return m_findText.size();
}

void ModelState::SetReplaceText(const stdstring& replace)
{
    m_replaceText = replace;
}

stdstring& ModelState::GetReplaceText()
{
    return m_replaceText;
}

size_t ModelState::GetReplaceTextLength()
{
    return m_replaceText.size();
}

qor::pipeline::PODBuffer<qor::platform::nswindows::TCHAR>& ModelState::Document()
{
    return m_document;
}

void ModelState::InsertTimeDate() 
{
    SYSTEMTIME st;
    GetLocalTime(&st);
    TCHAR date[64], time[64], stamp[128];
    GetDateFormat(LOCALE_USER_DEFAULT, DATE_SHORTDATE, &st, NULL, date, ARRAYSIZE(date));
    GetTimeFormat(LOCALE_USER_DEFAULT, TIME_NOSECONDS, &st, NULL, time, ARRAYSIZE(time));
    StringCchPrintf(stamp, ARRAYSIZE(stamp), TEXT("%s %s"), time, date);
    //TODO: SendMessage(g_app.hwndEdit, EM_REPLACESEL, TRUE, (LPARAM)stamp);
}

void ModelState::DoFileNew()
{
    m_currentPath.clear();
    m_encoding = ENC_UTF8;
    m_modified = false;
}


bool ModelState::FindInText(const TCHAR *needle, bool matchCase, bool searchDown, unsigned long startPos, unsigned long *outStart, unsigned long *outEnd) 
{
    if (!needle || needle[0] == TEXT('\0')) return false;

    size_t length = -1;
    TCHAR* text = (TCHAR*)(m_document.ReadRequest(length));

    if(length == 0 || text == nullptr)
    {
        return false;
    }

    int len = (int)(length);

    size_t needleLen = _tcslen(needle);
    TCHAR *haystack = text;
    TCHAR *needleBuf = (TCHAR *)HeapAlloc(GetProcessHeap(), 0, (needleLen + 1) * sizeof(TCHAR));
    if (!needleBuf) 
    {
        return false;
    }
    StringCchCopy(needleBuf, needleLen + 1, needle);

    if (!matchCase) 
    {
        CharLowerBuff(haystack, len);
        CharLowerBuff(needleBuf, (DWORD)needleLen);
    }

    if (startPos > (DWORD)len) startPos = (DWORD)len;

    TCHAR *found = NULL;
    if (searchDown) 
    {
        found = _tcsstr(haystack + startPos, needleBuf);
        if (!found && startPos > 0) 
        {
            found = _tcsstr(haystack, needleBuf);
        }
    } 
    else 
    {
        TCHAR *p = haystack;
        while ((p = _tcsstr(p, needleBuf)) != NULL) 
        {
            DWORD idx = (DWORD)(p - haystack);
            if (idx < startPos) 
            {
                found = p;
                p++;
            } 
            else 
            {
                break;
            }
        }
        if (!found && startPos < (DWORD)len) 
        {
            p = haystack + startPos;
            while ((p = _tcsstr(p, needleBuf)) != NULL) 
            {
                found = p;
                p++;
            }
        }
    }

    bool result = false;
    if (found) 
    {
        DWORD pos = (DWORD)(found - haystack);
        *outStart = pos;
        *outEnd = pos + (DWORD)needleLen;
        result = true;
    }

    m_document.ReadReject(length);
    HeapFree(GetProcessHeap(), 0, needleBuf);
    return result;
}

bool ModelState::LoadDocumentFromPath(stdstring path)
{
    TCHAR *text = NULL;
    TextEncoding enc = ENC_UTF8;
/*    
    if (!LoadTextFile(path, &text, NULL, &enc)) 
    {
        return false;
    }

    TODO: Update View
    hwndEdit->SetText(text);
    HeapFree(GetProcessHeap(), 0, text);
    */
    m_currentPath = path;
    m_encoding = enc;
    /*    
    hwndEdit->ProcessMessage(EM_SETMODIFY, FALSE, 0);
    */
    m_modified = false;
    //TODO:UpdateTitle(hwnd);
    //TODO:UpdateStatusBar(hwnd);
    return true;
}

int ModelState::ReplaceAllOccurrences(bool matchCase) 
{
    TCHAR *needle = GetFindText().data();
    TCHAR *replacement = GetReplaceText().data();

    if (!needle || needle[0] == TEXT('\0')) return 0;

    size_t length = -1;
    TCHAR *text = (TCHAR*)(m_document.ReadReject(length));
    int len = (int)(length);
    if(len == 0 || text == nullptr)
    {
        return 0;
    }

    size_t needleLen = _tcslen(needle);
    size_t replLen = replacement ? _tcslen(replacement) : 0;

    TCHAR *searchBuf = (TCHAR *)HeapAlloc(GetProcessHeap(), 0, (len + 1) * sizeof(TCHAR));
    TCHAR *needleBuf = (TCHAR *)HeapAlloc(GetProcessHeap(), 0, (needleLen + 1) * sizeof(TCHAR));
    if (!searchBuf || !needleBuf) 
    {
        m_document.ReadReject(length);
        if (searchBuf) HeapFree(GetProcessHeap(), 0, searchBuf);
        if (needleBuf) HeapFree(GetProcessHeap(), 0, needleBuf);
        return 0;
    }
    StringCchCopy(searchBuf, len + 1, text);
    StringCchCopy(needleBuf, needleLen + 1, needle);

    if (!matchCase) {
        CharLowerBuff(searchBuf, len);
        CharLowerBuff(needleBuf, (DWORD)needleLen);
    }

    int count = 0;
    TCHAR *p = searchBuf;
    while ((p = _tcsstr(p, needleBuf)) != NULL) 
    {
        count++;
        p += needleLen;
    }
    if (count == 0) 
    {
        m_document.ReadReject(length);
        HeapFree(GetProcessHeap(), 0, searchBuf);
        HeapFree(GetProcessHeap(), 0, needleBuf);
        return 0;
    }

    size_t newLen = (size_t)len - (size_t)count * needleLen + (size_t)count * replLen;
    TCHAR *result = (TCHAR *)HeapAlloc(GetProcessHeap(), 0, (newLen + 1) * sizeof(TCHAR));
    if (!result) 
    {
        m_document.ReadReject(length);
        HeapFree(GetProcessHeap(), 0, searchBuf);
        HeapFree(GetProcessHeap(), 0, needleBuf);
        return 0;
    }

    TCHAR *dst = result;
    TCHAR *searchCur = searchBuf;
    TCHAR *origCur = text;
    while ((p = _tcsstr(searchCur, needleBuf)) != NULL) 
    {
        size_t delta = (size_t)(p - searchCur);
        CopyMemory(dst, origCur, delta * sizeof(TCHAR));
        dst += delta;
        origCur += delta;
        searchCur += delta;

        if (replLen) {
            CopyMemory(dst, replacement, replLen * sizeof(TCHAR));
            dst += replLen;
        }
        origCur += needleLen;
        searchCur += needleLen;
    }
    size_t tail = _tcslen(origCur);
    CopyMemory(dst, origCur, tail * sizeof(TCHAR));
    dst += tail;
    *dst = TEXT('\0');

    //TODO: Update the view
    /*
    SetWindowText(hwndEdit, result);
    HeapFree(GetProcessHeap(), 0, text);
    HeapFree(GetProcessHeap(), 0, searchBuf);
    HeapFree(GetProcessHeap(), 0, needleBuf);
    HeapFree(GetProcessHeap(), 0, result);
    SendMessage(hwndEdit, EM_SETMODIFY, TRUE, 0);
    */
    m_modified = true;
    /*
    UpdateTitle(hwndMain);
    */
    return count;
}

bool ModelState::Save(const stdstring& path)
{
    return true;
}

bool ModelState::SaveTextFile(const stdstring& path, LPCTSTR text, size_t length, TextEncoding encoding)
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
        m_encoding = ENC_UTF8;
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
   return true;
}

