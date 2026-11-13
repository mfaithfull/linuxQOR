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

    //34 wrappers - one per user32.h:70-104 "//Keyboard Input functions" declaration. All 34 declarations are static and
    //all 34 are defined here in header order (:16-247), so this TU has no U4 (declared, never defined) and no U13
    //(defined, never declared static) instance. The section is U21-shaped: 7 of the 34 are SDK *Window* entry points
    //rather than keyboard ones - BlockInput (winuser.h:14437), EnableWindow (:7143), GetActiveWindow (:5906),
    //GetFocus (:5912), IsWindowEnabled (:7150), SetActiveWindow (:8508), SetFocus (:5900) - and not one of the seven is
    //declared in the header's own "//Window functions" block (user32.h:128-213). Recorded against U21, not moved.
    //
    //Binding (D13): 7 of the 34 stems are A/W families and use qor_pp_useswinapiAW; the other 27 bind a genuine single
    //export with bare qor_pp_useswinapi. Measured against the export table (tmp/_tmp_kb_awpair_s20.ps1): all 27 are
    //bare-only with no <stem>A/<stem>W entry anywhere in its 1,014 parsed names, and all 7 families are A/W-only with
    //no bare export - 1,014 of the table's 1,018 names parse, the four `(forwarded to NTDLL...)` rows having no RVA
    //for the script's ordinal+RVA regex to match (P4). Three of the seven were broken until this session:
    //GetKeyboardLayoutNameT (:82), VkKeyScanT (:243) and VkKeyScanExT (:250) used the bare macro, so the member's own
    //T suffix stringized into the requested name (kernel32.h:50, _ATXT(_NAME)) and GetProcAddress asked for
    //"GetKeyboardLayoutNameT"/"VkKeyScanT"/"VkKeyScanExT" - no user32.dll export ends in an uppercase T. Each cached a
    //NULL pFunc and threw "Missing library function." on first call (P1, G5); the fix is qor_pp_useswinapiAW over the
    //bare stem. cl /EP now emits 34 name literals, 27 bare plus 7 *W (tmp/_tmp_kb_bindings_s20_post.txt from
    //tmp/_tmp_kb_bindlits_s20.ps1), and the live probe (tmp/_tmp_kb_probe_s20.txt) resolves all 34 while reporting
    //MISSING for the three superseded *T spellings and for the three bare stems of those families
    //(GetKeyboardLayoutName 16D/16E, VkKeyScan 3E1/3E4, VkKeyScanEx 3E2/3E3). GetKBCodePage is a real function and a
    //real export (winuser.h:5918, ordinal 167), not the TEB macro older documentation describes.
    //
    //Signatures: every parameter and return type now matches the SDK prototype. Two divergences stood until T8 step 1 on
    //6 Nov 2026 - GetKeyboardLayoutList returned UINT here against the SDK's int (winuser.h:1313), and keybd_event took
    //void* dwExtraInfo against the SDK's ULONG_PTR (winuser.h:6071); "same width on x86 and x64" is no longer grounds to
    //leave one (D17 voided that argument, D18 fixed the site), and the parameter names are the SDK's since step 4.
    //Zero-argument lists are
    //(void) (U14c), and all 34 Call/voidCall sites now name their template arguments - this TU held 2 of the folder's
    //6 deducing sites (U14b).
    //
    //Availability (U16): 16 of the 34 SDK prototypes are emitted only when a WINVER/_WIN32_WINNT guard passes and the
    //other 18 only under the Desktop partition (per-name list in tmp/_tmp_kb_guards_s19.txt). The extremes are
    //GetAsyncKeyState, whose second declaration needs _WIN32_WINNT >= 0x0604 (winuser.h:630), and SendInput's at
    //0x0603 (:15968). No build-wide version pin is set; the pin is deferred to the end of the review (session 5).
    //
    //TCHAR width (U26): 5 of the 7 A/W wrappers take a TCHAR-derived parameter (LPTSTR/LPCTSTR/TCHAR) while the AW
    //macro token-pastes the W export unconditionally, so the pair only holds while _UNICODE is in scope. It is: the
    //chain here is configuration.h -> user32.h -> kernel32.h:7-10, which defines UNICODE/_UNICODE before <Windows.h>
    //at :12, so in this TU LPCTSTR is LPCWSTR and matches the binding. A consumer that reaches <Windows.h> first gets
    //the narrow spelling of these five published signatures and fails at the link (U26), not silently.
    //
    //Contracts the wrappers do not enforce: GetKeyboardLayoutNameT writes KL_NAMELENGTH TCHARs, 9 of them
    //(winuser.h:1233), and the caller owns that size; GetLastInputInfo needs plii->cbSize filled in first; SendInput
    //needs cbSize == sizeof(INPUT); ToUnicode/ToUnicodeEx are stateful - a -1 return means a dead key is now in the
    //per-thread key state and the buffer contents are undefined; ToAscii/ToAsciiEx expect a 256-byte key-state image
    //and are the ANSI-era answer ToUnicode replaced; keybd_event is documented as superseded by SendInput.
    //GetActiveWindow, GetFocus, SetActiveWindow, SetFocus, GetKeyState, GetKeyboardState, SetKeyboardState,
    //GetAsyncKeyState and GetKeyboardLayout(0) all answer about the *calling thread's* input queue and its
    //AttachThreadInput state, not about the process, so NULL/0 from a thread that does not own the window is a
    //legitimate result - and with no Win32ErrorHandler and no CheckReturn (the U1/U2 policy) a legitimate zero and a
    //swallowed SEH fault are indistinguishable (U7).
    //
    //Liveness (U15): 29 of the 34 have no reachable caller in src/examples/test. The five that do are EnableWindow,
    //IsWindowEnabled, RegisterHotKey, UnregisterHotKey and SetFocus, all from ui/gui/controllers/windowcontroller.cpp
    //(:72, :77, :82, :87, :839) in the built winqor_gui target. U19: 5 LAYER / 7 INDIRECT / 11 BYPASS / 2 MEMBER - the
    //folder's largest bypass count. The bypasses are live source (comment-state checked) but none of them is compiled
    //today: ui/gl/session.cpp:152,154,162 (MapVirtualKeyW, ToUnicode x2) is in no target's source list at all,
    //ui/gl/windowimpl.cpp:414-424 (GetKeyState x6) is in winqor_gl, whose add_subdirectory(gl) is commented out at
    //ui/CMakeLists.txt:4, and retropad.cpp:842 (SetFocus) is in a target whose add_subdirectory(retropad) is commented
    //out at examples/experimental/osspecific/windows/CMakeLists.txt:8. One of the 11 BYPASS rows is not a call but the
    //member declaration Window* SetFocus() at ui/gui/controllers/windowcontroller.h:310, so 10 are real SDK bypasses;
    //2 of the 7 INDIRECT are Linux (os/linux/ui/x/xinput/device.cpp:104,109) and cannot reach this layer at all.

    //Return policy (U2, table row ActivateKeyboardLayout / keyboard.cpp, 12 Nov 2026): the row reads checked with checkClass
    //CheckNonZeroHandle< HKL > - docQuote "The return value is of type HKL . If the function succeeds, the return value is the
    //previous input locale identifier. Otherwise, it is zero. To get extended error information, use the GetLastError
    //function." so zero is reserved for a failed call and the identifier the caller gets back is the previous locale, which
    //a caller that receives zero has no use for; deliberately unlike the GetKeyboardLayout row, whose page names no failure
    //value at all. The row's sdkRetType column is blank, so the class base is the published HKL. AUDIT: the table counts 0
    //in-tree callers for the exact stem, so the check is change-safe and no call site needs reading.
    HKL User32::ActivateKeyboardLayout(HKL hkl, UINT Flags)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, ActivateKeyboardLayout);
        CheckReturn< HKL, CheckNonZeroHandle< HKL > >::TType hResult = Library::Call< HKL, HKL, UINT >(pFunc, hkl, Flags);
        return hResult;
    }

    //Return policy (U2, table row BlockInput / keyboard.cpp, 12 Nov 2026): the row reads unchecked with no checkClass, so the
    //call stays bare and carries no token - docQuote "If the function succeeds, the return value is nonzero. If input is
    //already blocked, the return value is zero. To get extended error information, call GetLastError ." is a question about
    //prior state, not a status: the zero reports the legal state "input already blocked", which the caller may legitimately
    //act on, so the boilerplate reserves nothing separable from that answer and no class fits; same shape as the EnableWindow
    //and IsWindowEnabled rows. AUDIT: the table counts 0 in-tree callers for the exact stem, so leaving the site bare is
    //change-safe.
    BOOL User32::BlockInput(BOOL fBlockIt)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, BlockInput);
        return Library::Call< BOOL, BOOL >(pFunc, fBlockIt);
    }

    //Return policy (U2, table row EnableWindow / keyboard.cpp, 12 Nov 2026): the row reads unchecked with no checkClass, so the
    //call stays bare and carries no token - docQuote "If the window was previously disabled, the return value is nonzero. If
    //the window was not previously disabled, the return value is zero." is a question about prior state: the zero is the
    //answer to "was it already enabled", a legal state, so keying BoolCheck would report a legal already-enabled window as a
    //failure. AUDIT: the table counts 1 in-tree caller, and the audit-before-landing rule gates a token move; no token moves
    //here, so no call site read is a precondition for leaving the site bare.
    BOOL User32::EnableWindow(HWND hWnd, BOOL bEnable)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, EnableWindow);
        return Library::Call< BOOL, HWND, BOOL >(pFunc, hWnd, bEnable);
    }

    //Return policy (U2, table row GetActiveWindow / keyboard.cpp, 12 Nov 2026): the row reads unchecked with no checkClass, so
    //the call stays bare - docQuote "The return value is the handle to the active window attached to the calling thread's
    //message queue. Otherwise, the return value is NULL ." is answer-shaped: a thread with no active window (a console or
    //non-GUI thread) is a legal state answering NULL, so nothing is reserved and HandleCheck / CheckNonZeroHandle would
    //report a legal no-active-window answer as a failure. AUDIT: the table counts 0 in-tree callers for the exact stem, so
    //the bare site is change-safe.
    HWND User32::GetActiveWindow(void)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, GetActiveWindow);
        return Library::Call< HWND >(pFunc);
    }

    //Return policy (U2, table row GetAsyncKeyState / keyboard.cpp, 12 Nov 2026): the row reads unchecked with no checkClass, so
    //the call stays bare - the docQuote is an answer-shaped bitmask, not a status: "The most significant bit of the return
    //value is set if the specified key is currently down. Since the most significant bit is the sign bit, a negative value
    //indicates that the key is currently down. ... All other bits of the result are reserved and should be ignored. The
    //return value is zero if the call fails." A key that is simply up answers 0 as a legal outcome, indistinguishable from
    //the documented failure zero, so keying CheckNonZero< SHORT > would report a legal up-key as a failure. AUDIT: the table
    //counts 0 in-tree callers for the exact stem, so the bare site is change-safe.
    SHORT User32::GetAsyncKeyState(int vKey)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, GetAsyncKeyState);
        return Library::Call< SHORT, int >(pFunc, vKey);
    }

    //Return policy (U2, table row GetFocus / keyboard.cpp, 12 Nov 2026): the row reads unchecked with no checkClass, so the call
    //stays bare - docQuote "The return value is the handle to the window with the keyboard focus. If the calling thread's
    //message queue does not have an associated window with the keyboard focus, the return value is NULL ." is answer-shaped:
    //a queue with no focused window is a legal state answering NULL, so nothing is reserved and a handle check would report a
    //legal no-focus answer as a failure; the SetFocus row shares this reading on its previous-handle shape. AUDIT: the table
    //counts 0 in-tree callers for the exact stem, so the bare site is change-safe.
    HWND User32::GetFocus(void)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, GetFocus);
        return Library::Call< HWND >(pFunc);
    }

    //Return policy (U2, table row GetKBCodePage / keyboard.cpp, 12 Nov 2026): the row reads unchecked with no checkClass, so the
    //call stays bare - docQuote "The return value is an OEM code-page identifier, or it is the default identifier if the
    //registry value is not readable. For a list of OEM code-page identifiers, see Code Page Identifiers ." names no failure
    //value: the call always answers a valid code-page identifier and falls back to the default when the registry value is
    //unreadable, so D51 step 2 reads no-check and nothing is reserved. AUDIT: the table counts 0 in-tree callers for the
    //exact stem, so the bare site is change-safe.
    UINT User32::GetKBCodePage(void)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, GetKBCodePage);
        return Library::Call< UINT >(pFunc);
    }

    //Return policy (U2, table row GetKeyboardLayout / keyboard.cpp, 12 Nov 2026): the row reads unchecked with no checkClass, so
    //the call stays bare - docQuote "The return value is the input locale identifier for the thread. The low word contains a
    //Language Identifier for the input language and the high word contains a device handle to the physical layout of the
    //keyboard." is the whole answer and the page names no failure value, so D51 step 2 reads no-check; deliberately unlike the
    //ActivateKeyboardLayout row above, whose page does reserve zero for a failed call. AUDIT: the table counts 0 in-tree
    //callers for the exact stem, so the bare site is change-safe.
    HKL User32::GetKeyboardLayout(DWORD idThread)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, GetKeyboardLayout);
        return Library::Call< HKL, DWORD >(pFunc, idThread);
    }

    //Return policy (U2, table row GetKeyboardLayoutList / keyboard.cpp, 12 Nov 2026): the row reads checked with checkClass
    //CheckNonZero< int > - docQuote "If the function succeeds, the return value is the number of input locale identifiers
    //copied to the buffer or, if nBuff is zero, the return value is the size, in array elements, of the buffer needed to
    //receive all current input locale identifiers. If the function fails, the return value is zero. To get extended error
    //information, call GetLastError ." so zero is reserved for a failed call while every legal answer is a count or a size,
    //never zero (a session always has at least one input locale); the list itself is the HKL* out-parameter. sdkRetType int
    //so the class base is int. AUDIT: the table counts 0 in-tree callers for the exact stem, so the check is change-safe.
    int User32::GetKeyboardLayoutList(int nBuff, HKL* lpList)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, GetKeyboardLayoutList);
        CheckReturn< int, CheckNonZero< int > >::TType hResult = Library::Call< int, int, HKL* >(pFunc, nBuff, lpList);
        return hResult;
    }

    //Return policy (U2, table row GetKeyboardLayoutName / keyboard.cpp, 12 Nov 2026): the row reads checked with checkClass
    //BoolCheck - docQuote "If the function succeeds, the return value is nonzero. If the function fails, the return value is
    //zero. To get extended error information, call GetLastError ." and the name the call reports is written into the LPTSTR
    //out-parameter, so the BOOL carries no answer of its own and zero is reserved. The row's sdkRetType is NO-PROTOTYPE, so
    //the class base is the published BOOL; the member keeps the T spelling and binds qor_pp_useswinapiAW over the bare stem.
    //AUDIT: the table counts 0 in-tree callers for the exact stem, so the check is change-safe.
    BOOL User32::GetKeyboardLayoutNameT(LPTSTR pwszKLID)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapiAW(user32, GetKeyboardLayoutName);
        CheckReturn< BOOL, BoolCheck >::TType hResult = Library::Call< BOOL, LPTSTR >(pFunc, pwszKLID);
        return hResult;
    }

    //Return policy (U2, table row GetKeyboardState / keyboard.cpp, 12 Nov 2026): the row reads checked with checkClass BoolCheck -
    //docQuote "If the function succeeds, the return value is nonzero. If the function fails, the return value is zero. To get
    //extended error information, call GetLastError ." and the 256-byte keyboard-state array the caller passes carries the
    //answer, so the BOOL carries none of its own and zero is reserved; this is the library's only SAL-annotated prototype
    //(ann _Check_return_ at winuser.h:5934), a hint that agrees with the page rather than deciding it. sdkRetType BOOL so
    //BoolCheck (returncheck.h:94) covers it - no new check class. AUDIT: the table counts 0 in-tree callers for the exact
    //stem, so the check is change-safe and no call site needs reading.
    BOOL User32::GetKeyboardState(PBYTE lpKeyState)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, GetKeyboardState);
        CheckReturn< BOOL, BoolCheck >::TType hResult = Library::Call< BOOL, PBYTE >(pFunc, lpKeyState);
        return hResult;
    }

    //Return policy (U2, table row GetKeyboardType / keyboard.cpp, 12 Nov 2026): the row reads unchecked with no checkClass, so the
    //call stays bare - docQuote "If the function succeeds, the return value specifies the requested information. If the
    //function fails and nTypeFlag is not 1, the return value is 0; 0 is a valid return value when nTypeFlag is 1 (keyboard
    //subtype)." - the page itself admits 0 as a valid answer for a subtype query, so the documented failure zero is
    //indistinguishable from a legal answer and the session-27 soundness test fails at that value; no class fits. AUDIT: the
    //table counts 0 in-tree callers for the exact stem, so the bare site is change-safe.
    int User32::GetKeyboardType(int nTypeFlag)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, GetKeyboardType);
        return Library::Call< int, int >(pFunc, nTypeFlag);
    }

    //Return policy (U2, table row GetKeyNameText / keyboard.cpp, 12 Nov 2026): the row reads unchecked with no checkClass, so the
    //call stays bare - docQuote "If the function succeeds, a null-terminated string is copied into the specified buffer, and
    //the return value is the length of the string, in characters, not counting the terminating null character. If the
    //function fails, the return value is zero." - the name goes into the LPTSTR out-parameter and the return is its length,
    //so an empty name answers 0 legitimately and that zero is indistinguishable from the documented failure value; the same
    //length-vs-failure shape as the DrawText and GetTabbedTextExtent rows. AW row with NO-PROTOTYPE. AUDIT: the table counts
    //0 in-tree callers for the exact stem, so the bare site is change-safe.
    int User32::GetKeyNameTextT(LONG lParam, LPTSTR lpString, int cchSize)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapiAW(user32, GetKeyNameText);
        return Library::Call< int, LONG, LPTSTR, int >(pFunc, lParam, lpString, cchSize);
    }

    //Return policy (U2, table row GetKeyState / keyboard.cpp, 12 Nov 2026): the row reads unchecked with no checkClass, so the
    //call stays bare - docQuote "The return value specifies the status of the specified virtual key, as follows: If the
    //high-order bit is 1, the key is down; otherwise, it is up. If the low-order bit is 1, the key is toggled. ... the key
    //is off and untoggled if the low-order bit is 0." - an up-and-untoggled key answers 0 legitimately, so nothing is
    //reserved and CheckNonZero< SHORT > would report a legal up-key as a failure; the GetAsyncKeyState row above is
    //unchecked on the same bitmask reading. AUDIT: the table counts 0 in-tree callers for the exact stem, so the bare site
    //is change-safe.
    SHORT User32::GetKeyState(int nVirtKey)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, GetKeyState);
        return Library::Call< SHORT, int >(pFunc, nVirtKey);
    }

    //Return policy (U2, table row GetLastInputInfo / keyboard.cpp, 12 Nov 2026): the row reads checked with checkClass BoolCheck -
    //docQuote "If the function succeeds, the return value is nonzero. If the function fails, the return value is zero." so
    //zero is reserved, and the LASTINPUTINFO structure the caller passes carries the time, so the BOOL carries no answer of
    //its own. sdkRetType BOOL so BoolCheck (returncheck.h:94) covers it - no new check class. AUDIT: the table counts 0 in-tree
    //callers for the exact stem, so the check is change-safe and no call site needs reading.
    BOOL User32::GetLastInputInfo(PLASTINPUTINFO plii)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, GetLastInputInfo);
        CheckReturn< BOOL, BoolCheck >::TType hResult = Library::Call< BOOL, PLASTINPUTINFO >(pFunc, plii);
        return hResult;
    }

    //Return policy (U2, table row IsWindowEnabled / keyboard.cpp, 12 Nov 2026): the row reads unchecked with no checkClass, so the
    //call stays bare - docQuote "If the window is enabled, the return value is nonzero. If the window is not enabled, the
    //return value is zero." is a question about the window's enabled state, not a status: the zero is the answer to "is the
    //window enabled", a legal state, so nothing is reserved and BoolCheck would report a legal disabled window as a failure;
    //same shape as the EnableWindow row. AUDIT: the table counts 1 in-tree caller, and the audit-before-landing rule gates a
    //token move; no token moves here, so no call site read is a precondition for leaving the site bare.
    BOOL User32::IsWindowEnabled(HWND hWnd)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, IsWindowEnabled);
        return Library::Call< BOOL, HWND >(pFunc, hWnd);
    }

    VOID User32::keybd_event(BYTE bVk, BYTE bScan, DWORD dwFlags, ULONG_PTR dwExtraInfo)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, keybd_event);
        return Library::voidCall< BYTE, BYTE, DWORD, ULONG_PTR >(pFunc, bVk, bScan, dwFlags, dwExtraInfo);
    }

    //Return policy (U2, table row LoadKeyboardLayout / keyboard.cpp, 12 Nov 2026): the row reads checked with checkClass
    //CheckNonZeroHandle< HKL > - docQuote "If the function succeeds, the return value is the input locale identifier
    //corresponding to the name specified in pwszKLID . If no matching locale is available, the return value is the default
    //language of the system. If the function fails, the return value is NULL. This can occur if the layout library is loaded
    //from the application directory." - both the success answer and the no-match fallback are nonzero HKLs, so only the
    //documented NULL is reserved and the session-27 test passes at every legal value; AW row with NO-PROTOTYPE so the base
    //is the published HKL and CheckNonZeroHandle (returncheck.h:137) fits - no new check class. AUDIT: the table counts 0
    //in-tree callers for the exact stem, so the check is change-safe and no call site needs reading.
    HKL User32::LoadKeyboardLayoutT(LPCTSTR pwszKLID, UINT Flags)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapiAW(user32, LoadKeyboardLayout);
        CheckReturn< HKL, CheckNonZeroHandle< HKL > >::TType hResult = Library::Call< HKL, LPCTSTR, UINT >(pFunc, pwszKLID, Flags);
        return hResult;
    }

    //Return policy (U2, table row MapVirtualKey / keyboard.cpp, 12 Nov 2026): the row reads unchecked with no checkClass, so the
    //call stays bare - docQuote "The return value is either a scan code, a virtual-key code, or a character value, depending
    //on the value of uCode and uMapType . If there is no translation, the return value is zero." - "no translation" is a
    //legal outcome answering 0, so nothing is reserved and CheckNonZero< UINT > would report a legal untranslated code as a
    //failure. AW row with NO-PROTOTYPE. AUDIT: the table counts 0 in-tree callers for the exact stem, so the bare site is
    //change-safe.
    UINT User32::MapVirtualKeyT(UINT uCode, UINT uMapType)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapiAW(user32, MapVirtualKey);
        return Library::Call< UINT, UINT, UINT >(pFunc, uCode, uMapType);
    }

    //Return policy (U2, table row MapVirtualKeyEx / keyboard.cpp, 12 Nov 2026): the row reads unchecked with no checkClass, so the
    //call stays bare - docQuote "The return value is either a scan code, a virtual-key code, or a character value, depending
    //on the value of uCode and uMapType . If there is no translation, the return value is zero." is the MapVirtualKey row's
    //Return value section word for word, so the same reading applies: the return is a scan / virtual-key / character value and
    //0 answers "no translation" legitimately, so nothing is reserved and CheckNonZero< UINT > would report a legal untranslated
    //code as a failure. The locale handle dwhkl is an input, not the answer. AW row with NO-PROTOTYPE, fetched via the suffixed
    //twin slug nf-winuser-mapvirtualkeyexa. AUDIT: the table counts 0 in-tree callers for the exact stem, so the bare site is
    //change-safe.
    UINT User32::MapVirtualKeyExT(UINT uCode, UINT uMapType, HKL dwhkl)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapiAW(user32, MapVirtualKeyEx);
        return Library::Call< UINT, UINT, UINT, HKL >(pFunc, uCode, uMapType, dwhkl);
    }

    //Return policy (U2, table row OemKeyScan / keyboard.cpp, 12 Nov 2026): the row reads unchecked with no checkClass, so the
    //call stays bare - docQuote "The low-order word of the return value contains the scan code of the OEM character, and the
    //high-order word contains the shift state, which can be a combination of the following bits. ... If the character cannot
    //be produced by a single keystroke using the current keyboard layout, the return value is -1." - the return is a packed
    //scan-code / shift-state answer and -1 is the informational outcome the caller acts on, not a hard failure, so nothing
    //is reserved and no class fits; the OEM character is the single argument. AUDIT: the table counts 0 in-tree callers for
    //the exact stem, so the bare site is change-safe.
    DWORD User32::OemKeyScan(WORD wOemChar)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, OemKeyScan);
        return Library::Call< DWORD, WORD >(pFunc, wOemChar);
    }

    //Return policy (U2, table row RegisterHotKey / keyboard.cpp, 12 Nov 2026): the row reads checked with checkClass BoolCheck -
    //docQuote "If the function succeeds, the return value is nonzero. If the function fails, the return value is zero. To get
    //extended error information, call GetLastError . This function fails if you try to associate a hot key with a window
    //created by another thread. Typically, RegisterHotKey also fails if the keystrokes specified for the hot key have already
    //been registered for another hot key." - the hot key and the window are parameters, so the BOOL carries no answer of its
    //own and zero is reserved for a failed registration; sdkRetType BOOL so the base is BOOL and BoolCheck (returncheck.h:94)
    //covers it - no new check class. AUDIT: the table counts 1 in-tree caller and the site-level read finds it at ui/gui/controllers/windowcontroller.cpp:82
    //as "return User32::RegisterHotKey(WINDOW_HANDLE, id, modifiers, vk) ? true : false;" in WindowController::RegisterHotKey;
    //CheckReturn returns the value unchanged, so the returned bool is identical and only a diagnostic is added at a documented
    //failure.
    BOOL User32::RegisterHotKey(HWND hWnd, int id, UINT fsModifiers, UINT vk)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, RegisterHotKey);
        CheckReturn< BOOL, BoolCheck >::TType hResult = Library::Call< BOOL, HWND, int, UINT, UINT >(pFunc, hWnd, id, fsModifiers, vk);
        return hResult;
    }

    //Return policy (U2, table row SendInput / keyboard.cpp, 12 Nov 2026): the row reads unchecked with no checkClass, so the
    //call stays bare - docQuote "The function returns the number of events that it successfully inserted into the keyboard or
    //mouse input stream. If the function returns zero, the input was already blocked by another thread. ... This function fails
    //when it is blocked by UIPI. Note that neither GetLastError nor the return value will indicate the failure was caused by
    //UIPI blocking." - a count answer with a legal zero AND an indistinguishable failure, so zero is the "already blocked"
    //state the caller acts on and CheckNonZero< UINT > would report a legal blocked-input state as a failure. AUDIT: the
    //table counts 0 in-tree callers for the exact stem, so the bare site is change-safe.
    UINT User32::SendInput(UINT cInputs, LPINPUT pInputs, int cbSize)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, SendInput);
        return Library::Call< UINT, UINT, LPINPUT, int >(pFunc, cInputs, pInputs, cbSize);
    }

    //Return policy (U2, table row SetActiveWindow / keyboard.cpp, 12 Nov 2026): the row reads unchecked with no checkClass,
    //so the call stays bare - docQuote "If the function succeeds, the return value is the handle to the window that was
    //previously active. If the function fails, the return value is NULL ." - the return is the answer to "which window was
    //active before", and the GetActiveWindow row establishes that a thread with no active window answers NULL legitimately,
    //so a NULL previous handle is a legal answer and a handle check would report it as a failure; deliberately unlike the
    //status-BOOL Set rows (SetKeyboardState below), whose BOOL carries no answer of its own. AUDIT: the table counts 0
    //in-tree callers for the exact stem, so the bare site is change-safe.
    HWND User32::SetActiveWindow(HWND hWnd)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, SetActiveWindow);
        return Library::Call< HWND, HWND >(pFunc, hWnd);
    }

    //Return policy (U2, table row SetFocus / keyboard.cpp, 12 Nov 2026): the row reads unchecked with no checkClass, so the
    //call stays bare - the SetActiveWindow row's previous-handle shape: docQuote "If the function succeeds, the return value
    //is the handle to the window that previously had the keyboard focus. If the hWnd parameter is invalid or the window is
    //not attached to the calling thread's message queue, the return value is NULL." - the return is the previously-focused
    //window handle, so NULL answers "no window had focus before" as a legal outcome and the documented NULL also doubles as
    //a bad-parameter report, leaving no value that is only ever a failure; the extended error ERROR_INVALID_PARAMETER (0x57)
    //for a disabled window is the page's own route rather than the return's. AUDIT: the table counts 1 in-tree caller for the
    //exact stem and the site-level read finds it at ui/gui/controllers/windowcontroller.cpp:851 as "HWND hwnd = User32::SetFocus(WINDOW_HANDLE);"
    //whose result is handed straight to m_perThread->FromHandle(hwnd) at :852 with no nonzero test, so the site already relies
    //on the previous-handle reading this row books; no token moves here, so the bare site needs no further call-site change.
    HWND User32::SetFocus(HWND hWnd)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, SetFocus);
        return Library::Call< HWND, HWND >(pFunc, hWnd);
    }

    //Return policy (U2, table row SetKeyboardState / keyboard.cpp, 12 Nov 2026): the row reads checked with checkClass BoolCheck
    //- docQuote "If the function succeeds, the return value is nonzero. If the function fails, the return value is zero. To get
    //extended error information, call GetLastError ." - the BYTE256 array the caller passes carries the keyboard state, so the
    //BOOL carries no answer and zero is reserved for a failed set; deliberately unlike the SetActiveWindow and SetFocus rows
    //above, whose returns are previous-handle answers with a legal NULL. sdkRetType BOOL so BoolCheck (returncheck.h:94) covers
    //it - no new check class. AUDIT: the table counts 0 in-tree callers for the exact stem, so the token move is change-safe.
    BOOL User32::SetKeyboardState(LPBYTE lpKeyState)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, SetKeyboardState);
        CheckReturn< BOOL, BoolCheck >::TType hResult = Library::Call< BOOL, LPBYTE >(pFunc, lpKeyState);
        return hResult;
    }

    //Return policy (U2, table row ToAscii / keyboard.cpp, 12 Nov 2026): the row reads unchecked with no checkClass, so the
    //call stays bare - docQuote "0 The specified virtual key has no translation for the current state of the keyboard. 1 One
    //character was copied to the buffer. 2 Two characters were copied to the buffer." - 0 is the legal no-translation answer
    //the caller acts on, 1/2 report how many characters the out-buffer received, so nothing is reserved and CheckNonZero< int >
    //would report a legal untranslated key as a failure; the character itself goes into the lpChar out-parameter. AUDIT: the
    //table counts 0 in-tree callers for the exact stem, so the bare site is change-safe.
    int User32::ToAscii(UINT uVirtKey, UINT uScanCode, PBYTE lpKeyState, LPWORD lpChar, UINT uFlags)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, ToAscii);
        return Library::Call< int, UINT, UINT, PBYTE, LPWORD, UINT >(pFunc, uVirtKey, uScanCode, lpKeyState, lpChar, uFlags);
    }

    //Return policy (U2, table row ToAsciiEx / keyboard.cpp, 12 Nov 2026): the row reads unchecked with no checkClass, so the
    //call stays bare - its docQuote is the ToAscii row's Return value section word for word, so the same reading applies: 0
    //answers "no translation" legitimately and 1/2 report the out-buffer's character count; the only difference from that
    //row is the explicit locale handle dwhkl. AUDIT: the table counts 0 in-tree callers for the exact stem, so the bare site
    //is change-safe.
    int User32::ToAsciiEx(UINT uVirtKey, UINT uScanCode, PBYTE lpKeyState, LPWORD lpChar, UINT uFlags, HKL dwhkl)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, ToAsciiEx);
        return Library::Call< int, UINT, UINT, PBYTE, LPWORD, UINT, HKL >(pFunc, uVirtKey, uScanCode, lpKeyState, lpChar, uFlags, dwhkl);
    }

    //Return policy (U2, table row ToUnicode / keyboard.cpp, 12 Nov 2026): the row reads unchecked with no checkClass, so the
    //call stays bare - docQuote "value < 0 The specified virtual key is a dead key character (accent or diacritic) ... 0 The
    //specified virtual key has no translation for the current state of the keyboard. Nothing was written to the buffer ...
    //value > 0 One or more UTF-16 code units were written to the buffer specified by pwszBuff ." - a signed count answer with
    //three legal bands, so negative / zero / positive are all answers to the caller's conversion question and no value is
    //reserved for failure; the batch marked this row differs-from-cache only because the fetched page carries the comparisons
    //as &lt; / &gt; entities while the probe cache stored the decoded < / >, wording identical. AUDIT: the table counts 0
    //in-tree callers for the exact stem, so the bare site is change-safe.
    int User32::ToUnicode(UINT wVirtKey, UINT wScanCode, const BYTE* lpKeyState, LPWSTR pwszBuff, int cchBuff, UINT wFlags)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, ToUnicode);
        return Library::Call< int, UINT, UINT, const BYTE*, LPWSTR, int, UINT >(pFunc, wVirtKey, wScanCode, lpKeyState, pwszBuff, cchBuff, wFlags);
    }

    //Return policy (U2, table row ToUnicodeEx / keyboard.cpp, 12 Nov 2026): the row reads unchecked with no checkClass, so the
    //call stays bare - its docQuote is the ToUnicode row's Return value section word for word, so the same signed-count reading
    //applies: negative (dead key) / zero (no translation) / positive (UTF-16 words written) are all legal answers and nothing
    //is reserved; the page also warns the buffer may hold more code units than the count names, whose extras are invalid.
    //AUDIT: the table counts 0 in-tree callers for the exact stem, so the bare site is change-safe.
    int User32::ToUnicodeEx(UINT wVirtKey, UINT wScanCode, const BYTE* lpKeyState, LPWSTR pwszBuff, int cchBuff, UINT wFlags, HKL dwhkl)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, ToUnicodeEx);
        return Library::Call< int, UINT, UINT, const BYTE*, LPWSTR, int, UINT, HKL >(pFunc, wVirtKey, wScanCode, lpKeyState, pwszBuff, cchBuff, wFlags, dwhkl);
    }

    //Return policy (U2, table row UnloadKeyboardLayout / keyboard.cpp, 12 Nov 2026): the row reads checked with checkClass
    //BoolCheck - docQuote "If the function succeeds, the return value is nonzero. If the function fails, the return value is
    //zero. The function can fail for the following reasons: An invalid input locale identifier was passed. The input locale
    //identifier was preloaded. The input locale identifier is in use. To get extended error information, call GetLastError ."
    //- the locale identifier hkl is a parameter, so the BOOL carries no answer and zero is reserved, with the failure causes
    //named on the page; the twin of the LoadKeyboardLayout row, which is checked on the handle shape. sdkRetType BOOL so BoolCheck
    //(returncheck.h:94) covers it - no new check class. AUDIT: the table counts 0 in-tree callers for the exact stem, so the
    //token move is change-safe.
    BOOL User32::UnloadKeyboardLayout(HKL hkl)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, UnloadKeyboardLayout);
        CheckReturn< BOOL, BoolCheck >::TType hResult = Library::Call< BOOL, HKL >(pFunc, hkl);
        return hResult;
    }

    //Return policy (U2, table row UnregisterHotKey / keyboard.cpp, 12 Nov 2026): the row reads checked with checkClass BoolCheck
    //- docQuote "If the function succeeds, the return value is nonzero. If the function fails, the return value is zero. To get
    //extended error information, call GetLastError ." - the hot key and the window are parameters, so the BOOL carries no answer
    //of its own and zero is reserved for a failed deregistration; the twin of the RegisterHotKey row above. sdkRetType BOOL so
    //BoolCheck (returncheck.h:94) covers it - no new check class. AUDIT: the table counts 1 in-tree caller and the site-level
    //read finds it at ui/gui/controllers/windowcontroller.cpp:87 as "return User32::UnregisterHotKey(WINDOW_HANDLE, id) ? true : false;"
    //in WindowController::UnregisterHotKey; CheckReturn returns the value unchanged, so the returned bool is identical and only
    //a diagnostic is added at a documented failure.
    BOOL User32::UnregisterHotKey(HWND hWnd, int id)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, UnregisterHotKey);
        CheckReturn< BOOL, BoolCheck >::TType hResult = Library::Call< BOOL, HWND, int >(pFunc, hWnd, id);
        return hResult;
    }

    //Return policy (U2, table row VkKeyScan / keyboard.cpp, 12 Nov 2026): the row reads unchecked with no checkClass, so the
    //call stays bare - docQuote "If the function succeeds, the low-order byte of the return value contains the virtual-key code
    //and the high-order byte contains the shift state, which can be a combination of the following flag bits. ... If the function
    //finds no key that translates to the passed character code, both the low-order and high-order bytes contain -1." - the return
    //is the packed virtual-key / shift-state answer and all-(-1) bytes answer "no key translates" as an informational outcome
    //the caller acts on, so nothing reserves failure; same shape as the OemKeyScan row above. AW row with NO-PROTOTYPE, fetched
    //via the suffixed twin slug nf-winuser-vkkeyscana. AUDIT: the table counts 0 in-tree callers for the exact stem, so the
    //bare site is change-safe.
    SHORT User32::VkKeyScanT(TCHAR ch)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapiAW(user32, VkKeyScan);
        return Library::Call< SHORT, TCHAR >(pFunc, ch);
    }

    //Return policy (U2, table row VkKeyScanEx / keyboard.cpp, 12 Nov 2026): the row reads unchecked with no checkClass, so the
    //call stays bare - its docQuote is the VkKeyScan row's Return value section word for word, so the same reading applies: the
    //return is the packed virtual-key / shift-state answer and all-(-1) bytes answer "no key translates" legitimately; the only
    //difference from that row is the explicit locale handle dwhkl. AW row with NO-PROTOTYPE, fetched via the suffixed twin slug
    //nf-winuser-vkkeyscanexa. AUDIT: the table counts 0 in-tree callers for the exact stem, so the bare site is change-safe.
    SHORT User32::VkKeyScanExT(TCHAR ch, HKL dwhkl)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapiAW(user32, VkKeyScanEx);
        return Library::Call< SHORT, TCHAR, HKL >(pFunc, ch, dwhkl);
    }

}}}//qor::win::api
