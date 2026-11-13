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
    //U2 pass (12 Nov 2026, LargeTUBatchRegionChecks / LargeTUBatchL1 item 4 "icon sites"): that include is back and this pass is
    //why it is live - the table reads 13 of the 14 rows checked and 1 unchecked (PrivateExtractIcons), so 13 sites build CheckReturn
    //over their Call and the one keeps the SDK value with a written reason at the site. Six rows take CheckNonZeroHandle< HICON >
    //(CopyIcon, CreateIcon, CreateIconFromResource, CreateIconFromResourceEx, CreateIconIndirect, LoadIcon), five take BoolCheck
    //(DestroyIcon, DrawIcon, DrawIconEx, GetIconInfo, GetIconInfoEx) and two CheckNonZero< int > (the LookupIconIdFromDirectory pair);
    //the line above that read "U2 policy: no CheckReturn added here, and U1: no Win32ErrorHandler" is superseded by this pass, and U1
    //has since put Win32ErrorHandler at all fourteen sites. Audit before landing (tmp/_tmp_u2_ic_callers.ps1): CopyIcon has 3 live call
    //sites - ui/gui/gdiobjects/cursor.cpp:91 Cursor::Clone(), ui/gui/gdiobjects/icon.cpp:18 Icon(const Icon&) and icon.cpp:102 Icon::Clone(),
    //each feeding the result straight into a PrimitiveHandle, so a NULL the SDK answered now throws instead of seeding a null handle;
    //the table's 6-caller row also counts the comment mentions of the stem, and the site-level read finds these 3. Census after the pass:
    //CheckReturn-code=13 == table-checked=13, returncheck-includes=1.

    //Return policy (U2, table row CopyIcon / icon.cpp, 12 Nov 2026): checked with checkClass CheckNonZeroHandle< HICON >, and the
    //table's verbatim docQuote is "If the function succeeds, the return value is a handle to the duplicate icon. If the function fails,
    //the return value is NULL" - the duplicate handle is the answer and a created handle is never NULL, so the session-27 soundness test
    //passes at NULL and CheckNonZeroHandle (returncheck.h:137, template< typename THandle > : public AutoErrorCheck< THandle >, Test false
    //only at 0x00000000) fits the HICON row type. Audit: 3 live call sites, ui/gui/gdiobjects/cursor.cpp:91 Cursor::Clone(), the Icon(const
    //Icon&) copy ctor at ui/gui/gdiobjects/icon.cpp:18 and Icon::Clone() at :102, each wrapping the result in a PrimitiveHandle - so a NULL
    //the SDK answered now throws instead of seeding a null handle, the behaviour change D51 accepts for a row that reserves NULL.
    HICON User32::CopyIcon(HICON hIcon)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, CopyIcon);
        CheckReturn< HICON, CheckNonZeroHandle< HICON > >::TType hResult = Library::Call<HICON, HICON>(pFunc, hIcon);
        return hResult;
    }

    //Return policy (U2, table row CreateIcon / icon.cpp, 12 Nov 2026): checked with checkClass CheckNonZeroHandle< HICON >; the table's
    //verbatim docQuote is "If the function succeeds, the return value is a handle to an icon. If the function fails, the return value is
    //NULL" - the icon the caller describes through its arguments is the answer, so the handle is never legitimately NULL and the session-27
    //test passes at NULL; HICON row type, so CheckNonZeroHandle (returncheck.h:137) covers it. Audit: 1 live caller, ui/gui/gdiobjects/icon.cpp:24
    //in the Icon(void*,...) ctor, which assigns it to m_handle and marks the icon owned - a NULL now throws rather than leaving an owning null handle.
    HICON User32::CreateIcon(HINSTANCE hInstance, int nWidth, int nHeight, BYTE cPlanes, BYTE cBitsPixel, const BYTE* lpbANDbits, const BYTE* lpbXORbits)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, CreateIcon);
        CheckReturn< HICON, CheckNonZeroHandle< HICON > >::TType hResult = Library::Call<HICON, HINSTANCE, int, int, BYTE, BYTE, const BYTE*, const BYTE*>(pFunc, hInstance, nWidth, nHeight, cPlanes, cBitsPixel, lpbANDbits, lpbXORbits);
        return hResult;
    }

    //Return policy (U2, table row CreateIconFromResource / icon.cpp, 12 Nov 2026): checked with checkClass CheckNonZeroHandle< HICON > -
    //the table's verbatim docQuote is "If the function succeeds, the return value is a handle to the icon or cursor. If the function fails,
    //the return value is NULL", and the resource the caller passes in presbits/dwResSize is the answer's subject, so the created handle is
    //never legitimately NULL and the session-27 test passes at NULL; HICON row type so CheckNonZeroHandle (returncheck.h:137) covers it.
    //Audit: 1 live caller, ui/gui/gdiobjects/icon.cpp:30 in the Icon(byte*,...) ctor, which stores it as an owning handle.
    HICON User32::CreateIconFromResource(PBYTE presbits, DWORD dwResSize, BOOL fIcon, DWORD dwVer)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, CreateIconFromResource);
        CheckReturn< HICON, CheckNonZeroHandle< HICON > >::TType hResult = Library::Call<HICON, PBYTE, DWORD, BOOL, DWORD>(pFunc, presbits, dwResSize, fIcon, dwVer);
        return hResult;
    }

    //Return policy (U2, table row CreateIconFromResourceEx / icon.cpp, 12 Nov 2026): checked with checkClass CheckNonZeroHandle< HICON > -
    //the table's verbatim docQuote is the non-Ex twin's "If the function fails, the return value is NULL", so the created handle is reserved
    //against NULL and the size/version selection lives in the arguments; HICON row type so CheckNonZeroHandle (returncheck.h:137) covers it.
    //Audit: 1 live caller, ui/gui/gdiobjects/icon.cpp:36 in the sized Icon(byte*,...) ctor, which stores it as an owning handle.
    HICON User32::CreateIconFromResourceEx(PBYTE presbits, DWORD dwResSize, BOOL fIcon, DWORD dwVer, int cxDesired, int cyDesired, UINT Flags)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, CreateIconFromResourceEx);
        CheckReturn< HICON, CheckNonZeroHandle< HICON > >::TType hResult = Library::Call<HICON, PBYTE, DWORD, BOOL, DWORD, int, int, UINT>(pFunc, presbits, dwResSize, fIcon, dwVer, cxDesired, cyDesired, Flags);
        return hResult;
    }

    //Return policy (U2, table row CreateIconIndirect / icon.cpp, 12 Nov 2026): checked with checkClass CheckNonZeroHandle< HICON > -
    //the table's verbatim docQuote is "If the function succeeds, the return value is a handle to the icon or cursor that is created. If the
    //function fails, the return value is NULL . To get extended error information, call GetLastError ." and the ICONINFO the caller describes
    //is an argument, so the created handle is never legitimately NULL and the session-27 test passes at NULL; HICON row type so CheckNonZeroHandle
    //(returncheck.h:137) covers it. Audit: 1 live call site, ui/gui/gdiobjects/icon.cpp:42 in the Icon(IconInfo&) ctor, which assigns it to
    //m_handle and sets m_owning - a NULL the SDK answered now throws instead of leaving an owning null handle that ~Icon would DestroyIcon.
    HICON User32::CreateIconIndirect(const ::ICONINFO* piconinfo)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, CreateIconIndirect);
        CheckReturn< HICON, CheckNonZeroHandle< HICON > >::TType hResult = Library::Call<HICON, const ::ICONINFO*>(pFunc, piconinfo);
        return hResult;
    }

    //Return policy (U2, table row DestroyIcon / icon.cpp, 12 Nov 2026): checked with checkClass BoolCheck; the table's verbatim docQuote is the
    //two-line status "If the function succeeds, the return value is nonzero. If the function fails, the return value is zero. To get extended
    //error information, call GetLastError ." - the icon being destroyed is an argument and the call answers nothing besides status, so zero is
    //reserved; sdkRetType BOOL so BoolCheck (returncheck.h:94) covers it. Audit: 1 live call site, ui/gui/gdiobjects/icon.cpp:91 in ~Icon, where
    //the value is discarded today - a failed destroy now raises continuable("A checked parameter failed a test") (returncheck.h:83) from inside a
    //destructor, so the hazard this site carries is an exception unwinding through ~Icon rather than a silently swallowed failure; recorded here
    //because the row is checked and no class fits a failed destroy any more tightly.
    BOOL User32::DestroyIcon(HICON hIcon)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, DestroyIcon);
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call<BOOL, HICON>(pFunc, hIcon);
        return bResult;
    }

    //Return policy (U2, table row DrawIcon / icon.cpp, 12 Nov 2026): checked with checkClass BoolCheck; the table's verbatim docQuote is the
    //two-line status "If the function succeeds, the return value is nonzero. If the function fails, the return value is zero. To get extended
    //error information, call GetLastError ." - the icon and the device context are arguments, so zero is reserved and the BOOL answers nothing
    //of its own; sdkRetType BOOL so BoolCheck (returncheck.h:94) covers it. Audit: 1 live call site, ui/gui/gdiobjects/icon.cpp:108 in Icon::Draw,
    //which folds the value with "? true : false" - a failed draw now raises instead of returning false from a method whose callers read false as "not drawn".
    BOOL User32::DrawIcon(HDC hDC, int X, int Y, HICON hIcon)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, DrawIcon);
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call<BOOL, HDC, int, int, HICON>(pFunc, hDC, X, Y, hIcon);
        return bResult;
    }

    //Return policy (U2, table row DrawIconEx / icon.cpp, 12 Nov 2026): checked with checkClass BoolCheck; the table's verbatim docQuote is the
    //non-Ex twin's wording line for line, "If the function succeeds, the return value is nonzero. If the function fails, the return value is zero.
    //To get extended error information, call GetLastError ." - the icon, the device context and the geometry are all arguments, so zero is reserved
    //and the BOOL carries no answer; the row's sdkRetType is NO-PROTOTYPE and the binding is the bare stem, so the type comes from the published
    //BOOL - and BoolCheck is unparameterised (returncheck.h:94, class BoolCheck : public AutoErrorCheck< BOOL >), so no class base is picked.
    //Audit: 1 live call site, ui/gui/gdiobjects/icon.cpp:113 in Icon::DrawEx, which folds the value with "? true : false".
    BOOL User32::DrawIconEx(HDC hdc, int xLeft, int yTop, HICON hIcon, int cxWidth, int cyWidth, UINT istepIfAniCur, HBRUSH hbrFlickerFreeDraw, UINT diFlags)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, DrawIconEx);
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call<BOOL, HDC, int, int, HICON, int, int, UINT, HBRUSH, UINT>(pFunc, hdc, xLeft, yTop, hIcon, cxWidth, cyWidth, istepIfAniCur, hbrFlickerFreeDraw, diFlags);
        return bResult;
    }

    //Return policy (U2, table row GetIconInfo / icon.cpp, 12 Nov 2026): checked with checkClass BoolCheck; the table's verbatim docQuote is
    //the status line that names its own out-parameter, "If the function succeeds, the return value is nonzero and the function fills in the
    //members of the specified ICONINFO structure. If the function fails, the return value is zero. To get extended error information, call
    //GetLastError ." - the information the caller asks for travels in the PICONINFO it passes, so the BOOL answers nothing of its own and zero
    //is reserved; sdkRetType BOOL so BoolCheck (returncheck.h:94) covers it. Audit: 1 live call site, ui/gui/gdiobjects/icon.cpp:118 in Icon::GetInfo,
    //which folds the value with "? true : false" - a failed query now raises instead of leaving the caller's IconInfo half-filled and reported false.
    BOOL User32::GetIconInfo(HICON hIcon, ::PICONINFO piconinfo)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, GetIconInfo);
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call<BOOL, HICON, ::PICONINFO>(pFunc, hIcon, piconinfo);
        return bResult;
    }

    //Return policy (U2, table row GetIconInfoEx / icon.cpp, 12 Nov 2026): checked with checkClass BoolCheck; the table's verbatim docQuote is the
    //whole Return value section on the corrected a-slug page nf-winuser-geticoninfoexa, "Type: BOOL TRUE indicates success, FALSE indicates failure."
    // - status only, with the ICONINFOEX the caller passes as an out-parameter, so the session-27 test passes at FALSE; the row's sdkRetType is
    //NO-PROTOTYPE and this is an AW binding, so the type comes from the published BOOL and BoolCheck's unparameterised base (returncheck.h:94) is used
    //rather than picked. Audit: the table reads 0 in-tree callers (change-safe) for the bare stem; the site-level read also finds the member spelling
    //User32::GetIconInfoExT called at ui/gui/gdiobjects/icon.cpp:123 in Icon::GetInfoExT, which folds the value with "? true : false".
    BOOL User32::GetIconInfoExT(HICON hicon, ::PICONINFOEX piconinfo)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapiAW(user32, GetIconInfoEx);
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call<BOOL, HICON, ::PICONINFOEX>(pFunc, hicon, piconinfo);
        return bResult;
    }

    //Return policy (U2, table row LoadIcon / icon.cpp, 12 Nov 2026): checked with checkClass CheckNonZeroHandle< HICON >; the table's verbatim docQuote
    //fetched off the corrected a-slug page nf-winuser-loadicona is "If the function succeeds, the return value is a handle to the newly loaded icon. If
    //the function fails, the return value is NULL . To get extended error information, call GetLastError ." - the icon the caller names is an argument, so
    //the loaded handle is never legitimately NULL and the session-27 test passes at NULL; the row's sdkRetType is NO-PROTOTYPE and this is an AW binding,
    //so the class base is the published HICON, which CheckNonZeroHandle (returncheck.h:137, template< typename THandle >) covers. Audit: the table reads 0
    //in-tree callers for the bare stem (change-safe); the site-level read finds the member spelling User32::LoadIconT live at ui/gui/gdiobjects/icon.cpp:48
    //(Icon(void*,std::string)) and :54 (Icon(const wchar_t*)), and the header note above records that windowfactory.cpp:92's Icon(nullptr) reaches :52-56,
    //where the SDK answers NULL with GetLastError 1813 (ERROR_RESOURCE_NAME_NOT_FOUND) - so this token turns that documented "no small icon" path into a
    //throw, the behaviour change D51 accepts for a row that reserves NULL, and the caller-side misuse the U20 / D47 note already records.
    HICON User32::LoadIconT(HINSTANCE hInstance, LPCTSTR lpIconName)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapiAW(user32, LoadIcon);
        CheckReturn< HICON, CheckNonZeroHandle< HICON > >::TType hResult = Library::Call<HICON, HINSTANCE, LPCTSTR>(pFunc, hInstance, lpIconName);
        return hResult;
    }

    //Return policy (U2, table row LookupIconIdFromDirectory / icon.cpp, 12 Nov 2026): checked with checkClass CheckNonZero< int >; the table's verbatim
    //docQuote is "If the function succeeds, the return value is an integer resource identifier for the icon ( RT_ICON ) or cursor ( RT_CURSOR ) that best
    //fits the current display device. If the function fails, the return value is zero. To get extended error information, call GetLastError ." - the answer
    //is a resource identifier and 0 is reserved for failure, so the session-27 test passes at every value; sdkRetType int matches the published int, so the
    //base is int and CheckNonZero (returncheck.h:187, template< typename TNumeric > : public AutoErrorCheck< TNumeric >) covers it. Audit: 1 live call site,
    //ui/gui/gdiobjects/icon.cpp:128 in Icon::LookupIdFromDirectory, whose return type is int and whose callers read 0 as "no match" today.
    int User32::LookupIconIdFromDirectory(PBYTE presbits, BOOL fIcon)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, LookupIconIdFromDirectory);
        CheckReturn< int, CheckNonZero< int > >::TType iResult = Library::Call<int, PBYTE, BOOL>(pFunc, presbits, fIcon);
        return iResult;
    }

    //Return policy (U2, table row LookupIconIdFromDirectoryEx / icon.cpp, 12 Nov 2026): checked with checkClass CheckNonZero< int >; the table's verbatim
    //docQuote is the LookupIconIdFromDirectory row's line for line - "If the function succeeds, the return value is an integer resource identifier for the icon ( RT_ICON ) or cursor ( RT_CURSOR ) that best
    //fits the current display device. If the function fails, the return value is zero. To get extended error information, call GetLastError ." - so the answer
    //is a resource identifier and 0 is reserved for failure, and the session-27 test passes at every value; sdkRetType int matches the published int, so the
    //base is int and CheckNonZero (returncheck.h:187, template< typename TNumeric > : public AutoErrorCheck< TNumeric >) covers it - the same class the sibling
    //row above takes. Audit: 1 live call site, ui/gui/gdiobjects/icon.cpp:133 in Icon::LookupIdFromDirectoryEx, whose return type is int (declared icon.h:99)
    //and which has no further in-tree caller of that member, so the landed check changes no other observed call-site reading; the member's int return is preserved.
    int User32::LookupIconIdFromDirectoryEx(PBYTE presbits, BOOL fIcon, int cxDesired, int cyDesired, UINT Flags)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, LookupIconIdFromDirectoryEx);
        CheckReturn< int, CheckNonZero< int > >::TType iResult = Library::Call<int, PBYTE, BOOL, int, int, UINT>(pFunc, presbits, fIcon, cxDesired, cyDesired, Flags);
        return iResult;
    }

    UINT User32::PrivateExtractIconsT(LPCTSTR szFileName, int nIconIndex, int cxIcon, int cyIcon, HICON* phicon, UINT* piconid, UINT nIcons, UINT flags)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapiAW(user32, PrivateExtractIcons);
        return Library::Call<UINT, LPCTSTR, int, int, int, HICON*, UINT*, UINT, UINT>(pFunc, szFileName, nIconIndex, cxIcon, cyIcon, phicon, piconid, nIcons, flags);
    }

}}}//qor::win::api
