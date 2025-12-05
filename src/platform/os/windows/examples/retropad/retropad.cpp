// retropad - a Petzold-style Win32 notepad clone implemented in mostly plain C.
// Keeps the classic menus/accelerators, word wrap, status bar, find/replace,
// font picker, and basic file load/save with BOM detection.
//#include <windows.h>
#include "src/configuration/configuration.h"

#include "src/platform/os/windows/common/stringconv.h"
#include "src/platform/os/windows/gui/windowclassregistration.h"
#include "src/platform/os/windows/gui/window.h"
#include "src/platform/os/windows/api_layer/kernel/kernel32.h"
#include <commdlg.h>
#include <commctrl.h>
#include <shellapi.h>
#include <strsafe.h>
#include <tchar.h>
#include "resource.h"
#include "fileio.h"

#define APP_TITLE      TEXT("Retropad")
#define UNTITLED_NAME  TEXT("Untitled")

using namespace qor::nswindows::api;
using namespace qor::platform::nswindows;

constexpr int MAX_PATH_BUFFER  = 1024;
constexpr int DEFAULT_WIDTH  = 640;
constexpr int DEFAULT_HEIGHT = 480;

typedef struct AppState 
{
    HWND hwndMain;
    HWND hwndEdit;
    HWND hwndStatus;
    HFONT hFont;
    TCHAR currentPath[MAX_PATH_BUFFER];
    bool wordWrap;
    bool statusVisible;
    bool statusBeforeWrap;
    BOOL modified;
    TextEncoding encoding;
    FINDREPLACE find;
    HWND hFindDlg;
    HWND hReplaceDlg;
    UINT findFlags;
    TCHAR findText[128];
    TCHAR replaceText[128];
} AppState;

static AppState g_app = {0};
static HINSTANCE g_hInst = NULL;
static UINT g_findMsg = 0;

static void UpdateTitle(HWND hwnd);
static void CreateEditControl(HWND hwnd);
static void UpdateLayout(HWND hwnd);
static bool PromptSaveChanges(HWND hwnd);
static bool DoFileOpen(HWND hwnd);
static bool DoFileSave(HWND hwnd, BOOL saveAs);
static void DoFileNew(HWND hwnd);
static void SetWordWrap(HWND hwnd, bool enabled);
static void ToggleStatusBar(HWND hwnd, bool visible);
static void UpdateStatusBar(HWND hwnd);
static void ShowFindDialog(HWND hwnd);
static void ShowReplaceDialog(HWND hwnd);
static bool DoFindNext(bool reverse);
static void DoSelectFont(HWND hwnd);
static void InsertTimeDate();
static void HandleFindReplace(LPFINDREPLACE lpfr);
static bool LoadDocumentFromPath(HWND hwnd, LPCTSTR path);
static INT_PTR CALLBACK GoToDlgProc(HWND dlg, UINT msg, WPARAM wParam, LPARAM lParam);
static INT_PTR CALLBACK AboutDlgProc(HWND dlg, UINT msg, WPARAM wParam, LPARAM lParam);

static bool GetEditText(HWND hwndEdit, TCHAR** bufferOut, int* lengthOut) 
{
    int length = GetWindowTextLength(hwndEdit);
    TCHAR* buffer = (TCHAR*)HeapAlloc(GetProcessHeap(), 0, (length + 1) * sizeof(TCHAR));
    if (!buffer) return false;
    GetWindowText(hwndEdit, buffer, length + 1);
    if (lengthOut) *lengthOut = length;
    *bufferOut = buffer;
    return true;
}

