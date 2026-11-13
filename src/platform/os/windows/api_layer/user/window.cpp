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

    //Return policy (U2, table row AdjustWindowRect / window.cpp, 12 Nov 2026): the row reads checked with checkClass
    //BoolCheck, and the table's verbatim docQuote is "If the function succeeds, the return value is nonzero. If the
    //function fails, the return value is zero. To get extended error information, call GetLastError ." - zero is reserved
    //and the RECT the call adjusts is an in/out parameter, so the BOOL carries no answer of its own and the session-27
    //soundness test passes at every value; sdkRetType BOOL so the base is BOOL and BoolCheck (returncheck.h:94, Test at
    //:101 is value ? true : false) is the class the library already has. 0 in-tree callers for the exact stem (D14
    //published surface); G6 keeps the check live on every call.
    BOOL User32::AdjustWindowRect(::LPRECT lpRect, DWORD dwStyle, BOOL bMenu)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, AdjustWindowRect);
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call< BOOL, ::LPRECT, DWORD, BOOL >(pFunc, lpRect, dwStyle, bMenu);
        return bResult;
    }

    //Return policy (U2, table row AdjustWindowRectEx / window.cpp, 12 Nov 2026): the row reads checked with checkClass
    //BoolCheck, and the docQuote is word for word the AdjustWindowRect row's - "If the function succeeds, the return value
    //is nonzero. If the function fails, the return value is zero. To get extended error information, call GetLastError ."
    //- so the same reading applies: status BOOL with the adjusted RECT in the parameter, zero reserved, the BOOL carrying
    //no answer; sdkRetType BOOL so BoolCheck (returncheck.h:94) covers it. 1 in-tree caller, audited before landing:
    //ui/gui/controllers/windowcontroller.cpp:62 spells it "return User32::AdjustWindowRectEx(...) ? true : false;", so
    //the caller already tests the result and the landed check leaves the returned value identical, adding only the
    //failure diagnostic; G6 keeps it live on every call.
    BOOL User32::AdjustWindowRectEx(::LPRECT lpRect, DWORD dwStyle, BOOL bMenu, DWORD dwExStyle)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, AdjustWindowRectEx);
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call< BOOL, ::LPRECT, DWORD, BOOL, DWORD >(pFunc, lpRect, dwStyle, bMenu, dwExStyle);
        return bResult;
    }

    //Return policy (U2, table row AdjustWindowRectExForDpi / window.cpp, 12 Nov 2026): the row reads checked with checkClass
    //BoolCheck, and the docQuote is the AdjustWindowRect row's status wording with the page's own "Type:" line absent -
    //"If the function succeeds, the return value is nonzero. If the function fails, the return value is zero. To get
    //extended error information, call GetLastError ." - so zero is reserved and the RECT is a parameter; sdkRetType BOOL
    //so BoolCheck (returncheck.h:94) covers it. 1 in-tree caller, audited before landing: windowcontroller.cpp:67 spells
    //it "return User32::AdjustWindowRectExForDpi(...) ? true : false;", already tested, so the check leaves the value
    //identical; G6 keeps it live. (The member sits under WINVER >= 0x0605.)
    BOOL User32::AdjustWindowRectExForDpi(::LPRECT lpRect, DWORD dwStyle, BOOL bMenu, DWORD dwExStyle, UINT dpi)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, AdjustWindowRectExForDpi);
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call< BOOL, ::LPRECT, DWORD, BOOL, DWORD, UINT >(pFunc, lpRect, dwStyle, bMenu, dwExStyle, dpi);
        return bResult;
    }

    //Return policy (U2, table row AllowSetForegroundWindow / window.cpp, 12 Nov 2026): the row reads checked with checkClass
    //BoolCheck, and the docQuote names its own failure condition - "If the function succeeds, the return value is nonzero.
    //If the function fails, the return value is zero. The function will fail if the calling process cannot set the
    //foreground window. To get extended error information, call GetLastError ." - the process whose foreground is being
    //allowed is the dwProcessId parameter (ASFW_ANY or a process id), so the BOOL carries no answer and zero is reserved;
    //sdkRetType BOOL so BoolCheck (returncheck.h:94) covers it. 0 in-tree callers for the exact stem (D14 published
    //surface); G6 keeps the check live on every call.
    BOOL User32::AllowSetForegroundWindow(DWORD dwProcessId)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, AllowSetForegroundWindow);
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call< BOOL, DWORD >(pFunc, dwProcessId);
        return bResult;
    }

    //Return policy (U2, table row AnimateWindow / window.cpp, 12 Nov 2026): the row reads checked with checkClass BoolCheck
    //against this leaf's expectation that AnimateWindow was answer-shaped and unchecked - the page enumerates the failure
    //states rather than legal zeros, so D51 step 2 reads a reserved zero: "If the function succeeds, the return value is
    //nonzero. If the function fails, the return value is zero. The function will fail in the following situations: If the
    //window is already visible and you are trying to show the window. If the window is already hidden and you are trying to
    //hide the window. If there is no direction specified for the slide or roll animation. When trying to animate a child
    //window with AW_BLEND . If the thread does not own the window. Note that, in this case, AnimateWindow fails but
    //GetLastError returns ERROR_SUCCESS . To get extended error information, call the GetLastError function." - the window
    //and the animation flags are parameters, so the BOOL carries no answer. The caveat is load-bearing for the diagnostic:
    //when the owning-thread case fails, GetLastError reads ERROR_SUCCESS, so the check cannot fall back on the error code
    //and reports only the reserved zero. sdkRetType BOOL so BoolCheck (returncheck.h:94) covers it. 1 in-tree caller,
    //audited before landing: windowcontroller.cpp:92 spells it "return User32::AnimateWindow(...) ? true : false;",
    //already tested, so the check leaves the returned value identical; G6 keeps it live.
    BOOL User32::AnimateWindow(HWND hWnd, DWORD dwTime, DWORD dwFlags)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, AnimateWindow);
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call< BOOL, HWND, DWORD, DWORD >(pFunc, hWnd, dwTime, dwFlags);
        return bResult;
    }

    BOOL User32::AnyPopup(void)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, AnyPopup);
        return Library::Call<BOOL>(pFunc);
    }

    //Return policy (U2, table row ArrangeIconicWindows / window.cpp, 12 Nov 2026): the row reads checked with checkClass
    //CheckNonZero< UINT >, and the docQuote reserves zero - "If the function succeeds, the return value is the height of
    //one row of icons. If the function fails, the return value is zero. To get extended error information, call
    //GetLastError ." - the window being arranged is the hWnd parameter, so the UINT carries no answer and a row of icons
    //has a nonzero height, so the session-27 soundness test passes at every value; sdkRetType UINT so the base is UINT
    //and CheckNonZero (returncheck.h:187) covers it. 1 in-tree caller, audited before landing: windowcontroller.cpp:96
    //spells it "return User32::ArrangeIconicWindows(WINDOW_HANDLE);", so the controller passes the height straight through
    //without testing it; the landed check leaves that value identical and only adds the diagnostic, and G6 keeps it live.
    UINT User32::ArrangeIconicWindows(HWND hWnd)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, ArrangeIconicWindows);
        CheckReturn< UINT, CheckNonZero< UINT > >::TType uResult = Library::Call< UINT, HWND >(pFunc, hWnd);
        return uResult;
    }

    //Return policy (U2, table row BeginDeferWindowPos / window.cpp, 12 Nov 2026): the row reads checked with checkClass
    //CheckNonZeroHandle< HDWP >, and the docQuote reserves NULL - "If the function succeeds, the return value identifies
    //the multiple-window-position structure. If insufficient system resources are available to allocate the structure,
    //the return value is NULL . To get extended error information, call GetLastError ." - a resource-exhaustion NULL is
    //a fault the caller must not carry into the DeferWindowPos / EndDeferWindowPos chain this handle feeds, so the
    //soundness test passes; the row's type is HDWP, so HandleCheck (returncheck.h:112, keys on (HANDLE)(-1)) does not
    //fit and HandleNullCheck (:124) has a fixed HANDLE base that mismatches, leaving CheckNonZeroHandle< HDWP >
    //(returncheck.h:137) as the class whose base equals the row's sdkRetType HDWP. 0 in-tree callers (change-safe).
    HDWP User32::BeginDeferWindowPos(int nNumWindows)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, BeginDeferWindowPos);
        CheckReturn< HDWP, CheckNonZeroHandle< HDWP > >::TType hResult = Library::Call< HDWP, int >(pFunc, nNumWindows);
        return hResult;
    }

    //Return policy (U2, table row BringWindowToTop / window.cpp, 12 Nov 2026): the row reads checked with checkClass BoolCheck,
    //and the docQuote is the AdjustWindowRect row's wording word for word - "If the function succeeds, the return value is
    //nonzero. If the function fails, the return value is zero. To get extended error information, call GetLastError ." -
    //the window being raised is the hWnd parameter, so the BOOL carries no answer and zero is reserved; sdkRetType BOOL
    //so BoolCheck (returncheck.h:94) covers it. 1 in-tree caller, audited before landing: windowcontroller.cpp:102 spells
    //it "return User32::BringWindowToTop(...) ? true : false;", already tested, so the check leaves the value identical;
    //G6 keeps it live.
    BOOL User32::BringWindowToTop(HWND hWnd)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, BringWindowToTop);
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call< BOOL, HWND >(pFunc, hWnd);
        return bResult;
    }

    WORD User32::CascadeChildWindows(HWND hWndParent, WORD wFlags)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, CascadeChildWindows);
        return Library::Call<WORD, HWND, WORD>(pFunc, hWndParent, wFlags);
    }

    //Return policy (U2, table row CascadeWindows / window.cpp, 12 Nov 2026): the row reads checked with checkClass
    //CheckNonZero< WORD >, and the docQuote reserves zero - "If the function succeeds, the return value is the number of
    //windows arranged. If the function fails, the return value is zero. To get extended error information, call
    //GetLastError ." - the page offers zero only as the failure value and never as an answer, so the count the caller
    //gets is either positive or a fault; the row is an AW row with sdkRetType NO-PROTOTYPE, so the audit's rule makes
    //the class base the published WORD and CheckNonZero< WORD > (returncheck.h:187) covers it - deliberately unlike the
    //GetWindowModuleFileName row below, whose page documents no failure value at all. 1 in-tree caller, audited before
    //landing: windowcontroller.cpp:118 spells it "return User32::CascadeWindows(WINDOW_HANDLE, how, (const ::RECT*)(&rect),
    //countKids, vecKids.data());", so the controller passes the arranged count straight through without testing it; the
    //landed check leaves that value identical and only adds the diagnostic, and G6 keeps it live.
    WORD User32::CascadeWindows(HWND hwndParent, UINT wHow, const ::RECT* lpRect, UINT cKids, const HWND* lpKids)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, CascadeWindows);
        CheckReturn< WORD, CheckNonZero< WORD > >::TType wResult = Library::Call< WORD, HWND, UINT, const ::RECT*, UINT, const HWND* >(pFunc, hwndParent, wHow, lpRect, cKids, lpKids);
        return wResult;
    }

    //Return policy (U2, table row ChangeWindowMessageFilter / window.cpp, 12 Nov 2026): the row reads checked with checkClass
    //BoolCheck, and the docQuote is the word-for-word twin of the ChangeWindowMessageFilterEx row in misc.cpp, booked
    //checked on the same wording - "TRUE if successful; otherwise, FALSE . To get extended error information, call
    //GetLastError . Note A message can be successfully removed from the filter, but that is not a guarantee that the
    //message will be blocked." - the note qualifies what a *success* means, not what zero means, so zero stays reserved
    //and the message plus its filter flag are parameters; sdkRetType BOOL so BoolCheck (returncheck.h:94) covers it. 0
    //in-tree callers for the exact stem (D14 published surface); G6 keeps the check live on every call.
    BOOL User32::ChangeWindowMessageFilter(UINT message, DWORD dwFlag)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, ChangeWindowMessageFilter);
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call< BOOL, UINT, DWORD >(pFunc, message, dwFlag);
        return bResult;
    }

    HWND User32::ChildWindowFromPoint(HWND hWndParent, ::POINT Point)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, ChildWindowFromPoint);
        return Library::Call<HWND, HWND, ::POINT>(pFunc, hWndParent, Point);
    }

    HWND User32::ChildWindowFromPointEx(HWND hwnd, ::POINT pt, UINT flags)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, ChildWindowFromPointEx);
        return Library::Call<HWND, HWND, ::POINT, UINT>(pFunc, hwnd, pt, flags);
    }

    //Return policy (U2, table row CloseWindow / window.cpp, 12 Nov 2026): the row reads checked with checkClass BoolCheck,
    //and the docQuote is the AdjustWindowRect row's wording word for word - "If the function succeeds, the return value is
    //nonzero. If the function fails, the return value is zero. To get extended error information, call GetLastError ." -
    //the window being closed is the hWnd parameter, so the BOOL carries no answer and zero is reserved; sdkRetType BOOL
    //so BoolCheck (returncheck.h:94) covers it. 1 in-tree caller, audited before landing: windowcontroller.cpp:142 spells
    //it "return User32::CloseWindow(...) ? true : false;", already tested, so the check leaves the value identical; G6
    //keeps it live. (Not the shell-hook CloseWindow in desktop.cpp's window-station family - different stem.)
    BOOL User32::CloseWindow(HWND hWnd)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, CloseWindow);
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call< BOOL, HWND >(pFunc, hWnd);
        return bResult;
    }

    //P3 CLOSED 12 Nov 2026: the CreateWindowT member is deleted, not rebound - CreateWindow/A/W are winuser.h function-like
    //macros with 0 export rows and CreateWindowExT above already wraps the export they forward to, so the twin had no caller
    //and no export to mirror.
    //Return policy (U2, table row CreateWindowEx / window.cpp, 12 Nov 2026): the row reads checked with checkClass
    //CheckNonZeroHandle< HWND > on the page's own status pair plus its failure list - "If the function succeeds, the return
    //value is a handle to the new window. If the function fails, the return value is NULL . To get extended error
    //information, call GetLastError . This function typically fails for one of the following reasons: an invalid parameter
    //value / the system class was registered by a different module / The WH_CBT hook is installed and returns a failure code" -
    //every one of those is an argument or hook fault rather than a legal window state, so a NULL handle is never a legitimate
    //answer here and the session-27 soundness test passes at every value. The row is an AW row with sdkRetType NO-PROTOTYPE,
    //so the class base is the published HWND; CheckNonZeroHandle< HWND > (returncheck.h:137, Test fails at 0x00000000 only)
    //is the class already booked for the CreateDialogParam / CreateDialogIndirectParam / CreateMDIWindow rows, all of which
    //also create a window - no new check class. 0 in-tree callers for the exact stem (change-safe); G6 keeps it live.
    HWND User32::CreateWindowExT(DWORD dwExStyle, LPCTSTR lpClassName, LPCTSTR lpWindowName, DWORD dwStyle, int X, int Y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance, LPVOID lpParam)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapiAW(user32, CreateWindowEx);
        CheckReturn< HWND, CheckNonZeroHandle< HWND > >::TType hResult = Library::Call< HWND, DWORD, LPCTSTR, LPCTSTR, DWORD, int, int, int, int, HWND, HMENU, HINSTANCE, LPVOID >(
            pFunc, dwExStyle, lpClassName, lpWindowName, dwStyle, X, Y, nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam);
        return hResult;
    }

    //Return policy (U2, table row DeferWindowPos / window.cpp, 12 Nov 2026): the row reads checked with checkClass
    //CheckNonZeroHandle< HDWP >, and the docQuote is the BeginDeferWindowPos row's HDWP chain with the handle-swapping
    //note - "The handle returned by this function may differ from the handle passed to the function. The new handle that
    //this function returns should be passed during the next call to the DeferWindowPos or EndDeferWindowPos function.
    //If insufficient system resources are available for the function to succeed, the return value is NULL ." - NULL
    //reserved for a resource fault and never a legal answer, so the soundness test passes; sdkRetType HDWP so the class
    //base is the SDK HDWP. 0 in-tree callers (change-safe).
    HDWP User32::DeferWindowPos(HDWP hWinPosInfo, HWND hWnd, HWND hWndInsertAfter, int x, int y, int cx, int cy, UINT uFlags)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, DeferWindowPos);
        CheckReturn< HDWP, CheckNonZeroHandle< HDWP > >::TType hResult = Library::Call< HDWP, HDWP, HWND, HWND, int, int, int, int, UINT >(
            pFunc, hWinPosInfo, hWnd, hWndInsertAfter, x, y, cx, cy, uFlags);
        return hResult;
    }

    //Return policy (U2, table row DeregisterShellHookWindow / window.cpp, 12 Nov 2026): the row reads checked with checkClass
    //BoolCheck on the page's own terse wording - "TRUE if the function succeeds; FALSE if the function fails." - the window
    //being deregistered is the hwnd parameter, so the BOOL carries no answer and FALSE is reserved; deliberately unlike the
    //RegisterForTooltipDismissNotification row in misc.cpp, whose page puts FALSE at legal already-registered / already-
    //unregistered states in its Remarks, this page names no such legal FALSE; sdkRetType BOOL so BoolCheck (returncheck.h:94)
    //covers it. 1 in-tree caller, audited before landing: windowcontroller.cpp:147 spells it "return User32::DeregisterShellHookWindow(...)
    //? true : false;", already tested, so the check leaves the value identical; G6 keeps it live. The shell-hook pair's
    //placement note lives in hook.cpp:76 - nothing moved here.
    BOOL User32::DeregisterShellHookWindow(HWND hwnd)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, DeregisterShellHookWindow);
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call< BOOL, HWND >(pFunc, hwnd);
        return bResult;
    }

    //Return policy (U2, table row DestroyWindow / window.cpp, 12 Nov 2026): the row reads checked with checkClass BoolCheck,
    //and the docQuote is the AdjustWindowRect row's wording word for word - "If the function succeeds, the return value is
    //nonzero. If the function fails, the return value is zero. To get extended error information, call GetLastError ." -
    //the window being destroyed is the hWnd parameter, so the BOOL carries no answer and zero is reserved; sdkRetType BOOL
    //so BoolCheck (returncheck.h:94) covers it. 1 in-tree caller, audited before landing: windowcontroller.cpp:858 spells
    //it "bool result = User32::DestroyWindow(WINDOW_HANDLE) ? true : false;", already tested, so the check leaves the value
    //identical and adds only the failure diagnostic; G6 keeps it live.
    BOOL User32::DestroyWindow(HWND hWnd)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, DestroyWindow);
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call< BOOL, HWND >(pFunc, hWnd);
        return bResult;
    }

    //Return policy (U2, table row EndDeferWindowPos / window.cpp, 12 Nov 2026): the row reads checked with checkClass BoolCheck,
    //and the docQuote is the AdjustWindowRect row's wording word for word - "If the function succeeds, the return value is
    //nonzero. If the function fails, the return value is zero. To get extended error information, call GetLastError ." -
    //the deferred window position is the HDWP the caller built with BeginDeferWindowPos, passed in, so the BOOL carries no
    //answer and zero is reserved; sdkRetType BOOL so BoolCheck (returncheck.h:94) covers it. 0 in-tree callers for the exact
    //stem (change-safe); G6 keeps the check live on every call.
    BOOL User32::EndDeferWindowPos(HDWP hWinPosInfo)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, EndDeferWindowPos);
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call< BOOL, HDWP >(pFunc, hWinPosInfo);
        return bResult;
    }

    //Return policy (U2, table row EndTask / window.cpp, 12 Nov 2026): the row reads checked with checkClass BoolCheck, and
    //the docQuote is the AdjustWindowRect row's status wording in the page's own FALSE spelling - "If the function succeeds,
    //the return value is nonzero. If the function fails, the return value is FALSE . To get extended error information, call
    //GetLastError ." - the task being ended is a parameter, so the BOOL carries no answer and FALSE is reserved; sdkRetType
    //BOOL so BoolCheck (returncheck.h:94) covers it. 1 in-tree caller, audited before landing: windowcontroller.cpp:152 spells
    //it "return User32::EndTask(WINDOW_HANDLE, shutDown, force) ? true : false;", already tested, so the check leaves the
    //value identical; G6 keeps it live.
    BOOL User32::EndTask(HWND hWnd, BOOL fShutDown, BOOL fForce)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, EndTask);
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call< BOOL, HWND, BOOL, BOOL >(pFunc, hWnd, fShutDown, fForce);
        return bResult;
    }

    BOOL User32::EnumChildWindows(HWND hWndParent, WNDENUMPROC lpEnumFunc, LPARAM lParam)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, EnumChildWindows);
        return Library::Call<BOOL, HWND, WNDENUMPROC, LPARAM>(pFunc, hWndParent, lpEnumFunc, lParam);
    }

    BOOL User32::EnumThreadWindows(DWORD dwThreadId, WNDENUMPROC lpfn, LPARAM lParam)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, EnumThreadWindows);
        return Library::Call<BOOL, DWORD, WNDENUMPROC, LPARAM>(pFunc, dwThreadId, lpfn, lParam);
    }

    BOOL User32::EnumWindows(WNDENUMPROC lpEnumFunc, LPARAM lParam)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, EnumWindows);
        return Library::Call<BOOL, WNDENUMPROC, LPARAM >(pFunc, lpEnumFunc, lParam);
    }

    //Return policy (U2, table row FindWindow / window.cpp, 12 Nov 2026): the row reads checked with checkClass
    //CheckNonZeroHandle< HWND > on the page's own status pair - "If the function succeeds, the return value is a handle to
    //the window that has the specified class name and window name. If the function fails, the return value is NULL . This
    //function does not modify the last error value." - NULL is reserved and a found window's handle is never legitimately
    //NULL, so the session-27 soundness test passes; the last sentence is load-bearing for the notes rather than the verdict,
    //since a landed check here cannot lean on GetLastError and the check itself is the only error signal this site gets.
    //The row is an AW row with sdkRetType NO-PROTOTYPE, so the base is the published HWND. 0 in-tree callers for the exact
    //stem (change-safe); G6 keeps the check live on every call.
    HWND User32::FindWindowT(LPCTSTR lpClassName, LPCTSTR lpWindowName)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapiAW(user32, FindWindow);
        CheckReturn< HWND, CheckNonZeroHandle< HWND > >::TType hResult = Library::Call< HWND, LPCTSTR, LPCTSTR>(pFunc, lpClassName, lpWindowName);
        return hResult;
    }

    //Return policy (U2, table row FindWindowEx / window.cpp, 12 Nov 2026): the row reads checked with checkClass
    //CheckNonZeroHandle< HWND > on the FindWindow row's status pair with the extended-error route restored - "If the function
    //succeeds, the return value is a handle to the window that has the specified class and window names. If the function
    //fails, the return value is NULL . To get extended error information, call GetLastError ." - NULL reserved and the
    //handle is the window the caller searched for, so the soundness test passes at every value; deliberately unlike the
    //ChildWindowFromPoint row in this TU, whose NULL is the documented answer for a point outside the parent rather than a
    //failed lookup. AW row with sdkRetType NO-PROTOTYPE, so the base is the published HWND. 0 in-tree callers (change-safe).
    HWND User32::FindWindowExT(HWND hWndParent, HWND hWndChildAfter, LPCTSTR lpszClass, LPCTSTR lpszWindow)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapiAW(user32, FindWindowEx);
        CheckReturn< HWND, CheckNonZeroHandle< HWND > >::TType hResult = Library::Call< HWND, HWND, HWND, LPCTSTR, LPCTSTR>(pFunc, hWndParent, hWndChildAfter, lpszClass, lpszWindow);
        return hResult;
    }

    //Return policy (U2, table row GetAltTabInfo / window.cpp, 12 Nov 2026): the row reads checked with checkClass BoolCheck,
    //and the docQuote is the AdjustWindowRect row's wording word for word - "If the function succeeds, the return value is
    //nonzero. If the function fails, the return value is zero. To get extended error information, call GetLastError ." -
    //the ALTTABINFO the caller fills is an out-parameter and the item text goes through pszItemText, so the BOOL carries no
    //answer and zero is reserved; the row's sdkRetType is NO-PROTOTYPE so the base is the published BOOL and BoolCheck
    //(returncheck.h:94) covers it. The page is reached through the suffixed ANSI-twin slug nf-winuser-getalttabinfoa (the
    //slice contract's CORRECTED leg), which matches this member's AW binding. 0 in-tree callers for the exact stem (change-safe);
    //G6 keeps the check live on every call.
    BOOL User32::GetAltTabInfoT(HWND hwnd, int iItem, ::PALTTABINFO pati, LPTSTR pszItemText, UINT cchItemText)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapiAW(user32, GetAltTabInfo);
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call< BOOL, HWND, int, ::PALTTABINFO, LPTSTR, UINT >(pFunc, hwnd, iItem, pati, pszItemText, cchItemText);
        return bResult;
    }

    //Return policy (U2, table row GetAncestor / window.cpp, 12 Nov 2026): the row reads checked with checkClass
    //CheckNonZeroHandle< HWND > on the page's status pair - "The return value is the handle to the ancestor window. If the
    //function fails, the return value is NULL . To get extended error information, call GetLastError ." - the page documents
    //no legal NULL (a top-level window answers its own handle, which is the GetParent row's wording), so NULL is reserved and
    //the soundness test passes at every value; deliberately unlike the GetParent row just below, whose NULL is the documented
    //answer for a top-level window. sdkRetType HWND, so the base is the SDK HWND. 1 in-tree caller, audited before landing:
    //windowcontroller.cpp:183 spells "HWND ancestor = User32::GetAncestor(WINDOW_HANDLE, flags);" and hands ancestor to
    //perThread->FromHandle without testing it - G6 makes TType the checked wrapper type only under qor_pp_parameter_checking
    //== qor_pp_yes, which is defined nowhere, so the value the caller receives is identical and the landing adds the
    //diagnostic alone.
    HWND User32::GetAncestor(HWND hwnd, UINT gaFlags)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, GetAncestor);
        CheckReturn< HWND, CheckNonZeroHandle< HWND > >::TType hResult = Library::Call< HWND, HWND, UINT>(pFunc, hwnd, gaFlags);
        return hResult;
    }

    //Return policy (U2, table row GetClientRect / window.cpp, 12 Nov 2026): the row reads checked with checkClass BoolCheck,
    //and the docQuote is the AdjustWindowRect row's wording word for word - "If the function succeeds, the return value is
    //nonzero. If the function fails, the return value is zero. To get extended error information, call GetLastError ." -
    //the RECT the caller fills is an out-parameter, so the BOOL carries no answer and zero is reserved; sdkRetType BOOL
    //so BoolCheck (returncheck.h:94) covers it. 1 in-tree caller, audited before landing: windowcontroller.cpp:190 spells
    //it "return User32::GetClientRect(WINDOW_HANDLE, reinterpret_cast<LPRECT>(&rect)) ? true : false;", already tested, so
    //the check leaves the value identical and adds only the failure diagnostic; G6 keeps it live.
    BOOL User32::GetClientRect(HWND hWnd, ::LPRECT lpRect)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, GetClientRect);
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call< BOOL, HWND, ::LPRECT >(pFunc, hWnd, lpRect);
        return bResult;
    }

    HWND User32::GetDesktopWindow(void)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, GetDesktopWindow);
        return Library::Call<HWND>(pFunc);
    }

    HWND User32::GetForegroundWindow(void)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, GetForegroundWindow);
        return Library::Call<HWND>(pFunc);
    }

    //Return policy (U2, table row GetGUIThreadInfo / window.cpp, 12 Nov 2026): the row reads checked with checkClass BoolCheck,
    //and the docQuote is the AdjustWindowRect row's wording word for word - "If the function succeeds, the return value is
    //nonzero. If the function fails, the return value is zero. To get extended error information, call GetLastError ." -
    //the GUITHREADINFO structure the caller fills is an out-parameter, so the BOOL carries no answer and zero is reserved;
    //sdkRetType BOOL so BoolCheck (returncheck.h:94) covers it. 0 in-tree callers for the exact stem (change-safe); G6
    //keeps the check live on every call. The p1 name divergence the SDK pairing recorded (winuser.h idThread vs this
    //member's idThread at window.cpp:47's note) is a name note, not a return decision - nothing moved here.
    BOOL User32::GetGUIThreadInfo(DWORD idThread, ::LPGUITHREADINFO pgui)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, GetGUIThreadInfo);
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call< BOOL, DWORD, ::LPGUITHREADINFO >(pFunc, idThread, pgui);
        return bResult;
    }

    HWND User32::GetLastActivePopup(HWND hWnd)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, GetLastActivePopup);
        return Library::Call<HWND, HWND>(pFunc, hWnd);
    }

    //Return policy (U2, table row GetLayeredWindowAttributes / window.cpp, 12 Nov 2026): the row reads checked with checkClass BoolCheck,
    //and the docQuote is the AdjustWindowRect row's wording word for word - "If the function succeeds, the return value is
    //nonzero. If the function fails, the return value is zero. To get extended error information, call GetLastError ." -
    //the colour and transparency the caller reads are out-parameters, so the BOOL carries no answer and zero is reserved;
    //sdkRetType BOOL so BoolCheck (returncheck.h:94) covers it. 2 in-tree callers, audited before landing: windowcontroller.cpp:205
    //spells it "return User32::GetLayeredWindowAttributes(WINDOW_HANDLE, &colourKey, nullptr, &flags) ? true : false;" and
    //windowcontroller.cpp:211 the &alpha twin - both already tested, so the check leaves the value identical and adds only
    //the failure diagnostic; G6 keeps it live. printing.cpp:48's note about the SDK declaration pair at :4601 is a name note.
    BOOL User32::GetLayeredWindowAttributes(HWND hwnd, COLORREF* pcrKey, BYTE* pbAlpha, DWORD* pdwFlags)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, GetLayeredWindowAttributes);
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call< BOOL, HWND, COLORREF *, BYTE *, DWORD * >(pFunc, hwnd, pcrKey, pbAlpha, pdwFlags);
        return bResult;
    }

    HWND User32::GetParent(HWND hWnd)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, GetParent);
        return Library::Call<HWND, HWND>(pFunc, hWnd);
    }

    //Return policy (U2, table row GetProcessDefaultLayout / window.cpp, 12 Nov 2026): the row reads checked with checkClass BoolCheck,
    //and the docQuote is the AdjustWindowRect row's wording word for word - "If the function succeeds, the return value is
    //nonzero. If the function fails, the return value is zero. To get extended error information, call GetLastError ." -
    //the layout flags the caller reads are an out-parameter, so the BOOL carries no answer and zero is reserved; sdkRetType
    //BOOL so BoolCheck (returncheck.h:94) covers it. 0 in-tree callers for the exact stem (change-safe); G6 keeps the check
    //live on every call.
    BOOL User32::GetProcessDefaultLayout(DWORD* pdwDefaultLayout)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, GetProcessDefaultLayout);
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call< BOOL, DWORD * >(pFunc, pdwDefaultLayout);
        return bResult;
    }

    HWND User32::GetShellWindow(void)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, GetShellWindow);
        return Library::Call<HWND>(pFunc);
    }

    //Return policy (U2, table row GetTitleBarInfo / window.cpp, 12 Nov 2026): the row reads checked with checkClass BoolCheck,
    //and the docQuote is the AdjustWindowRect row's wording word for word - "If the function succeeds, the return value is
    //nonzero. If the function fails, the return value is zero. To get extended error information, call GetLastError ." -
    //the TITLEBAR structure the caller fills is an out-parameter, so the BOOL carries no answer and zero is reserved; sdkRetType
    //BOOL so BoolCheck (returncheck.h:94) covers it. 1 in-tree caller, audited before landing: windowcontroller.cpp:216 spells
    //it "return User32::GetTitleBarInfo(WINDOW_HANDLE, reinterpret_cast<PTITLEBARINFO>(&ti)) ? true : false;", already tested,
    //so the check leaves the value identical and adds only the failure diagnostic; G6 keeps it live.
    BOOL User32::GetTitleBarInfo(HWND hwnd, ::PTITLEBARINFO pti)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, GetTitleBarInfo);
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call< BOOL, HWND, ::PTITLEBARINFO >(pFunc, hwnd, pti);
        return bResult;
    }

    HWND User32::GetTopWindow(HWND hWnd)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, GetTopWindow);
        return Library::Call<HWND, HWND>(pFunc, hWnd);
    }

    HWND User32::GetWindow(HWND hWnd, UINT uCmd)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, GetWindow);
        return Library::Call<HWND, HWND, UINT>(pFunc, hWnd, uCmd);
    }

    //Return policy (U2, table row GetWindowInfo / window.cpp, 12 Nov 2026): the row reads checked with checkClass BoolCheck,
    //and the docQuote is the AdjustWindowRect row's wording word for word - "If the function succeeds, the return value is
    //nonzero. If the function fails, the return value is zero. To get extended error information, call GetLastError ." -
    //the WINDOWINFO structure the caller fills is an out-parameter, so the BOOL carries no answer and zero is reserved; sdkRetType
    //BOOL so BoolCheck (returncheck.h:94) covers it. 1 in-tree caller, audited before landing: windowcontroller.cpp:239 spells
    //it "return User32::GetWindowInfo(WINDOW_HANDLE, reinterpret_cast<PWINDOWINFO>(&wi)) ? true : false;", already tested,
    //so the check leaves the value identical and adds only the failure diagnostic; G6 keeps it live.
    BOOL User32::GetWindowInfo(HWND hwnd, ::PWINDOWINFO pwi)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, GetWindowInfo);
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call< BOOL, HWND, ::PWINDOWINFO >(pFunc, hwnd, pwi);
        return bResult;
    }

    UINT User32::GetWindowModuleFileNameT(HWND hwnd, LPTSTR pszFileName, UINT cchFileNameMax)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapiAW(user32, GetWindowModuleFileName);
        return Library::Call<UINT, HWND, LPTSTR, UINT>(pFunc, hwnd, pszFileName, cchFileNameMax);
    }

    //Return policy (U2, table row GetWindowPlacement / window.cpp, 12 Nov 2026): the row reads checked with checkClass BoolCheck,
    //and the docQuote is the AdjustWindowRect row's wording word for word - "If the function succeeds, the return value is
    //nonzero. If the function fails, the return value is zero. To get extended error information, call GetLastError ." -
    //the WINDOWPLACEMENT structure the caller fills is an out-parameter, so the BOOL carries no answer and zero is reserved;
    //sdkRetType BOOL so BoolCheck (returncheck.h:94) covers it. 1 in-tree caller, audited before landing: windowcontroller.cpp:252
    //spells it "return User32::GetWindowPlacement(WINDOW_HANDLE, reinterpret_cast<WINDOWPLACEMENT*>(&placement)) ? true : false;",
    //already tested, so the check leaves the value identical and adds only the failure diagnostic; G6 keeps it live. The
    //SetWindowPlacement twin is a later slice and stays untouched here.
    BOOL User32::GetWindowPlacement(HWND hWnd, ::WINDOWPLACEMENT* lpwndpl)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, GetWindowPlacement);
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call< BOOL, HWND, ::WINDOWPLACEMENT * >(pFunc, hWnd, lpwndpl);
        return bResult;
    }

    //Return policy (U2, table row GetWindowRect / window.cpp, 12 Nov 2026): the row reads checked with checkClass BoolCheck,
    //and the docQuote is the AdjustWindowRect row's wording word for word - "If the function succeeds, the return value is
    //nonzero. If the function fails, the return value is zero. To get extended error information, call GetLastError ." -
    //the RECT the caller fills is an out-parameter, so the BOOL carries no answer and zero is reserved; sdkRetType BOOL
    //so BoolCheck (returncheck.h:94) covers it. 1 in-tree caller, audited before landing: windowcontroller.cpp:257 spells
    //it "return User32::GetWindowRect(WINDOW_HANDLE, reinterpret_cast<LPRECT>(&rc)) ? true : false;", already tested, so
    //the check leaves the value identical and adds only the failure diagnostic; G6 keeps it live.
    BOOL User32::GetWindowRect(HWND hWnd, ::LPRECT lpRect)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, GetWindowRect);
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call< BOOL, HWND, ::LPRECT >(pFunc, hWnd, lpRect);
        return bResult;
    }

    int User32::GetWindowTextT(HWND hWnd, LPTSTR lpString, int nMaxCount)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapiAW(user32, GetWindowText);
        return Library::Call<int, HWND, LPTSTR, int>(pFunc, hWnd, lpString, nMaxCount);
    }

    int User32::GetWindowTextLengthT(HWND hWnd)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapiAW(user32, GetWindowTextLength);
        return Library::Call<int, HWND>(pFunc, hWnd);
    }

    //Return policy (U2, table row GetWindowThreadProcessId / window.cpp, 12 Nov 2026): the row reads checked with checkClass
    //CheckNonZero< DWORD >, and the docQuote reserves zero - "If the function succeeds, the return value is the identifier
    //of the thread that created the window. If the window handle is invalid, the return value is zero. To get extended
    //error information, call GetLastError ." - the page offers zero only for a bad handle (a caller bug, which under G6
    //a check is allowed to report), and a created window's thread identifier is never zero, so the soundness test passes
    //at every value; sdkRetType DWORD so the class base is the SDK DWORD and CheckNonZero< DWORD > (returncheck.h:187)
    //is the class already booked at processthread.cpp:87 / :97 / :115 for this same shape. 1 in-tree caller, audited
    //before landing: windowcontroller.cpp:274 spells it "return User32::GetWindowThreadProcessId(WINDOW_HANDLE,
    //reinterpret_cast<LPDWORD>(&processId));", so the controller passes the thread identifier straight through without
    //testing it; the landed check leaves that value identical and only adds the diagnostic, and G6 keeps it live.
    DWORD User32::GetWindowThreadProcessId(HWND hWnd, LPDWORD lpdwProcessId)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, GetWindowThreadProcessId);
        CheckReturn< DWORD, CheckNonZero< DWORD > >::TType dwResult = Library::Call< DWORD, HWND, LPDWORD >(pFunc, hWnd, lpdwProcessId);
        return dwResult;
    }

    int User32::InternalGetWindowText(HWND hWnd, LPWSTR pString, int cchMaxCount)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, InternalGetWindowText);
        return Library::Call<int, HWND, LPWSTR, int>(pFunc, hWnd, pString, cchMaxCount);
    }

    BOOL User32::IsChild(HWND hWndParent, HWND hWnd)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, IsChild);
        return Library::Call<BOOL, HWND, HWND>(pFunc, hWndParent, hWnd);
    }

    BOOL User32::IsGUIThread(BOOL bConvert)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, IsGUIThread);
        return Library::Call<BOOL, BOOL>(pFunc, bConvert);
    }

    BOOL User32::IsHungAppWindow(HWND hwnd)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, IsHungAppWindow);
        return Library::Call<BOOL, HWND>(pFunc, hwnd);
    }

    BOOL User32::IsIconic(HWND hWnd)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, IsIconic);
        return Library::Call<BOOL, HWND>(pFunc, hWnd);
    }

    BOOL User32::IsProcessDPIAware(void)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, IsProcessDPIAware);
        return Library::Call<BOOL>(pFunc);
    }

    BOOL User32::IsWindow(HWND hWnd)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, IsWindow);
        return Library::Call<BOOL, HWND>(pFunc, hWnd);
    }

    BOOL User32::IsWindowUnicode(HWND hWnd)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, IsWindowUnicode);
        return Library::Call<BOOL, HWND>(pFunc, hWnd);
    }

    BOOL User32::IsWindowVisible(HWND hWnd)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, IsWindowVisible);
        return Library::Call<BOOL, HWND>(pFunc, hWnd);
    }

    BOOL User32::IsZoomed(HWND hWnd)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, IsZoomed);
        return Library::Call<BOOL, HWND>(pFunc, hWnd);
    }

    //Return policy (U2, table row LockSetForegroundWindow / window.cpp, 12 Nov 2026): the row reads checked with checkClass
    //BoolCheck, and the docQuote is the AdjustWindowRect row's status wording word for word - "If the function succeeds, the
    //return value is nonzero. If the function fails, the return value is zero. To get extended error information, call
    //GetLastError ." - so zero is reserved and the lock mode (LSFW_LOCK / LSFW_UNLOCK) is the parameter, so the BOOL carries
    //no answer of its own and the session-27 soundness test passes at every value; sdkRetType BOOL so the base is BOOL and
    //BoolCheck (returncheck.h:94, Test at :101 is value ? true : false) is the class the library already has. 0 in-tree
    //callers for the exact stem (D14 published surface); G6 keeps the check live on every call.
    BOOL User32::LockSetForegroundWindow(UINT uLockCode)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, LockSetForegroundWindow);
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call< BOOL, UINT >(pFunc, uLockCode);
        return bResult;
    }

    BOOL User32::LogicalToPhysicalPoint(HWND hWnd, ::LPPOINT lpPoint)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, LogicalToPhysicalPoint);
        //The SDK returns BOOL (winuser.h:9526) - FALSE when the point is not inside a physical display area of
        //this window's monitor - so the old void + Library::voidCall discarded a result the caller needs.
        return Library::Call<BOOL, HWND, ::LPPOINT>(pFunc, hWnd, lpPoint);
    }

    //Return policy (U2, table row LogicalToPhysicalPointForPerMonitorDPI / window.cpp, 12 Nov 2026): the row reads checked
    //with checkClass BoolCheck, and the docQuote is "Returns TRUE if successful, or FALSE otherwise." - a status BOOL whose
    //FALSE is reserved for a failed call, since the POINT the caller translates is an in/out parameter and the HWND is an
    //argument, so the BOOL carries no answer; deliberately unlike the LogicalToPhysicalPoint row just above, whose page names
    //no return value at all and so stays unchecked. sdkRetType BOOL so BoolCheck (returncheck.h:94) covers it. 0 in-tree
    //callers for the exact stem (D14 published surface); G6 keeps the check live on every call.
    BOOL User32::LogicalToPhysicalPointForPerMonitorDPI(HWND hWnd, LPPOINT lpPoint)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, LogicalToPhysicalPointForPerMonitorDPI);
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call< BOOL, HWND, LPPOINT >(pFunc, hWnd, lpPoint);
        return bResult;
    }

    //Return policy (U2, table row MoveWindow / window.cpp, 12 Nov 2026): the row reads checked with checkClass BoolCheck,
    //and the docQuote is the AdjustWindowRect row's status wording word for word - "If the function succeeds, the return
    //value is nonzero. If the function fails, the return value is zero. To get extended error information, call
    //GetLastError ." - so zero is reserved and the target x/y and size are parameters with the window a handle, so the
    //BOOL carries no answer of its own. 1 in-tree caller, audited before landing: ui/gui/controllers/windowcontroller.cpp:320
    //spells it "return User32::MoveWindow(WINDOW_HANDLE, x, y, width, height, repaint) ? true : false;", so the caller
    //already tests the result and the landed check leaves the returned value identical, adding only the failure diagnostic;
    //sdkRetType BOOL so BoolCheck (returncheck.h:94) covers it; G6 keeps it live on every call.
    BOOL User32::MoveWindow(HWND hWnd, int X, int Y, int nWidth, int nHeight, BOOL bRepaint)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, MoveWindow);
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call< BOOL, HWND, int, int, int, int, BOOL >(pFunc, hWnd, X, Y, nWidth, nHeight, bRepaint);
        return bResult;
    }

    //Return policy (U2, table row OpenIcon / window.cpp, 12 Nov 2026): the row reads checked with checkClass BoolCheck,
    //and the docQuote is the AdjustWindowRect row's status wording word for word - "If the function succeeds, the return
    //value is nonzero. If the function fails, the return value is zero. To get extended error information, call
    //GetLastError ." - so zero is reserved and the window whose icon is being restored is a handle, so the BOOL carries
    //no answer of its own. 1 in-tree caller, audited before landing: ui/gui/controllers/windowcontroller.cpp:325 spells it
    //"return User32::OpenIcon(WINDOW_HANDLE) ? true : false;", so the caller already tests the result and the landed check
    //leaves the returned value identical, adding only the failure diagnostic; sdkRetType BOOL so BoolCheck
    //(returncheck.h:94) covers it; G6 keeps it live on every call.
    BOOL User32::OpenIcon(HWND hWnd)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, OpenIcon);
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call< BOOL, HWND >(pFunc, hWnd);
        return bResult;
    }

    BOOL User32::PhysicalToLogicalPoint(HWND hWnd, ::LPPOINT lpPoint)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, PhysicalToLogicalPoint);
        //The SDK returns BOOL (winuser.h:9533) - FALSE when the point is not inside the logical window area -
        //so the old void + Library::voidCall discarded a result the caller needs.
        return Library::Call<BOOL, HWND, ::LPPOINT>(pFunc, hWnd, lpPoint);
    }

    //Return policy (U2, table row PhysicalToLogicalPointForPerMonitorDPI / window.cpp, 12 Nov 2026): the row reads checked
    //with checkClass BoolCheck, and the docQuote is the LogicalToPhysicalPointForPerMonitorDPI row's wording word for word -
    //"Returns TRUE if successful, or FALSE otherwise." - a status BOOL whose FALSE is reserved for a failed call, since the
    //POINT the caller translates is an in/out parameter and the HWND is an argument, so the BOOL carries no answer;
    //deliberately unlike the PhysicalToLogicalPoint row just above, whose page names no return value at all and so stays
    //unchecked. sdkRetType BOOL so BoolCheck (returncheck.h:94) covers it. 0 in-tree callers for the exact stem (change-safe);
    //G6 keeps the check live on every call.
    BOOL User32::PhysicalToLogicalPointForPerMonitorDPI(HWND hWnd, LPPOINT lpPoint)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, PhysicalToLogicalPointForPerMonitorDPI);
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call< BOOL, HWND, LPPOINT >(pFunc, hWnd, lpPoint);
        return bResult;
    }

    HWND User32::RealChildWindowFromPoint(HWND hwndParent, ::POINT ptParentClientCoords)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, RealChildWindowFromPoint);
        return Library::Call<HWND, HWND, ::POINT>(pFunc, hwndParent, ptParentClientCoords);
    }

    //Return policy (U2, table row RealGetWindowClass / window.cpp, 12 Nov 2026): the row reads checked with checkClass
    //CheckNonZero< UINT >, and the docQuote reserves zero - "Type: UINT If the function succeeds, the return value is the
    //number of characters copied to the specified buffer. If the function fails, the return value is zero. To get
    //extended error information, call GetLastError ." - unlike the CharLowerBuffA rows (whose pages document no failure at
    //all) this page reserves zero, and a window class name is never the empty string, so a valid window always copies at
    //least one character and the count is never legitimately 0; sdkRetType NO-PROTOTYPE so the class base is the published
    //UINT and the template CheckNonZero (returncheck.h:187) covers it. 0 in-tree callers (change-safe). The wrapper is the
    //ANSI twin header RealGetWindowClassT, the page being nf-winuser-realgetwindowclassa.
    UINT User32::RealGetWindowClassT(HWND hwnd, LPTSTR ptszClassName, UINT cchClassNameMax)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapiAW(user32, RealGetWindowClass);
        CheckReturn< UINT, CheckNonZero< UINT > >::TType uResult = Library::Call<UINT, HWND, LPTSTR, UINT>(pFunc, hwnd, ptszClassName, cchClassNameMax);
        return uResult;
    }

    //Return policy (U2, table row RegisterShellHookWindow / window.cpp, 12 Nov 2026): the row reads checked with checkClass
    //BoolCheck, and the docQuote is "Type: BOOL TRUE if the function succeeds; otherwise, FALSE ." - a status BOOL whose
    //FALSE is reserved, since the window being registered as the shell hook is a handle and the hook message index arrives
    //through the WM Shell Hook messages rather than the return; the page names no legal FALSE, deliberately unlike the
    //RegisterForTooltipDismissNotification row in misc.cpp, whose Remarks put FALSE at legal already-registered / already-
    //unregistered states. sdkRetType BOOL so BoolCheck (returncheck.h:94) covers it. 1 in-tree caller, audited before
    //landing: ui/gui/controllers/windowcontroller.cpp:353 spells it "return User32::RegisterShellHookWindow(WINDOW_HANDLE)
    //? true : false;", so the caller already tests the result and the landed check leaves the returned value identical,
    //adding only the failure diagnostic; G6 keeps it live on every call.
    BOOL User32::RegisterShellHookWindow(HWND hwnd)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, RegisterShellHookWindow);
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call< BOOL, HWND >(pFunc, hwnd);
        return bResult;
    }

    BOOL User32::SetForegroundWindow(HWND hWnd)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, SetForegroundWindow);
        return Library::Call<BOOL, HWND>(pFunc, hWnd);
    }

    //Return policy (U2, table row SetLayeredWindowAttributes / window.cpp, 12 Nov 2026): the row reads checked with checkClass
    //BoolCheck, and the docQuote is the AdjustWindowRect row's status wording word for word - "If the function succeeds, the
    //return value is nonzero. If the function fails, the return value is zero. To get extended error information, call
    //GetLastError ." - so zero is reserved and the colour key, the alpha and the flags are parameters with the window a
    //handle, so the BOOL carries no answer; the GetLayeredWindowAttributes row above it reads checked on the same status
    //wording, its attributes arriving through the out-parameters rather than the return. sdkRetType BOOL so
    //BoolCheck (returncheck.h:94) covers it. 2 in-tree callers, audited before landing: windowcontroller.cpp:363 and :368
    //both spell "return User32::SetLayeredWindowAttributes(...) ? true : false;", so both already test the result and the
    //landed check leaves the returned value identical, adding only the failure diagnostic; G6 keeps it live on every call.
    BOOL User32::SetLayeredWindowAttributes(HWND hwnd, COLORREF crKey, BYTE bAlpha, DWORD dwFlags)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, SetLayeredWindowAttributes);
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call< BOOL, HWND, COLORREF, BYTE, DWORD >(pFunc, hwnd, crKey, bAlpha, dwFlags);
        return bResult;
    }

    //Return policy (U2, table row SetParent / window.cpp, 12 Nov 2026): the row reads checked with checkClass
    //CheckNonZeroHandle< HWND > on the page's status pair - "If the function succeeds, the return value is a handle to the
    //previous parent window. If the function fails, the return value is NULL . To get extended error information, call
    //GetLastError ." - the previous parent of a window that was parented to the desktop is the desktop window handle rather
    //than NULL, and the Remarks' NULL mentions ("if hWndNewParent is NULL , you should also clear the WS_CHILD bit and set
    //the WS_POPUP style after calling SetParent") are about the input parameter rather than the return, so NULL is never a
    //legitimate answer here and the soundness test passes at every value; deliberately unlike the GetParent row in this TU,
    //whose NULL is the documented answer for a top-level window. sdkRetType HWND, so the base is the SDK HWND; of the
    //HandleCheck / HandleNullCheck shapes the leaf's context named, HandleCheck keys on (HANDLE)(-1) and HandleNullCheck has
    //a fixed HANDLE base that mismatches this row's HWND, so CheckNonZeroHandle< HWND > (returncheck.h:137, Test fails at
    //0x00000000 only) is the class that fits the row's type and matches the precedent booked for the GetDlgItem rows. 1 in-tree
    //caller, audited before landing: windowcontroller.cpp:375 spells "HWND hwnd = User32::SetParent(WINDOW_HANDLE, ...);" and
    //hands hwnd to perThread->FromHandle without testing it, so the landing adds the diagnostic alone (TType is the plain
    //return type under G6).
    HWND User32::SetParent(HWND hWndChild, HWND hWndNewParent)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, SetParent);
        CheckReturn< HWND, CheckNonZeroHandle< HWND > >::TType hResult = Library::Call< HWND, HWND, HWND>(pFunc, hWndChild, hWndNewParent);
        return hResult;
    }

    //Return policy (U2, table row SetProcessDefaultLayout / window.cpp, 12 Nov 2026): the row reads checked with checkClass
    //BoolCheck, and the docQuote is the AdjustWindowRect row's status wording word for word - "If the function succeeds, the
    //return value is nonzero. If the function fails, the return value is zero. To get extended error information, call
    //GetLastError ." - so zero is reserved and the layout flags are parameters, so the BOOL carries no answer; the
    //GetProcessDefaultLayout row above it reads checked on the same wording, its layout arriving through the out-parameter.
    //sdkRetType BOOL so BoolCheck (returncheck.h:94) covers it. 0 in-tree callers for the exact stem (change-safe); G6 keeps
    //the check live on every call.
    BOOL User32::SetProcessDefaultLayout(DWORD dwDefaultLayout)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, SetProcessDefaultLayout);
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call< BOOL, DWORD >(pFunc, dwDefaultLayout);
        return bResult;
    }

    //Return policy (U2, table row SetProcessDPIAware / window.cpp, 12 Nov 2026): the row reads checked with checkClass BoolCheck,
    //and the docQuote is explicit success/failure wording with no answer carried - "If the function succeeds, the return value
    //is nonzero. Otherwise, the return value is zero." - the call takes no parameters, so the BOOL cannot be an answer about
    //an argument and zero is reserved; deliberately unlike the IsProcessDPIAware row, whose BOOL is the question about the
    //process and stays unchecked. The page notes the call has no effect once the process has started, which is a documented
    //no-op rather than a legal zero answer. sdkRetType BOOL so BoolCheck (returncheck.h:94) covers it. 0 in-tree callers for
    //the exact stem (change-safe); G6 keeps the check live on every call.
    BOOL User32::SetProcessDPIAware(void)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, SetProcessDPIAware);
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call< BOOL >(pFunc);
        return bResult;
    }

    //Return policy (U2, table row SetWindowPlacement / window.cpp, 12 Nov 2026): the row reads checked with checkClass BoolCheck,
    //and the docQuote is the AdjustWindowRect row's status wording word for word - "If the function succeeds, the return value
    //is nonzero. If the function fails, the return value is zero. To get extended error information, call GetLastError ." -
    //so zero is reserved and the WINDOWPLACEMENT structure is a parameter with the window a handle, so the BOOL carries no
    //answer; the GetWindowPlacement row above it reads checked on the same wording, its placement arriving through that
    //same structure. sdkRetType
    //BOOL so BoolCheck (returncheck.h:94) covers it. 1 in-tree caller, audited before landing: windowcontroller.cpp:382 spells
    //it "return User32::SetWindowPlacement(...) ? true : false;", so the caller already tests the result and the landed check
    //leaves the returned value identical, adding only the failure diagnostic; G6 keeps it live on every call.
    BOOL User32::SetWindowPlacement(HWND hWnd, const ::WINDOWPLACEMENT* lpwndpl)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, SetWindowPlacement);
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call< BOOL, HWND, const ::WINDOWPLACEMENT* >(pFunc, hWnd, lpwndpl);
        return bResult;
    }

    //Return policy (U2, table row SetWindowPos / window.cpp, 12 Nov 2026): the row reads checked with checkClass BoolCheck,
    //and the docQuote is the AdjustWindowRect row's status wording word for word - "If the function succeeds, the return value
    //is nonzero. If the function fails, the return value is zero. To get extended error information, call GetLastError ." -
    //so zero is reserved and the x/y, the cx/cy and the flags are parameters with the window a handle, so the BOOL carries no
    //answer. This is the densest call site in this TU: 5 in-tree callers, all audited before landing, at windowcontroller.cpp
    //:387 (the insertAfter branch), :392 (HWND_TOP), :397 (HWND_BOTTOM), :402 (HWND_TOPMOST) and :407 (HWND_NOTOPMOST), each
    //spelled "return User32::SetWindowPos(...) ? true : false;", so every caller already tests the result and the landed check
    //leaves the returned value identical at all five, adding only the failure diagnostic; sdkRetType BOOL so BoolCheck
    //(returncheck.h:94) covers it; G6 keeps it live on every call.
    BOOL User32::SetWindowPos(HWND hWnd, HWND hWndInsertAfter, int X, int Y, int cx, int cy, UINT uFlags)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, SetWindowPos);
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call< BOOL, HWND, HWND, int, int, int, int, UINT >(pFunc, hWnd, hWndInsertAfter, X, Y, cx, cy, uFlags);
        return bResult;
    }

    //Return policy (U2, table row SetWindowText / window.cpp, 12 Nov 2026): the row reads checked with checkClass BoolCheck,
    //and the docQuote is the AdjustWindowRect row's status wording word for word - "If the function succeeds, the return value
    //is nonzero. If the function fails, the return value is zero. To get extended error information, call GetLastError ." -
    //so zero is reserved and the text and the window handle are parameters, so the BOOL carries no answer. The row's sdkRetType
    //is NO-PROTOTYPE so the class base is the published BOOL and BoolCheck (returncheck.h:94) covers it; the page is reached
    //via the suffixed ANSI-twin slug nf-winuser-setwindowtexta (the CORRECTED leg of the slice contract). 0 in-tree callers for
    //the exact stem (change-safe); G6 keeps the check live on every call.
    BOOL User32::SetWindowTextT(HWND hWnd, LPCTSTR lpString)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapiAW(user32, SetWindowText);
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call< BOOL, HWND, LPCTSTR >(pFunc, hWnd, lpString);
        return bResult;
    }

    //Return policy (U2, table row ShowOwnedPopups / window.cpp, 12 Nov 2026): the row reads checked with checkClass BoolCheck,
    //and the docQuote is the AdjustWindowRect row's status wording word for word - "If the function succeeds, the return value
    //is nonzero. If the function fails, the return value is zero. To get extended error information, call GetLastError ." -
    //so zero is reserved and the window whose pop-ups are being shown or hidden is a handle with the show/hide flag a parameter,
    //so the BOOL carries no answer; this is a deviation from this leaf's ## Context, which listed ShowOwnedPopups among the
    //answer-shaped unchecked rows - the page's Return value section is the status boilerplate with a GetLastError route and the
    //pop-up state itself is never carried in the return (the AnyPopup / ShowWindow rows carry that question instead). sdkRetType
    //BOOL so BoolCheck (returncheck.h:94) covers it. 1 in-tree caller, audited before landing: windowcontroller.cpp:417 spells
    //it "return User32::ShowOwnedPopups(WINDOW_HANDLE, show ? TRUE : FALSE) ? true : false;", so the caller already tests the
    //result and the landed check leaves the returned value identical, adding only the failure diagnostic; G6 keeps it live.
    BOOL User32::ShowOwnedPopups(HWND hWnd, BOOL fShow)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, ShowOwnedPopups);
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call< BOOL, HWND, BOOL >(pFunc, hWnd, fShow);
        return bResult;
    }

    BOOL User32::ShowWindow(HWND hWnd, int nCmdShow)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, ShowWindow);
        return Library::Call<BOOL, HWND, int>(pFunc, hWnd, nCmdShow);
    }

    //Return policy (U2, table row ShowWindowAsync / window.cpp, 12 Nov 2026): the row reads checked with checkClass BoolCheck,
    //and the docQuote is the whole Return value section - "If the operation was successfully started, the return value is
    //nonzero." - so the page documents only the success value: the BOOL carries no answer about the window (the handle and the
    //show command are parameters), and zero is the operation not having been started, a failure state rather than a legal answer.
    //The page gives no GetLastError route, which is why a landed check here can only see the boolean; the ShowWindow row above is
    //unchecked on the opposite (previous-state) wording - "If the window was previously visible, the return value is nonzero."
    //sdkRetType BOOL so BoolCheck (returncheck.h:94) covers it. 1 in-tree caller, audited before landing: windowcontroller.cpp:426
    //spells "return User32::ShowWindowAsync(WINDOW_HANDLE, cmd) ? true : false;", so the caller already tests the result and the
    //landed check leaves the returned value identical, adding only the failure diagnostic; G6 keeps it live.
    BOOL User32::ShowWindowAsync(HWND hWnd, int nCmdShow)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, ShowWindowAsync);
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call< BOOL, HWND, int >(pFunc, hWnd, nCmdShow);
        return bResult;
    }

    //Return policy (U2, table row SoundSentry / window.cpp, 12 Nov 2026): the row reads checked with checkClass BoolCheck, and the
    //docQuote names the failure value explicitly - "This function returns one of the following values. Return code Description
    //TRUE The visual signal was or will be displayed correctly. FALSE An error prevented the signal from being displayed." - so
    //FALSE is reserved for an error and the call takes no parameters at all, so the BOOL carries no answer; deliberately unlike
    //the AnyPopup row, whose zero is the answer to the caller's question. sdkRetType BOOL so BoolCheck (returncheck.h:94) covers
    //it. 0 in-tree callers for the exact stem (change-safe); G6 keeps the check live on every call.
    BOOL User32::SoundSentry(void)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, SoundSentry);
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call< BOOL >(pFunc);
        return bResult;
    }

    void User32::SwitchToThisWindow(HWND hwnd, BOOL fUnknown)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, SwitchToThisWindow);
        return Library::voidCall<HWND, BOOL>(pFunc, hwnd, fUnknown);
    }

    //Return policy (U2, table row TileWindows / window.cpp, 12 Nov 2026): the row reads checked with checkClass
    //CheckNonZero< WORD >, and the docQuote is the CascadeWindows row's status wording word for word - "Type: WORD If the
    //function succeeds, the return value is the number of windows arranged. If the function fails, the return value is zero.
    //To get extended error information, call GetLastError ." - so zero is reserved and the Remarks ("Calling TileWindows
    //causes all maximized windows to be restored to their previous size.") name no legal zero answer; sdkRetType WORD so the
    //template CheckNonZero (returncheck.h:187) covers it at the row's own type. 1 in-tree caller, audited before landing:
    //windowcontroller.cpp:443 spells it "return User32::TileWindows(WINDOW_HANDLE, how, reinterpret_cast<RECT*>(&rc),
    //static_cast<UINT>(vecChildren.size()), vecChildren.data());", so the controller passes the count straight through
    //without testing it; the landed check leaves that value identical and only adds the diagnostic, and G6 keeps it live.
    WORD User32::TileWindows(HWND hwndParent, UINT wHow, const ::RECT* lpRect, UINT cKids, const HWND* lpKids)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, TileWindows);
        CheckReturn< WORD, CheckNonZero< WORD > >::TType uResult = Library::Call<WORD, HWND, UINT, const ::RECT*, UINT, const HWND*>(pFunc, hwndParent, wHow, lpRect, cKids, lpKids);
        return uResult;
    }

    //Return policy (U2, table row UpdateLayeredWindow / window.cpp, 12 Nov 2026): the row reads checked with checkClass BoolCheck,
    //and the docQuote is the AdjustWindowRect row's status wording word for word - "If the function succeeds, the return value is
    //nonzero. If the function fails, the return value is zero. To get extended error information, call GetLastError ." - so zero
    //is reserved and the DCX/destination parameters plus the window handle carry everything else, so the BOOL carries no answer.
    //sdkRetType BOOL so BoolCheck (returncheck.h:94) covers it. 1 in-tree caller, audited before landing: windowcontroller.cpp:447
    //spells "return User32::UpdateLayeredWindow(...) ? true : false;", so the caller already tests the result and the landed check
    //leaves the returned value identical, adding only the failure diagnostic; G6 keeps it live. The UpdateLayeredWindowIndirect
    //row below stays bare - its page has no Return value section at all, so it is booked unchecked on its own probe line.
    BOOL User32::UpdateLayeredWindow(HWND hWnd, HDC hdcDst, ::POINT* pptDst, ::SIZE* psize, HDC hdcSrc, ::POINT* pptSrc, COLORREF crKey, ::BLENDFUNCTION* pblend, DWORD dwFlags)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, UpdateLayeredWindow);
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call<BOOL, HWND, HDC, ::POINT*, ::SIZE*, HDC, ::POINT*, COLORREF, ::BLENDFUNCTION*, DWORD>(
            pFunc, hWnd, hdcDst, pptDst, psize, hdcSrc, pptSrc, crKey, pblend, dwFlags);
        return bResult;
    }

    BOOL User32::UpdateLayeredWindowIndirect(HWND hWnd, const ::UPDATELAYEREDWINDOWINFO* pULWInfo)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, UpdateLayeredWindowIndirect);
        return Library::Call<BOOL, HWND, const ::UPDATELAYEREDWINDOWINFO*>(pFunc, hWnd, pULWInfo);
    }

    HWND User32::WindowFromPhysicalPoint(::POINT Point)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, WindowFromPhysicalPoint);
        return Library::Call<HWND, ::POINT>(pFunc, Point);
    }

    HWND User32::WindowFromPoint(::POINT Point)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, WindowFromPoint);
        return Library::Call<HWND, ::POINT>(pFunc, Point);
    }

    //Return policy (U2, table row GetDpiForWindow / window.cpp, 12 Nov 2026): the row reads checked with checkClass
    //CheckNonZero< UINT >, and the docQuote puts the sentinel on invalid input only - "The DPI for the window, which
    //depends on the DPI_AWARENESS of the window. See the Remarks section for more information. An invalid hwnd value will
    //result in a return value of 0." - the Remarks table then enumerates every legal case (DPI_AWARENESS_UNAWARE answers the
    //base value 96, DPI_AWARENESS_SYSTEM_AWARE answers the system DPI, DPI_AWARENESS_PER_MONITOR_AWARE answers the monitor's
    //DPI), so every DPI_AWARENESS answers a nonzero DPI and 0 only ever comes from an invalid hwnd, the same reading the
    //GetWindowDpiAwarenessContext row is checked on ("If the window is not valid, the return value is NULL ."); deliberately
    //unlike the GetSystemMetricsForDpi and GetWindowTextLength rows, whose zero is a legal metric/length; sdkRetType UINT so
    //the template CheckNonZero (returncheck.h:187) covers it. 1 in-tree caller, audited before landing: windowcontroller.cpp:792
    //inside "unsigned int WindowController::GetDPI()" spells it "return User32::GetDpiForWindow(WINDOW_HANDLE);", so the
    //controller passes the DPI straight through without testing it; the landed check leaves that value identical and only
    //adds the diagnostic, and G6 keeps it live. windowimpl.cpp's four bare GetDpiForWindow sites (:361, :1130, :1212, :1462)
    //are all inside block comment (dpi.cpp:85-86 records the ranges 325-408, 530-1301, 530-1301, 1303-1511), so there is no
    //live bypass of this name.
    UINT User32::GetDpiForWindow(HWND hwnd)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, GetDpiForWindow);
        CheckReturn< UINT, CheckNonZero< UINT > >::TType uResult = Library::Call<UINT, HWND>(pFunc, hwnd);
        return uResult;
    }

}}}//qor::win::api
