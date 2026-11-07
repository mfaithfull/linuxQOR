// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_OS_WINDOWS_API_USER
#define QOR_PP_H_OS_WINDOWS_API_USER

#include "src/platform/os/windows/api_layer/kernel/kernel32.h"

#undef RegisterDeviceNotification
#undef WinHelp
#undef SystemParametersInfo
#undef GetRawInputDeviceInfo
#undef DlgDirList
#undef DlgDirSelectEx
#undef DlgDirListComboBox
#undef DlgDirSelectComboBoxEx
#undef LoadBitmap

typedef enum _CONSOLECONTROL
{
    Reserved1,
    ConsoleNotifyConsoleApplication,
    Reserved2,
    ConsoleSetCaretInfo,
    Reserved3,
    ConsoleSetForeground,
    ConsoleSetWindowOwner,
    ConsoleEndTask,
} CONSOLECONTROL;

typedef struct _CONSOLEENDTASK
{
    HANDLE ProcessId;
    HWND hwnd;
    ULONG ConsoleEventCode;
    ULONG ConsoleFlags;
} CONSOLEENDTASK, *PCONSOLEENDTASK;

typedef struct _CONSOLEWINDOWOWNER
{
    HWND hwnd;
    ULONG ProcessId;
    ULONG ThreadId;
} CONSOLEWINDOWOWNER, *PCONSOLEWINDOWOWNER;

typedef struct _CONSOLESETFOREGROUND
{
    HANDLE hProcess;
    BOOL bForeground;
} CONSOLESETFOREGROUND, *PCONSOLESETFOREGROUND;

typedef struct _CONSOLE_PROCESS_INFO
{
    IN DWORD dwProcessID;
    IN DWORD dwFlags;
} CONSOLE_PROCESS_INFO, *PCONSOLE_PROCESS_INFO;

typedef struct _CONSOLE_CARET_INFO
{
    IN HWND hwnd;
    IN RECT rc;
} CONSOLE_CARET_INFO, *PCONSOLE_CARET_INFO;

namespace qor { namespace win { namespace api {

	class qor_pp_module_interface(QOR_WINAPIUSER) User32
	{
	public:

        //Keyboard Input functions
        static HKL ActivateKeyboardLayout(HKL hkl, UINT Flags);
        static BOOL BlockInput(BOOL fBlockIt);
        static BOOL EnableWindow(HWND hWnd, BOOL bEnable);
        static HWND GetActiveWindow(void);
        static SHORT GetAsyncKeyState(int vKey);
        static HWND GetFocus(void);
        static UINT GetKBCodePage(void);
        static HKL GetKeyboardLayout(DWORD idThread);
        static int GetKeyboardLayoutList(int nBuff, HKL* lpList);
        static BOOL GetKeyboardLayoutNameT(LPTSTR pwszKLID);
        static BOOL GetKeyboardState(PBYTE lpKeyState);
        static int GetKeyboardType(int nTypeFlag);
        static int GetKeyNameTextT(LONG lParam, LPTSTR lpString, int cchSize);
        static SHORT GetKeyState(int nVirtKey);
        static BOOL GetLastInputInfo(PLASTINPUTINFO plii);
        static BOOL IsWindowEnabled(HWND hWnd);
        static VOID keybd_event(BYTE bVk, BYTE bScan, DWORD dwFlags, ULONG_PTR dwExtraInfo);
        static HKL LoadKeyboardLayoutT(LPCTSTR pwszKLID, UINT Flags);
        static UINT MapVirtualKeyT(UINT uCode, UINT uMapType);
        static UINT MapVirtualKeyExT(UINT uCode, UINT uMapType, HKL dwhkl);
        static DWORD OemKeyScan(WORD wOemChar);
        static BOOL RegisterHotKey(HWND hWnd, int id, UINT fsModifiers, UINT vk);
        static UINT SendInput(UINT cInputs, LPINPUT pInputs, int cbSize);
        static HWND SetActiveWindow(HWND hWnd);
        static HWND SetFocus(HWND hWnd);
        static BOOL SetKeyboardState(LPBYTE lpKeyState);
        static int ToAscii(UINT uVirtKey, UINT uScanCode, PBYTE lpKeyState, LPWORD lpChar, UINT uFlags);
        static int ToAsciiEx(UINT uVirtKey, UINT uScanCode, PBYTE lpKeyState, LPWORD lpChar, UINT uFlags, HKL dwhkl);
        static int ToUnicode(UINT wVirtKey, UINT wScanCode, const PBYTE lpKeyState, LPWSTR pwszBuff, int cchBuff, UINT wFlags);
        static int ToUnicodeEx(UINT wVirtKey, UINT wScanCode, const PBYTE lpKeyState, LPWSTR pwszBuff, int cchBuff, UINT wFlags, HKL dwhkl);
        static BOOL UnloadKeyboardLayout(HKL hkl);
        static BOOL UnregisterHotKey(HWND hWnd, int id);
        static SHORT VkKeyScanT(TCHAR ch);
        static SHORT VkKeyScanExT(TCHAR ch, HKL dwhkl);

        //Clipboard functions
        static BOOL AddClipboardFormatListener(HWND hwnd);
        static BOOL ChangeClipboardChain(HWND hWndRemove, HWND hWndNewNext);
        static BOOL CloseClipboard(void);
        static int CountClipboardFormats(void);
        static BOOL EmptyClipboard(void);
        static UINT EnumClipboardFormats(UINT format);
        static HANDLE GetClipboardData(UINT uFormat);
        static int GetClipboardFormatNameT(UINT format, LPTSTR lpszFormatName, int cchMaxCount);
        static HWND GetClipboardOwner(void);
        static DWORD GetClipboardSequenceNumber(void);
        static HWND GetClipboardViewer(void);
        static HWND GetOpenClipboardWindow(void);
        static int GetPriorityClipboardFormat(UINT* paFormatPriorityList, int cFormats);
        static BOOL GetUpdatedClipboardFormats(PUINT lpuiFormats, UINT cFormats, PUINT pcFormatsOut);
        static BOOL IsClipboardFormatAvailable(UINT format);
        static BOOL OpenClipboard(HWND hWndNewOwner);
        static UINT RegisterClipboardFormatT(LPCTSTR lpszFormat);
        static BOOL RemoveClipboardFormatListener(HWND hwnd);
        static HANDLE SetClipboardData(UINT uFormat, HANDLE hMem);
        static HWND SetClipboardViewer(HWND hWndNewViewer);