static bool FindInEdit(HWND hwndEdit, const TCHAR *needle, BOOL matchCase, BOOL searchDown, DWORD startPos, DWORD *outStart, DWORD *outEnd) 
{
    if (!needle || needle[0] == TEXT('\0')) return false;

    TCHAR *text = NULL;
    int len = 0;
    if (!GetEditText(hwndEdit, &text, &len)) return false;

    size_t needleLen = _tcslen(needle);
    TCHAR *haystack = text;
    TCHAR *needleBuf = (TCHAR *)HeapAlloc(GetProcessHeap(), 0, (needleLen + 1) * sizeof(TCHAR));
    if (!needleBuf) 
    {
        HeapFree(GetProcessHeap(), 0, text);
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

    HeapFree(GetProcessHeap(), 0, text);
    HeapFree(GetProcessHeap(), 0, needleBuf);
    return result;
}

static int ReplaceAllOccurrences(HWND hwndEdit, const TCHAR *needle, const TCHAR *replacement, BOOL matchCase) 
{
    if (!needle || needle[0] == TEXT('\0')) return 0;

    TCHAR *text = NULL;
    int len = 0;
    if (!GetEditText(hwndEdit, &text, &len)) return 0;

    size_t needleLen = _tcslen(needle);
    size_t replLen = replacement ? _tcslen(replacement) : 0;

    TCHAR *searchBuf = (TCHAR *)HeapAlloc(GetProcessHeap(), 0, (len + 1) * sizeof(TCHAR));
    TCHAR *needleBuf = (TCHAR *)HeapAlloc(GetProcessHeap(), 0, (needleLen + 1) * sizeof(TCHAR));
    if (!searchBuf || !needleBuf) 
    {
        HeapFree(GetProcessHeap(), 0, text);
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
        HeapFree(GetProcessHeap(), 0, text);
        HeapFree(GetProcessHeap(), 0, searchBuf);
        HeapFree(GetProcessHeap(), 0, needleBuf);
        return 0;
    }

    size_t newLen = (size_t)len - (size_t)count * needleLen + (size_t)count * replLen;
    TCHAR *result = (TCHAR *)HeapAlloc(GetProcessHeap(), 0, (newLen + 1) * sizeof(TCHAR));
    if (!result) 
    {
        HeapFree(GetProcessHeap(), 0, text);
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

    SetWindowText(hwndEdit, result);
    HeapFree(GetProcessHeap(), 0, text);
    HeapFree(GetProcessHeap(), 0, searchBuf);
    HeapFree(GetProcessHeap(), 0, needleBuf);
    HeapFree(GetProcessHeap(), 0, result);
    SendMessage(hwndEdit, EM_SETMODIFY, TRUE, 0);
    g_app.modified = TRUE;
    UpdateTitle(g_app.hwndMain);
    return count;
}

static void UpdateTitle(HWND hwnd) 
{
    TCHAR name[MAX_PATH_BUFFER];
    if (g_app.currentPath[0]) 
    {
        TCHAR *fileName = _tcsrchr(g_app.currentPath, TEXT('\\'));
        fileName = fileName ? fileName + 1 : g_app.currentPath;
        StringCchCopy(name, MAX_PATH_BUFFER, fileName);
    } 
    else 
    {
        StringCchCopy(name, MAX_PATH_BUFFER, UNTITLED_NAME);
    }

    TCHAR title[MAX_PATH_BUFFER + 32];
    StringCchPrintf(title, ARRAYSIZE(title), TEXT("%s%s - %s"), (g_app.modified ? TEXT("*") : TEXT("")), name, APP_TITLE);
    SetWindowText(hwnd, title);
}

static void ApplyFontToEdit(HWND hwndEdit, HFONT font) 
{
    SendMessageW(hwndEdit, WM_SETFONT, (WPARAM)font, TRUE);
}

static void CreateEditControl(HWND hwnd) 
{
    if (g_app.hwndEdit) 
    {
        DestroyWindow(g_app.hwndEdit);
    }

    DWORD style = WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_MULTILINE | ES_AUTOVSCROLL | ES_WANTRETURN | ES_NOHIDESEL;
    if (!g_app.wordWrap) 
    {
        style |= WS_HSCROLL | ES_AUTOHSCROLL;
    }

    g_app.hwndEdit = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("EDIT"), NULL, style, 0, 0, 0, 0, hwnd, (HMENU)1, g_hInst, NULL);
    if (g_app.hwndEdit && g_app.hFont) 
    {
        ApplyFontToEdit(g_app.hwndEdit, g_app.hFont);
    }
    SendMessage(g_app.hwndEdit, EM_SETLIMITTEXT, 0, 0); // allow large files
    UpdateLayout(hwnd);
}

static void ToggleStatusBar(HWND hwnd, bool visible) 
{
    g_app.statusVisible = visible;
    if (visible) 
    {
        if (!g_app.hwndStatus) 
        {
            g_app.hwndStatus = CreateStatusWindow(WS_CHILD | SBARS_SIZEGRIP, TEXT(""), hwnd, 2);
        }
        ShowWindow(g_app.hwndStatus, SW_SHOW);
    } 
    else if (g_app.hwndStatus) 
    {
        ShowWindow(g_app.hwndStatus, SW_HIDE);
    }
    UpdateLayout(hwnd);
    UpdateStatusBar(hwnd);
}

static void UpdateLayout(HWND hwnd) 
{
    RECT rc;
    GetClientRect(hwnd, &rc);

    int statusHeight = 0;
    if (g_app.statusVisible && g_app.hwndStatus) 
    {
        SendMessage(g_app.hwndStatus, WM_SIZE, 0, 0);
        RECT sbrc;
        GetWindowRect(g_app.hwndStatus, &sbrc);
        statusHeight = sbrc.bottom - sbrc.top;
        MoveWindow(g_app.hwndStatus, 0, rc.bottom - statusHeight, rc.right, statusHeight, TRUE);
    }

    if (g_app.hwndEdit) {
        MoveWindow(g_app.hwndEdit, 0, 0, rc.right, rc.bottom - statusHeight, TRUE);
    }
}

static bool PromptSaveChanges(HWND hwnd) 
{
    if (!g_app.modified) return true;

    TCHAR prompt[MAX_PATH_BUFFER + 64];
    const TCHAR* name = g_app.currentPath[0] ? g_app.currentPath : UNTITLED_NAME;
    StringCchPrintf(prompt, ARRAYSIZE(prompt), TEXT("Do you want to save changes to %s?"), name);
    int res = MessageBox(hwnd, prompt, APP_TITLE, MB_ICONQUESTION | MB_YESNOCANCEL);
    if (res == IDYES) 
    {
        return DoFileSave(hwnd, FALSE);
    }
    return res == IDNO;
}

static bool LoadDocumentFromPath(HWND hwnd, LPCTSTR path) 
{
    TCHAR *text = NULL;
    TextEncoding enc = ENC_UTF8;
    if (!LoadTextFile(hwnd, path, &text, NULL, &enc)) 
    {
        return false;
    }

    SetWindowText(g_app.hwndEdit, text);
    HeapFree(GetProcessHeap(), 0, text);
    StringCchCopy(g_app.currentPath, ARRAYSIZE(g_app.currentPath), path);
    g_app.encoding = enc;
    SendMessageW(g_app.hwndEdit, EM_SETMODIFY, FALSE, 0);
    g_app.modified = FALSE;
    UpdateTitle(hwnd);
    UpdateStatusBar(hwnd);
    return true;
}

static bool DoFileOpen(HWND hwnd) 
{
    if (!PromptSaveChanges(hwnd)) return false;

    TCHAR path[MAX_PATH_BUFFER] = TEXT("");
    if (!OpenFileDialog(hwnd, path, ARRAYSIZE(path))) 
    {
        return false;
    }
    return LoadDocumentFromPath(hwnd, path);
}

static bool DoFileSave(HWND hwnd, BOOL saveAs) 
{
    TCHAR path[MAX_PATH_BUFFER];
    if (saveAs || g_app.currentPath[0] == L'\0') 
    {
        path[0] = L'\0';
        if (g_app.currentPath[0]) 
        {
            StringCchCopy(path, ARRAYSIZE(path), g_app.currentPath);
        }
        if (!SaveFileDialog(hwnd, path, ARRAYSIZE(path))) 
        {
            return false;
        }
        StringCchCopy(g_app.currentPath, ARRAYSIZE(g_app.currentPath), path);
    } 
    else 
    {
        StringCchCopy(path, ARRAYSIZE(path), g_app.currentPath);
    }

    int len = GetWindowTextLength(g_app.hwndEdit);
    TCHAR *buffer = (TCHAR *)HeapAlloc(GetProcessHeap(), 0, (len + 1) * sizeof(TCHAR));
    if (!buffer) 
    {
        return false;
    }
    GetWindowText(g_app.hwndEdit, buffer, len + 1);

    bool ok = SaveTextFile(hwnd, path, buffer, len, g_app.encoding);
    HeapFree(GetProcessHeap(), 0, buffer);
    if (ok) 
    {
        SendMessage(g_app.hwndEdit, EM_SETMODIFY, FALSE, 0);
        g_app.modified = FALSE;
        UpdateTitle(hwnd);
    }
    return ok;
}

static void DoFileNew(HWND hwnd) 
{
    if (!PromptSaveChanges(hwnd)) return;
    SetWindowText(g_app.hwndEdit, TEXT(""));
    g_app.currentPath[0] = TEXT('\0');
    g_app.encoding = ENC_UTF8;
    SendMessage(g_app.hwndEdit, EM_SETMODIFY, FALSE, 0);
    g_app.modified = FALSE;
    UpdateTitle(hwnd);
    UpdateStatusBar(hwnd);
}

static void SetWordWrap(HWND hwnd, bool enabled) 
{
    if (g_app.wordWrap == enabled) 
    {
        return;
    }
    g_app.wordWrap = enabled;
    HWND edit = g_app.hwndEdit;
    TCHAR *text = nullptr;
    int len = 0;
    if (!GetEditText(edit, &text, &len)) 
    {
        return;
    }
    DWORD start = 0, end = 0;
    SendMessage(edit, EM_GETSEL, (WPARAM)&start, (LPARAM)&end);

    CreateEditControl(hwnd);
    SetWindowText(g_app.hwndEdit, text);
    SendMessage(g_app.hwndEdit, EM_SETSEL, start, end);
    HeapFree(GetProcessHeap(), 0, text);

    if (enabled) 
    {
        g_app.statusBeforeWrap = g_app.statusVisible;
        ToggleStatusBar(hwnd, false);
        EnableMenuItem(GetMenu(hwnd), IDM_VIEW_STATUS_BAR, MF_BYCOMMAND | MF_GRAYED);
        EnableMenuItem(GetMenu(hwnd), IDM_EDIT_GOTO, MF_BYCOMMAND | MF_GRAYED);
    } else {
        ToggleStatusBar(hwnd, g_app.statusBeforeWrap);
        EnableMenuItem(GetMenu(hwnd), IDM_VIEW_STATUS_BAR, MF_BYCOMMAND | MF_ENABLED);
        EnableMenuItem(GetMenu(hwnd), IDM_EDIT_GOTO, MF_BYCOMMAND | MF_ENABLED);
    }
    UpdateTitle(hwnd);
    UpdateStatusBar(hwnd);
}

static void UpdateStatusBar(HWND hwnd) 
{
    if (!g_app.statusVisible || !g_app.hwndStatus) return;
    DWORD selStart = 0, selEnd = 0;
    SendMessage(g_app.hwndEdit, EM_GETSEL, (WPARAM)&selStart, (LPARAM)&selEnd);
    int line = (int)SendMessage(g_app.hwndEdit, EM_LINEFROMCHAR, selStart, 0) + 1;
    int col = (int)(selStart - SendMessage(g_app.hwndEdit, EM_LINEINDEX, line - 1, 0)) + 1;
    int lines = (int)SendMessage(g_app.hwndEdit, EM_GETLINECOUNT, 0, 0);

    TCHAR status[128];
    StringCchPrintf(status, ARRAYSIZE(status), TEXT("Ln %d, Col %d    Lines: %d"), line, col, lines);
    SendMessage(g_app.hwndStatus, SB_SETTEXT, 0, (LPARAM)status);
}

static void ShowFindDialog(HWND hwnd) 
{
    if (g_app.hFindDlg) 
    {
        SetForegroundWindow(g_app.hFindDlg);
        return;
    }

    ZeroMemory(&g_app.find, sizeof(g_app.find));
    g_app.find.lStructSize = sizeof(FINDREPLACE);
    g_app.find.hwndOwner = hwnd;
    g_app.find.lpstrFindWhat = g_app.findText;
    g_app.find.wFindWhatLen = ARRAYSIZE(g_app.findText);
    g_app.find.Flags = g_app.findFlags;
    g_app.hFindDlg = FindText(&g_app.find);
}

static void ShowReplaceDialog(HWND hwnd) 
{
    if (g_app.hReplaceDlg) 
    {
        SetForegroundWindow(g_app.hReplaceDlg);
        return;
    }

    ZeroMemory(&g_app.find, sizeof(g_app.find));
    g_app.find.lStructSize = sizeof(FINDREPLACE);
    g_app.find.hwndOwner = hwnd;
    g_app.find.lpstrFindWhat = g_app.findText;
    g_app.find.lpstrReplaceWith = g_app.replaceText;
    g_app.find.wFindWhatLen = ARRAYSIZE(g_app.findText);
    g_app.find.wReplaceWithLen = ARRAYSIZE(g_app.replaceText);
    g_app.find.Flags = g_app.findFlags;

    g_app.hReplaceDlg = ReplaceText(&g_app.find);
}

static bool DoFindNext(bool reverse) 
{
    if (g_app.findText[0] == TEXT('\0')) 
    {
        ShowFindDialog(g_app.hwndMain);
        return false;
    }

    DWORD start = 0, end = 0;
    SendMessage(g_app.hwndEdit, EM_GETSEL, (WPARAM)&start, (LPARAM)&end);
    bool matchCase = (g_app.findFlags & FR_MATCHCASE) != 0;
    bool down = (g_app.findFlags & FR_DOWN) != 0;
    if (reverse) 
    {
        down = !down;
    }
    DWORD searchStart = down ? end : start;
    DWORD outStart = 0, outEnd = 0;
    if (FindInEdit(g_app.hwndEdit, g_app.findText, matchCase, down, searchStart, &outStart, &outEnd)) 
    {
        SendMessage(g_app.hwndEdit, EM_SETSEL, outStart, outEnd);
        SendMessage(g_app.hwndEdit, EM_SCROLLCARET, 0, 0);
        return true;
    }
    MessageBox(g_app.hwndMain, TEXT("Cannot find the text."), APP_TITLE, MB_ICONINFORMATION);
    return false;
}

static INT_PTR CALLBACK GoToDlgProc(HWND dlg, UINT msg, WPARAM wParam, LPARAM lParam) 
{
    switch (msg) {
    case WM_INITDIALOG: 
    {
        SetDlgItemInt(dlg, IDC_GOTO_EDIT, 1, FALSE);
        HWND edit = GetDlgItem(dlg, IDC_GOTO_EDIT);
        SendMessageW(edit, EM_SETLIMITTEXT, 10, 0);
        return TRUE;
    }
    case WM_COMMAND:
        switch (LOWORD(wParam)) 
        {
        case IDOK: 
        {
            BOOL ok = FALSE;
            UINT line = GetDlgItemInt(dlg, IDC_GOTO_EDIT, &ok, FALSE);
            if (!ok || line == 0) 
            {
                MessageBox(dlg, TEXT("Enter a valid line number."), APP_TITLE, MB_ICONWARNING);
                return TRUE;
            }
            int maxLine = (int)SendMessage(g_app.hwndEdit, EM_GETLINECOUNT, 0, 0);
            if ((int)line > maxLine) line = (UINT)maxLine;
            int charIndex = (int)SendMessage(g_app.hwndEdit, EM_LINEINDEX, line - 1, 0);
            if (charIndex >= 0) 
            {
                SendMessage(g_app.hwndEdit, EM_SETSEL, charIndex, charIndex);
                SendMessage(g_app.hwndEdit, EM_SCROLLCARET, 0, 0);
            }
            EndDialog(dlg, IDOK);
            return TRUE;
        }
        case IDCANCEL:
            EndDialog(dlg, IDCANCEL);
            return TRUE;
        }
        break;
    }
    return FALSE;
}

static void DoSelectFont(HWND hwnd) 
{
    LOGFONT lf = {0};
    if (g_app.hFont) 
    {
        GetObject(g_app.hFont, sizeof(LOGFONT), &lf);
    } 
    else 
    {
        SystemParametersInfo(SPI_GETICONTITLELOGFONT, sizeof(LOGFONT), &lf, 0);
    }

    CHOOSEFONT cf = {0};
    cf.lStructSize = sizeof(cf);
    cf.hwndOwner = hwnd;
    cf.lpLogFont = &lf;
    cf.Flags = CF_SCREENFONTS | CF_INITTOLOGFONTSTRUCT;

    if (ChooseFont(&cf)) 
    {
        HFONT newFont = CreateFontIndirect(&lf);
        if (newFont) 
        {
            if (g_app.hFont) DeleteObject(g_app.hFont);
            g_app.hFont = newFont;
            ApplyFontToEdit(g_app.hwndEdit, g_app.hFont);
            UpdateLayout(hwnd);
        }
    }
}

static void InsertTimeDate() 
{
    SYSTEMTIME st;
    GetLocalTime(&st);
    TCHAR date[64], time[64], stamp[128];
    GetDateFormat(LOCALE_USER_DEFAULT, DATE_SHORTDATE, &st, NULL, date, ARRAYSIZE(date));
    GetTimeFormat(LOCALE_USER_DEFAULT, TIME_NOSECONDS, &st, NULL, time, ARRAYSIZE(time));
    StringCchPrintf(stamp, ARRAYSIZE(stamp), TEXT("%s %s"), time, date);
    SendMessage(g_app.hwndEdit, EM_REPLACESEL, TRUE, (LPARAM)stamp);
}

static void HandleFindReplace(LPFINDREPLACE lpfr) 
{
    if (lpfr->Flags & FR_DIALOGTERM) 
    {
        g_app.hFindDlg = NULL;
        g_app.hReplaceDlg = NULL;
        return;
    }

    g_app.findFlags = lpfr->Flags;
    if (lpfr->lpstrFindWhat && lpfr->lpstrFindWhat[0]) 
    {
        StringCchCopy(g_app.findText, ARRAYSIZE(g_app.findText), lpfr->lpstrFindWhat);
    }
    if (lpfr->lpstrReplaceWith) 
    {
        StringCchCopy(g_app.replaceText, ARRAYSIZE(g_app.replaceText), lpfr->lpstrReplaceWith);
    }

    BOOL matchCase = (lpfr->Flags & FR_MATCHCASE) != 0;
    BOOL down = (lpfr->Flags & FR_DOWN) != 0;

    if (lpfr->Flags & FR_FINDNEXT) 
    {
        DWORD start = 0, end = 0;
        SendMessage(g_app.hwndEdit, EM_GETSEL, (WPARAM)&start, (LPARAM)&end);
        DWORD searchStart = down ? end : start;
        DWORD outStart = 0, outEnd = 0;
        if (FindInEdit(g_app.hwndEdit, g_app.findText, matchCase, down, searchStart, &outStart, &outEnd)) 
        {
            SendMessage(g_app.hwndEdit, EM_SETSEL, outStart, outEnd);
            SendMessage(g_app.hwndEdit, EM_SCROLLCARET, 0, 0);
        } 
        else 
        {
            MessageBox(g_app.hwndMain, TEXT("Cannot find the text."), APP_TITLE, MB_ICONINFORMATION);
        }
    } 
    else if (lpfr->Flags & FR_REPLACE) 
    {
        DWORD start = 0, end = 0;
        SendMessage(g_app.hwndEdit, EM_GETSEL, (WPARAM)&start, (LPARAM)&end);
        DWORD outStart = 0, outEnd = 0;
        if (FindInEdit(g_app.hwndEdit, g_app.findText, matchCase, down, start, &outStart, &outEnd)) 
        {
            SendMessage(g_app.hwndEdit, EM_SETSEL, outStart, outEnd);
            SendMessage(g_app.hwndEdit, EM_REPLACESEL, TRUE, (LPARAM)g_app.replaceText);
            SendMessage(g_app.hwndEdit, EM_SCROLLCARET, 0, 0);
            g_app.modified = TRUE;
            UpdateTitle(g_app.hwndMain);
        } 
        else 
        {
            MessageBox(g_app.hwndMain, TEXT("Cannot find the text."), APP_TITLE, MB_ICONINFORMATION);
        }
    } 
    else if (lpfr->Flags & FR_REPLACEALL) 
    {
        int replaced = ReplaceAllOccurrences(g_app.hwndEdit, g_app.findText, g_app.replaceText, matchCase);
        TCHAR msg[64];
        StringCchPrintf(msg, ARRAYSIZE(msg), TEXT("Replaced %d occurrence%s."), replaced, replaced == 1 ? TEXT("") : TEXT("s"));
        MessageBox(g_app.hwndMain, msg, APP_TITLE, MB_OK | MB_ICONINFORMATION);
    }
}

static void UpdateMenuStates(HWND hwnd) 
{
    HMENU menu = GetMenu(hwnd);
    if (!menu) return;

    UINT wrapState = g_app.wordWrap ? MF_CHECKED : MF_UNCHECKED;
    UINT statusState = g_app.statusVisible ? MF_CHECKED : MF_UNCHECKED;
    CheckMenuItem(menu, IDM_FORMAT_WORD_WRAP, MF_BYCOMMAND | wrapState);
    CheckMenuItem(menu, IDM_VIEW_STATUS_BAR, MF_BYCOMMAND | statusState);

    BOOL canGoTo = !g_app.wordWrap;
    EnableMenuItem(menu, IDM_EDIT_GOTO, MF_BYCOMMAND | (canGoTo ? MF_ENABLED : MF_GRAYED));
    if (g_app.wordWrap) 
    {
        EnableMenuItem(menu, IDM_VIEW_STATUS_BAR, MF_BYCOMMAND | MF_GRAYED);
    } 
    else 
    {
        EnableMenuItem(menu, IDM_VIEW_STATUS_BAR, MF_BYCOMMAND | MF_ENABLED);
    }

    BOOL modified = (SendMessage(g_app.hwndEdit, EM_GETMODIFY, 0, 0) != 0);
    EnableMenuItem(menu, IDM_FILE_SAVE, MF_BYCOMMAND | (modified ? MF_ENABLED : MF_GRAYED));
}

static void HandleCommand(HWND hwnd, WPARAM wParam, LPARAM lParam) 
{
    switch (LOWORD(wParam)) 
    {
    case IDM_FILE_NEW:
        DoFileNew(hwnd);
        break;
    case IDM_FILE_OPEN:
        DoFileOpen(hwnd);
        break;
    case IDM_FILE_SAVE:
        DoFileSave(hwnd, FALSE);
        break;
    case IDM_FILE_SAVE_AS:
        DoFileSave(hwnd, TRUE);
        break;
    case IDM_FILE_PAGE_SETUP:
    case IDM_FILE_PRINT:
        MessageBox(hwnd, TEXT("Printing is not implemented in Retropad."), APP_TITLE, MB_ICONINFORMATION);
        break;
    case IDM_FILE_EXIT:
        PostMessage(hwnd, WM_CLOSE, 0, 0);
        break;

    case IDM_EDIT_UNDO:
        SendMessage(g_app.hwndEdit, EM_UNDO, 0, 0);
        break;
    case IDM_EDIT_CUT:
        SendMessage(g_app.hwndEdit, WM_CUT, 0, 0);
        break;
    case IDM_EDIT_COPY:
        SendMessage(g_app.hwndEdit, WM_COPY, 0, 0);
        break;
    case IDM_EDIT_PASTE:
        SendMessage(g_app.hwndEdit, WM_PASTE, 0, 0);
        break;
    case IDM_EDIT_DELETE:
        SendMessage(g_app.hwndEdit, WM_CLEAR, 0, 0);
        break;
    case IDM_EDIT_FIND:
        ShowFindDialog(hwnd);
        break;
    case IDM_EDIT_FIND_NEXT:
        DoFindNext(false);
        break;
    case IDM_EDIT_REPLACE:
        ShowReplaceDialog(hwnd);
        break;
    case IDM_EDIT_GOTO:
        if (g_app.wordWrap) 
        {
            MessageBox(hwnd, TEXT("Go To is unavailable when Word Wrap is on."), APP_TITLE, MB_ICONINFORMATION);
        } else {
            DialogBox(g_hInst, MAKEINTRESOURCE(IDD_GOTO), hwnd, GoToDlgProc);
        }
        break;
    case IDM_EDIT_SELECT_ALL:
        SendMessage(g_app.hwndEdit, EM_SETSEL, 0, -1);
        break;
    case IDM_EDIT_TIME_DATE:
        InsertTimeDate();
        break;

    case IDM_FORMAT_WORD_WRAP:
        SetWordWrap(hwnd, !g_app.wordWrap);
        break;
    case IDM_FORMAT_FONT:
        DoSelectFont(hwnd);
        break;

    case IDM_VIEW_STATUS_BAR:
        ToggleStatusBar(hwnd, !g_app.statusVisible);
        break;

    case IDM_HELP_VIEW_HELP:
        MessageBox(hwnd, TEXT("No help file is available for Retropad."), APP_TITLE, MB_ICONINFORMATION);
        break;
    case IDM_HELP_ABOUT:
        DialogBox(g_hInst, MAKEINTRESOURCE(IDD_ABOUT), hwnd, AboutDlgProc);
        break;
    }
}

static INT_PTR CALLBACK AboutDlgProc(HWND dlg, UINT msg, WPARAM wParam, LPARAM lParam) 
{
    switch (msg) {
    case WM_INITDIALOG:
        return TRUE;
    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) 
        {
            EndDialog(dlg, LOWORD(wParam));
            return TRUE;
        }
        break;
    }
    return FALSE;
}

