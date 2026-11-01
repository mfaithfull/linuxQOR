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

    //The "Icon functions" section (user32.h:426-440): fourteen declarations, all static, and all fourteen defined here in
    //header order - the first section of this library with no declared-but-undefined member, so 0 U4 here, and 0 U11 /
    //0 U13 (every definition is qualified User32:: and pairs with a static declaration).
    //Bindings: eleven bare qor_pp_useswinapi + three qor_pp_useswinapiAW, and the AW set is exactly the three Icon A/W
    //families the SDK has (winuser.h:10750 LoadIconW, :10775 PrivateExtractIconsW, :11026 GetIconInfoExW; the A/W mapping
    //macros are at :10754-:10758, :10779-:10783, :11028-:11032). cl /EP (tmp/_tmp_icon_pp.txt through tmp/_tmp_pp_literals.ps1,
    //tmp/_tmp_icon_pre18.txt lines 30-44) shows the fourteen literals actually emitted: "CopyIcon", "CreateIcon",
    //"CreateIconFromResource", "CreateIconFromResourceEx", "CreateIconIndirect", "DestroyIcon", "DrawIcon", "DrawIconEx",
    //"GetIconInfo", "GetIconInfoExW", "LoadIconW", "LookupIconIdFromDirectory", "LookupIconIdFromDirectoryEx",
    //"PrivateExtractIconsW" - 14 literals, 0 that are not exports, and the wrong-stem scan flags 0 sites here. The three AW
    //bindings are load-bearing (live probe tmp/_tmp_name_resolve_probe.cpp: bare LoadIcon, PrivateExtractIcons and
    //GetIconInfoEx all MISSING, each W half RESOLVED); the eleven bare ones are genuine single exports (ordinals 59, 6B,
    //6C, 6D, 6E, B1, D5, D6, 160, 26F, 270 in tmp/_tmp_user32_exports.txt) with no A/W family, so bare is their D13 form.
    //Signatures pair with the SDK field for field (tmp/_tmp_sigcmp.ps1 -Tu icon.cpp -First 427 -Last 440: 14/14 decl==def ok
    //and 14/14 Call ok; tmp/_tmp_sdk_param_name_scan.ps1: 0 divergent names, 0 return-shape notes), and every return type
    //matches winuser.h - HICON for the six acquirers, BOOL for DestroyIcon/DrawIcon/DrawIconEx/GetIconInfo/GetIconInfoExW,
    //int for the two LookupIconIdFromDirectory forms, UINT for PrivateExtractIconsW - so 0 U18 instances. DrawIcon,
    //DrawIconEx and GetIconInfoExW returning BOOL is a genuine success flag, while the six HICON returns fold
    //NULL-on-failure onto a legitimate empty handle (U2 policy: no CheckReturn added here, and U1: no Win32ErrorHandler).
    //Fixed in this session: seven parameter names that contradicted the SDK prototypes (CreateIconFromResourceEx
    //pbIconBits/cbIconBits/dwVersion/uFlags -> presbits/dwResSize/dwVer/Flags per winuser.h:10840, GetIconInfoExT
    //hIcon/piconinfoex -> hicon/piconinfo per :11026, PrivateExtractIconsT lpszFile -> szFileName per :10775), each in the
    //user32.h declaration (:430, :436, :440) and the definition together - the folder's parameter-name class, 4th TU after
    //devicemanagement (2), desktop and hook; name-level and type-level consistency are measured separately now, and both
    //scans read clean here. Plus the ::-spelling split in one Call<> template argument (`const ICONINFO*` at :48 against
    //`const ::ICONINFO*` in the declaration and the SDK) cured by spelling the site like the declaration, and a missing
    //final newline (HEAD ends "}}//qor::win::api" with no LF; the file is now pure ASCII, 0 tabs, 0 trailing whitespace).
    //U16: four of the fourteen prototypes are version-gated, and one is the ill-formed-type sub-case with the highest
    //threshold measured so far. LookupIconIdFromDirectoryEx (winuser.h:10819) and CreateIconFromResourceEx (:10840) sit
    //under #if(WINVER >= 0x0400) (:10815, :10836), DrawIconEx (:10948) under :10883 (also 0x0400), and GetIconInfoExA/W
    //(:11020, :11026) under #if(_WIN32_WINNT >= 0x0600) (:10986, closed :11034) - inside which ICONINFOEXA/ICONINFOEXW and
    //the PICONINFOEX typedef also live (:10987-:11014), so user32.h:436 does not just lose its prototype below 0x0600, it
    //becomes ill-formed on an unknown type (the HDEVNOTIFY / DEVMODE / MSGBOXPARAMSW / PFLASHWINFO class). The remaining
    //nine sit under nothing but the Desktop partition + NOUSER, so there is no opt-out that breaks them.
    //Handle contract - the section splits three ways and the wrapper returns all of it raw. Six entry points ACQUIRE:
    //CopyIcon, CreateIcon, CreateIconFromResource, CreateIconFromResourceEx, CreateIconIndirect, LoadIconT (and
    //PrivateExtractIconsT fills a caller-sized HICON array of acquired handles), each released only by DestroyIcon.
    //Two more ACQUIRE through an out parameter: GetIconInfo and GetIconInfoExW succeed by creating two new bitmaps,
    //ICONINFO.hbmMask and hbmColor, which the caller must DeleteObject - nothing here names ICONINFO to the caller or
    //deletes them, so the folder's rule "an acquired handle is returned for the caller to release" has to be read as
    //covering those two as well. Nothing here releases anything it acquires, and DrawIcon/DrawIconEx/Lookup*/DestroyIcon
    //take caller-owned handles.
    //Liveness (U15): the sixteen stems have 16 LAYER call sites, all in one built file - ui/gui/gdiobjects/icon.cpp:18,
    //:24, :30, :36, :42, :48, :54, :62, :73, :79, :84, :89, :94, :99, :104, :109 (gui/CMakeLists.txt:49) - which makes the
    //question one level up rather than here. Measured answer: no method of that Icon class has a call site anywhere in
    //src/examples/test (Clone, Draw, DrawEx, GetInfo, GetInfoExT, LookupIdFromDirectory, LookupIdFromDirectoryEx and
    //PrivateExtractT appear only as definitions), and only three of its constructors are reached: Icon(const wchar_t*) at
    //ui/gui/factories/windowfactory.cpp:89 and Icon(const PrimitiveHandle&, own) at buttoncontroller.cpp:87,:92,
    //staticcontroller.cpp:70,:94 and view/handlers/rendered/rendering.cpp:135 (all built). So of this TU's fourteen
    //wrappers only LoadIconT and DestroyIcon are reachable from built code; the other twelve are compiled-through, exported
    //and unreachable. U19 (tmp/_tmp_bypass_scan.ps1 over the 17 suffix variants): 16 LAYER / 0 INDIRECT / 7 BYPASS /
    //0 MEMBER, and all 7 BYPASS are in unbuilt targets (5 in ui/gl/windowimpl.cpp:191,:1603,:1608,:1642,:1645 -
    //ui/CMakeLists.txt:4 #add_subdirectory(gl); 2 in examples/.../retropad/controller.cpp:43 + retropad.cpp:940 -
    //osspecific/windows/CMakeLists.txt:9) -> 0 bypass in built code.

    HICON User32::CopyIcon(HICON hIcon)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, CopyIcon);
        return Library::Call<HICON, HICON>(pFunc, hIcon);
    }

    HICON User32::CreateIcon(HINSTANCE hInstance, int nWidth, int nHeight, BYTE cPlanes, BYTE cBitsPixel, const BYTE* lpbANDbits, const BYTE* lpbXORbits)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, CreateIcon);
        return Library::Call<HICON, HINSTANCE, int, int, BYTE, BYTE, const BYTE*, const BYTE*>(pFunc, hInstance, nWidth, nHeight, cPlanes, cBitsPixel, lpbANDbits, lpbXORbits);
    }

    HICON User32::CreateIconFromResource(PBYTE presbits, DWORD dwResSize, BOOL fIcon, DWORD dwVer)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, CreateIconFromResource);
        return Library::Call<HICON, PBYTE, DWORD, BOOL, DWORD>(pFunc, presbits, dwResSize, fIcon, dwVer);
    }

    HICON User32::CreateIconFromResourceEx(PBYTE presbits, DWORD dwResSize, BOOL fIcon, DWORD dwVer, int cxDesired, int cyDesired, UINT Flags)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, CreateIconFromResourceEx);
        return Library::Call<HICON, PBYTE, DWORD, BOOL, DWORD, int, int, UINT>(pFunc, presbits, dwResSize, fIcon, dwVer, cxDesired, cyDesired, Flags);
    }

    HICON User32::CreateIconIndirect(const ::ICONINFO* piconinfo)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, CreateIconIndirect);
        return Library::Call<HICON, const ::ICONINFO*>(pFunc, piconinfo);
    }

    BOOL User32::DestroyIcon(HICON hIcon)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, DestroyIcon);
        return Library::Call<BOOL, HICON>(pFunc, hIcon);
    }

    BOOL User32::DrawIcon(HDC hDC, int X, int Y, HICON hIcon)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, DrawIcon);
        return Library::Call<BOOL, HDC, int, int, HICON>(pFunc, hDC, X, Y, hIcon);
    }

    BOOL User32::DrawIconEx(HDC hdc, int xLeft, int yTop, HICON hIcon, int cxWidth, int cyWidth, UINT istepIfAniCur, HBRUSH hbrFlickerFreeDraw, UINT diFlags)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, DrawIconEx);
        return Library::Call<BOOL, HDC, int, int, HICON, int, int, UINT, HBRUSH, UINT>(pFunc, hdc, xLeft, yTop, hIcon, cxWidth, cyWidth, istepIfAniCur, hbrFlickerFreeDraw, diFlags);
    }

    BOOL User32::GetIconInfo(HICON hIcon, ::PICONINFO piconinfo)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, GetIconInfo);
        return Library::Call<BOOL, HICON, ::PICONINFO>(pFunc, hIcon, piconinfo);
    }

    BOOL User32::GetIconInfoExT(HICON hicon, ::PICONINFOEX piconinfo)
    {
        qor_pp_fcontext;
        qor_pp_useswinapiAW(user32, GetIconInfoEx);
        return Library::Call<BOOL, HICON, ::PICONINFOEX>(pFunc, hicon, piconinfo);
    }

    HICON User32::LoadIconT(HINSTANCE hInstance, LPCTSTR lpIconName)
    {
        qor_pp_fcontext;
        qor_pp_useswinapiAW(user32, LoadIcon);
        return Library::Call<HICON, HINSTANCE, LPCTSTR>(pFunc, hInstance, lpIconName);
    }

    int User32::LookupIconIdFromDirectory(PBYTE presbits, BOOL fIcon)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, LookupIconIdFromDirectory);
        return Library::Call<int, PBYTE, BOOL>(pFunc, presbits, fIcon);
    }

    int User32::LookupIconIdFromDirectoryEx(PBYTE presbits, BOOL fIcon, int cxDesired, int cyDesired, UINT Flags)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, LookupIconIdFromDirectoryEx);
        return Library::Call<int, PBYTE, BOOL, int, int, UINT>(pFunc, presbits, fIcon, cxDesired, cyDesired, Flags);
    }

    UINT User32::PrivateExtractIconsT(LPCTSTR szFileName, int nIconIndex, int cxIcon, int cyIcon, HICON* phicon, UINT* piconid, UINT nIcons, UINT flags)
    {
        qor_pp_fcontext;
        qor_pp_useswinapiAW(user32, PrivateExtractIcons);
        return Library::Call<UINT, LPCTSTR, int, int, int, HICON*, UINT*, UINT, UINT>(pFunc, szFileName, nIconIndex, cxIcon, cyIcon, phicon, piconid, nIcons, flags);
    }

}}}//qor::win::api
