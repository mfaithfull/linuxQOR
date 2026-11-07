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

    HKL User32::ActivateKeyboardLayout(HKL hkl, UINT Flags)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, ActivateKeyboardLayout);
        return Library::Call< HKL, HKL, UINT >(pFunc, hkl, Flags);
    }

    BOOL User32::BlockInput(BOOL fBlockIt)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, BlockInput);
        return Library::Call< BOOL, BOOL >(pFunc, fBlockIt);
    }

    BOOL User32::EnableWindow(HWND hWnd, BOOL bEnable)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, EnableWindow);
        return Library::Call< BOOL, HWND, BOOL >(pFunc, hWnd, bEnable);
    }

    HWND User32::GetActiveWindow(void)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, GetActiveWindow);
        return Library::Call< HWND >(pFunc);
    }

    SHORT User32::GetAsyncKeyState(int vKey)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, GetAsyncKeyState);
        return Library::Call< SHORT, int >(pFunc, vKey);
    }

    HWND User32::GetFocus(void)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, GetFocus);
        return Library::Call< HWND >(pFunc);
    }

    UINT User32::GetKBCodePage(void)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, GetKBCodePage);
        return Library::Call< UINT >(pFunc);
    }

    HKL User32::GetKeyboardLayout(DWORD idThread)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, GetKeyboardLayout);
        return Library::Call< HKL, DWORD >(pFunc, idThread);
    }

    int User32::GetKeyboardLayoutList(int nBuff, HKL* lpList)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, GetKeyboardLayoutList);
        return Library::Call< int, int, HKL* >(pFunc, nBuff, lpList);
    }

    BOOL User32::GetKeyboardLayoutNameT(LPTSTR pwszKLID)
    {
        qor_pp_fcontext;
        qor_pp_useswinapiAW(user32, GetKeyboardLayoutName);
        return Library::Call< BOOL, LPTSTR >(pFunc, pwszKLID);
    }

    BOOL User32::GetKeyboardState(PBYTE lpKeyState)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, GetKeyboardState);
        return Library::Call< BOOL, PBYTE >(pFunc, lpKeyState);
    }

    int User32::GetKeyboardType(int nTypeFlag)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, GetKeyboardType);
        return Library::Call< int, int >(pFunc, nTypeFlag);
    }

    int User32::GetKeyNameTextT(LONG lParam, LPTSTR lpString, int cchSize)
    {
        qor_pp_fcontext;
        qor_pp_useswinapiAW(user32, GetKeyNameText);
        return Library::Call< int, LONG, LPTSTR, int >(pFunc, lParam, lpString, cchSize);
    }

    SHORT User32::GetKeyState(int nVirtKey)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, GetKeyState);
        return Library::Call< SHORT, int >(pFunc, nVirtKey);
    }

    BOOL User32::GetLastInputInfo(PLASTINPUTINFO plii)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, GetLastInputInfo);
        return Library::Call< BOOL, PLASTINPUTINFO >(pFunc, plii);
    }

    BOOL User32::IsWindowEnabled(HWND hWnd)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, IsWindowEnabled);
        return Library::Call< BOOL, HWND >(pFunc, hWnd);
    }

    VOID User32::keybd_event(BYTE bVk, BYTE bScan, DWORD dwFlags, ULONG_PTR dwExtraInfo)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, keybd_event);
        return Library::voidCall< BYTE, BYTE, DWORD, ULONG_PTR >(pFunc, bVk, bScan, dwFlags, dwExtraInfo);
    }

    HKL User32::LoadKeyboardLayoutT(LPCTSTR pwszKLID, UINT Flags)
    {
        qor_pp_fcontext;
        qor_pp_useswinapiAW(user32, LoadKeyboardLayout);
        return Library::Call< HKL, LPCTSTR, UINT >(pFunc, pwszKLID, Flags);
    }

    UINT User32::MapVirtualKeyT(UINT uCode, UINT uMapType)
    {
        qor_pp_fcontext;
        qor_pp_useswinapiAW(user32, MapVirtualKey);
        return Library::Call< UINT, UINT, UINT >(pFunc, uCode, uMapType);
    }

    UINT User32::MapVirtualKeyExT(UINT uCode, UINT uMapType, HKL dwhkl)
    {
        qor_pp_fcontext;
        qor_pp_useswinapiAW(user32, MapVirtualKeyEx);
        return Library::Call< UINT, UINT, UINT, HKL >(pFunc, uCode, uMapType, dwhkl);
    }

    DWORD User32::OemKeyScan(WORD wOemChar)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, OemKeyScan);
        return Library::Call< DWORD, WORD >(pFunc, wOemChar);
    }

    BOOL User32::RegisterHotKey(HWND hWnd, int id, UINT fsModifiers, UINT vk)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, RegisterHotKey);
        return Library::Call< BOOL, HWND, int, UINT, UINT >(pFunc, hWnd, id, fsModifiers, vk);
    }

    UINT User32::SendInput(UINT cInputs, LPINPUT pInputs, int cbSize)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, SendInput);
        return Library::Call< UINT, UINT, LPINPUT, int >(pFunc, cInputs, pInputs, cbSize);
    }

    HWND User32::SetActiveWindow(HWND hWnd)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, SetActiveWindow);
        return Library::Call< HWND, HWND >(pFunc, hWnd);
    }

    HWND User32::SetFocus(HWND hWnd)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, SetFocus);
        return Library::Call< HWND, HWND >(pFunc, hWnd);
    }

    BOOL User32::SetKeyboardState(LPBYTE lpKeyState)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, SetKeyboardState);
        return Library::Call< BOOL, LPBYTE >(pFunc, lpKeyState);
    }

    int User32::ToAscii(UINT uVirtKey, UINT uScanCode, PBYTE lpKeyState, LPWORD lpChar, UINT uFlags)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, ToAscii);
        return Library::Call< int, UINT, UINT, PBYTE, LPWORD, UINT >(pFunc, uVirtKey, uScanCode, lpKeyState, lpChar, uFlags);
    }

    int User32::ToAsciiEx(UINT uVirtKey, UINT uScanCode, PBYTE lpKeyState, LPWORD lpChar, UINT uFlags, HKL dwhkl)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, ToAsciiEx);
        return Library::Call< int, UINT, UINT, PBYTE, LPWORD, UINT, HKL >(pFunc, uVirtKey, uScanCode, lpKeyState, lpChar, uFlags, dwhkl);
    }

    int User32::ToUnicode(UINT wVirtKey, UINT wScanCode, const BYTE* lpKeyState, LPWSTR pwszBuff, int cchBuff, UINT wFlags)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, ToUnicode);
        return Library::Call< int, UINT, UINT, const BYTE*, LPWSTR, int, UINT >(pFunc, wVirtKey, wScanCode, lpKeyState, pwszBuff, cchBuff, wFlags);
    }

    int User32::ToUnicodeEx(UINT wVirtKey, UINT wScanCode, const BYTE* lpKeyState, LPWSTR pwszBuff, int cchBuff, UINT wFlags, HKL dwhkl)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, ToUnicodeEx);
        return Library::Call< int, UINT, UINT, const BYTE*, LPWSTR, int, UINT, HKL >(pFunc, wVirtKey, wScanCode, lpKeyState, pwszBuff, cchBuff, wFlags, dwhkl);
    }

    BOOL User32::UnloadKeyboardLayout(HKL hkl)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, UnloadKeyboardLayout);
        return Library::Call< BOOL, HKL >(pFunc, hkl);
    }

    BOOL User32::UnregisterHotKey(HWND hWnd, int id)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, UnregisterHotKey);
        return Library::Call< BOOL, HWND, int >(pFunc, hWnd, id);
    }

    SHORT User32::VkKeyScanT(TCHAR ch)
    {
        qor_pp_fcontext;
        qor_pp_useswinapiAW(user32, VkKeyScan);
        return Library::Call< SHORT, TCHAR >(pFunc, ch);
    }

    SHORT User32::VkKeyScanExT(TCHAR ch, HKL dwhkl)
    {
        qor_pp_fcontext;
        qor_pp_useswinapiAW(user32, VkKeyScanEx);
        return Library::Call< SHORT, TCHAR, HKL >(pFunc, ch, dwhkl);
    }

}}}//qor::win::api