static LRESULT CALLBACK MainWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) 
{
    if (msg == g_findMsg) 
    {
        HandleFindReplace((LPFINDREPLACE)lParam);
        return 0;
    }

    switch (msg) 
    {
    case WM_CREATE: 
    {
        INITCOMMONCONTROLSEX icc = { sizeof(icc), ICC_BAR_CLASSES };
        InitCommonControlsEx(&icc);
        CreateEditControl(hwnd);
        ToggleStatusBar(hwnd, TRUE);
        UpdateTitle(hwnd);
        UpdateStatusBar(hwnd);
        DragAcceptFiles(hwnd, TRUE);
        return 0;
    }
    case WM_SETFOCUS:
        if (g_app.hwndEdit) SetFocus(g_app.hwndEdit);
        return 0;
    case WM_SIZE:
        UpdateLayout(hwnd);
        UpdateStatusBar(hwnd);
        return 0;
    case WM_DROPFILES: 
    {
        HDROP hDrop = (HDROP)wParam;
        TCHAR path[MAX_PATH_BUFFER];
        if (DragQueryFile(hDrop, 0, path, ARRAYSIZE(path))) 
        {
            if (PromptSaveChanges(hwnd)) 
            {
                LoadDocumentFromPath(hwnd, path);
            }
        }
        DragFinish(hDrop);
        return 0;
    }
    case WM_COMMAND:
        if (HIWORD(wParam) == EN_CHANGE && (HWND)lParam == g_app.hwndEdit) 
        {
            g_app.modified = (SendMessageW(g_app.hwndEdit, EM_GETMODIFY, 0, 0) != 0);
            UpdateTitle(hwnd);
            UpdateStatusBar(hwnd);
            return 0;
        } 
        else if (HIWORD(wParam) == EN_UPDATE && (HWND)lParam == g_app.hwndEdit) 
        {
            UpdateStatusBar(hwnd);
            return 0;
        }
        HandleCommand(hwnd, wParam, lParam);
        return 0;
    case WM_INITMENUPOPUP:
        UpdateMenuStates(hwnd);
        return 0;
    case WM_CLOSE:
        if (PromptSaveChanges(hwnd)) 
        {
            DestroyWindow(hwnd);
        }
        return 0;
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hwnd, msg, wParam, lParam);
}

