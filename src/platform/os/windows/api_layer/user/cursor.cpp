// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"
#include "src/qor/module/module.h"
#include "src/qor/flyers/interception/functioncontext.h"
#include "src/qor/flyers/error/error.h"

//Windows specific headers must be last to prevent contaminating generic headers with Windows specific types and definitions
#include "user32.h"
#include "../returncheck.h"
#include "../library.h"

namespace qor { namespace win { namespace api {

    //The "Cursor functions" section (user32.h:382-397): sixteen wrappers, sixteen bindings, in header order. Fourteen bind
    //the bare qor_pp_useswinapi stem and each names a genuine single user32.dll export - ClipCursor (4E), CopyIcon (59),
    //CreateCursor (60), DestroyCursor (AF), GetClipCursor (134), GetCursor (13F), GetCursorInfo (141), GetCursorPos (142),
    //GetPhysicalCursorPos (193), SetCursor (32C), SetCursorPos (32E), SetPhysicalCursorPos (354), SetSystemCursor (36A),
    //ShowCursor (395) - and two are A/W switches, LoadCursorT -> LoadCursorW (259) and LoadCursorFromFileT ->
    //LoadCursorFromFileW (258); the A halves (256, 257) are exported but not selectable through this surface. cl /EP
    //resolves all sixteen literals and a live GetModuleHandleW(L"user32") + GetProcAddress probe resolves all sixteen too
    //(tmp/_tmp_cursor_probe.cpp), and none of the sixteen is one of the nine stems user32.h:9-17 #undef's (G2). No wrapper
    //here can call the SDK by name: inside this class an unqualified bare stem resolves to the wrapper itself, which MSVC
    //diagnoses as "recursive on all control paths" (measured, tmp/_tmp_recursion_probe.cpp - warning C4717 at /W4).
    //CopyCursorT is the folder's one wrapper whose member name is not its export: the SDK has no CopyCursor entry point,
    //it is a cast macro (winuser.h:10681 "#define CopyCursor(pcur) ((HCURSOR)CopyIcon((HICON)(pcur)))", live probe
    //CopyCursor MISSING / CopyIcon RESOLVED), so the binding is right and only the name is indirect - the U17/P3 mirror
    //case. The name implies no type conversion in this build: windef.h:88 is DECLARE_HANDLE(HICON) and :131 is
    //"typedef HICON HCURSOR" (with the header's own note "HICONs & HCURSORs are polymorphic"); the separate
    //DECLARE_HANDLE(HCURSOR) at :133 is the "_MAC"-only arm of that same #ifndef and is not what this TU gets - /EP of
    //cursor.cpp has exactly one HCURSOR typedef, "typedef HICON HCURSOR;" (tmp/_tmp_cursor_pp.txt:174186). So HCURSOR and
    //HICON are one type here and Call< HCURSOR, HCURSOR > against the HICON WINAPI(HICON) export is type-identical; the
    //only calling risk in that path is the U6 __stdcall-vs-__cdecl fPtr mismatch, which every wrapper in the folder shares.
    //Ownership: CreateCursor, LoadCursorFromFileT and CopyCursorT each return a handle the caller must DestroyCursor,
    //while LoadCursorT and GetCursor return shared system handles that must not be destroyed. SetSystemCursor takes no
    //ownership of hcur (winuser.h:10728-10730 carries no ownership SAL and documents no transfer), and it and
    //ShowSystemCursor remain exports - 36A, 399 - although Microsoft documents both as unsupported from Windows 8 up
    //(a documentation claim, not re-measured here). GetCursorInfo requires pci->cbSize set by the caller first. ClipCursor is _In_opt_: NULL
    //clears the clip rect instead of setting one. ShowCursor returns the new display count, so 0 or negative is a normal
    //hide, not a failure - there is nothing for a return check to decide here.
    //Measured against this user32.dll: GetCursorPos (142) and GetPhysicalCursorPos (193) share RVA 0002B230, and
    //SetCursorPos (32E) and SetPhysicalCursorPos (354) share RVA 0002FA50 - each logical/physical pair is one
    //implementation exported under two names, so the DPI distinction their SDK declarations draw cannot be observed at
    //run time in this build. Those two plus GetCursorInfo are also this TU's whole U16 exposure: GetPhysicalCursorPos
    //(:9439) and SetPhysicalCursorPos (:9406) pair with declarations inside "#if(WINVER >= 0x0600)" (:9435, :9402), and
    //GetCursorInfo (:14661) with the CURSORINFO type it takes (:14644-14650) inside "#if(WINVER >= 0x0500)" (:13594) plus
    //"#ifndef NOWINABLE" (:13716); the other thirteen pair with declarations under nothing but NOUSER and a
    //WINAPI_FAMILY_PARTITION. Not wrapped and not declared anywhere in user32.h: GetCursorFrameInfo (140),
    //SetCursorContents (32D), ShowSystemCursor (399) (all RESOLVED), plus SetThreadCursorCreationScaling (36D),
    //GetPointerCursorId (194), GetPointerDeviceCursors (196) and MITGetCursorUpdateHandle (274).

