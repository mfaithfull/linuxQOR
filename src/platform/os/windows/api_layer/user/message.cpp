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
    //Twenty-four wrappers - the whole "user32.h:289 //Message and Message Queue functions" section, declared :290-313 and
    //defined here in header order (:106 BroadcastSystemMessageT .. :267 WaitMessage). All 24 declarations are static and all
    //24 are defined here with the User32:: qualifier, so 0 U4, 0 U11 and 0 U13 instances, and the decl-vs-def signature
    //compare over the section gives 0 flags (tmp/_tmp_msg_pre_s23.txt).
    //
    //Binding (D13): twelve stems have an SDK A/W family and bind through qor_pp_useswinapiAW; the other twelve bind bare,
    //and each of those twelve is a genuine single export - tmp/_tmp_msg_class_s23.ps1 checks every stem against the export
    //table (tmp/_tmp_user32_exports.txt, 1018 names) and against winuser.h, and for all twelve the bare name is exported
    //while no <stem>A/<stem>W spelling exists and winuser.h defines no "<stem> -> <stem>W" macro, so bare is the only
    //spelling that could ever resolve. The twelve AW stems are exactly the twelve with a live winuser.h macro, and none of
    //their bare spellings is an export except BroadcastSystemMessage, so bare there would be a run-time throw (G1/G2).
    //Four of the twelve were bare until this session - RegisterWindowMessage (:214), SendMessageTimeout (:235), SendMessage
    //(:242) and SendNotifyMessage (:249), the whole remaining pre-scan P2 list. That class is latent, not broken: the SDK
    //macro rewrote the bare stem before it was stringized, so cl /EP emitted the same four "*W" literals before the change
    //as after it, now reached by ##W pasting (kernel32.h:52) instead of by the SDK macro - behaviour-neutral, as with
    //dialog.cpp's MessageBox (session 13), the combobox pair and menu's pair. Proof: pre- and post-fix /EP over this TU
    //differ in two lines only, and both are the InSendMessageEx(void) fix below (tmp/_tmp_msg_pp_pre_s23.txt against
    //tmp/_tmp_msg_pp_post_s23.txt). Post-fix this TU holds 0 of the folder's unresolvable bindings: the folder census goes
    //15 -> 11 (tmp/_tmp_p1census_post_s23.txt) with no message.cpp line left in it, and cl /EP over this TU yields 24
    //literals, all 24 in the export table.
    //
    //One P6 shape is present and avoided: bare BroadcastSystemMessage IS exported (ordinal 14) at RVA 00092270 - the same
    //RVA as BroadcastSystemMessageA - while BroadcastSystemMessageW is a different entry point at 00007240. A bare binding
    //there would resolve and run the ANSI entry point on wide arguments instead of throwing, which is the CallMsgFilter
    //trap (registers/p6-u27.md); the AW binding asks for the W name, so the trap does not apply to this wrapper.
    //
    //Signatures: 0 return-type divergences over all 24. tmp/_tmp_msg_ret_s23.ps1 reads the SDK return type off its own
    //line above the stem (winuser.h puts it there, so a declaration-line instrument cannot see it - the session-22
    //caveat): 11 BOOL, 3 LRESULT (DispatchMessageT, SendMessageT, SendMessageTimeoutT), 2 long (BroadcastSystemMessageT,
    //BroadcastSystemMessageExT), 2 LPARAM (GetMessageExtraInfo, SetMessageExtraInfo), 3 DWORD (GetMessagePos,
    //GetQueueStatus, InSendMessageEx), 1 LONG (GetMessageTime), 1 UINT (RegisterWindowMessageT), 1 VOID (PostQuitMessage)
    //- every one agrees with the SDK, so 0 U18. Every parameter type agrees too; the one deliberate deviation is
    //InSendMessageEx (:176), published with no parameter where the SDK has _Reserved_ LPVOID lpReserved (winuser.h:4253).
    //The Call<DWORD, LPVOID> list passes nullptr into that reserved slot, so the call shape matches the export; the
    //pre-flag "CALL TYPE MISMATCH" on this line was an instrument artifact - the instrument read this library's own
    //prototype as the SDK side. PostQuitMessage keeps the SDK's VOID return and uses voidCall<int>.
    //
    //U14: (b) 24 of 24 Call/voidCall sites explicit, tight style, so 0 sites in the folder's deducing list. (c) this TU
    //held the folder's last zero-argument "()" - InSendMessageEx at :176 is now "(void)", declaration user32.h:300, which
    //closes U14(c): the folder now measures 0 definitions and 0 declarations spelling a zero-argument list "()".
    //
    //U26 (the hand-off from session 21): four of the 24 take the SDK MSG type - DispatchMessageT and TranslateMessage as
    //const MSG*, GetMessageT and PeekMessageT as LPMSG - and GetMessage/PeekMessage WRITE the struct the caller supplies.
    //The tree's only compiled caller (ui/gui/view/handlers/messagehandler.cpp:136, :146, :181, :236) hands in a
    //ui/gui/view/handlers/message.h Message* through reinterpret_cast. Re-measured here for both architectures with
    //tmp/_tmp_msg_mirror_s21.bat: on x64 the mirror and MSG are identical (48 bytes, same field offsets), on x86 MSG is
    //28 bytes and the mirror 40, diverging from lParam onward, so on a 32-bit target the export would fill 28 bytes while
    //the caller reads 40 - wrong lParam, time and pt, with no compile error. Only x64 is configured, so it stays latent;
    //recorded in registers/u26.md, not repaired inside a TU session. RegisterWindowMessageT(LPCTSTR) is this TU's one
    //TCHAR-width parameter and joins the same entry's *T-surface list: the binding is the W one, so a consumer that saw
    //<windows.h> before kernel32.h gets the narrow mangled name and fails at the link.
    //
    //U17: 0 instances - all twelve A/W wrappers already carry the T suffix. U25: this section spells the const pointer
    //"const MSG*" with no scope qualifier at all (definitions :120 and :260, Call<> arguments :124 and :264, declarations
    //spelling alongside the counted "CONST::X*" (17) and "const ::X*" (7) families - user32.h:292, :312. Same denoted type,
    //so no defect - it belongs with the folder-wide spelling pass, not with this TU.
    //
    //U16: 4 of the 24 pair with version-guarded SDK declarations (tmp/_tmp_msg_guards_s23.txt) - SetMessageExtraInfo
    //inside #if(WINVER >= 0x0400) (winuser.h:3683), BroadcastSystemMessage inside #if(WINVER >= 0x0400) (:3863),
    //BroadcastSystemMessageEx inside #if(_WIN32_WINNT >= 0x0501) (:3825) and InSendMessageEx inside
    //#if(WINVER >= 0x0500) (:4245); the other twenty sit under nothing but NOUSER and the Desktop-family partition, and
    //GetInputState/GetQueueStatus additionally under WINAPI_PARTITION_PC_APP (:6892). Nothing in this build pins
    //WINVER/_WIN32_WINNT, so the exposure is the folder-wide one. The sharper guard here is a type guard: the MSG/LPMSG
    //parameters of the four MSG-taking wrappers exist only while NOMSG is undefined, and NOMSG is defined nowhere in this
    //build - a consumer that defines it breaks this library's published header outright (the shape first seen at
    //user32.h:498 in session 21, which pointed the case here).
    //
    //U21 (fifth instance): the section is not one SDK family. tmp/_tmp_msg_regions_s23.ps1 walks the SDK's own region
    //markers with a depth counter: the #ifndef NOMSG region that owns this section's core is winuser.h:3395-3552 and holds
    //only 5 of the 24 stems (GetMessage, PeekMessage, DispatchMessage, TranslateMessage and SetMessageQueue), while the
    //other 19 pair with declarations in four other guard regions - RegisterWindowMessage in the NOWINMESSAGES region
    //(:1976-:2787), the send/post family unguarded by any message macro (:3695-:4260), and GetInputState/GetQueueStatus
    //at :6892. Conversely that NOMSG region's other members are parked in four other sections of user32.h and defined in
    //four other TUs (CallMsgFilterT -> hook.cpp, TranslateAcceleratorT -> accelerator.cpp, IsDialogMessageT ->
    //dialog.cpp, TranslateMDISysAccel -> mdi.cpp), and one member of the region, SetMessageQueue (export ordinal 351 at
    //0008EF10), is declared nowhere in this library at all. Auditing the message surface from this TU therefore
    //under-states both the section and the region. Recorded, not moved.
    //
    //U19: measured over all 60 stem/suffix spellings with tmp/_tmp_msg_bypass_s23.ps1 - 172 through-layer, 12
    //intermediate-qualified, 68 SDK-bypass, 2 member-call. SendMessage dominates (147 layer sites, mostly the
    //ui/gui/controllers classes, 48 bypass). Every bypass is in a file no target builds: retropad is commented out at
    //examples/experimental/osspecific/windows/CMakeLists.txt:7, and ui/gl/helperwindow.cpp and ui/gl/platform.cpp are not
    //in winqor_gl's source list (its CMakeLists names winqor_gl.cpp, cursorimpl.cpp, windowimpl.cpp, wgl.cpp). The three
    //"GetMessage" bypass hits (src/framework/event/events/eventsource.h:20 and the two eventer example sites the scan
    //reports as MEMBER) are the framework event Source's own member function, not the SDK call. So 0 bypass of this
    //section in built code. U15 is not measured (D14, closed 2 Nov 2026).
    //
    //Sites a future return-check policy would take (policy settled 29 Oct 2026 - leave as-is, record only): the 11
    //BOOL-returning wrappers plus PostQuitMessage's voidCall. A Win32 FALSE from GetMessage/PeekMessage/PostMessage is
    //therefore silent here, and an SEH fault is indistinguishable from a legitimate zero (U1, U2, U7).

    long User32::BroadcastSystemMessageT(DWORD flags, LPDWORD lpInfo, UINT Msg, WPARAM wParam, LPARAM lParam)
    {
        qor_pp_fcontext;
        qor_pp_useswinapiAW(user32, BroadcastSystemMessage);
        return Library::Call<long, DWORD, LPDWORD, UINT, WPARAM, LPARAM>(pFunc, flags, lpInfo, Msg, wParam, lParam);
    }

    long User32::BroadcastSystemMessageExT(DWORD flags, LPDWORD lpInfo, UINT Msg, WPARAM wParam, LPARAM lParam, ::PBSMINFO pbsmInfo)
    {
        qor_pp_fcontext;
        qor_pp_useswinapiAW(user32, BroadcastSystemMessageEx);
        return Library::Call<long, DWORD, LPDWORD, UINT, WPARAM, LPARAM, ::PBSMINFO>(pFunc, flags, lpInfo, Msg, wParam, lParam, pbsmInfo);
    }

    LRESULT User32::DispatchMessageT(const MSG* lpMsg)
    {
        qor_pp_fcontext;
        qor_pp_useswinapiAW(user32, DispatchMessage);
        return Library::Call<LRESULT, const MSG*>(pFunc, lpMsg);
    }

    BOOL User32::GetInputState(void)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, GetInputState);
        return Library::Call<BOOL>(pFunc);
    }

    BOOL User32::GetMessageT(LPMSG lpMsg, HWND hWnd, UINT wMsgFilterMin, UINT wMsgFilterMax)
    {
        qor_pp_fcontext;
        qor_pp_useswinapiAW(user32, GetMessage);
        return Library::Call<BOOL, LPMSG, HWND, UINT, UINT>(pFunc, lpMsg, hWnd, wMsgFilterMin, wMsgFilterMax);
    }

    LPARAM User32::GetMessageExtraInfo(void)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, GetMessageExtraInfo);
        return Library::Call<LPARAM>(pFunc);
    }

    DWORD User32::GetMessagePos(void)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, GetMessagePos);
        return Library::Call<DWORD>(pFunc);
    }

    LONG User32::GetMessageTime(void)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, GetMessageTime);
        return Library::Call<LONG>(pFunc);
    }

    DWORD User32::GetQueueStatus(UINT flags)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, GetQueueStatus);
        return Library::Call<DWORD, UINT>(pFunc, flags);
    }

    BOOL User32::InSendMessage(void)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, InSendMessage);
        return Library::Call<BOOL>(pFunc);
    }

    DWORD User32::InSendMessageEx(void)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, InSendMessageEx);
        return Library::Call<DWORD, LPVOID>(pFunc, nullptr);
    }

    BOOL User32::PeekMessageT(LPMSG lpMsg, HWND hWnd, UINT wMsgFilterMin, UINT wMsgFilterMax, UINT wRemoveMsg)
    {
        qor_pp_fcontext;
        qor_pp_useswinapiAW(user32, PeekMessage);
        return Library::Call<BOOL, LPMSG, HWND, UINT, UINT, UINT>(pFunc, lpMsg, hWnd, wMsgFilterMin, wMsgFilterMax, wRemoveMsg);
    }

    BOOL User32::PostMessageT(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
    {
        qor_pp_fcontext;
        qor_pp_useswinapiAW(user32, PostMessage);
        return Library::Call<BOOL, HWND, UINT, WPARAM, LPARAM>(pFunc, hWnd, Msg, wParam, lParam);
    }

    void User32::PostQuitMessage(int nExitCode)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, PostQuitMessage);
        return Library::voidCall<int>(pFunc, nExitCode);
    }

    BOOL User32::PostThreadMessageT(DWORD idThread, UINT Msg, WPARAM wParam, LPARAM lParam)
    {
        qor_pp_fcontext;
        qor_pp_useswinapiAW(user32, PostThreadMessage);
        return Library::Call<BOOL, DWORD, UINT, WPARAM, LPARAM>(pFunc, idThread, Msg, wParam, lParam);
    }

    UINT User32::RegisterWindowMessageT(LPCTSTR lpString)
    {
        qor_pp_fcontext;
        qor_pp_useswinapiAW(user32, RegisterWindowMessage);
        return Library::Call<UINT, LPCTSTR>(pFunc, lpString);
    }

    BOOL User32::ReplyMessage(LRESULT lResult)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, ReplyMessage);
        return Library::Call<BOOL, LRESULT>(pFunc, lResult);
    }

    BOOL User32::SendMessageCallbackT(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam, ::SENDASYNCPROC lpResultCallBack, ULONG_PTR dwData)
    {
        qor_pp_fcontext;
        qor_pp_useswinapiAW(user32, SendMessageCallback);
        return Library::Call<BOOL, HWND, UINT, WPARAM, LPARAM, ::SENDASYNCPROC, ULONG_PTR>(pFunc, hWnd, Msg, wParam, lParam, lpResultCallBack, dwData);
    }

    LRESULT User32::SendMessageTimeoutT(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam, UINT fuFlags, UINT uTimeout, PDWORD_PTR lpdwResult)
    {
        qor_pp_fcontext;
        qor_pp_useswinapiAW(user32, SendMessageTimeout);
        return Library::Call<LRESULT, HWND, UINT, WPARAM, LPARAM, UINT, UINT, PDWORD_PTR>(pFunc, hWnd, Msg, wParam, lParam, fuFlags, uTimeout, lpdwResult);
    }

    LRESULT User32::SendMessageT(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
    {
        qor_pp_fcontext;
        qor_pp_useswinapiAW(user32, SendMessage);
        return Library::Call<LRESULT, HWND, UINT, WPARAM, LPARAM>(pFunc, hWnd, Msg, wParam, lParam);
    }

    BOOL User32::SendNotifyMessageT(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
    {
        qor_pp_fcontext;
        qor_pp_useswinapiAW(user32, SendNotifyMessage);
        return Library::Call<BOOL, HWND, UINT, WPARAM, LPARAM>(pFunc, hWnd, Msg, wParam, lParam);
    }

    LPARAM User32::SetMessageExtraInfo(LPARAM lParam)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, SetMessageExtraInfo);
        return Library::Call<LPARAM, LPARAM>(pFunc, lParam);
    }

    BOOL User32::TranslateMessage(const MSG* lpMsg)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, TranslateMessage);
        return Library::Call<BOOL, const MSG*>(pFunc, lpMsg);
    }

    BOOL User32::WaitMessage(void)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, WaitMessage);
        return Library::Call<BOOL>(pFunc);
    }

}}}//qor::win::api
