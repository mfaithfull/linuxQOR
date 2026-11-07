// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"
#include "src/qor/module/module.h"
#include "src/qor/flyers/interception/functioncontext.h"
#include "src/qor/flyers/error/error.h"

//Windows specific headers must be last to prevent contaminating generic headers with Windows specific types and definitions
#include "user32.h"
#include "../library.h"

namespace qor { namespace win { namespace api {

    //=== api_layer conventions ========================================= Reviewed 4 Nov 2026
    //   (session 33: correctness / consistency / safety / UB / conventions; 85 wrappers)
    //
    //The folder's largest TU: the whole "user32.h:129-212 //Window functions" section - 84 declarations, all static,
    //all defined here through the class scope qualifier and in header order - plus GetDpiForWindow (:685), the only
    //definition in the folder whose declaration sits in another header section (user32.h:639, System Information).
    //84/84 in-section declarations pair with a definition here and 85/85 definitions are qualified: 0 U4 (declared,
    //never defined), 0 U11 (unqualified definition), 0 U13 (defined, declared non-static), 0 U17 (A/W family without
    //the folder's T suffix). Full narrative: archive/tu/window.md.
    //
    //Binding (D13): 75 qor_pp_useswinapi + 10 qor_pp_useswinapiAW, and at /EP all 85 binding literals are real exports
    //("CreateWindowExW" x2, "FindWindowW", ... ; 85 binding lines, 84 distinct names, 0 missing, 0 ...WW, 0 ...T,
    //module literal L"user32" 85/85 - tmp/_tmp_win_pp_s33.bat + _tmp_win_ppclass_s33.ps1). One defect was fixed here:
    //CreateWindowT used to bind "CreateWindowW", which is not an export at all - winuser.h:4468 (and :4464 for A)
    //defines CreateWindowW/A as function-like macros forwarding to CreateWindowExW/A with dwExStyle 0, and
    //qor_pp_useswinapiAW stringizes the pasted name before a macro can expand (G1), so the DefProc was always null and
    //Library::Call threw "Missing library function." on every call. It now binds CreateWindowEx and passes the same
    //zero extended style. Folder-wide P1 after this: 428 bindings, 0 unresolvable (P1 CLOSED - this was the last one).
    //The other nine AW sites each paste a real "*W" export (10/10 literal-exported at /EP); none of those nine stems
    //has a bare export, so AW is the only spelling that resolves at all - 0 P2 here (SDK stem macros at :8991, :4456).
    //Signature fixes (3 more): LogicalToPhysicalPoint (:505) and PhysicalToLogicalPoint (:528) returned void through
    //Library::voidCall while the SDK returns BOOL (winuser.h:9526, :9533 - FALSE is the "point is not in a
    //physical/logical display area" case a DPI-aware caller must handle), so the result was discarded; now BOOL +
    //Library::Call<BOOL, HWND, ::LPPOINT> - this closes U18, which was opened over exactly these two members.
    //SetWindowPlacement (:593) and TileWindows (:649) dropped a const the SDK promises (_In_ const WINDOWPLACEMENT *
    //winuser.h:4775; _In_opt_ CONST RECT * :12240) - now const in definition and header. CloseWindow (:182),
    //GetLastActivePopup (:323) and GetWindowModuleFileNameT (:386) were the folder's last three argument-deducing
    //Call<RETURN>(pFunc, ...) sites; explicit now (U14(b): 428 explicit / 0 bare), and U14's frozen spaced/tight
    //tally (163/265, D15) is untouched.
    //
    //SDK pairing (winuser.h 10.0.26100, tmp/_tmp_s33_pre.ps1 section D): 1 type divergence - GetGUIThreadInfo p1 is
    //PGUITHREADINFO in the SDK and LPGUITHREADINFO here, the same typedef, so recorded, not churned; 2 members with no
    //SDK prototype - CascadeChildWindows (:147), an undocumented user32 export the layer wraps (compare
    //InternalGetWindowText (:428), undocumented but declared), and CreateWindow itself (the macro above); parameter
    //names diverge on 6 members (ChildWindowFromPointEx, GetGUIThreadInfo, GetWindowModuleFileNameT,
    //InternalGetWindowText, RealGetWindowClassT, SwitchToThisWindow) - published names, left alone.
    //
    //Version floors (U16): 34 of 85 members sit under one - 11x WINVER >= 0x0500, 7x WINVER >= 0x0400,
    //5x _WIN32_WINNT >= 0x0500, 4x WINVER >= 0x0600, 2x _WIN32_WINNT >= 0x0600 + WINVER >= 0x0500, one each
    //WINVER >= 0x0605 (AdjustWindowRectExForDpi), WINVER >= 0x0605 + 0x0500 (GetDpiForWindow), _WIN32_WINNT >= 0x0501,
    //_WIN32_WINNT >= 0x0600, _WIN32_WINNT >= 0x0501 + 0x0500. Guard names new to the review: NOWINABLE (11 members),
    //NODEFERWINDOWPOS (3), NOMDI (3 - ArrangeIconicWindows, CascadeWindows, TileWindows). 83/85 are
    //WINAPI_PARTITION_DESKTOP, 9 of those also GAMES, and SoundSentry (:635) is the only member also in PC_APP.
    //
    //Safety: 8 caller-side structs reach this TU through reinterpret_cast in ui/gui/controllers/windowcontroller.cpp
    //(Point, Rect, Size, BlendFunction, WindowPlacement, WindowInfo, UpdateLayeredWindowInfo, AltTabInfo); probe
    //tmp/_tmp_win_mirror_s33.cpp gives equal sizeof and equal field offsets on x64 AND x86 (WINDOWPLACEMENT 44,
    //WINDOWINFO 60, UPDATELAYEREDWINDOWINFO 80/40, ALTTABINFO 40), so the casts are layout-sound. The contract the
    //layer cannot state: the SDK requires lpwndpl->length == sizeof(WINDOWPLACEMENT) before Get/SetWindowPlacement, and
    //WindowPlacement::length (windowcontroller.h:86) has no initializer - latent, GetPlacement has 0 in-tree callers,
    //and the fix belongs to the published winqor_gui header, not here.
    //
    //U19 over 340 stem variants: 71 through-layer (the class scope), 49 intermediate-qualified, 49 "SDK bypass", 74
    //member-call. Only two groups are genuine SDK calls that skip this layer: ui/gl/helperwindow.cpp:59,:75
    //(::CreateWindowExW, ::ShowWindow) and ui/gl/windowimpl.cpp (AdjustWindowRectEx x5, AdjustWindowRectExForDpi x5);
    //the rest are same-named non-SDK members or the raw-SDK examples/experimental/.../retropad. U1/U2: 0
    //Win32ErrorHandler, 0 CheckReturn (its dead returncheck.h include went - T7, 5 Nov 2026). U25: 1 `CONST` token on a code
    //line in this TU - CascadeWindows' call list at :158 spells it `CONST ::RECT*` while that member's own definition
    //(:154) spells it `const ::RECT*`, one member holding both spellings of the same type; the file's 11 other const
    //tokens are all lowercase, over 8 code lines (:154, :158, :593, :597, :649, :653, :664, :668). No new shape ->
    //U25 stays as recorded. P5 call-paren: 85 tight / 0 spaced (the census reads the character just after Call<);
    //:206 and :264 differ only before the closing >.
    //U26: this TU mirrors no SDK struct - RECT, POINT, SIZE,
    //WINDOWPLACEMENT, WINDOWINFO, TITLEBARINFO, ALTTABINFO, BLENDFUNCTION and UPDATELAYEREDWINDOWINFO are all used as
    //:: types, and the
    //`TagRECT` lead handed to this session names nothing in the folder (no TagRECT/tagRECT declaration exists in src),
    //so that lead is discharged here.