    BOOL User32::ClipCursor(const RECT* lpRect)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, ClipCursor);
        return Library::Call<BOOL, const RECT*>(pFunc, lpRect);
    }

    HCURSOR User32::CopyCursorT(HCURSOR pcur)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, CopyIcon);
        return Library::Call< HCURSOR, HCURSOR >(pFunc, pcur);
    }

    HCURSOR User32::CreateCursor(HINSTANCE hInst, int xHotSpot, int yHotSpot, int nWidth, int nHeight, const VOID* pvANDPlane, const VOID* pvXORPlane)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, CreateCursor);
        return Library::Call< HCURSOR, HINSTANCE, int, int, int, int, const VOID*, const VOID* >(pFunc, hInst, xHotSpot, yHotSpot, nWidth, nHeight, pvANDPlane, pvXORPlane);
    }

    BOOL User32::DestroyCursor(HCURSOR hCursor)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, DestroyCursor);
        return Library::Call< BOOL, HCURSOR >(pFunc, hCursor);
    }

    BOOL User32::GetClipCursor(LPRECT lpRect)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, GetClipCursor);
        return Library::Call< BOOL, LPRECT >(pFunc, lpRect);
    }

    HCURSOR User32::GetCursor(void)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, GetCursor);
        return Library::Call< HCURSOR >(pFunc);
    }

    BOOL User32::GetCursorInfo(PCURSORINFO pci)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, GetCursorInfo);
        return Library::Call< BOOL, PCURSORINFO >(pFunc, pci);
    }

    BOOL User32::GetCursorPos(LPPOINT lpPoint)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, GetCursorPos);
        return Library::Call< BOOL, LPPOINT >(pFunc, lpPoint);
    }

    BOOL User32::GetPhysicalCursorPos(LPPOINT lpPoint)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, GetPhysicalCursorPos);
        return Library::Call< BOOL, LPPOINT >(pFunc, lpPoint);
    }

    HCURSOR User32::LoadCursorT(HINSTANCE hInstance, LPCTSTR lpCursorName)
    {
        qor_pp_fcontext;
        qor_pp_useswinapiAW(user32, LoadCursor);
        return Library::Call< HCURSOR, HINSTANCE, LPCTSTR >(pFunc, hInstance, lpCursorName);
    }

    HCURSOR User32::LoadCursorFromFileT(LPCTSTR lpFileName)
    {
        qor_pp_fcontext;
        qor_pp_useswinapiAW(user32, LoadCursorFromFile);
        return Library::Call< HCURSOR, LPCTSTR >(pFunc, lpFileName);
    }

    BOOL User32::SetCursorPos(int X, int Y)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, SetCursorPos);
        return Library::Call< BOOL, int, int >(pFunc, X, Y);
    }

    BOOL User32::SetPhysicalCursorPos(int X, int Y)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, SetPhysicalCursorPos);
        return Library::Call< BOOL, int, int >(pFunc, X, Y);
    }

    int User32::ShowCursor(BOOL bShow)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, ShowCursor);
        return Library::Call< int, BOOL >(pFunc, bShow);
    }

    HCURSOR User32::SetCursor(HCURSOR hCursor)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, SetCursor);
        return Library::Call< HCURSOR, HCURSOR >(pFunc, hCursor);
    }

    BOOL User32::SetSystemCursor(HCURSOR hcur, DWORD id)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, SetSystemCursor);
        return Library::Call< BOOL, HCURSOR, DWORD >(pFunc, hcur, id);
    }

}}}//qor::win::api