struct ImageDOSHeader
{												// DOS .EXE header
    unsigned short e_magic;                     // Magic number
    unsigned short e_cblp;                      // Bytes on last page of file
    unsigned short e_cp;                        // Pages in file
    unsigned short e_crlc;                      // Relocations
    unsigned short e_cparhdr;                   // Size of header in paragraphs
    unsigned short e_minalloc;                  // Minimum extra paragraphs needed
    unsigned short e_maxalloc;                  // Maximum extra paragraphs needed
    unsigned short e_ss;                        // Initial (relative) SS value
    unsigned short e_sp;                        // Initial SP value
    unsigned short e_csum;                      // Checksum
    unsigned short e_ip;                        // Initial IP value
    unsigned short e_cs;                        // Initial (relative) CS value
    unsigned short e_lfarlc;                    // File address of relocation table
    unsigned short e_ovno;                      // Overlay number
    unsigned short e_res[4];                    // Reserved words
    unsigned short e_oemid;                     // OEM identifier (for e_oeminfo)
    unsigned short e_oeminfo;                   // OEM information; e_oemid specific
    unsigned short e_res2[10];                  // Reserved words
    long e_lfanew;								// File address of new exe header
};

#ifdef __MINGW32__
#	define __ImageBase _image_base__
#endif

	extern "C" const ImageDOSHeader __ImageBase;//This must be injected by the Compiler for Windows builds