        //Window functions
        static BOOL AdjustWindowRect(::LPRECT lpRect, DWORD dwStyle, BOOL bMenu);
        static BOOL AdjustWindowRectEx(::LPRECT lpRect, DWORD dwStyle, BOOL bMenu, DWORD dwExStyle);
        static BOOL AdjustWindowRectExForDpi(::LPRECT lpRect, DWORD dwStyle, BOOL bMenu, DWORD dwExStyle, UINT dpi);
        static BOOL AllowSetForegroundWindow(DWORD dwProcessId);
        static BOOL AnimateWindow(HWND hWnd, DWORD dwTime, DWORD dwFlags);
        static BOOL AnyPopup(void);
        static UINT ArrangeIconicWindows(HWND hWnd);
        static HDWP BeginDeferWindowPos(int nNumWindows);
        static BOOL BringWindowToTop(HWND hWnd);
        static WORD CascadeChildWindows(HWND hWndParent, WORD wFlags);
        static WORD CascadeWindows(HWND hwndParent, UINT wHow, const ::RECT* lpRect, UINT cKids, const HWND* lpKids);
        static BOOL ChangeWindowMessageFilter(UINT message, DWORD dwFlag);
        static HWND ChildWindowFromPoint(HWND hWndParent, ::POINT Point);
        static HWND ChildWindowFromPointEx(HWND hwnd, ::POINT pt, UINT flags);
        static BOOL CloseWindow(HWND hWnd);
        static HWND CreateWindowT(LPCTSTR lpClassName, LPCTSTR lpWindowName, DWORD dwStyle, int x, int y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance, LPVOID lpParam);
        static HWND CreateWindowExT(DWORD dwExStyle, LPCTSTR lpClassName, LPCTSTR lpWindowName, DWORD dwStyle, int X, int Y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance, LPVOID lpParam);
        static HDWP DeferWindowPos(HDWP hWinPosInfo, HWND hWnd, HWND hWndInsertAfter, int x, int y, int cx, int cy, UINT uFlags);
        static BOOL DeregisterShellHookWindow(HWND hwnd);
        static BOOL DestroyWindow(HWND hWnd);
        static BOOL EndDeferWindowPos(HDWP hWinPosInfo);
        static BOOL EndTask(HWND hWnd, BOOL fShutDown, BOOL fForce);
        static BOOL EnumChildWindows(HWND hWndParent, WNDENUMPROC lpEnumFunc, LPARAM lParam);
        static BOOL EnumThreadWindows(DWORD dwThreadId, WNDENUMPROC lpfn, LPARAM lParam);
        static BOOL EnumWindows(WNDENUMPROC lpEnumFunc, LPARAM lParam);
        static HWND FindWindowT(LPCTSTR lpClassName, LPCTSTR lpWindowName);
        static HWND FindWindowExT(HWND hWndParent, HWND hWndChildAfter, LPCTSTR lpszClass, LPCTSTR lpszWindow);
        static BOOL GetAltTabInfoT(HWND hwnd, int iItem, ::PALTTABINFO pati, LPTSTR pszItemText, UINT cchItemText);
        static HWND GetAncestor(HWND hwnd, UINT gaFlags);
        static BOOL GetClientRect(HWND hWnd, ::LPRECT lpRect);
        static HWND GetDesktopWindow(void);
        static HWND GetForegroundWindow(void);
        static BOOL GetGUIThreadInfo(DWORD idThread, ::LPGUITHREADINFO pgui);
        static HWND GetLastActivePopup(HWND hWnd);
        static BOOL GetLayeredWindowAttributes(HWND hwnd, COLORREF* pcrKey, BYTE* pbAlpha, DWORD* pdwFlags);
        static HWND GetParent(HWND hWnd);
        static BOOL GetProcessDefaultLayout(DWORD* pdwDefaultLayout);
        static HWND GetShellWindow(void);
        static BOOL GetTitleBarInfo(HWND hwnd, ::PTITLEBARINFO pti);
        static HWND GetTopWindow(HWND hWnd);
        static HWND GetWindow(HWND hWnd, UINT uCmd);
        static BOOL GetWindowInfo(HWND hwnd, ::PWINDOWINFO pwi);
        static UINT GetWindowModuleFileNameT(HWND hwnd, LPTSTR pszFileName, UINT cchFileNameMax);
        static BOOL GetWindowPlacement(HWND hWnd, ::WINDOWPLACEMENT* lpwndpl);
        static BOOL GetWindowRect(HWND hWnd, ::LPRECT lpRect);
        static int GetWindowTextT(HWND hWnd, LPTSTR lpString, int nMaxCount);
        static int GetWindowTextLengthT(HWND hWnd);
        static DWORD GetWindowThreadProcessId(HWND hWnd, LPDWORD lpdwProcessId);
        static int InternalGetWindowText(HWND hWnd, LPWSTR pString, int cchMaxCount);
        static BOOL IsChild(HWND hWndParent, HWND hWnd);
        static BOOL IsGUIThread(BOOL bConvert);
        static BOOL IsHungAppWindow(HWND hwnd);
        static BOOL IsIconic(HWND hWnd);
        static BOOL IsProcessDPIAware(void);
        static BOOL IsWindow(HWND hWnd);
        static BOOL IsWindowUnicode(HWND hWnd);
        static BOOL IsWindowVisible(HWND hWnd);
        static BOOL IsZoomed(HWND hWnd);
        static BOOL LockSetForegroundWindow(UINT uLockCode);
        static BOOL LogicalToPhysicalPoint(HWND hWnd, ::LPPOINT lpPoint);
        static BOOL LogicalToPhysicalPointForPerMonitorDPI(HWND hWnd, LPPOINT lpPoint);
        static BOOL MoveWindow(HWND hWnd, int X, int Y, int nWidth, int nHeight, BOOL bRepaint);
        static BOOL OpenIcon(HWND hWnd);
        static BOOL PhysicalToLogicalPoint(HWND hWnd, ::LPPOINT lpPoint);
        static BOOL PhysicalToLogicalPointForPerMonitorDPI(HWND hWnd, LPPOINT lpPoint);
        static HWND RealChildWindowFromPoint(HWND hwndParent, ::POINT ptParentClientCoords);
        static UINT RealGetWindowClassT(HWND hwnd, LPTSTR ptszClassName, UINT cchClassNameMax);
        static BOOL RegisterShellHookWindow(HWND hwnd);
        static BOOL SetForegroundWindow(HWND hWnd);
        static BOOL SetLayeredWindowAttributes(HWND hwnd, COLORREF crKey, BYTE bAlpha, DWORD dwFlags);
        static HWND SetParent(HWND hWndChild, HWND hWndNewParent);
        static BOOL SetProcessDefaultLayout(DWORD dwDefaultLayout);
        static BOOL SetProcessDPIAware(void);
        static BOOL SetWindowPlacement(HWND hWnd, const ::WINDOWPLACEMENT* lpwndpl);
        static BOOL SetWindowPos(HWND hWnd, HWND hWndInsertAfter, int X, int Y, int cx, int cy, UINT uFlags);
        static BOOL SetWindowTextT(HWND hWnd, LPCTSTR lpString);
        static BOOL ShowOwnedPopups(HWND hWnd, BOOL fShow);
        static BOOL ShowWindow(HWND hWnd, int nCmdShow);
        static BOOL ShowWindowAsync(HWND hWnd, int nCmdShow);
        static BOOL SoundSentry(void);
        static void SwitchToThisWindow(HWND hwnd, BOOL fUnknown);
        static WORD TileWindows(HWND hwndParent, UINT wHow, const ::RECT* lpRect, UINT cKids, const HWND* lpKids);
        static BOOL UpdateLayeredWindow(HWND hWnd, HDC hdcDst, ::POINT* pptDst, ::SIZE* psize, HDC hdcSrc, ::POINT* pptSrc, COLORREF crKey, ::BLENDFUNCTION* pblend, DWORD dwFlags);
        static BOOL UpdateLayeredWindowIndirect(HWND hWnd, const ::UPDATELAYEREDWINDOWINFO* pULWInfo);
        static HWND WindowFromPhysicalPoint(::POINT Point);
        static HWND WindowFromPoint(::POINT Point);

        //Menu functions
        static BOOL AppendMenuT(HMENU hMenu, UINT uFlags, UINT_PTR uIDNewItem, LPCTSTR lpNewItem);
        static DWORD CheckMenuItem(HMENU hMenu, UINT uIDCheckItem, UINT uCheck);
        static BOOL CheckMenuRadioItem(HMENU hmenu, UINT first, UINT last, UINT check, UINT flags);
        static HMENU CreateMenu(void);
        static HMENU CreatePopupMenu(void);
        static BOOL DeleteMenu(HMENU hMenu, UINT uPosition, UINT uFlags);
        static BOOL DestroyMenu(HMENU hMenu);
        static BOOL DrawMenuBar(HWND hWnd);
        static BOOL EnableMenuItem(HMENU hMenu, UINT uIDEnableItem, UINT uEnable);
        static BOOL EndMenu(void);
        static HMENU GetMenu(HWND hWnd);
        static BOOL GetMenuBarInfo(HWND hwnd, LONG idObject, LONG idItem, PMENUBARINFO pmbi);
        static LONG GetMenuCheckMarkDimensions(void);
        static UINT GetMenuDefaultItem(HMENU hMenu, UINT fByPos, UINT gmdiFlags);
        static BOOL GetMenuInfo(HMENU hmenu, ::LPCMENUINFO lpcmi);
        static int GetMenuItemCount(HMENU hMenu);
        static UINT GetMenuItemID(HMENU hMenu, int nPos);
        static BOOL GetMenuItemInfoT(HMENU hmenu, UINT item, BOOL fByPosition, LPMENUITEMINFO lpmii);
        static BOOL GetMenuItemRect(HWND hWnd, HMENU hMenu, UINT uItem, ::LPRECT lprcItem);
        static UINT GetMenuState(HMENU hMenu, UINT uId, UINT uFlags);
        static int GetMenuStringT(HMENU hMenu, UINT uIDItem, LPTSTR lpString, int cchMax, UINT flags);
        static HMENU GetSubMenu(HMENU hMenu, int nPos);
        static HMENU GetSystemMenu(HWND hWnd, BOOL bRevert);
        static BOOL HiliteMenuItem(HWND hWnd, HMENU hMenu, UINT uIDHiliteItem, UINT uHilite);
        static BOOL InsertMenuT(HMENU hMenu, UINT uPosition, UINT uFlags, UINT_PTR uIDNewItem, LPCTSTR lpNewItem);
        static BOOL InsertMenuItemT(HMENU hmenu, UINT item, BOOL fByPosition, LPCMENUITEMINFO lpmi);
        static BOOL IsMenu(HMENU hMenu);
        static HMENU LoadMenuT(HINSTANCE hInstance, LPCTSTR lpMenuName);
        static HMENU LoadMenuIndirectT(CONST MENUTEMPLATE* lpMenuTemplate);
        static int MenuItemFromPoint(HWND hWnd, HMENU hMenu, POINT ptScreen);
        static BOOL ModifyMenuT(HMENU hMnu, UINT uPosition, UINT uFlags, UINT_PTR uIDNewItem, LPCTSTR lpNewItem);
        static BOOL RemoveMenu(HMENU hMenu, UINT uPosition, UINT uFlags);
        static BOOL SetMenu(HWND hWnd, HMENU hMenu);
        static BOOL SetMenuDefaultItem(HMENU hMenu, UINT uItem, UINT fByPos);
        static BOOL SetMenuInfo(HMENU hmenu, ::LPCMENUINFO lpcmi);
        static BOOL SetMenuItemBitmaps(HMENU hMenu, UINT uPosition, UINT uFlags, HBITMAP hBitmapUnchecked, HBITMAP hBitmapChecked);
        static BOOL SetMenuItemInfoT(HMENU hmenu, UINT item, BOOL fByPositon, LPMENUITEMINFO lpmii);
        static BOOL TrackPopupMenu(HMENU hMenu, UINT uFlags, int x, int y, int nReserved, HWND hWnd, const RECT* prcRect);
        static BOOL TrackPopupMenuEx(HMENU hMenu, UINT uFlags, int x, int y, HWND hwnd, ::LPTPMPARAMS lptpm);