    BOOL User32::AdjustWindowRect(::LPRECT lpRect, DWORD dwStyle, BOOL bMenu)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, AdjustWindowRect);
        return Library::Call<BOOL, ::LPRECT, DWORD, BOOL>(pFunc, lpRect, dwStyle, bMenu);
    }

    BOOL User32::AdjustWindowRectEx(::LPRECT lpRect, DWORD dwStyle, BOOL bMenu, DWORD dwExStyle)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, AdjustWindowRectEx);
        return Library::Call<BOOL, ::LPRECT, DWORD, BOOL, DWORD>(pFunc, lpRect, dwStyle, bMenu, dwExStyle);
    }

    BOOL User32::AdjustWindowRectExForDpi(::LPRECT lpRect, DWORD dwStyle, BOOL bMenu, DWORD dwExStyle, UINT dpi)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, AdjustWindowRectExForDpi);
        return Library::Call<BOOL, ::LPRECT, DWORD, BOOL, DWORD, UINT>(pFunc, lpRect, dwStyle, bMenu, dwExStyle, dpi);
    }

    BOOL User32::AllowSetForegroundWindow(DWORD dwProcessId)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, AllowSetForegroundWindow);
        return Library::Call<BOOL, DWORD>(pFunc, dwProcessId);
    }

    BOOL User32::AnimateWindow(HWND hWnd, DWORD dwTime, DWORD dwFlags)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, AnimateWindow);
        return Library::Call<BOOL, HWND, DWORD, DWORD>(pFunc, hWnd, dwTime, dwFlags);
    }

    BOOL User32::AnyPopup(void)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, AnyPopup);
        return Library::Call<BOOL>(pFunc);
    }

    UINT User32::ArrangeIconicWindows(HWND hWnd)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, ArrangeIconicWindows);
        return Library::Call<UINT, HWND>(pFunc, hWnd);
    }

    HDWP User32::BeginDeferWindowPos(int nNumWindows)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, BeginDeferWindowPos);
        return Library::Call<HDWP, int>(pFunc, nNumWindows);
    }

    BOOL User32::BringWindowToTop(HWND hWnd)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, BringWindowToTop);
        return Library::Call<BOOL, HWND>(pFunc, hWnd);
    }

    WORD User32::CascadeChildWindows(HWND hWndParent, WORD wFlags)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, CascadeChildWindows);
        return Library::Call<WORD, HWND, WORD>(pFunc, hWndParent, wFlags);
    }

    WORD User32::CascadeWindows(HWND hwndParent, UINT wHow, const ::RECT* lpRect, UINT cKids, const HWND* lpKids)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, CascadeWindows);
        return Library::Call<WORD, HWND, UINT, const ::RECT*, UINT, const HWND*>(pFunc, hwndParent, wHow, lpRect, cKids, lpKids);
    }

    BOOL User32::ChangeWindowMessageFilter(UINT message, DWORD dwFlag)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, ChangeWindowMessageFilter);
        return Library::Call<BOOL, UINT, DWORD>(pFunc, message, dwFlag);
    }

    HWND User32::ChildWindowFromPoint(HWND hWndParent, ::POINT Point)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, ChildWindowFromPoint);
        return Library::Call<HWND, HWND, ::POINT>(pFunc, hWndParent, Point);
    }

    HWND User32::ChildWindowFromPointEx(HWND hwnd, ::POINT pt, UINT flags)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, ChildWindowFromPointEx);
        return Library::Call<HWND, HWND, ::POINT, UINT>(pFunc, hwnd, pt, flags);
    }

    BOOL User32::CloseWindow(HWND hWnd)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, CloseWindow);
        return Library::Call<BOOL, HWND>(pFunc, hWnd);
    }

    HWND User32::CreateWindowT(LPCTSTR lpClassName, LPCTSTR lpWindowName, DWORD dwStyle, int x, int y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance, LPVOID lpParam)
    {
        qor_pp_fcontext;
        //CreateWindow is not a user32.dll export at all: winuser.h:4468 defines CreateWindowW (and :4464 CreateWindowA)
        //as function-like macros that forward to CreateWindowExW/A with a zero extended style, and qor_pp_useswinapiAW
        //stringizes the pasted name before any macro can expand, so binding "CreateWindowW" could only ever give a null
        //DefProc and Library::Call would throw "Missing library function." on every call. Bind the export the SDK macro
        //itself calls and pass the same zero dwExStyle.
        qor_pp_useswinapiAW(user32, CreateWindowEx);
        return Library::Call<HWND, DWORD, LPCTSTR, LPCTSTR, DWORD, int, int, int, int, HWND, HMENU, HINSTANCE, LPVOID>(
            pFunc, 0, lpClassName, lpWindowName, dwStyle, x, y, nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam);
    }

    HWND User32::CreateWindowExT(DWORD dwExStyle, LPCTSTR lpClassName, LPCTSTR lpWindowName, DWORD dwStyle, int X, int Y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance, LPVOID lpParam)
    {
        qor_pp_fcontext;
        qor_pp_useswinapiAW(user32, CreateWindowEx);
        return Library::Call<HWND, DWORD, LPCTSTR, LPCTSTR, DWORD, int, int, int, int, HWND, HMENU, HINSTANCE, LPVOID >(
            pFunc, dwExStyle, lpClassName, lpWindowName, dwStyle, X, Y, nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam);
    }

    HDWP User32::DeferWindowPos(HDWP hWinPosInfo, HWND hWnd, HWND hWndInsertAfter, int x, int y, int cx, int cy, UINT uFlags)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, DeferWindowPos);
        return Library::Call<HDWP, HDWP, HWND, HWND, int, int, int, int, UINT>(
            pFunc, hWinPosInfo, hWnd, hWndInsertAfter, x, y, cx, cy, uFlags);
    }

    BOOL User32::DeregisterShellHookWindow(HWND hwnd)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, DeregisterShellHookWindow);
        return Library::Call<BOOL, HWND>(pFunc, hwnd);
    }

    BOOL User32::DestroyWindow(HWND hWnd)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, DestroyWindow);
        return Library::Call<BOOL, HWND>(pFunc, hWnd);
    }

    BOOL User32::EndDeferWindowPos(HDWP hWinPosInfo)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, EndDeferWindowPos);
        return Library::Call<BOOL, HDWP>(pFunc, hWinPosInfo);
    }

    BOOL User32::EndTask(HWND hWnd, BOOL fShutDown, BOOL fForce)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, EndTask);
        return Library::Call<BOOL, HWND, BOOL, BOOL>(pFunc, hWnd, fShutDown, fForce);
    }

    BOOL User32::EnumChildWindows(HWND hWndParent, WNDENUMPROC lpEnumFunc, LPARAM lParam)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, EnumChildWindows);
        return Library::Call<BOOL, HWND, WNDENUMPROC, LPARAM>(pFunc, hWndParent, lpEnumFunc, lParam);
    }

    BOOL User32::EnumThreadWindows(DWORD dwThreadId, WNDENUMPROC lpfn, LPARAM lParam)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, EnumThreadWindows);
        return Library::Call<BOOL, DWORD, WNDENUMPROC, LPARAM>(pFunc, dwThreadId, lpfn, lParam);
    }

    BOOL User32::EnumWindows(WNDENUMPROC lpEnumFunc, LPARAM lParam)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, EnumWindows);
        return Library::Call<BOOL, WNDENUMPROC, LPARAM >(pFunc, lpEnumFunc, lParam);
    }

    HWND User32::FindWindowT(LPCTSTR lpClassName, LPCTSTR lpWindowName)
    {
        qor_pp_fcontext;
        qor_pp_useswinapiAW(user32, FindWindow);
        return Library::Call<HWND, LPCTSTR, LPCTSTR>(pFunc, lpClassName, lpWindowName);
    }

    HWND User32::FindWindowExT(HWND hWndParent, HWND hWndChildAfter, LPCTSTR lpszClass, LPCTSTR lpszWindow)
    {
        qor_pp_fcontext;
        qor_pp_useswinapiAW(user32, FindWindowEx);
        return Library::Call<HWND, HWND, HWND, LPCTSTR, LPCTSTR>(pFunc, hWndParent, hWndChildAfter, lpszClass, lpszWindow);
    }

    BOOL User32::GetAltTabInfoT(HWND hwnd, int iItem, ::PALTTABINFO pati, LPTSTR pszItemText, UINT cchItemText)
    {
        qor_pp_fcontext;
        qor_pp_useswinapiAW(user32, GetAltTabInfo);
        return Library::Call<BOOL, HWND, int, ::PALTTABINFO, LPTSTR, UINT>(pFunc, hwnd, iItem, pati, pszItemText, cchItemText);
    }

    HWND User32::GetAncestor(HWND hwnd, UINT gaFlags)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, GetAncestor);
        return Library::Call<HWND, HWND, UINT>(pFunc, hwnd, gaFlags);
    }

    BOOL User32::GetClientRect(HWND hWnd, ::LPRECT lpRect)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, GetClientRect);
        return Library::Call<BOOL, HWND, ::LPRECT>(pFunc, hWnd, lpRect);
    }

    HWND User32::GetDesktopWindow(void)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, GetDesktopWindow);
        return Library::Call<HWND>(pFunc);
    }

    HWND User32::GetForegroundWindow(void)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, GetForegroundWindow);
        return Library::Call<HWND>(pFunc);
    }

    BOOL User32::GetGUIThreadInfo(DWORD idThread, ::LPGUITHREADINFO pgui)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, GetGUIThreadInfo);
        return Library::Call<BOOL, DWORD, ::LPGUITHREADINFO>(pFunc, idThread, pgui);
    }

    HWND User32::GetLastActivePopup(HWND hWnd)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, GetLastActivePopup);
        return Library::Call<HWND, HWND>(pFunc, hWnd);
    }

    BOOL User32::GetLayeredWindowAttributes(HWND hwnd, COLORREF* pcrKey, BYTE* pbAlpha, DWORD* pdwFlags)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, GetLayeredWindowAttributes);
        return Library::Call<BOOL, HWND, COLORREF*, BYTE*, DWORD*>(pFunc, hwnd, pcrKey, pbAlpha, pdwFlags);
    }

    HWND User32::GetParent(HWND hWnd)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, GetParent);
        return Library::Call<HWND, HWND>(pFunc, hWnd);
    }

    BOOL User32::GetProcessDefaultLayout(DWORD* pdwDefaultLayout)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, GetProcessDefaultLayout);
        return Library::Call<BOOL, DWORD*>(pFunc, pdwDefaultLayout);
    }

    HWND User32::GetShellWindow(void)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, GetShellWindow);
        return Library::Call<HWND>(pFunc);
    }

    BOOL User32::GetTitleBarInfo(HWND hwnd, ::PTITLEBARINFO pti)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, GetTitleBarInfo);
        return Library::Call<BOOL, HWND, ::PTITLEBARINFO>(pFunc, hwnd, pti);
    }

    HWND User32::GetTopWindow(HWND hWnd)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, GetTopWindow);
        return Library::Call<HWND, HWND>(pFunc, hWnd);
    }

    HWND User32::GetWindow(HWND hWnd, UINT uCmd)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, GetWindow);
        return Library::Call<HWND, HWND, UINT>(pFunc, hWnd, uCmd);
    }

    BOOL User32::GetWindowInfo(HWND hwnd, ::PWINDOWINFO pwi)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, GetWindowInfo);
        return Library::Call<BOOL, HWND, ::PWINDOWINFO>(pFunc, hwnd, pwi);
    }

    UINT User32::GetWindowModuleFileNameT(HWND hwnd, LPTSTR pszFileName, UINT cchFileNameMax)
    {
        qor_pp_fcontext;
        qor_pp_useswinapiAW(user32, GetWindowModuleFileName);
        return Library::Call<UINT, HWND, LPTSTR, UINT>(pFunc, hwnd, pszFileName, cchFileNameMax);
    }

    BOOL User32::GetWindowPlacement(HWND hWnd, ::WINDOWPLACEMENT* lpwndpl)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, GetWindowPlacement);
        return Library::Call<BOOL, HWND, ::WINDOWPLACEMENT*>(pFunc, hWnd, lpwndpl);
    }

    BOOL User32::GetWindowRect(HWND hWnd, ::LPRECT lpRect)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, GetWindowRect);
        return Library::Call<BOOL, HWND, ::LPRECT>(pFunc, hWnd, lpRect);
    }

    int User32::GetWindowTextT(HWND hWnd, LPTSTR lpString, int nMaxCount)
    {
        qor_pp_fcontext;
        qor_pp_useswinapiAW(user32, GetWindowText);
        return Library::Call<int, HWND, LPTSTR, int>(pFunc, hWnd, lpString, nMaxCount);
    }

    int User32::GetWindowTextLengthT(HWND hWnd)
    {
        qor_pp_fcontext;
        qor_pp_useswinapiAW(user32, GetWindowTextLength);
        return Library::Call<int, HWND>(pFunc, hWnd);
    }

    DWORD User32::GetWindowThreadProcessId(HWND hWnd, LPDWORD lpdwProcessId)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, GetWindowThreadProcessId);
        return Library::Call<DWORD, HWND, LPDWORD>(pFunc, hWnd, lpdwProcessId);
    }

    int User32::InternalGetWindowText(HWND hWnd, LPWSTR pString, int cchMaxCount)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, InternalGetWindowText);
        return Library::Call<int, HWND, LPWSTR, int>(pFunc, hWnd, pString, cchMaxCount);
    }

    BOOL User32::IsChild(HWND hWndParent, HWND hWnd)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, IsChild);
        return Library::Call<BOOL, HWND, HWND>(pFunc, hWndParent, hWnd);
    }

    BOOL User32::IsGUIThread(BOOL bConvert)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, IsGUIThread);
        return Library::Call<BOOL, BOOL>(pFunc, bConvert);
    }

    BOOL User32::IsHungAppWindow(HWND hwnd)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, IsHungAppWindow);
        return Library::Call<BOOL, HWND>(pFunc, hwnd);
    }

    BOOL User32::IsIconic(HWND hWnd)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, IsIconic);
        return Library::Call<BOOL, HWND>(pFunc, hWnd);
    }

    BOOL User32::IsProcessDPIAware(void)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, IsProcessDPIAware);
        return Library::Call<BOOL>(pFunc);
    }

    BOOL User32::IsWindow(HWND hWnd)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, IsWindow);
        return Library::Call<BOOL, HWND>(pFunc, hWnd);
    }

    BOOL User32::IsWindowUnicode(HWND hWnd)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, IsWindowUnicode);
        return Library::Call<BOOL, HWND>(pFunc, hWnd);
    }

    BOOL User32::IsWindowVisible(HWND hWnd)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, IsWindowVisible);
        return Library::Call<BOOL, HWND>(pFunc, hWnd);
    }

    BOOL User32::IsZoomed(HWND hWnd)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, IsZoomed);
        return Library::Call<BOOL, HWND>(pFunc, hWnd);
    }

    BOOL User32::LockSetForegroundWindow(UINT uLockCode)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, LockSetForegroundWindow);
        return Library::Call<BOOL, UINT>(pFunc, uLockCode);
    }

    BOOL User32::LogicalToPhysicalPoint(HWND hWnd, ::LPPOINT lpPoint)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, LogicalToPhysicalPoint);
        //The SDK returns BOOL (winuser.h:9526) - FALSE when the point is not inside a physical display area of
        //this window's monitor - so the old void + Library::voidCall discarded a result the caller needs.
        return Library::Call<BOOL, HWND, ::LPPOINT>(pFunc, hWnd, lpPoint);
    }

    BOOL User32::LogicalToPhysicalPointForPerMonitorDPI(HWND hWnd, LPPOINT lpPoint)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, LogicalToPhysicalPointForPerMonitorDPI);
        return Library::Call<BOOL, HWND, LPPOINT>(pFunc, hWnd, lpPoint);
    }

    BOOL User32::MoveWindow(HWND hWnd, int X, int Y, int nWidth, int nHeight, BOOL bRepaint)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, MoveWindow);
        return Library::Call<BOOL, HWND, int, int, int, int, BOOL>(pFunc, hWnd, X, Y, nWidth, nHeight, bRepaint);
    }

    BOOL User32::OpenIcon(HWND hWnd)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, OpenIcon);
        return Library::Call<BOOL, HWND>(pFunc, hWnd);
    }

    BOOL User32::PhysicalToLogicalPoint(HWND hWnd, ::LPPOINT lpPoint)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, PhysicalToLogicalPoint);
        //The SDK returns BOOL (winuser.h:9533) - FALSE when the point is not inside the logical window area -
        //so the old void + Library::voidCall discarded a result the caller needs.
        return Library::Call<BOOL, HWND, ::LPPOINT>(pFunc, hWnd, lpPoint);
    }

    BOOL User32::PhysicalToLogicalPointForPerMonitorDPI(HWND hWnd, LPPOINT lpPoint)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, PhysicalToLogicalPointForPerMonitorDPI);
        return Library::Call<BOOL, HWND, LPPOINT>(pFunc, hWnd, lpPoint);
    }

    HWND User32::RealChildWindowFromPoint(HWND hwndParent, ::POINT ptParentClientCoords)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, RealChildWindowFromPoint);
        return Library::Call<HWND, HWND, ::POINT>(pFunc, hwndParent, ptParentClientCoords);
    }

    UINT User32::RealGetWindowClassT(HWND hwnd, LPTSTR ptszClassName, UINT cchClassNameMax)
    {
        qor_pp_fcontext;
        qor_pp_useswinapiAW(user32, RealGetWindowClass);
        return Library::Call<UINT, HWND, LPTSTR, UINT>(pFunc, hwnd, ptszClassName, cchClassNameMax);
    }

    BOOL User32::RegisterShellHookWindow(HWND hwnd)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, RegisterShellHookWindow);
        return Library::Call<BOOL, HWND>(pFunc, hwnd);
    }

    BOOL User32::SetForegroundWindow(HWND hWnd)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, SetForegroundWindow);
        return Library::Call<BOOL, HWND>(pFunc, hWnd);
    }

    BOOL User32::SetLayeredWindowAttributes(HWND hwnd, COLORREF crKey, BYTE bAlpha, DWORD dwFlags)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, SetLayeredWindowAttributes);
        return Library::Call<BOOL, HWND, COLORREF, BYTE, DWORD>(pFunc, hwnd, crKey, bAlpha, dwFlags);
    }

    HWND User32::SetParent(HWND hWndChild, HWND hWndNewParent)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, SetParent);
        return Library::Call<HWND, HWND, HWND>(pFunc, hWndChild, hWndNewParent);
    }

    BOOL User32::SetProcessDefaultLayout(DWORD dwDefaultLayout)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, SetProcessDefaultLayout);
        return Library::Call<BOOL, DWORD>(pFunc, dwDefaultLayout);
    }

    BOOL User32::SetProcessDPIAware(void)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, SetProcessDPIAware);
        return Library::Call<BOOL>(pFunc);
    }

    BOOL User32::SetWindowPlacement(HWND hWnd, const ::WINDOWPLACEMENT* lpwndpl)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, SetWindowPlacement);
        return Library::Call<BOOL, HWND, const ::WINDOWPLACEMENT*>(pFunc, hWnd, lpwndpl);
    }

    BOOL User32::SetWindowPos(HWND hWnd, HWND hWndInsertAfter, int X, int Y, int cx, int cy, UINT uFlags)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, SetWindowPos);
        return Library::Call<BOOL, HWND, HWND, int, int, int, int, UINT>(pFunc, hWnd, hWndInsertAfter, X, Y, cx, cy, uFlags);
    }

    BOOL User32::SetWindowTextT(HWND hWnd, LPCTSTR lpString)
    {
        qor_pp_fcontext;
        qor_pp_useswinapiAW(user32, SetWindowText);
        return Library::Call<BOOL, HWND, LPCTSTR>(pFunc, hWnd, lpString);
    }

    BOOL User32::ShowOwnedPopups(HWND hWnd, BOOL fShow)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, ShowOwnedPopups);
        return Library::Call<BOOL, HWND, BOOL>(pFunc, hWnd, fShow);
    }

    BOOL User32::ShowWindow(HWND hWnd, int nCmdShow)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, ShowWindow);
        return Library::Call<BOOL, HWND, int>(pFunc, hWnd, nCmdShow);
    }

    BOOL User32::ShowWindowAsync(HWND hWnd, int nCmdShow)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, ShowWindowAsync);
        return Library::Call<BOOL, HWND, int>(pFunc, hWnd, nCmdShow);
    }

    BOOL User32::SoundSentry(void)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, SoundSentry);
        return Library::Call<BOOL>(pFunc);
    }

    void User32::SwitchToThisWindow(HWND hwnd, BOOL fUnknown)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, SwitchToThisWindow);
        return Library::voidCall<HWND, BOOL>(pFunc, hwnd, fUnknown);
    }

    WORD User32::TileWindows(HWND hwndParent, UINT wHow, const ::RECT* lpRect, UINT cKids, const HWND* lpKids)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, TileWindows);
        return Library::Call<WORD, HWND, UINT, const ::RECT*, UINT, const HWND*>(pFunc, hwndParent, wHow, lpRect, cKids, lpKids);
    }

    BOOL User32::UpdateLayeredWindow(HWND hWnd, HDC hdcDst, ::POINT* pptDst, ::SIZE* psize, HDC hdcSrc, ::POINT* pptSrc, COLORREF crKey, ::BLENDFUNCTION* pblend, DWORD dwFlags)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, UpdateLayeredWindow);
        return Library::Call<BOOL, HWND, HDC, ::POINT*, ::SIZE*, HDC, ::POINT*, COLORREF, ::BLENDFUNCTION*, DWORD>(
            pFunc, hWnd, hdcDst, pptDst, psize, hdcSrc, pptSrc, crKey, pblend, dwFlags);
    }

    BOOL User32::UpdateLayeredWindowIndirect(HWND hWnd, const ::UPDATELAYEREDWINDOWINFO* pULWInfo)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, UpdateLayeredWindowIndirect);
        return Library::Call<BOOL, HWND, const ::UPDATELAYEREDWINDOWINFO*>(pFunc, hWnd, pULWInfo);
    }

    HWND User32::WindowFromPhysicalPoint(::POINT Point)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, WindowFromPhysicalPoint);
        return Library::Call<HWND, ::POINT>(pFunc, Point);
    }

    HWND User32::WindowFromPoint(::POINT Point)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, WindowFromPoint);
        return Library::Call<HWND, ::POINT>(pFunc, Point);
    }

    UINT User32::GetDpiForWindow(HWND hwnd)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, GetDpiForWindow);
        return Library::Call<UINT, HWND>(pFunc, hwnd);
    }

}}}//qor::win::api
