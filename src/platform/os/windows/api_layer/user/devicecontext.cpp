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

    //The "DC functions" section (user32.h:325-333): eight declarations, eight definitions here, in the header's order,
    //and each definition matches its declaration parameter-for-parameter. Five of the entry points have an SDK A/W family
    //(winuser.h:13077-13093 ChangeDisplaySettings, :13095-13117 ChangeDisplaySettingsEx, :13123-13141 EnumDisplaySettings,
    //:13145-13165 EnumDisplaySettingsEx, :13171-13191 EnumDisplayDevices), so those five wrappers spell the binding
    //qor_pp_useswinapiAW and carry the folder's T suffix; the other three - GetDC (143), GetDCEx (144), ReleaseDC (2FF) -
    //are single exports with no A/W variants and no stem -> stemW macro, so they use the bare qor_pp_useswinapi. cl /EP
    //on this TU resolves "ChangeDisplaySettingsW", "ChangeDisplaySettingsExW", "EnumDisplayDevicesW",
    //"EnumDisplaySettingsW", "EnumDisplaySettingsExW", "GetDC", "GetDCEx", "ReleaseDC" (tmp/_tmp_dc_pp.txt:453278-453327)
    //and all eight are in the export table; the A halves (26, 27, FC, FF, 100) are exported too but are not selectable
    //through this surface. The wrong-stem scan reports no binding-vs-member disagreement in this file.
    //Return conventions differ inside this one section, which is why no blanket return check fits: ChangeDisplaySettingsT
    //and ChangeDisplaySettingsExT return DISP_CHANGE_* codes in which zero IS success (DISP_CHANGE_SUCCESSFUL == 0,
    //positive codes mean a reboot is needed, negative codes are the failures), while the three EnumDisplay* wrappers
    //return BOOL FALSE on failure and ReleaseDC returns 0 for failure and 1 for success. A CheckReturn<> over the
    //ChangeDisplaySettings pair would read every success as a failure - the eight sites are recorded as U1/U2 instances
    //and left alone (session-3 policy), and the SEH swallow (U7) is indistinguishable from a legitimate DISP_CHANGE_* code.
    //Parameter contracts: ChangeDisplaySettingsExT's hwnd is _Reserved_ in the SDK (winuser.h:13101, :13110 - must be
    //NULL) but is exposed here as a free HWND. The device-name parameters of all five display wrappers are _In_opt_
    //(NULL selects the default device), ChangeDisplaySettingsT's lpDevMode is _In_opt_ (NULL resets to default), and the
    //DEVMODE of EnumDisplaySettingsT/EnumDisplaySettingsExT is _Inout_ - the caller must zero it (dmSize, dmDriverExtra)
    //first. Only the EnumDisplaySettingsEx pair accepts the EDS_RAWMODE/EDS_ROTATEDMODE flags (winuser.h:13168-13169).
    //Handle contracts: GetDC and GetDCEx return a window DC that only ReleaseDC releases - not Kernel32::CloseHandle -
    //and ReleaseDC fails (0) when the DC belongs to another thread or the HWND/HDC pair does not match. GetDCEx is the
    //only member of the pair that takes a clip region plus DCX_* flags. The two remaining members of the SDK's DC block
    //are wrapped elsewhere: GetWindowDC (1E2) and WindowFromDC (3EE) are declared at user32.h:274, :287 in the "Painting
    //functions" section and defined in painting.cpp:93, :184, so the DC neighbourhood winuser.h:8560-8614 declares across
    //the two adjacent Desktop-family sub-blocks (:8443-8580, :8601-8654) is split across two QOR sections and two TUs,
    //while five of this TU's eight members are display-configuration entry points with no HDC at all - open item U21
    //covers the naming/placement question. The mismatch clause is not theoretical: the live path
    //ui/gui/controllers/windowcontroller.cpp:615-619 takes the HDC from GetDC(WINDOW_HANDLE) here, wraps it in a
    //DeviceContext created by PerThread::DeviceContextFromHandle (which leaves that object's HWND member 0) and
    //MustRelease()s it, so the destructor's ReleaseDC call (ui/gui/gdiobjects/devicecontext.cpp:65) passes a NULL HWND
    //against a DC that belongs to a window - it returns 0 and the DC is never returned. Recorded as U22, not fixed here.
    //U16 exposure: EnumDisplaySettingsExA/W (winuser.h:13148, :13156) and EnumDisplayDevicesA/W (:13174, :13182) sit
    //inside :13143 #if(WINVER >= 0x0500); the three other A/W families are not version-gated, and GetDC (:8569) /
    //GetDCEx (:8575) / ReleaseDC (:8612) sit only under #if WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_DESKTOP) (:8443,
    //:8601). The whole display block is additionally inside #ifdef _WINGDI_ / #ifndef NOGDI / #ifndef NOSYSPARAMSINFO
    //(:12433, :13072-13075), and these signatures name DEVMODE / DISPLAY_DEVICE types from wingdi.h, so a consumer that
    //defines NOGDI loses the very types the declarations need - a harder dependency than the version guards. The build
    //defines neither (grep of cmake/ and every CMakeLists.txt), and no WINVER is pinned, so sdkddkver.h's 0x0A00 default
    //keeps all of them visible today.
    //
    //Addendum, 12 Nov 2026 (U2 Small TU batch S5, item devicecontext sites): the paragraphs above are the review's record
    //and stand superseded - the endpoint table reads 7 of the 8 rows of this TU checked and ReleaseDC unchecked, so this
    //TU lands 7 CheckReturn sites (2 CheckNonZeroHandle< HDC > over the GetDC / GetDCEx pair, 2 TSuccessCheck< LONG >
    //over the ChangeDisplaySettings pair and 3 BoolCheck over the EnumDisplay* trio) and gained its one ../returncheck.h
    //include (0 -> 1). The decision is the table's, not re-derived here: the DC pages state "If the function fails, the
    //return value is NULL" with NULL reserved for the failure - a NULL hWnd is a parameter asking for the whole-screen
    //DC, not a returned answer - the ChangeDisplaySettings pages name "DISP_CHANGE_SUCCESSFUL The settings change was
    //successful" as the only success code and list only failure codes after it, so the test reads value == 0 and the
    //landed class is the opposite polarity to the nonzero reflex the review warned against (the pass-through the review
    //describes is exactly the value TSuccessCheck leaves in place), and the EnumDisplay* pages reserve zero for failure
    //with the end of the enumeration as that documented failure, not a legal answer. ReleaseDC keeps its pass-through:
    //its zero is the documented answer for a class or private DC, so the session-27 soundness test fails there and the
    //row stays unchecked with the reason. The 5 AW rows count 0 callers for their bare stem, but the T wrappers are
    //called in-tree (ui/gdi/displaydevices.cpp:42,:47,:61,:70 and ui/gui/controllers/i_m/monitor.cpp:94,:177,:195,:213),
    //and GetDC / GetDCEx have 3 and 2 User32:: call sites (ui/gui/controllers/i_m/monitor.cpp:146, then :615 and :627
    //in ui/gui/controllers/windowcontroller.cpp, then :44 and :52 in ui/gui/gdiobjects/devicecontext.cpp) - every one
    //of those call sites was read before its token moved, and each row's verbatim docQuote and class sits at its site below.

    //Return policy (U2, table row ChangeDisplaySettings / devicecontext.cpp, 12 Nov 2026): the row reads checked with
    //checkClass TSuccessCheck< LONG >, and the table's verbatim docQuote opens "The ChangeDisplaySettings function
    //returns one of the following values. Return code Description DISP_CHANGE_SUCCESSFUL The settings change was
    //successful." and then lists only failure entries - BADDUALVIEW, BADFLAGS, BADMODE, BADPARAM, FAILED, NOTUPDATED,
    //RESTART - so the single success code is 0 and every other value the page names is a documented failure: the test
    //must read value == 0, not value != 0. TSuccessCheck (returncheck.h:174, Test at :179 is (value == 0) ? true :
    //false) is the vocabulary entry for that polarity, and the AW row carries NO-PROTOTYPE, so the class base is the
    //published LONG. 0 in-tree callers for this bare stem (change-safe), so no call site needed re-reading before this
    //token moved.
    LONG User32::ChangeDisplaySettingsT(::LPDEVMODE lpDevMode, DWORD dwFlags)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapiAW(user32, ChangeDisplaySettings);
        CheckReturn< LONG, TSuccessCheck< LONG > >::TType lResult = Library::Call< LONG, ::LPDEVMODE, DWORD >(pFunc, lpDevMode, dwFlags);
        return lResult;
    }

    //Return policy (U2, table row ChangeDisplaySettingsEx / devicecontext.cpp, 12 Nov 2026): the row reads checked with
    //checkClass TSuccessCheck< LONG > - the same return-code table as the non-Ex twin above, whose verbatim docQuote
    //opens "The ChangeDisplaySettingsEx function returns one of the following values. Return code Description
    //DISP_CHANGE_SUCCESSFUL The settings change was successful." and then names only failure codes - so 0 is the
    //reserved success answer and the test reads value == 0. TSuccessCheck (returncheck.h:174, Test at :179 is
    //(value == 0) ? true : false), and the AW row has NO-PROTOTYPE, so the class base is the published LONG. The
    //table counts 0 callers for the bare stem, but the T wrapper has 1 in-tree call site, re-read before this token
    //moved: ui/gdi/displaydevices.cpp:70 hands the LONG straight out of DisplayDevices::ChangeSettings, so a
    //documented failure code now raises a continuable issue the Win32ErrorHandler above reports while the code still
    //reaches that caller unchanged.
    LONG User32::ChangeDisplaySettingsExT(LPCTSTR lpszDeviceName, ::LPDEVMODE lpDevMode, HWND hwnd, DWORD dwflags, LPVOID lParam)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapiAW(user32, ChangeDisplaySettingsEx);
        CheckReturn< LONG, TSuccessCheck< LONG > >::TType lResult = Library::Call< LONG, LPCTSTR, ::LPDEVMODE, HWND, DWORD, LPVOID >(pFunc, lpszDeviceName, lpDevMode, hwnd, dwflags, lParam);
        return lResult;
    }

    //Return policy (U2, table row EnumDisplayDevices / devicecontext.cpp, 12 Nov 2026): the row reads checked with
    //checkClass BoolCheck, and the table's verbatim docQuote is "If the function succeeds, the return value is nonzero.
    //If the function fails, the return value is zero. The function fails if iDevNum is greater than the largest device
    //index." - zero is reserved for the failure and running past the last device index is that documented failure, not
    //a legal answer. BoolCheck (returncheck.h:94, Test at :99 is value ? true : false) fails on zero only. The AW row
    //has NO-PROTOTYPE, so the class base is the published BOOL. The table counts 0 callers for the bare stem, but the
    //T wrapper has 4 in-tree call sites, all re-read before this token moved: ui/gdi/displaydevices.cpp:42,:47 and
    //ui/gui/controllers/i_m/monitor.cpp:195,:213 use the BOOL as a while-condition, so the end-of-list FALSE now also
    //raises a continuable issue while the loop still ends on the same value.
    BOOL User32::EnumDisplayDevicesT(LPCTSTR lpDevice, DWORD iDevNum, ::PDISPLAY_DEVICE lpDisplayDevice, DWORD dwFlags)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapiAW(user32, EnumDisplayDevices);
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call< BOOL, LPCTSTR, DWORD, ::PDISPLAY_DEVICE, DWORD >(pFunc, lpDevice, iDevNum, lpDisplayDevice, dwFlags);
        return bResult;
    }

    //Return policy (U2, table row EnumDisplaySettings / devicecontext.cpp, 12 Nov 2026): the row reads checked with
    //checkClass BoolCheck, and the table's verbatim docQuote is "If the function succeeds, the return value is nonzero.
    //If the function fails, the return value is zero." - the page reserves zero for the failure and the end of the mode
    //list is that documented failure, not a legal answer. BoolCheck (returncheck.h:94, Test at :99 is value ? true :
    //false) fails on zero only; the AW row has NO-PROTOTYPE, so the class base is the published BOOL. The table counts
    //0 callers for the bare stem, but the T wrapper has 1 in-tree call site, re-read before this token moved:
    //ui/gui/controllers/i_m/monitor.cpp:177 throws the return away and reads the DEVMODE afterwards, so a FALSE from
    //ENUM_CURRENT_SETTINGS now raises a continuable issue the Win32ErrorHandler above reports while the DEVMODE that
    //caller reads afterwards is left exactly as the SDK left it.
    BOOL User32::EnumDisplaySettingsT(LPCTSTR lpszDeviceName, DWORD iModeNum, ::LPDEVMODE lpDevMode)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapiAW(user32, EnumDisplaySettings);
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call< BOOL, LPCTSTR, DWORD, ::LPDEVMODE >(pFunc, lpszDeviceName, iModeNum, lpDevMode);
        return bResult;
    }

    //Return policy (U2, table row EnumDisplaySettingsEx / devicecontext.cpp, 12 Nov 2026): the row reads checked with
    //checkClass BoolCheck - the table's verbatim docQuote is the same two-line status wording as the non-Ex twin above,
    //"If the function succeeds, the return value is nonzero. If the function fails, the return value is zero.", so zero
    //is reserved and the end of the mode list is that documented failure. BoolCheck (returncheck.h:94, Test at :99 is
    //value ? true : false) fails on zero only; the AW row has NO-PROTOTYPE, so the class base is the published BOOL.
    //The table counts 0 callers for the bare stem, but the T wrapper has 2 in-tree call sites, both re-read before this
    //token moved: ui/gdi/displaydevices.cpp:61 drives a while-loop over modeNumber++, and ui/gui/controllers/i_m/
    //monitor.cpp:94 returns the BOOL as Monitor::GetDeviceMode's own bool - the FALSE that ends an enumeration now
    //also raises a continuable issue, and both callers still branch on the same value.
    BOOL User32::EnumDisplaySettingsExT(LPCTSTR lpszDeviceName, DWORD iModeNum, ::LPDEVMODE lpDevMode, DWORD dwFlags)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapiAW(user32, EnumDisplaySettingsEx);
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call< BOOL, LPCTSTR, DWORD, ::LPDEVMODE, DWORD >(pFunc, lpszDeviceName, iModeNum, lpDevMode, dwFlags);
        return bResult;
    }

    //Return policy (U2, table row GetDC / devicecontext.cpp, 12 Nov 2026): the row reads checked with checkClass
    //CheckNonZeroHandle< HDC >, and the table's verbatim docQuote is "If the function succeeds, the return value is a
    //handle to the DC for the specified window's client area. If the function fails, the return value is NULL ." - NULL
    //is reserved for the failure here, and the NULL-hWnd whole-screen route is a parameter the caller passes rather than
    //an answer the function returns, so no legal answer collides with the tested value. sdkRetType is HDC on both sides,
    //so the class base is that SDK type. CheckNonZeroHandle (returncheck.h:137, Test at :144 is (value == 0x00000000) ?
    //false : true) fails on NULL only. 3 in-tree call sites, re-read before this token moved: ui/gui/controllers/i_m/
    //monitor.cpp:146 asks for the whole-screen DC and feeds GetDeviceCaps with what comes back; windowcontroller.cpp:615
    //(same folder) guards hdc != nullptr before wrapping it; ui/gui/gdiobjects/devicecontext.cpp:44 hands the handle
    //straight to the DeviceContext ctor. A NULL now raises a continuable issue the Win32ErrorHandler above reports, and
    //the NULL itself still reaches each caller unchanged.
    HDC User32::GetDC(HWND hWnd)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, GetDC);
        CheckReturn< HDC, CheckNonZeroHandle< HDC > >::TType hResult = Library::Call< HDC, HWND >(pFunc, hWnd);
        return hResult;
    }

    //Return policy (U2, table row GetDCEx / devicecontext.cpp, 12 Nov 2026): the row reads checked with checkClass
    //CheckNonZeroHandle< HDC >, and the table's verbatim docQuote is "If the function succeeds, the return value is the
    //handle to the DC for the specified window. If the function fails, the return value is NULL . An invalid value for
    //the hWnd parameter will cause the function to fail." - NULL is reserved for the failure here, and the clip region
    //plus the DCX_* flags are parameters the caller passes rather than answers the function returns, so no legal
    //answer collides with the tested value. sdkRetType is HDC on both sides, so the class base is that SDK type.
    //CheckNonZeroHandle (returncheck.h:137, Test at :144 is (value == 0x00000000) ? false : true) fails on NULL only.
    //2 in-tree call sites, both re-read before this token moved: ui/gui/controllers/windowcontroller.cpp:627 keeps its
    //own hdc != nullptr guard and returns the default DeviceContext on the NULL route, while ui/gui/gdiobjects/
    //devicecontext.cpp:52 hands the handle straight to the DeviceContext ctor with no guard at all. A NULL now raises
    //a continuable issue the Win32ErrorHandler above reports, and the NULL itself still reaches each caller unchanged.
    HDC User32::GetDCEx(HWND hWnd, HRGN hrgnClip, DWORD flags)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, GetDCEx);
        CheckReturn< HDC, CheckNonZeroHandle< HDC > >::TType hResult = Library::Call< HDC, HWND, HRGN, DWORD >(pFunc, hWnd, hrgnClip, flags);
        return hResult;
    }

    //Return policy (U2, table row ReleaseDC / devicecontext.cpp, 12 Nov 2026): the row reads UNCHECKED with no check
    //class, and the table's verbatim docQuote is "The return value indicates whether the DC was released. If the DC
    //was released, the return value is 1. If the DC was not released, the return value is zero." - the Remarks add
    //"It frees only common and window DCs. It has no effect on class or private DCs", so a class or private DC
    //legitimately answers zero: zero is an outcome of the question the caller asked, not a value reserved for the
    //failure, so the session-27 soundness test fails at that value and a check would report a legal answer as a
    //failure. The table counts 3 in-tree callers and all 3 were re-read before this verdict was accepted:
    //ui/gui/controllers/i_m/monitor.cpp:149 discards the int, ui/gui/controllers/windowcontroller.cpp:638 returns it
    //as WindowController::ReleaseDC's own int, and ui/gui/gdiobjects/devicecontext.cpp:91 returns it from the
    //DeviceContext release route the U22 note above describes. This site keeps 0 CheckReturn and hands the int over
    //as written.
    int User32::ReleaseDC(HWND hWnd, HDC hDC)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, ReleaseDC);
        return Library::Call< int, HWND, HDC >(pFunc, hWnd, hDC);
    }


}}}//qor::win::api