        //Process and Thread functions
        static BOOL AttachThreadInput(DWORD idAttach, DWORD idAttachTo, BOOL fAttach);
        static DWORD GetGuiResources(HANDLE hProcess, DWORD uiFlags);
        static BOOL IsWow64Message(void);
        static BOOL UserHandleGrantAccess(HANDLE hUserHandle, HANDLE hJob, BOOL bGrant);
        static DWORD WaitForInputIdle(HANDLE hProcess, DWORD dwMilliseconds);

        //Painting functions
        static HDC BeginPaint(HWND hWnd, ::LPPAINTSTRUCT lpPaint);
        static BOOL DrawAnimatedRects(HWND hwnd, int idAni, CONST::RECT* lprcFrom, CONST::RECT* lprcTo);
        static BOOL DrawCaption(HWND hwnd, HDC hdc, ::LPCRECT lprect, UINT flags);
        static BOOL DrawEdge(HDC hdc, ::LPRECT qrc, UINT edge, UINT grfFlags);
        static BOOL DrawFocusRect(HDC hDC, CONST::RECT* lprc);
        static BOOL DrawFrameControl(HDC hdc, ::LPRECT lprc, UINT uType, UINT uState);
        static BOOL DrawStateT(HDC hdc, HBRUSH hbrFore, ::DRAWSTATEPROC qfnCallBack, LPARAM lData, WPARAM wData, int x, int y, int cx, int cy, UINT uFlags);
        static BOOL EndPaint(HWND hWnd, CONST::PAINTSTRUCT* lpPaint);
        static int ExcludeUpdateRgn(HDC hDC, HWND hWnd);
        static BOOL GetUpdateRect(HWND hWnd, ::LPRECT lpRect, BOOL bErase);
        static int GetUpdateRgn(HWND hWnd, HRGN hRgn, BOOL bErase);
        static HDC GetWindowDC(HWND hWnd);
        static int GetWindowRgn(HWND hWnd, HRGN hRgn);
        static int GetWindowRgnBox(HWND hWnd, ::LPRECT lprc);
        static BOOL GrayStringT(HDC hDC, HBRUSH hBrush, ::GRAYSTRINGPROC lpOutputFunc, LPARAM lpData, int nCount, int X, int Y, int nWidth, int nHeight);
        static BOOL InvalidateRect(HWND hWnd, CONST::RECT* lpRect, BOOL bErase);
        static BOOL InvalidateRgn(HWND hWnd, HRGN hRgn, BOOL bErase);
        static BOOL LockWindowUpdate(HWND hWndLock);
        static BOOL PaintDesktop(HDC hdc);
        static BOOL RedrawWindow(HWND hWnd, CONST::RECT* lprcUpdate, HRGN hrgnUpdate, UINT flags);
        static int SetWindowRgn(HWND hWnd, HRGN hRgn, BOOL bRedraw);
        static BOOL UpdateWindow(HWND hWnd);
        static BOOL ValidateRect(HWND hWnd, CONST::RECT* lpRect);
        static BOOL ValidateRgn(HWND hWnd, HRGN hRgn);
        static HWND WindowFromDC(HDC hDC);

