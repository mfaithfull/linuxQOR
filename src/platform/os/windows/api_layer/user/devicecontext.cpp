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

    LONG User32::ChangeDisplaySettingsT(::LPDEVMODE lpDevMode, DWORD dwflags)
    {
        qor_pp_fcontext;
        qor_pp_useswinapiAW(user32, ChangeDisplaySettings);
        return Library::Call< LONG, ::LPDEVMODE, DWORD >(pFunc, lpDevMode, dwflags);
    }

    LONG User32::ChangeDisplaySettingsExT(LPCTSTR lpszDeviceName, ::LPDEVMODE lpDevMode, HWND hwnd, DWORD dwflags, LPVOID lParam)
    {
        qor_pp_fcontext;
        qor_pp_useswinapiAW(user32, ChangeDisplaySettingsEx);
        return Library::Call< LONG, LPCTSTR, ::LPDEVMODE, HWND, DWORD, LPVOID >(pFunc, lpszDeviceName, lpDevMode, hwnd, dwflags, lParam);
    }

    BOOL User32::EnumDisplayDevicesT(LPCTSTR lpDevice, DWORD iDevNum, ::PDISPLAY_DEVICE lpDisplayDevice, DWORD dwFlags)
    {
        qor_pp_fcontext;
        qor_pp_useswinapiAW(user32, EnumDisplayDevices);
        return Library::Call< BOOL, LPCTSTR, DWORD, ::PDISPLAY_DEVICE, DWORD >(pFunc, lpDevice, iDevNum, lpDisplayDevice, dwFlags);
    }

    BOOL User32::EnumDisplaySettingsT(LPCTSTR lpszDeviceName, DWORD iModeNum, ::LPDEVMODE lpDevMode)
    {
        qor_pp_fcontext;
        qor_pp_useswinapiAW(user32, EnumDisplaySettings);
        return Library::Call< BOOL, LPCTSTR, DWORD, ::LPDEVMODE >(pFunc, lpszDeviceName, iModeNum, lpDevMode);
    }

    BOOL User32::EnumDisplaySettingsExT(LPCTSTR lpszDeviceName, DWORD iModeNum, ::LPDEVMODE lpDevMode, DWORD dwFlags)
    {
        qor_pp_fcontext;
        qor_pp_useswinapiAW(user32, EnumDisplaySettingsEx);
        return Library::Call< BOOL, LPCTSTR, DWORD, ::LPDEVMODE, DWORD >(pFunc, lpszDeviceName, iModeNum, lpDevMode, dwFlags);
    }

    HDC User32::GetDC(HWND hWnd)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, GetDC);
        return Library::Call< HDC, HWND >(pFunc, hWnd);
    }

    HDC User32::GetDCEx(HWND hWnd, HRGN hrgnClip, DWORD flags)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, GetDCEx);
        return Library::Call< HDC, HWND, HRGN, DWORD >(pFunc, hWnd, hrgnClip, flags);
    }

    int User32::ReleaseDC(HWND hWnd, HDC hDC)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, ReleaseDC);
        return Library::Call< int, HWND, HDC >(pFunc, hWnd, hDC);
    }


}}}//qor::win::api