int main();

int __stdcall WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
    return main();
}

int main()
{
    HINSTANCE hInstance = (HINSTANCE)(const_cast<ImageDOSHeader*>(&__ImageBase));
    g_hInst = hInstance;
    g_findMsg = RegisterWindowMessage(FINDMSGSTRING);
    g_app.wordWrap = false;
    g_app.statusVisible = true;
    g_app.statusBeforeWrap = true;
    g_app.encoding = ENC_UTF8;
    g_app.findFlags = FR_DOWN;

    Icon icon(LoadIcon(hInstance, MAKEINTRESOURCE(IDI_RETROPAD)));    
    Cursor cursor(LoadCursor(NULL, IDC_IBEAM));
    Brush backgroundBrush(COLOR_BACKGROUND);
    Menu menu;
    WindowClass windowClass;
    windowClass.SetStyle( CS_HREDRAW | CS_VREDRAW );
    windowClass.SetMessageFunction((messageFunc)MainWndProc);
    windowClass.SetInstance(hInstance);
    windowClass.SetIcon(icon);
    windowClass.SetSmallIcon(icon);
    windowClass.SetCursor(cursor);
    windowClass.SetBackgroundBrush(backgroundBrush);
    windowClass.SetName(TEXT("RETROPAD_WINDOW"));
    windowClass.SetMenuName((const TCHAR*)1000);

    WindowClassRegistration reg(windowClass);
    Window mainWindow(windowClass.Name(), APP_TITLE, WS_OVERLAPPEDWINDOW, 0, CW_USEDEFAULT, CW_USEDEFAULT, DEFAULT_WIDTH, DEFAULT_HEIGHT, NULL, menu, hInstance, NULL);
 
    g_app.hwndMain = (HWND)(mainWindow.GetHandle().Use());
    mainWindow.Show(SW_SHOWNORMAL);
    mainWindow.Update();
    
    AcceleratorTable accTable(LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_RETROPAD)));
    Message msg;

    while (GetMessage((MSG*)&msg, NULL, 0, 0)) 
    {        
        if (!accTable.GetHandle().Use() || !mainWindow.TranslateAccel(accTable, &msg)) 
        {
            TranslateMessage((MSG*)&msg);
            DispatchMessageW((MSG*)&msg);
        }
    }

    return (int)msg.wParam;
}