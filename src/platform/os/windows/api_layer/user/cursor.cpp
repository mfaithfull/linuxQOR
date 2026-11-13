// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"
#include "src/qor/module/module.h"
#include "src/qor/flyers/interception/functioncontext.h"
#include "src/qor/flyers/error/error.h"
#include "../kernel/errorhandler.h"

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
    //P3 CLOSED 12 Nov 2026: CopyCursorT is deleted from user32.h and this TU, and ui/gui/gdiobjects/cursor.cpp Clone()
    //calls User32::CopyIcon directly. The mirror case it carried is measured: CopyCursor has 0 export rows, winuser.h:10681
    //is the cast macro "#define CopyCursor(pcur) ((HCURSOR)CopyIcon((HICON)(pcur)))", and CopyIcon (59) is wrapped here.
    //HCURSOR and HICON are one type in this build: windef.h:88 is DECLARE_HANDLE(HICON) and :131 is
    //"typedef HICON HCURSOR" (with the header's own note "HICONs & HCURSORs are polymorphic"); the separate
    //DECLARE_HANDLE(HCURSOR) at :133 is the "_MAC"-only arm of that same #ifndef and is not what this TU gets - /EP of
    //cursor.cpp has exactly one HCURSOR typedef, "typedef HICON HCURSOR;" (tmp/_tmp_cursor_pp.txt:174186). So HCURSOR and
    //HICON are one type here and Call< HCURSOR, HCURSOR > against the HICON WINAPI(HICON) export is type-identical; the
    //only calling risk in that path was the U6 __stdcall-vs-__cdecl fPtr mismatch, which every wrapper in the folder
    //shared until it was closed on 1 Nov 2026 (library.h:46/:68 now spell fPtr __stdcall, like DefProc at :16).
    //Ownership: CreateCursor and LoadCursorFromFileT (and CopyIcon, whose result a caller clones) return a handle the caller must DestroyCursor,
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

    //Return policy (U2, table row ClipCursor / cursor.cpp, 12 Nov 2026): checked with checkClass BoolCheck; the page's verbatim
    //docQuote is "Type: BOOL If the function succeeds, the return value is nonzero. If the function fails, the return value is zero. To get extended error information, call GetLastError ." - the RECT is the parameter the caller supplies (NULL clears the clip rect rather than setting one, which is an argument property, not a return), so the BOOL carries no answer of its own and zero is reserved for a failed call; sdkRetType BOOL so BoolCheck (returncheck.h:94) covers it. Audit: the table reports 1 in-tree caller and the site-level read finds User32::ClipCursor live at ui/gui/gdiobjects/cursor.cpp:99 as "return User32::ClipCursor(&r) ? true : false;" - the site reads the BOOL as a success flag and CheckReturn returns the value unchanged, so the site's reading is identical and only a diagnostic is added at a documented failure.
    BOOL User32::ClipCursor(const RECT* lpRect)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, ClipCursor);
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call<BOOL, const RECT*>(pFunc, lpRect);
        return bResult;
    }

    //Return policy (U2, table row CreateCursor / cursor.cpp, 12 Nov 2026): checked with checkClass CheckNonZeroHandle< HCURSOR >; the page's verbatim
    //docQuote is "Type: HCURSOR If the function succeeds, the return value is a handle to the cursor. If the function fails, the return value is NULL . To get extended error information, call GetLastError ." - the handle is what the caller holds and hands to DestroyCursor, so no legal answer is NULL and NULL is reserved for a failed creation; sdkRetType HCURSOR so the class base matches and CheckNonZeroHandle (returncheck.h:137) covers it. Audit: the table reports 1 in-tree caller and the site-level read finds User32::CreateCursor live at ui/gui/gdiobjects/cursor.cpp:30 in the Cursor(void*,int,int,int,int,const void*,const void*) ctor, which stores the return into m_handle and sets m_owning = true with no test on it; CheckReturn returns the value unchanged, so the stored handle is identical and only a diagnostic is added at a documented NULL - the ctor's untested store is the U20/D47 ownership hazard already recorded at registers/u20.md, not something this check introduces or repairs.
    HCURSOR User32::CreateCursor(HINSTANCE hInst, int xHotSpot, int yHotSpot, int nWidth, int nHeight, const VOID* pvANDPlane, const VOID* pvXORPlane)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, CreateCursor);
        CheckReturn< HCURSOR, CheckNonZeroHandle< HCURSOR > >::TType hResult = Library::Call< HCURSOR, HINSTANCE, int, int, int, int, const VOID*, const VOID* >(pFunc, hInst, xHotSpot, yHotSpot, nWidth, nHeight, pvANDPlane, pvXORPlane);
        return hResult;
    }

    //Return policy (U2, table row DestroyCursor / cursor.cpp, 12 Nov 2026): checked with checkClass BoolCheck; the page's verbatim
    //docQuote is "Type: BOOL If the function succeeds, the return value is nonzero. If the function fails, the return value is zero. To get extended error information, call GetLastError ." - the handle is the parameter the caller hands over and the BOOL reports only whether the release happened, so it carries no answer and zero is reserved; sdkRetType BOOL so BoolCheck (returncheck.h:94) covers it. Audit: the table reports 1 in-tree caller and the site-level read finds User32::DestroyCursor live at ui/gui/gdiobjects/cursor.cpp:84 inside Cursor::~Cursor() under "if(m_owning)", where the return is discarded outright; CheckReturn returns the value unchanged, so no control flow at that site changes and only a diagnostic is added when the release of an owned handle fails.
    BOOL User32::DestroyCursor(HCURSOR hCursor)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, DestroyCursor);
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call< BOOL, HCURSOR >(pFunc, hCursor);
        return bResult;
    }

    //Return policy (U2, table row GetClipCursor / cursor.cpp, 12 Nov 2026): checked with checkClass BoolCheck; the page's verbatim
    //docQuote is "Type: BOOL If the function succeeds, the return value is nonzero. If the function fails, the return value is zero. To get extended error information, call GetLastError ." - the LPRECT is an out-parameter the caller supplies and fills, so the BOOL carries no answer and zero is reserved for a failed query; sdkRetType BOOL so BoolCheck (returncheck.h:94) covers it. Audit: the table reports 1 in-tree caller and the site-level read finds User32::GetClipCursor live at ui/gui/gdiobjects/cursor.cpp:105 as "bool result = User32::GetClipCursor(&r) ? true : false;" whose result gates copying the RECT into the caller's Rect; CheckReturn returns the value unchanged, so that gate reads exactly as before and only a diagnostic is added at a documented failure.
    BOOL User32::GetClipCursor(LPRECT lpRect)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, GetClipCursor);
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call< BOOL, LPRECT >(pFunc, lpRect);
        return bResult;
    }

    //Return policy (U2, table row GetCursor / cursor.cpp, 12 Nov 2026): unchecked, no CheckReturn token; the page's verbatim
    //docQuote is "Type: HCURSOR The return value is the handle to the current cursor. If there is no cursor, the return value is NULL ." - NULL names a window state the caller can legitimately be in rather than a failed call, and this page carries no failure branch and no GetLastError extended info, unlike the CreateCursor and LoadCursor rows beside it, which do; the session-27 soundness test has no reserved value to key on. Audit: the table reports 1 in-tree caller and the site-level read finds User32::GetCursor live at ui/gui/gdiobjects/cursor.cpp:118 as PrimitiveHandle h(User32::GetCursor()) in Cursor::GetCurrent(), which wraps the handle as a non-owning one (m_owning = false) - the reading this TU's banner records for the shared system handles.
    HCURSOR User32::GetCursor(void)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, GetCursor);
        return Library::Call< HCURSOR >(pFunc);
    }

    //Return policy (U2, table row GetCursorInfo / cursor.cpp, 12 Nov 2026): checked with checkClass BoolCheck; the page's verbatim
    //docQuote is "Type: BOOL If the function succeeds, the return value is nonzero. If the function fails, the return value is zero. To get extended error information, call GetLastError ." - the CURSORINFO is an out-parameter the caller supplies (and this TU's banner records that pci->cbSize must be set by the caller first), so the BOOL carries no answer and zero is reserved; sdkRetType BOOL so BoolCheck (returncheck.h:94) covers it. Audit: the table reports 1 in-tree caller and the site-level read finds User32::GetCursorInfo live at ui/gui/gdiobjects/cursor.cpp:126 as "return User32::GetCursorInfo(reinterpret_cast<PCURSORINFO>(&ci)) ? true : false;" in Cursor::GetInfo, which sets ci.cbSize before the call; CheckReturn returns the value unchanged, so the returned bool is identical and only a diagnostic is added at a documented failure.
    BOOL User32::GetCursorInfo(PCURSORINFO pci)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, GetCursorInfo);
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call< BOOL, PCURSORINFO >(pFunc, pci);
        return bResult;
    }

    //Return policy (U2, table row GetCursorPos / cursor.cpp, 12 Nov 2026): checked with checkClass BoolCheck; the page's verbatim
    //docQuote is "Type: BOOL Returns nonzero if successful or zero otherwise. To get extended error information, call GetLastError ." - the LPPOINT is an out-parameter the caller supplies, so the BOOL carries no answer and zero is reserved for a failed query; sdkRetType BOOL so BoolCheck (returncheck.h:94) covers it. Audit: the table reports 1 in-tree caller and the site-level read finds User32::GetCursorPos live at ui/gui/gdiobjects/cursor.cpp:133 as "bool result = User32::GetCursorPos(&p) ? true : false;" whose result gates copying the POINT into the caller's Point; CheckReturn returns the value unchanged, so that gate reads exactly as before and only a diagnostic is added at a documented failure. This TU's banner records that GetCursorPos (142) and GetPhysicalCursorPos (193) share RVA 0002B230, so the two rows land the same check against one implementation - recorded, not merged.
    BOOL User32::GetCursorPos(LPPOINT lpPoint)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, GetCursorPos);
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call< BOOL, LPPOINT >(pFunc, lpPoint);
        return bResult;
    }

    //Return policy (U2, table row GetPhysicalCursorPos / cursor.cpp, 12 Nov 2026): checked with checkClass BoolCheck; the page's verbatim
    //docQuote is "Type: BOOL TRUE if successful; otherwise FALSE . GetLastError can be called to get more information about any error that is generated." - the LPPOINT is an out-parameter the caller supplies, so the BOOL carries no answer and FALSE is the failure branch; sdkRetType BOOL so BoolCheck (returncheck.h:94) covers it. Audit: the table reports 1 in-tree caller and the site-level read finds User32::GetPhysicalCursorPos live at ui/gui/gdiobjects/cursor.cpp:145 as "bool result = User32::GetPhysicalCursorPos(&p) ? true : false;" whose result gates copying the POINT into the caller's Point; CheckReturn returns the value unchanged, so that gate reads exactly as before and only a diagnostic is added at a documented failure.
    BOOL User32::GetPhysicalCursorPos(LPPOINT lpPoint)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, GetPhysicalCursorPos);
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call< BOOL, LPPOINT >(pFunc, lpPoint);
        return bResult;
    }

    //Return policy (U2, table row LoadCursor / cursor.cpp, 12 Nov 2026): checked with checkClass CheckNonZeroHandle< HCURSOR >; the page's verbatim
    //docQuote is "Type: HCURSOR If the function succeeds, the return value is the handle to the newly loaded cursor. If the function fails, the return value is NULL . To get extended error information, call GetLastError ." - the handle is what the caller holds, so no legal answer is NULL and NULL is reserved; the row's AW binding leaves NO-PROTOTYPE in the table, so the class base is the published HCURSOR and CheckNonZeroHandle (returncheck.h:137) covers it. Audit: the table reports 0 in-tree callers for the bare stem (change-safe); the site-level T-tolerant read finds the member spelling User32::LoadCursorT live twice at ui/gui/gdiobjects/cursor.cpp:37 (Cursor(void*, const std::string&) ctor) and :51 (Cursor(const wchar_t*) ctor), both storing the return into m_handle with m_owning = false, the shared-system-handle case this TU's banner records. At :51 the NULL is the hInstance *argument* (the desktop's default cursor), not the return, so the NULL the page reserves stays reserved nonzero and is not confused with that argument; CheckReturn returns the value unchanged, so neither ctor's stored handle changes and only a diagnostic is added at a documented NULL.
    HCURSOR User32::LoadCursorT(HINSTANCE hInstance, LPCTSTR lpCursorName)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapiAW(user32, LoadCursor);
        CheckReturn< HCURSOR, CheckNonZeroHandle< HCURSOR > >::TType hResult = Library::Call< HCURSOR, HINSTANCE, LPCTSTR >(pFunc, hInstance, lpCursorName);
        return hResult;
    }

    //Return policy (U2, table row LoadCursorFromFile / cursor.cpp, 12 Nov 2026): checked with checkClass CheckNonZeroHandle< HCURSOR >; the page's verbatim
    //docQuote is "Type: HCURSOR If the function is successful, the return value is a handle to the new cursor. If the function fails, the return value is NULL . To get extended error information, call GetLastError . GetLastError may return the following value. Return code Description ERROR_FILE_NOT_FOUND The specified file cannot be found." - the handle is what the caller holds and this TU's banner records as one the caller must DestroyCursor, so no legal answer is NULL and NULL is reserved, with the named ERROR_FILE_NOT_FOUND as the extended info for a missing file; the row's AW binding leaves NO-PROTOTYPE in the table, so the class base is the published HCURSOR and CheckNonZeroHandle (returncheck.h:137) covers it. Audit: the table reports 0 in-tree callers for the bare stem (change-safe); the site-level T-tolerant read finds the member spelling User32::LoadCursorFromFileT live once at ui/gui/gdiobjects/cursor.cpp:45 (Cursor(const std::string&) ctor), storing the return into m_handle with m_owning = true; CheckReturn returns the value unchanged, so the stored handle is identical and only a diagnostic is added at a documented NULL.
    HCURSOR User32::LoadCursorFromFileT(LPCTSTR lpFileName)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapiAW(user32, LoadCursorFromFile);
        CheckReturn< HCURSOR, CheckNonZeroHandle< HCURSOR > >::TType hResult = Library::Call< HCURSOR, LPCTSTR >(pFunc, lpFileName);
        return hResult;
    }

    //Return policy (U2, table row SetCursorPos / cursor.cpp, 12 Nov 2026): checked with checkClass BoolCheck; the page's verbatim
    //docQuote is "Type: BOOL Returns nonzero if successful or zero otherwise. To get extended error information, call GetLastError ." - the coordinates are parameters, so the BOOL carries no answer and zero is reserved for a failed move; sdkRetType BOOL so BoolCheck (returncheck.h:94) covers it. Audit: the table reports 1 in-tree caller and the site-level read finds User32::SetCursorPos live at ui/gui/gdiobjects/cursor.cpp:156 as "return User32::SetCursorPos(x, y) ? true : false;" in Cursor::SetPosition; CheckReturn returns the value unchanged, so the returned bool is identical and only a diagnostic is added at a documented failure. This TU's banner records that SetCursorPos (32E) and SetPhysicalCursorPos (354) share RVA 0002FA50, so the pair lands the same reading against one implementation.
    BOOL User32::SetCursorPos(int X, int Y)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, SetCursorPos);
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call< BOOL, int, int >(pFunc, X, Y);
        return bResult;
    }

    //Return policy (U2, table row SetPhysicalCursorPos / cursor.cpp, 12 Nov 2026): checked with checkClass BoolCheck; the page's verbatim
    //docQuote is "Type: BOOL TRUE if successful; otherwise FALSE ." - the coordinates are parameters, so the BOOL carries no answer and FALSE is the failure branch; sdkRetType BOOL so BoolCheck (returncheck.h:94) covers it, the same shape the GetPhysicalCursorPos row in slice 1 takes. Audit: the table reports 1 in-tree caller and the site-level read finds User32::SetPhysicalCursorPos live at ui/gui/gdiobjects/cursor.cpp:161 as "return User32::SetPhysicalCursorPos(x, y) ? true : false;" in Cursor::SetPhysicalPosition; CheckReturn returns the value unchanged, so the returned bool is identical and only a diagnostic is added at a documented failure. The page names no GetLastError route in its Return value section - recorded as the page's own brevity, not a missing check.
    BOOL User32::SetPhysicalCursorPos(int X, int Y)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, SetPhysicalCursorPos);
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call< BOOL, int, int >(pFunc, X, Y);
        return bResult;
    }

    //Return policy (U2, table row ShowCursor / cursor.cpp, 12 Nov 2026): unchecked, no CheckReturn token; the page's verbatim
    //docQuote is "Type: int The return value specifies the new display counter." - the int is an answer, not a status, and the page makes 0 reachable by construction ("If bShow is TRUE, the display count is incremented by one. If bShow is FALSE, the display count is decremented by one" plus the Remarks' "The cursor is displayed only if the display count is greater than or equal to 0. If a mouse is installed, the initial display count is 0"), so no value is reserved for failure and the session-27 test has nothing to key on; this TU's banner already carries the same reading at :40-41. The site that misreads it is recorded, not repaired, at ui/gui/gdiobjects/cursor.cpp:164-181 (the U20 note, closed by D47): Cursor::Show returns "User32::ShowCursor(show ? 1 : 0) ? true : false", which reads the signed count as a flag - the note's own words are "the first Show(false) takes the count to -1 and reports failure for a hide that worked, and the next Show(true) takes it back to 0 and reports success while changing nothing" - a winqor_gui behaviour change the api_layer review records rather than performs.
    int User32::ShowCursor(BOOL bShow)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, ShowCursor);
        return Library::Call< int, BOOL >(pFunc, bShow);
    }

    //Return policy (U2, table row SetCursor / cursor.cpp, 12 Nov 2026): unchecked, no CheckReturn token; the page's verbatim
    //docQuote is "Type: HCURSOR The return value is the handle to the previous cursor, if there was one. If there was no previous cursor, the return value is NULL ." - NULL names a legal state the caller can be in (a window with no cursor set yet) rather than a failed call, and this page gives no failure branch and no GetLastError extended info, unlike the CreateCursor and LoadCursor rows beside it, which do; the session-27 soundness test has no reserved value to key on, and CheckNonZeroHandle< HCURSOR > would report a first SetCursor as a failure. Audit: the table reports 1 in-tree caller and the site-level read finds User32::SetCursor live at ui/gui/gdiobjects/cursor.cpp:185-186 as "User32::SetCursor( reinterpret_cast<HCURSOR>(m_handle.Use()));" in Cursor::SetAsCurrent(), whose return type is void - the previous-cursor handle the page describes is discarded there, so the reading this row books is the one the only in-tree caller already relies on and no site misreads it.
    HCURSOR User32::SetCursor(HCURSOR hCursor)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, SetCursor);
        return Library::Call< HCURSOR, HCURSOR >(pFunc, hCursor);
    }

    //Return policy (U2, table row SetSystemCursor / cursor.cpp, 12 Nov 2026): checked with checkClass BoolCheck; the page's verbatim
    //docQuote is "Type: BOOL If the function succeeds, the return value is nonzero. If the function fails, the return value is zero. To get extended error information, call GetLastError ." - the handle and the id are parameters (this TU's banner records that winuser.h:10728-10730 carries no ownership SAL and documents no transfer for hcur), so the BOOL carries no answer and zero is reserved; sdkRetType BOOL so BoolCheck (returncheck.h:94) covers it. Audit: the table reports 1 in-tree caller and the site-level read finds User32::SetSystemCursor live at ui/gui/gdiobjects/cursor.cpp:191 as "return User32::SetSystemCursor(reinterpret_cast<HCURSOR>(m_handle.Use()), id) ? true : false;" in Cursor::SetAsSystemCursor; CheckReturn returns the value unchanged, so the returned bool is identical and only a diagnostic is added at a documented failure. This TU's banner also records the documentation claim that Microsoft documents this call (and ShowSystemCursor) as unsupported from Windows 8 up - not re-measured here - so the check is landed on a call the layer keeps for export completeness.
    BOOL User32::SetSystemCursor(HCURSOR hcur, DWORD id)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, SetSystemCursor);
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call< BOOL, HCURSOR, DWORD >(pFunc, hcur, id);
        return bResult;
    }

}}}//qor::win::api