        //Message and Message Queue functions
        static long BroadcastSystemMessageT(DWORD flags, LPDWORD lpInfo, UINT Msg, WPARAM wParam, LPARAM lParam);
        static long BroadcastSystemMessageExT(DWORD flags, LPDWORD lpInfo, UINT Msg, WPARAM wParam, LPARAM lParam, ::PBSMINFO pbsmInfo);
        static LRESULT DispatchMessageT(const MSG* lpMsg);
        static BOOL GetInputState(void);
        static BOOL GetMessageT(LPMSG lpMsg, HWND hWnd, UINT wMsgFilterMin, UINT wMsgFilterMax);
        static LPARAM GetMessageExtraInfo(void);
        static DWORD GetMessagePos(void);
        static LONG GetMessageTime(void);
        static DWORD GetQueueStatus(UINT flags);
        static BOOL InSendMessage(void);
        static DWORD InSendMessageEx(void);
        static BOOL PeekMessageT(LPMSG lpMsg, HWND hWnd, UINT wMsgFilterMin, UINT wMsgFilterMax, UINT wRemoveMsg);
        static BOOL PostMessageT(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
        static void PostQuitMessage(int nExitCode);
        static BOOL PostThreadMessageT(DWORD idThread, UINT Msg, WPARAM wParam, LPARAM lParam);
        static UINT RegisterWindowMessageT(LPCTSTR lpString);
        static BOOL ReplyMessage(LRESULT lResult);
        static BOOL SendMessageCallbackT(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam, ::SENDASYNCPROC lpResultCallBack, ULONG_PTR dwData);
        static LRESULT SendMessageTimeoutT(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam, UINT fuFlags, UINT uTimeout, PDWORD_PTR lpdwResult);
        static LRESULT SendMessageT(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
        static BOOL SendNotifyMessageT(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
        static LPARAM SetMessageExtraInfo(LPARAM lParam);
        static BOOL TranslateMessage(const MSG* lpMsg);
        static BOOL WaitMessage(void);

        //Hook functions
        static BOOL CallMsgFilterT(LPMSG lpMsg, int nCode);
        static LRESULT CallNextHookEx(HHOOK hhk, int nCode, WPARAM wParam, LPARAM lParam);
        static HHOOK SetWindowsHookExT(int idHook, HOOKPROC lpfn, HINSTANCE hmod, DWORD dwThreadId);
        static BOOL UnhookWindowsHookEx(HHOOK hhk);

        //Window Procedure functions
        static LRESULT CallWindowProcT(WNDPROC lpPrevWndFunc, HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
        static LRESULT DefWindowProcT(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);

        //DC functions
        static LONG ChangeDisplaySettingsT(::LPDEVMODE lpDevMode, DWORD dwFlags);
        static LONG ChangeDisplaySettingsExT(LPCTSTR lpszDeviceName, ::LPDEVMODE lpDevMode, HWND hwnd, DWORD dwflags, LPVOID lParam);
        static BOOL EnumDisplayDevicesT(LPCTSTR lpDevice, DWORD iDevNum, ::PDISPLAY_DEVICE lpDisplayDevice, DWORD dwFlags);
        static BOOL EnumDisplaySettingsT(LPCTSTR lpszDeviceName, DWORD iModeNum, ::LPDEVMODE lpDevMode);
        static BOOL EnumDisplaySettingsExT(LPCTSTR lpszDeviceName, DWORD iModeNum, ::LPDEVMODE lpDevMode, DWORD dwFlags);
        static HDC GetDC(HWND hWnd);
        static HDC GetDCEx(HWND hWnd, HRGN hrgnClip, DWORD flags);
        static int ReleaseDC(HWND hWnd, HDC hDC);

        //String functions
        LPSTR CharLowerA(LPSTR lpsz);
        LPWSTR CharLowerW(LPWSTR lpwsz);
        DWORD CharLowerBuffA(LPSTR lpsz, DWORD cchLength);
        DWORD CharLowerBuffW(LPWSTR lpwsz, DWORD cchLength);
        LPSTR CharNextA(LPCSTR lpsz);
        LPWSTR CharNextW(LPCWSTR lpwsz);
        LPSTR CharNextExA(WORD CodePage, LPCSTR lpCurrentChar, DWORD dwFlags);
        LPSTR CharPrevA(LPCSTR lpszStart, LPCSTR lpszCurrent);
        LPWSTR CharPrevW(LPCWSTR lpwszStart, LPCWSTR lpwszCurrent);
        LPSTR CharPrevExA(WORD CodePage, LPCSTR lpStart, LPCSTR lpCurrentChar, DWORD dwFlags);
        BOOL CharToOemA(LPCSTR lpszSrc, LPSTR lpszDst);
        BOOL CharToOemW(LPCWSTR lpwszSrc, LPSTR lpszDst);
        BOOL CharToOemBuffA(LPCSTR lpszSrc, LPSTR lpszDst, DWORD cchDstLength);
        BOOL CharToOemBuffW(LPCWSTR lpwszSrc, LPSTR lpszDst, DWORD cchDstLength);
        LPSTR CharUpperA(LPSTR lpsz);
        LPWSTR CharUpperW(LPWSTR lpwsz);
        DWORD CharUpperBuffA(LPSTR lpsz, DWORD cchLength);
        DWORD CharUpperBuffW(LPWSTR lpwsz, DWORD cchLength);
        BOOL IsCharAlphaA(char ch);
        BOOL IsCharAlphaW(wchar_t wch);
        BOOL IsCharAlphaNumericA(char ch);
        BOOL IsCharAlphaNumericW(wchar_t wch);
        BOOL IsCharLowerA(char ch);
        BOOL IsCharLowerW(wchar_t wch);
        BOOL IsCharUpperA(char ch);
        BOOL IsCharUpperW(wchar_t wch);
        int LoadStringA(HINSTANCE hInstance, UINT uID, LPSTR lpBuffer, int nBufferMax);
        int LoadStringW(HINSTANCE hInstance, UINT uID, LPWSTR lpBuffer, int nBufferMax);
        BOOL OemToCharA(LPCSTR lpszSrc, LPSTR lpszDst);
        BOOL OemToCharW(LPCSTR lpszSrc, LPWSTR lpwszDst);
        BOOL OemToCharBuffA(LPCSTR lpszSrc, LPSTR lpszDst, DWORD cchDstLength);
        BOOL OemToCharBuffW(LPCWSTR lpwszSrc, LPWSTR lpwszDst, DWORD cchDstLength);
        int wvsprintfA(LPSTR lpOutput, LPCSTR lpFmt, va_list arglist);
        int wvsprintfW(LPWSTR lpOutput, LPCWSTR lpFmt, va_list arglist);

        //Button Control functions
        BOOL CheckDlgButton(HWND hDlg, int nIDButton, UINT uCheck);
        BOOL CheckRadioButton(HWND hDlg, int nIDFirstButton, int nIDLastButton, int nIDCheckButton);
        UINT IsDlgButtonChecked(HWND hDlg, int nIDButton);

        //Coordinate Transform functions
        static BOOL ClientToScreen(HWND hWnd, ::LPPOINT lpPoint);
        static BOOL ScreenToClient(HWND hWnd, ::LPPOINT lpPoint);
        static int MapWindowPoints(HWND hWndFrom, HWND hWndTo, ::LPPOINT lpPoints, UINT cPoints);

        //Cursor functions
        static BOOL ClipCursor(const RECT* lpRect);
        static HCURSOR CopyCursorT(HCURSOR hIcon);
        static HCURSOR CreateCursor(HINSTANCE hInst, int xHotSpot, int yHotSpot, int nWidth, int nHeight, const VOID* pvANDPlane, const VOID* pvXORPlane);
        static BOOL DestroyCursor(HCURSOR hCursor);
        static BOOL GetClipCursor(LPRECT lpRect);
        static HCURSOR GetCursor(void);
        static BOOL GetCursorInfo(PCURSORINFO pci);
        static BOOL GetCursorPos(LPPOINT lpPoint);
        static BOOL GetPhysicalCursorPos(LPPOINT lpPoint);
        static HCURSOR LoadCursorT(HINSTANCE hInstance, LPCTSTR lpCursorName);
        static HCURSOR LoadCursorFromFileT(LPCTSTR lpFileName);
        static BOOL SetCursorPos(int X, int Y);
        static BOOL SetPhysicalCursorPos(int X, int Y);
        static int ShowCursor(BOOL bShow);
        static HCURSOR SetCursor(HCURSOR hCursor);
        static BOOL SetSystemCursor(HCURSOR hcur, DWORD id);

        //Window Station and Desktop functions
        static HDESK OpenDesktopT(LPCTSTR lpszDesktop, DWORD dwFlags, BOOL fInherit, ACCESS_MASK dwDesiredAccess);
        static BOOL CloseDesktop(HDESK hDesktop);
        static HDESK CreateDesktopT(LPCTSTR lpszDesktop, LPCTSTR lpszDevice, ::LPDEVMODE pDevmode, DWORD dwFlags, ACCESS_MASK dwDesiredAccess, ::LPSECURITY_ATTRIBUTES lpsa);
        static HDESK CreateDesktopExT(LPCTSTR lpszDesktop, LPCTSTR lpszDevice, ::LPDEVMODE pDevmode, DWORD dwFlags, ACCESS_MASK dwDesiredAccess, ::LPSECURITY_ATTRIBUTES lpsa, ULONG ulHeapSize, PVOID pvoid);
        static BOOL EnumDesktopsT(HWINSTA hwinsta, DESKTOPENUMPROC lpEnumFunc, LPARAM lParam);
        static HWINSTA CreateWindowStationT(LPCTSTR lpwinsta, DWORD dwFlags, ACCESS_MASK dwDesiredAccess, ::LPSECURITY_ATTRIBUTES lpsa);
        static BOOL CloseWindowStation(HWINSTA hWinSta);
        static BOOL EnumWindowStationsT(WINSTAENUMPROC lpEnumFunc, LPARAM lParam);
        static HWINSTA GetProcessWindowStation(void);
        static HDESK GetThreadDesktop(DWORD dwThreadId);
        static BOOL GetUserObjectInformationT(HANDLE hObj, int nIndex, PVOID pvInfo, DWORD nLength, LPDWORD lpnLengthNeeded);
        static HDESK OpenInputDesktop(DWORD dwFlags, BOOL fInherit, ACCESS_MASK dwDesiredAccess);
        static HWINSTA OpenWindowStationT(LPCTSTR lpszWinSta, BOOL fInherit, ACCESS_MASK dwDesiredAccess);
        static BOOL SetProcessWindowStation(HWINSTA hWinSta);
        static BOOL SetThreadDesktop(HDESK hDesktop);
        static BOOL SetUserObjectInformationT(HANDLE hObj, int nIndex, PVOID pvInfo, DWORD nLength);
        static BOOL SwitchDesktop(HDESK hDesktop);
        static BOOL EnumDesktopWindows(HDESK hDesktop, ::WNDENUMPROC lpfn, LPARAM lParam);

        //Keyboard Accelerator functions
        static int CopyAcceleratorTableT(HACCEL hAccelSrc, LPACCEL lpAccelDst, int cAccelEntries);
        static HACCEL CreateAcceleratorTableT(LPACCEL paccel, int cAccel);
        static BOOL DestroyAcceleratorTable(HACCEL hAccel);
        static HACCEL LoadAcceleratorsT(HINSTANCE hInstance, LPCTSTR lpTableName);
        static int TranslateAcceleratorT(HWND hWnd, HACCEL hAccTable, LPMSG lpMsg);

        //Icon functions
        static HICON CopyIcon(HICON hIcon);
        static HICON CreateIcon(HINSTANCE hInstance, int nWidth, int nHeight, BYTE cPlanes, BYTE cBitsPixel, const BYTE* lpbANDbits, const BYTE* lpbXORbits);
        static HICON CreateIconFromResource(PBYTE presbits, DWORD dwResSize, BOOL fIcon, DWORD dwVer);
        static HICON CreateIconFromResourceEx(PBYTE presbits, DWORD dwResSize, BOOL fIcon, DWORD dwVer, int cxDesired, int cyDesired, UINT Flags);
        static HICON CreateIconIndirect(const ::ICONINFO* piconinfo);
        static BOOL DestroyIcon(HICON hIcon);
        static BOOL DrawIcon(HDC hDC, int X, int Y, HICON hIcon);
        static BOOL DrawIconEx(HDC hdc, int xLeft, int yTop, HICON hIcon, int cxWidth, int cyWidth, UINT istepIfAniCur, HBRUSH hbrFlickerFreeDraw, UINT diFlags);
        static BOOL GetIconInfo(HICON hIcon, ::PICONINFO piconinfo);
        static BOOL GetIconInfoExT(HICON hicon, ::PICONINFOEX piconinfo);
        static HICON LoadIconT(HINSTANCE hInstance, LPCTSTR lpIconName);
        static int LookupIconIdFromDirectory(PBYTE presbits, BOOL fIcon);
        static int LookupIconIdFromDirectoryEx(PBYTE presbits, BOOL fIcon, int cxDesired, int cyDesired, UINT Flags);
        static UINT PrivateExtractIconsT(LPCTSTR szFileName, int nIconIndex, int cxIcon, int cyIcon, HICON* phicon, UINT* piconid, UINT nIcons, UINT flags);

        //Resource functions
        static HANDLE CopyImage(HANDLE h, UINT type, int cx, int cy, UINT flags);
        static HANDLE LoadImageT(HINSTANCE hInst, LPCTSTR name, UINT type, int cx, int cy, UINT fuLoad);

        //Rectangle functions
        static BOOL CopyRect(::LPRECT lprcDst, CONST::RECT* lprcSrc);
        static BOOL EqualRect(CONST::RECT* lprc1, CONST::RECT* lprc2);
        static BOOL InflateRect(::LPRECT lprc, int dx, int dy);
        static BOOL IntersectRect(::LPRECT lprcDst, CONST::RECT* lprcSrc1, CONST::RECT* lprcSrc2);
        static BOOL IsRectEmpty(CONST::RECT* lprc);
        static BOOL OffsetRect(::LPRECT lprc, int dx, int dy);
        static BOOL PtInRect(CONST::RECT* lprc, POINT pt);
        static BOOL SetRect(::LPRECT lprc, int xLeft, int yTop, int xRight, int yBottom);
        static BOOL SetRectEmpty(::LPRECT lprc);
        static BOOL SubtractRect(::LPRECT lprcDst, CONST::RECT* lprcSrc1, CONST::RECT* lprcSrc2);
        static BOOL UnionRect(::LPRECT lprcDst, CONST::RECT* lprcSrc1, CONST::RECT* lprcSrc2);

        //Caret functions
        static BOOL CreateCaret(HWND hWnd, HBITMAP hBitmap, int nWidth, int nHeight);
        static BOOL DestroyCaret(void);
        static UINT GetCaretBlinkTime(void);
        static BOOL GetCaretPos(LPPOINT lpPoint);
        static BOOL HideCaret(HWND hWnd);
        static BOOL SetCaretBlinkTime(UINT uMSeconds);
        static BOOL SetCaretPos(int X, int Y);
        static BOOL ShowCaret(HWND hWnd);

        //Dialog functions
        static HWND CreateDialogIndirectParamT(HINSTANCE hInstance, LPCDLGTEMPLATE lpTemplate, HWND hWndParent, ::DLGPROC lpDialogFunc, LPARAM dwInitParam);
        static HWND CreateDialogParamT(HINSTANCE hInstance, LPCTSTR lpTemplateName, HWND hWndParent, ::DLGPROC lpDialogFunc, LPARAM dwInitParam);
        static LRESULT DefDlgProcT(HWND hDlg, UINT Msg, WPARAM wParam, LPARAM lParam);
        static INT_PTR DialogBoxIndirectParamT(HINSTANCE hInstance, LPCDLGTEMPLATE hDialogTemplate, HWND hWndParent, ::DLGPROC lpDialogFunc, LPARAM dwInitParam);
        static INT_PTR DialogBoxParamT(HINSTANCE hInstance, LPCTSTR lpTemplateName, HWND hWndParent, ::DLGPROC lpDialogFunc, LPARAM dwInitParam);
        static BOOL EndDialog(HWND hDlg, INT_PTR nResult);
        static LONG GetDialogBaseUnits(void);
        static int GetDlgCtrlID(HWND hWnd);
        static HWND GetDlgItem(HWND hDlg, int nIDDlgItem);
        static UINT GetDlgItemInt(HWND hDlg, int nIDDlgItem, BOOL* lpTranslated, BOOL bSigned);
        static UINT GetDlgItemTextT(HWND hDlg, int nIDDlgItem, LPTSTR lpString, int cchMax);
        static HWND GetNextDlgGroupItem(HWND hDlg, HWND hCtl, BOOL bPrevious);
        static HWND GetNextDlgTabItem(HWND hDlg, HWND hCtl, BOOL bPrevious);
        static BOOL IsDialogMessageT(HWND hDlg, LPMSG lpMsg);
        static BOOL MapDialogRect(HWND hDlg, LPRECT lpRect);
        static int MessageBoxT(HWND hWnd, LPCTSTR lpText, LPCTSTR lpCaption, UINT uType);
        static int MessageBoxExT(HWND hWnd, LPCTSTR lpText, LPCTSTR lpCaption, UINT uType, WORD wLanguageId);
        static int MessageBoxIndirectT(const ::LPMSGBOXPARAMS lpmbp);
        //NOTE: Thanks to CPian Maurizio Pisano
        static int MessageBoxTimeout(HWND hWnd, LPCTSTR lpText, LPCTSTR lpCaption, UINT uType, WORD wLanguageId, DWORD dwMilliseconds);
        static LRESULT SendDlgItemMessageT(HWND hDlg, int nIDDlgItem, UINT Msg, WPARAM wParam, LPARAM lParam);
        static BOOL SetDlgItemInt(HWND hDlg, int nIDDlgItem, UINT uValue, BOOL bSigned);
        static BOOL SetDlgItemTextT(HWND hDlg, int nIDDlgItem, LPCTSTR lpString);

        //Multi Document Interface functions
        static HWND CreateMDIWindowT(LPCTSTR lpClassName, LPCTSTR lpWindowName, DWORD dwStyle, int X, int Y, int nWidth, int nHeight, HWND hWndParent, HINSTANCE hInstance, LPARAM lParam);
        static LRESULT DefFrameProcT(HWND hWnd, HWND hWndMDIClient, UINT uMsg, WPARAM wParam, LPARAM lParam);
        static LRESULT DefMDIChildProcT(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
        static BOOL TranslateMDISysAccel(HWND hWndClient, LPMSG lpMsg);

        //Dynamic Data Exchange functions
        BOOL DdeAbandonTransaction(DWORD idInst, HCONV hConv, DWORD idTransaction) const;
        LPBYTE DdeAccessData(HDDEDATA hData, LPDWORD pcbDataSize, DWORD idInst) const;
        BOOL DdeUnaccessData(HDDEDATA hData, DWORD idInst) const;
        HDDEDATA DdeAddData(HDDEDATA hData, LPBYTE pSrc, DWORD cb, DWORD cbOff, DWORD idInst) const;
        HDDEDATA DdeClientTransaction(LPBYTE pData, DWORD cbData, HCONV hConv, HSZ hszItem, UINT wFmt, UINT wType, DWORD dwTimeout, LPDWORD pdwResult, DWORD idInst) const;
        int DdeCmpStringHandles(HSZ hsz1, HSZ hsz2) const;
        HCONV DdeConnect(DWORD idInst, HSZ hszService, HSZ hszTopic, PCONVCONTEXT pCC) const;
        HCONVLIST DdeConnectList(DWORD idInst, HSZ hszService, HSZ hszTopic, HCONVLIST hConvList, PCONVCONTEXT pCC) const;
        HDDEDATA DdeCreateDataHandle(DWORD idInst, LPBYTE pSrc, DWORD cb, DWORD cbOff, HSZ hszItem, UINT wFmt, UINT afCmd) const;
        HSZ mxDdeCreateStringHandle(DWORD idInst, LPTSTR psz, int iCodePage) const;
        BOOL DdeDisconnect(HCONV hConv, DWORD idInst) const;
        BOOL DdeDisconnectList(HCONVLIST hConvList, DWORD idInst) const;
        BOOL DdeEnableCallback(DWORD idInst, HCONV hConv, UINT wCmd) const;
        BOOL DdeFreeDataHandle(HDDEDATA hData, DWORD idInst) const;
        BOOL DdeFreeStringHandle(DWORD idInst, HSZ hsz) const;
        DWORD DdeGetData(HDDEDATA hData, LPBYTE pDst, DWORD cbMax, DWORD cbOff, DWORD idInst) const;
        UINT DdeGetLastError(DWORD idInst) const;
        BOOL DdeImpersonateClient(HCONV hConv, DWORD idInst) const;
        UINT DdeInitialize(LPDWORD pidInst, PFNCALLBACK pfnCallback, DWORD afCmd, DWORD ulRes) const;
        BOOL DdeKeepStringHandle(DWORD idInst, HSZ hsz) const;
        HDDEDATA DdeNameService(DWORD idInst, UINT hsz1, UINT hsz2, UINT afCmd) const;
        BOOL DdePostAdvise(DWORD idInst, HSZ hszTopic, HSZ hszItem) const;
        UINT DdeQueryConvInfo(HCONV hConv, DWORD idTransaction, PCONVINFO pConvInfo, DWORD idInst) const;
        HCONV DdeQueryNextServer(HCONVLIST hConvList, HCONV hConvPrev) const;
        DWORD DdeQueryString(DWORD idInst, HSZ hsz, LPTSTR psz, DWORD cchMax, int iCodePage) const;
        HCONV DdeReconnect(HCONV hConv, DWORD idInst) const;
        BOOL DdeSetQualityOfService(HWND hwndClient, const SECURITY_QUALITY_OF_SERVICE* pqosNew, PSECURITY_QUALITY_OF_SERVICE pqosPrev) const;
        BOOL DdeSetUserHandle(HCONV hConv, DWORD id, DWORD_PTR hUser, DWORD idInst) const;
        BOOL DdeUninitialize(DWORD idInst) const;
        BOOL FreeDDElParam(UINT msg, LPARAM lParam) const;
        LPARAM PackDDElParam(UINT msg, UINT_PTR uiLo, UINT_PTR uiHi) const;
        BOOL ImpersonateDdeClientWindow(HWND hWndClient, HWND hWndServer) const;
        LPARAM ReuseDDElParam(LPARAM lParam, UINT msgIn, UINT msgOut, UINT_PTR uiLo, UINT_PTR uiHi) const;
        BOOL UnpackDDElParam(UINT msg, LPARAM lParam, PUINT_PTR puiLo, PUINT_PTR puiHi) const;

        //Raw Input functions
        LRESULT DefRawInputProc(PRAWINPUT* paRawInput, INT nInput, UINT cbSizeHeader);
        UINT GetRawInputBuffer(PRAWINPUT pData, PUINT pcbSize, UINT cbSizeHeader);
        UINT GetRawInputData(HRAWINPUT hRawInput, UINT uiCommand, LPVOID pData, PUINT pcbSize, UINT cbSizeHeader);
        UINT GetRawInputDeviceInfo(HANDLE hDevice, UINT uiCommand, LPVOID pData, PUINT pcbSize);
        UINT GetRawInputDeviceList(PRAWINPUTDEVICELIST pRawInputDeviceList, PUINT puiNumDevices, UINT cbSize);
        UINT GetRegisteredRawInputDevices(PRAWINPUTDEVICE pRawInputDevices, PUINT puiNumDevices, UINT cbSize);
        BOOL RegisterRawInputDevices(PCRAWINPUTDEVICE pRawInputDevices, UINT uiNumDevices, UINT cbSize);

        //List Box functions
        int DlgDirList(HWND hDlg, LPTSTR lpPathSpec, int nIDListBox, int nIDStaticPath, UINT uFileType);
        BOOL DlgDirSelectEx(HWND hDlg, LPTSTR lpString, int nCount, int nIDListBox);
        DWORD GetListBoxInfo(HWND hwnd);

        //Combo Box functions
        static int DlgDirListComboBoxT(HWND hDlg, LPTSTR lpPathSpec, int nIDComboBox, int nIDStaticPath, UINT uFiletype);
        static BOOL DlgDirSelectComboBoxExT(HWND hwndDlg, LPTSTR lpString, int cchOut, int idComboBox);
        static BOOL GetComboBoxInfo(HWND hwndCombo, PCOMBOBOXINFO pcbi);

        //Mouse functions
        static BOOL DragDetect(HWND hwnd, ::POINT pt);
        static HWND GetCapture(void);
        static UINT GetDoubleClickTime(void);
        static int GetMouseMovePointsEx(UINT cbSize, LPMOUSEMOVEPOINT lppt, LPMOUSEMOVEPOINT lpptBuf, int nBufPoints, DWORD resolution);
        static VOID mouse_event(DWORD dwFlags, DWORD dx, DWORD dy, DWORD dwData, ULONG_PTR dwExtraInfo);
        static BOOL ReleaseCapture(void);
        static HWND SetCapture(HWND hWnd);
        static BOOL SetDoubleClickTime(UINT uInterval);
        static BOOL SwapMouseButton(BOOL fSwap);
        static BOOL TrackMouseEvent(LPTRACKMOUSEEVENT lpEventTrack);

        //Pointer input functions
        static HSYNTHETICPOINTERDEVICE CreateSyntheticPointerDevice(POINTER_INPUT_TYPE pointerType, ULONG maxCount, POINTER_FEEDBACK_MODE mode);
        static void DestroySyntheticPointerDevice(HSYNTHETICPOINTERDEVICE device);
        static BOOL EnableMouseInPointer(BOOL fEnable);
        static BOOL GetPointerCursorId(UINT32 pointerId, UINT32* cursorId);
        static BOOL GetPointerDevice(HANDLE device, POINTER_DEVICE_INFO* pointerDevice);
        static BOOL GetPointerDeviceCursors(HANDLE device, UINT32* cursorCount, POINTER_DEVICE_CURSOR_INFO* deviceCursors);
        static BOOL GetPointerDeviceProperties(HANDLE device, UINT32* propertyCount, POINTER_DEVICE_PROPERTY* pointerProperties);
        static BOOL GetPointerDeviceRects(HANDLE device, RECT* pointerDeviceRect, RECT* displayRect);
        static BOOL GetPointerDevices(UINT32* deviceCount, POINTER_DEVICE_INFO* pointerDevices);
        static BOOL GetPointerFrameInfo(UINT32 pointerId, UINT32* pointerCount, POINTER_INFO* pointerInfo);
        static BOOL GetPointerFrameInfoHistory(UINT32 pointerId, UINT32* entriesCount, UINT32* pointerCount, POINTER_INFO* pointerInfo);
        static BOOL GetPointerFramePenInfo(UINT32 pointerId, UINT32* pointerCount, POINTER_PEN_INFO* penInfo);
        static BOOL GetPointerFramePenInfoHistory(UINT32 pointerId, UINT32* entriesCount, UINT32* pointerCount, POINTER_PEN_INFO* penInfo);
        static BOOL GetPointerFrameTouchInfo(UINT32 pointerId, UINT32* pointerCount, POINTER_TOUCH_INFO* touchInfo);
        static BOOL GetPointerFrameTouchInfoHistory(UINT32 pointerId, UINT32* entriesCount, UINT32* pointerCount, POINTER_TOUCH_INFO* touchInfo);
        static BOOL GetPointerInfo(UINT32 pointerId, POINTER_INFO* pointerInfo);
        static BOOL GetPointerInfoHistory(UINT32 pointerId, UINT32* entriesCount, POINTER_INFO* pointerInfo);
        static BOOL GetPointerInputTransform(UINT32 pointerId, UINT32 historyCount, INPUT_TRANSFORM* inputTransform);
        static BOOL GetPointerPenInfo(UINT32 pointerId, POINTER_PEN_INFO* penInfo);
        static BOOL GetPointerPenInfoHistory(UINT32 pointerId, UINT32* entriesCount, POINTER_PEN_INFO* penInfo);
        static BOOL GetPointerTouchInfo(UINT32 pointerId, POINTER_TOUCH_INFO* touchInfo);
        static BOOL GetPointerTouchInfoHistory(UINT32 pointerId, UINT32* entriesCount, POINTER_TOUCH_INFO* touchInfo);
        static BOOL GetPointerType(UINT32 pointerId, POINTER_INPUT_TYPE* pointerType);
        static BOOL GetRawPointerDeviceData(UINT32 pointerId, UINT32 historyCount, UINT32 propertiesCount, POINTER_DEVICE_PROPERTY* pProperties, LONG* pValues);
        static BOOL InjectSyntheticPointerInput(HSYNTHETICPOINTERDEVICE device, CONST POINTER_TYPE_INFO* pointerInfo, UINT32 count);
        static BOOL IsMouseInPointerEnabled(void);
        static BOOL RegisterPointerDeviceNotifications(HWND window, BOOL notifyRange);
        static BOOL RegisterPointerInputTarget(HWND hwnd, POINTER_INPUT_TYPE pointerType);
        static BOOL RegisterPointerInputTargetEx(HWND hwnd, POINTER_INPUT_TYPE pointerType, BOOL fObserve);
        static BOOL SkipPointerFrameMessages(UINT32 pointerId);
        static BOOL UnregisterPointerInputTarget(HWND hwnd, POINTER_INPUT_TYPE pointerType);
        static BOOL UnregisterPointerInputTargetEx(HWND hwnd, POINTER_INPUT_TYPE pointerType);

        //DPI awareness functions
        static BOOL AreDpiAwarenessContextsEqual(DPI_AWARENESS_CONTEXT dpiContextA, DPI_AWARENESS_CONTEXT dpiContextB);
        static BOOL EnableNonClientDpiScaling(HWND hwnd);
        static DPI_AWARENESS GetAwarenessFromDpiAwarenessContext(DPI_AWARENESS_CONTEXT value);
        static DIALOG_CONTROL_DPI_CHANGE_BEHAVIORS GetDialogControlDpiChangeBehavior(HWND hWnd);
        static DIALOG_DPI_CHANGE_BEHAVIORS GetDialogDpiChangeBehavior(HWND hDlg);
        //GetDpiAwarenessContextForProcess is the only member here whose SDK declaration is guarded by
        //NTDDI_VERSION (#if (NTDDI_VERSION >= NTDDI_WIN10_19H1) at winuser.h:14545) rather than WINVER -
        //live under the sdkddkver.h default, but a caller that pins NTDDI_VERSION lower loses the prototype.
        static DPI_AWARENESS_CONTEXT GetDpiAwarenessContextForProcess(HANDLE hProcess);
        static UINT GetDpiForSystem(void);
        static UINT GetDpiFromDpiAwarenessContext(DPI_AWARENESS_CONTEXT value);
        static UINT GetSystemDpiForProcess(HANDLE hProcess);
        static int GetSystemMetricsForDpi(int nIndex, UINT dpi);
        static DPI_AWARENESS_CONTEXT GetThreadDpiAwarenessContext(void);
        static DPI_HOSTING_BEHAVIOR GetThreadDpiHostingBehavior(void);
        static DPI_AWARENESS_CONTEXT GetWindowDpiAwarenessContext(HWND hwnd);
        static DPI_HOSTING_BEHAVIOR GetWindowDpiHostingBehavior(HWND hwnd);
        static BOOL IsValidDpiAwarenessContext(DPI_AWARENESS_CONTEXT value);
        static BOOL SetDialogControlDpiChangeBehavior(HWND hWnd, DIALOG_CONTROL_DPI_CHANGE_BEHAVIORS mask, DIALOG_CONTROL_DPI_CHANGE_BEHAVIORS values);
        static BOOL SetDialogDpiChangeBehavior(HWND hDlg, DIALOG_DPI_CHANGE_BEHAVIORS mask, DIALOG_DPI_CHANGE_BEHAVIORS values);
        static BOOL SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT value);
        static DPI_AWARENESS_CONTEXT SetThreadDpiAwarenessContext(DPI_AWARENESS_CONTEXT dpiContext);
        static DPI_HOSTING_BEHAVIOR SetThreadDpiHostingBehavior(DPI_HOSTING_BEHAVIOR value);
        static BOOL SystemParametersInfoForDpi(UINT uiAction, UINT uiParam, PVOID pvParam, UINT fWinIni, UINT dpi);

        //Touch and gesture functions
        //HTOUCHINPUT (winuser.h:6193) and HGESTUREINFO (winuser.h:15691) are handles the OS hands out in the lParam
        //of WM_TOUCH and WM_GESTURE - no wrapper here creates one, and the two Close* members close exactly those.
        static BOOL CloseGestureInfoHandle(HGESTUREINFO hGestureInfo);
        static BOOL CloseTouchInputHandle(HTOUCHINPUT hTouchInput);
        static BOOL GetGestureConfig(HWND hwnd, DWORD dwReserved, DWORD dwFlags, PUINT pcIDs, PGESTURECONFIG pGestureConfig, UINT cbSize);
        static BOOL GetGestureExtraArgs(HGESTUREINFO hGestureInfo, UINT cbExtraArgs, PBYTE pExtraArgs);
        static BOOL GetGestureInfo(HGESTUREINFO hGestureInfo, PGESTUREINFO pGestureInfo);
        static BOOL GetTouchInputInfo(HTOUCHINPUT hTouchInput, UINT cInputs, PTOUCHINPUT pInputs, int cbSize);
        static BOOL InitializeTouchInjection(UINT32 maxCount, DWORD dwMode);
        static BOOL InjectTouchInput(UINT32 count, CONST POINTER_TOUCH_INFO *contacts);
        static BOOL IsTouchWindow(HWND hwnd, PULONG pulFlags);
        //Returns LRESULT, not BOOL: the value is what a WM_TOUCHHITTESTING handler returns, not a success flag.
        static LRESULT PackTouchHitTestingProximityEvaluation(const TOUCH_HIT_TESTING_INPUT *pHitTestingInput, const TOUCH_HIT_TESTING_PROXIMITY_EVALUATION *pProximityEval);
        static BOOL RegisterTouchHitTestingWindow(HWND hwnd, ULONG value);
        static BOOL RegisterTouchWindow(HWND hwnd, ULONG ulFlags);
        static BOOL SetGestureConfig(HWND hwnd, DWORD dwReserved, UINT cIDs, PGESTURECONFIG pGestureConfig, UINT cbSize);
        static BOOL UnregisterTouchWindow(HWND hwnd);

        //Display configuration functions
        //Five of these return LONG - a Win32 error code, ERROR_SUCCESS on success - rather than a BOOL, so no member
        //here takes a CheckReturn: non-zero IS the failure. Their DISPLAYCONFIG_* types are defined in wingdi.h
        //(:2931 MODE_INFO, :3005 PATH_INFO, :3022 TOPOLOGY_ID, :3062 DEVICE_INFO_HEADER), which is why winuser.h
        //declares those five only inside #ifdef _WINGDI_ + #ifndef NOGDI; the QDC_* / SDC_* flag groups the flags
        //parameters take are in wingdi.h too (:3330, :3341), not in winuser.h. QueryDisplayConfig's two count
        //parameters are in/out (capacity in, used count out) - GetDisplayConfigBufferSizes exists to fill them
        //first. SetDisplayConfig is the only member of this library that changes machine state.
        static LONG DisplayConfigGetDeviceInfo(DISPLAYCONFIG_DEVICE_INFO_HEADER* requestPacket);
        static LONG DisplayConfigSetDeviceInfo(DISPLAYCONFIG_DEVICE_INFO_HEADER* setPacket);
        static BOOL GetDisplayAutoRotationPreferences(ORIENTATION_PREFERENCE* pOrientation);
        static LONG GetDisplayConfigBufferSizes(UINT32 flags, UINT32* numPathArrayElements, UINT32* numModeInfoArrayElements);
        static LONG QueryDisplayConfig(UINT32 flags, UINT32* numPathArrayElements, DISPLAYCONFIG_PATH_INFO* pathArray, UINT32* numModeInfoArrayElements, DISPLAYCONFIG_MODE_INFO* modeInfoArray, DISPLAYCONFIG_TOPOLOGY_ID* currentTopologyId);
        static BOOL SetDisplayAutoRotationPreferences(ORIENTATION_PREFERENCE orientation);
        static LONG SetDisplayConfig(UINT32 numPathArrayElements, DISPLAYCONFIG_PATH_INFO* pathArray, UINT32 numModeInfoArrayElements, DISPLAYCONFIG_MODE_INFO* modeInfoArray, UINT32 flags);

        //Font and Text functions
        static int DrawTextT(HDC hdc, LPCTSTR lpchText, int cchText, ::LPRECT lprc, UINT format);
        static int DrawTextExT(HDC hdc, LPTSTR lpchText, int cchText, ::LPRECT lprc, UINT format, ::LPDRAWTEXTPARAMS lpdtp);
        static DWORD GetTabbedTextExtentT(HDC hdc, LPCTSTR lpString, int chCount, int nTabPositions, CONST LPINT lpnTabStopPositions);
        static LONG TabbedTextOutT(HDC hdc, int x, int y, LPCTSTR lpString, int chCount, int nTabPositions, CONST::LPINT lpnTabStopPositions, int nTabOrigin);

        //ScrollBar functions
        static BOOL EnableScrollBar(HWND hWnd, UINT wSBflags, UINT wArrows);
        static BOOL GetScrollBarInfo(HWND hwnd, LONG idObject, ::PSCROLLBARINFO psbi);
        static BOOL GetScrollInfo(HWND hwnd, int nBar, ::LPSCROLLINFO lpsi);
        static int GetScrollPos(HWND hWnd, int nBar);
        static BOOL GetScrollRange(HWND hWnd, int nBar, LPINT lpMinPos, LPINT lpMaxPos);
        static BOOL ScrollDC(HDC hDC, int dx, int dy, const ::RECT *lprcScroll, const ::RECT *lprcClip, HRGN hrgnUpdate, ::LPRECT lprcUpdate);
        static BOOL ScrollWindow(HWND hWnd, int XAmount, int YAmount, const ::RECT* lpRect, const ::RECT* lpClipRect);
        static int ScrollWindowEx(HWND hWnd, int dx, int dy, const ::RECT* prcScroll, const ::RECT* prcClip, HRGN hrgnUpdate, ::LPRECT prcUpdate, UINT flags);
        static int SetScrollInfo(HWND hwnd, int nBar, ::LPCSCROLLINFO lpsi, BOOL redraw);
        static int SetScrollPos(HWND hWnd, int nBar, int nPos, BOOL bRedraw);
        static BOOL SetScrollRange(HWND hWnd, int nBar, int nMinPos, int nMaxPos, BOOL bRedraw);
        static BOOL ShowScrollBar(HWND hWnd, int wBar, BOOL bShow);

        //Monitor functions
        static BOOL EnumDisplayMonitors(HDC hdc, LPCRECT lprcClip, ::MONITORENUMPROC lpfnEnum, LPARAM dwData);
        static BOOL GetMonitorInfoT(HMONITOR hMonitor, ::LPMONITORINFO lpmi);
        static HMONITOR MonitorFromPoint(::POINT pt, DWORD dwFlags);
        static HMONITOR MonitorFromRect(LPCRECT lprc, DWORD dwFlags);
        static HMONITOR MonitorFromWindow(HWND hwnd, DWORD dwFlags);

        //Window Properties functions
        static int EnumPropsT(HWND hWnd, PROPENUMPROC lpEnumFunc);
        static int EnumPropsExT(HWND hWnd, PROPENUMPROCEX lpEnumFunc, LPARAM lParam);
        static HANDLE GetPropT(HWND hWnd, LPCTSTR lpString);
        static HANDLE RemovePropT(HWND hWnd, LPCTSTR lpString);
        static BOOL SetPropT(HWND hWnd, LPCTSTR lpString, HANDLE hData);

        //System Shutdwown functions
        static BOOL ExitWindowsEx(UINT uFlags, DWORD dwReason);
        static BOOL CancelShutdown(void);
        static BOOL LockWorkStation(void);
        static BOOL ShutdownBlockReasonCreate(HWND hWnd, LPCWSTR pwszReason);
        static BOOL ShutdownBlockReasonDestroy(HWND hWnd);
        static BOOL ShutdownBlockReasonQuery(HWND hWnd, LPWSTR pwszBuff, DWORD* pcchBuff);

        //Filled Shapes functions
        static int FillRect(HDC hDC, CONST::RECT* lprc, HBRUSH hbr);
        static int FrameRect(HDC hDC, CONST::RECT* lprc, HBRUSH hbr);
        static BOOL InvertRect(HDC hDC, CONST::RECT* lprc);

        //Error Handling functions
        static BOOL FlashWindow(HWND hWnd, BOOL bInvert);
        static BOOL FlashWindowEx(::PFLASHWINFO pfwi);
        static BOOL MessageBeep(UINT uType);
        static void SetLastErrorEx(DWORD dwErrCode, DWORD dwType);

        //Window-Class functions
        static BOOL GetClassInfoT(HINSTANCE hInstance, LPCTSTR lpClassName, LPWNDCLASS lpWndClass);
        static BOOL GetClassInfoExT(HINSTANCE hInstance, LPCTSTR lpszClass, LPWNDCLASSEX lpwcx);
        static ULONG_PTR GetClassLongPtrT(HWND hWnd, int nIndex);
        static int GetClassNameT(HWND hWnd, LPTSTR lpClassName, int nMaxCount);
        static WORD GetClassWord(HWND hWnd, int nIndex);
        static LONG GetWindowLongT(HWND hWnd, int nIndex);
        static ATOM RegisterClassT(CONST WNDCLASS* lpWndClass);
        static ATOM RegisterClassExT(CONST WNDCLASSEX* lpwcx);
        static ULONG_PTR SetClassLongPtrT(HWND hWnd, int nIndex, LONG_PTR dwNewLong);
        static WORD SetClassWord(HWND hWnd, int nIndex, WORD wNewWord);
        static LONG SetWindowLongT(HWND hWnd, int nIndex, LONG dwNewLong);
        static LONG_PTR SetWindowLongPtrT(HWND hWnd, int nIndex, LONG_PTR dwNewLong);
        static LONG_PTR GetWindowLongPtrT(HWND hWnd, int nIndex);
        static BOOL UnregisterClassT(LPCTSTR lpClassName, HINSTANCE hInstance);

        void DisableProcessWindowsGhosting(void);
        static LRESULT SendIMEMessageEx(HWND hwnd, LPARAM lParam);
        BOOL WINNLSEnableIME(HWND hwnd, BOOL bFlag);
        //WINNLSEnableIME is obsolete - do not use
        static UINT GetDpiForWindow(HWND hwnd);

        //Shell functions
        BOOL SetMenuContextHelpId(HMENU hmenu, DWORD dwContextHelpId);
        DWORD GetMenuContextHelpId(HMENU hmenu);
        static BOOL SetWindowContextHelpId(HWND hwnd, DWORD dwContextHelpId);
        static DWORD GetWindowContextHelpId(HWND hwnd);
        static BOOL WinHelp(HWND hWndMain, LPCTSTR lpszHelp, UINT uCommand, ULONG_PTR dwData);

        //System Information functions
        DWORD GetSysColor(int nIndex);
        int GetSystemMetrics(int nIndex);
        BOOL SetSysColors(int cElements, const INT* lpaElements, const COLORREF* lpaRgbValues);
        BOOL SystemParametersInfo(UINT uiAction, UINT uiParam, PVOID pvParam, UINT fWinIni);

        //Brush functions
        static HBRUSH GetSysColorBrush(int nIndex);

        //Authorization functions
        BOOL GetUserObjectSecurity(HANDLE hObj, PSECURITY_INFORMATION pSIRequested, PSECURITY_DESCRIPTOR pSD, DWORD nLength, LPDWORD lpnLengthNeeded);
        BOOL SetUserObjectSecurity(HANDLE hObj, PSECURITY_INFORMATION pSIRequested, PSECURITY_DESCRIPTOR pSID);

        //Active Accessibility functions
        static HWINEVENTHOOK SetWinEventHook(UINT eventMin, UINT eventMax, HMODULE hmodWinEventProc, WINEVENTPROC pfnWinEventProc, DWORD idProcess, DWORD idThread, UINT dwFlags);
        static BOOL UnhookWinEvent(HWINEVENTHOOK hWinEventHook);
        static BOOL IsWinEventHookInstalled(DWORD event);
        static void NotifyWinEvent(DWORD event, HWND hwnd, LONG idObject, LONG idChild);

        //Timer functions
        static BOOL KillTimer(HWND hWnd, UINT_PTR uIDEvent);
        static UINT_PTR SetTimer(HWND hWnd, UINT_PTR nIDEvent, UINT uElapse, TIMERPROC lpTimerFunc);

        //Bitmap functions
        HBITMAP LoadBitmap(HINSTANCE hInstance, LPCTSTR lpBitmapName);

        //Synchronization functions
        DWORD MsgWaitForMultipleObjects(DWORD nCount, const HANDLE* pHandles, BOOL bWaitAll, DWORD dwMilliseconds, DWORD dwWakeMask);
        DWORD MsgWaitForMultipleObjectsEx(DWORD nCount, const HANDLE* pHandles, DWORD dwMilliseconds, DWORD dwWakeMask, DWORD dwFlags);

        //Printing functions
        BOOL PrintWindow(HWND hwnd, HDC hdcBlt, UINT nFlags);

        //Device Management functions
        static HDEVNOTIFY RegisterDeviceNotification(HANDLE hRecipient, LPVOID NotificationFilter, DWORD Flags);
        static BOOL UnregisterDeviceNotification(HDEVNOTIFY Handle);

        //Power Management functions
        HPOWERNOTIFY RegisterPowerSettingNotification(HANDLE hRecipient, LPCGUID PowerSettingGuid, DWORD Flags);
        BOOL UnregisterPowerSettingNotification(HPOWERNOTIFY Handle);
        static HPOWERNOTIFY RegisterSuspendResumeNotification(HANDLE hRecipient, DWORD Flags);
        static BOOL UnregisterSuspendResumeNotification(HPOWERNOTIFY Handle);

        //Misc functions (T8 step 6f, added 7 Nov 2026) - the bucket the coverage instrument prints as "Other": the
        //declared-and-unwrapped user32.dll exports that belong to no family tranche. Signatures are the winuser.h
        //prototypes verbatim, parameter names included (D18). ChangeMenuT is the only member of this section not bound
        //bare: ChangeMenuA/W is an A/W pair, so it is one T-suffixed member over qor_pp_useswinapiAW.
        static BOOL CalculatePopupWindowPosition(const ::POINT *anchorPoint, const ::SIZE *windowSize, UINT flags, ::RECT *excludeRect, ::RECT *popupWindowPosition);
        static BOOL ChangeMenuT(HMENU hMenu, UINT cmd, LPCWSTR lpszNewItem, UINT cmdInsert, UINT flags);
        static BOOL ChangeWindowMessageFilterEx(HWND hwnd, UINT message, DWORD action, PCHANGEFILTERSTRUCT pChangeFilterStruct);
        static BOOL EvaluateProximityToPolygon(UINT32 numVertices, const ::POINT *controlPolygon, const TOUCH_HIT_TESTING_INPUT *pHitTestingInput, TOUCH_HIT_TESTING_PROXIMITY_EVALUATION *pProximityEval);
        static BOOL EvaluateProximityToRect(const ::RECT *controlBoundingBox, const TOUCH_HIT_TESTING_INPUT *pHitTestingInput, TOUCH_HIT_TESTING_PROXIMITY_EVALUATION *pProximityEval);
        static BOOL GetAutoRotationState(PAR_STATE pState);
        static BOOL GetCIMSSM(INPUT_MESSAGE_SOURCE *inputMessageSource);
        static BOOL GetClipboardMetadata(UINT format, PGETCLIPBMETADATA metadata);
        static BOOL GetCurrentInputMessageSource(INPUT_MESSAGE_SOURCE *inputMessageSource);
        static DWORD GetUnpredictedMessagePos(void);
        static BOOL GetWindowDisplayAffinity(HWND hWnd, DWORD* pdwAffinity);
        static BOOL GetWindowFeedbackSetting(HWND hwnd, FEEDBACK_TYPE feedback, DWORD dwFlags, UINT32* pSize, VOID* config);
        static BOOL InheritWindowMonitor(HWND hwnd, HWND hwndInherit);
        static BOOL IsImmersiveProcess(HANDLE hProcess);
        static BOOL IsWindowArranged(HWND hwnd);
        static BOOL RegisterForTooltipDismissNotification(HWND hWnd, TOOLTIP_DISMISS_FLAGS tdFlags);
        static BOOL SetAdditionalForegroundBoostProcesses(HWND topLevelWindow, DWORD processHandleCount, HANDLE *processHandleArray);
        static UINT_PTR SetCoalescableTimer(HWND hWnd, UINT_PTR nIDEvent, UINT uElapse, TIMERPROC lpTimerFunc, ULONG uToleranceDelay);
        static void SetDebugErrorLevel(DWORD dwLevel);
        static BOOL SetProcessRestrictionExemption(BOOL fEnableExemption);
        static UINT SetThreadCursorCreationScaling(UINT cursorDpi);
        static BOOL SetWindowDisplayAffinity(HWND hWnd, DWORD dwAffinity);
        static BOOL SetWindowFeedbackSetting(HWND hwnd, FEEDBACK_TYPE feedback, DWORD dwFlags, UINT32 size, const VOID* configuration);

        //Console functions
        static NTSTATUS ConsoleControl(CONSOLECONTROL Command, PVOID ConsoleInformation, DWORD ConsoleInformationLength);
    };

}}}//qor::win::api

#endif//QOR_PP_H_OS_WINDOWS_API_USER
