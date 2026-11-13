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

    //Return policy (U2, table row BroadcastSystemMessage / message.cpp, 12 Nov 2026): the row reads checked with checkClass
    //TCheckFailureValue< long, -1 > - docQuote "Type: long If the function succeeds, the return value is a positive value. If the
    //function is unable to broadcast the message, the return value is –1. If the dwFlags parameter is BSF_QUERY and at least one
    //recipient returned BROADCAST_QUERY_DENY to the corresponding message, the return value is zero. To get extended error
    //information, call GetLastError ." so the check keys on -1 alone: zero is the documented legal answer for a denied query and
    //keying CheckNonZero< long > would report a legal denial as a failure. sdkRetType long so the base is long and TCheckFailureValue
    //(returncheck.h:213, Test is (value == iFailure) ? false : true) is already in the vocabulary - no new check class. 0 in-tree
    //callers for the exact stem (D14 published surface), so nothing built today reaches this site; G6 keeps the check live on
    //every call, so a broadcast that cannot run now raises a continuable issue the Win32ErrorHandler above reports rather than
    //travelling as a silent -1.
    long User32::BroadcastSystemMessageT(DWORD flags, LPDWORD lpInfo, UINT Msg, WPARAM wParam, LPARAM lParam)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapiAW(user32, BroadcastSystemMessage);
        CheckReturn< long, TCheckFailureValue< long, -1 > >::TType lResult = Library::Call<long, DWORD, LPDWORD, UINT, WPARAM, LPARAM>(pFunc, flags, lpInfo, Msg, wParam, lParam);
        return lResult;
    }

    //Return policy (U2, table row BroadcastSystemMessageEx / message.cpp, 12 Nov 2026): the row reads checked with checkClass
    //TCheckFailureValue< long, -1 > - its page (reached via the suffixed ANSI-twin slug nf-winuser-broadcastsystemmessageexa)
    //carries the BroadcastSystemMessage row's Return value section word for word, so the same reading applies: -1 is reserved
    //for "unable to broadcast", zero is the legal BSF_QUERY denial answer, positive is the recipient count. The row's sdkRetType
    //is NO-PROTOTYPE, so the base is the published long and TCheckFailureValue< long, -1 > (returncheck.h:213) covers it - no
    //new check class. 0 in-tree callers for the exact stem (D14 published surface), so nothing built today reaches this site;
    //G6 keeps the check live on every call, so a broadcast that cannot run now raises a continuable issue the Win32ErrorHandler
    //above reports rather than travelling as a silent -1.
    long User32::BroadcastSystemMessageExT(DWORD flags, LPDWORD lpInfo, UINT Msg, WPARAM wParam, LPARAM lParam, ::PBSMINFO pbsmInfo)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapiAW(user32, BroadcastSystemMessageEx);
        CheckReturn< long, TCheckFailureValue< long, -1 > >::TType lResult = Library::Call<long, DWORD, LPDWORD, UINT, WPARAM, LPARAM, ::PBSMINFO>(pFunc, flags, lpInfo, Msg, wParam, lParam, pbsmInfo);
        return lResult;
    }

    //Return policy (U2, table row DispatchMessage / message.cpp, 12 Nov 2026): the row reads unchecked with no checkClass - the
    //table's verbatim docQuote "Type: LRESULT The return value specifies the value returned by the window procedure. Although
    //its meaning depends on the message being dispatched, the return value generally is ignored." means the return is whatever
    //the window procedure the caller dispatched chose to answer with, so every value is a legal answer and the page names no
    //failure value at all (D51 step 2 reads no-check); 0 in-tree callers for the exact stem (D14 published surface), so the
    //row is change-safe and no site audit is owed; the token stays unlanded.
    LRESULT User32::DispatchMessageT(const MSG* lpMsg)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapiAW(user32, DispatchMessage);
        return Library::Call<LRESULT, const MSG*>(pFunc, lpMsg);
    }

    //Return policy (U2, table row GetInputState / message.cpp, 12 Nov 2026): the row reads unchecked with no checkClass - the
    //table's verbatim docQuote "Type: BOOL If the queue contains one or more new mouse-button or keyboard messages, the
    //return value is nonzero. If there are no new mouse-button or keyboard messages in the queue, the return value is
    //zero." is a question about the queue, not a status: the zero answers the caller's question, an empty queue being a
    //legal state, so nothing is reserved and keying BoolCheck would report a legal empty queue as a failure; the row's
    //sdkRetType is BOOL, so BoolCheck would have type-matched and the reading is the only reason it is not landed. AUDIT:
    //1 in-tree caller, so the site-level read is owed before any token moves - none is landed.
    BOOL User32::GetInputState(void)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, GetInputState);
        return Library::Call<BOOL>(pFunc);
    }

    //Return policy (U2, table row GetMessage / message.cpp, 12 Nov 2026): the row reads checked with checkClass
    //TCheckFailureValue< BOOL, -1 > - docQuote "Type: BOOL If the function retrieves a message other than WM_QUIT , the return
    //value is nonzero. If the function retrieves the WM_QUIT message, the return value is zero. If there is an error, the return
    //value is -1. For example, the function fails if hWnd is an invalid window handle or lpMsg is an invalid pointer." and the
    //page then warns against exactly the naive test - "Because the return value can be nonzero, zero, or -1, avoid code like this:
    //while (GetMessage( lpMsg, hWnd, 0, 0))" with its own recommended test "while( (bRet = GetMessage( &msg, hWnd, 0, 0 )) != 0)
    //{ ... if (bRet == -1) { // handle the error and possibly exit }" - so zero is a legal outcome (the WM_QUIT loop terminator)
    //and BoolCheck / CheckNonZero would report it as a failure, while the page's error test is exactly -1. The row's sdkRetType
    //is NO-PROTOTYPE, so the base is the published BOOL, which the preprocessed message TU shows as typedef int BOOL
    //(_tmp_msg_pp_final_s23.txt:145773), so -1 survives the comparison and TCheckFailureValue< BOOL, -1 > (returncheck.h:213)
    //flags only the documented error - no new check class. 0 in-tree callers for the exact stem (D14 published surface); G6
    //keeps the check live on every call, so an invalid hWnd now raises a continuable issue the Win32ErrorHandler above reports
    //rather than travelling as a silent -1 the caller's loop cannot tell from WM_QUIT.
    BOOL User32::GetMessageT(LPMSG lpMsg, HWND hWnd, UINT wMsgFilterMin, UINT wMsgFilterMax)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapiAW(user32, GetMessage);
        CheckReturn< BOOL, TCheckFailureValue< BOOL, -1 > >::TType bResult = Library::Call<BOOL, LPMSG, HWND, UINT, UINT>(pFunc, lpMsg, hWnd, wMsgFilterMin, wMsgFilterMax);
        return bResult;
    }

    //Return policy (U2, table row GetMessageExtraInfo / message.cpp, 12 Nov 2026): the row reads unchecked with no checkClass - the
    //table's verbatim docQuote "Type: LPARAM The return value specifies the extra information. The meaning of the extra
    //information is device specific." is the whole Return value section, so D51 step 2 reads no-check: the value is whatever
    //the caller put there with SetMessageExtraInfo and no value is reserved. AUDIT: 1 in-tree caller, so the site-level read
    //is owed before any token moves - none is landed.
    LPARAM User32::GetMessageExtraInfo(void)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, GetMessageExtraInfo);
        return Library::Call<LPARAM>(pFunc);
    }

    //Return policy (U2, table row GetMessagePos / message.cpp, 12 Nov 2026): the row reads unchecked with no checkClass - the
    //table's verbatim docQuote "Type: DWORD The return value specifies the x- and y-coordinates of the cursor position. The
    //x-coordinate is the low order short and the y-coordinate is the high-order short ." is a packed coordinate answer with
    //no documented failure value, so D51 step 2 reads no-check: a cursor position with a zero word is a legal coordinate and
    //the page names no failure value. AUDIT: 1 in-tree caller, so the site-level read is owed before any token moves - none
    //is landed.
    DWORD User32::GetMessagePos(void)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, GetMessagePos);
        return Library::Call<DWORD>(pFunc);
    }

    //Return policy (U2, table row GetMessageTime / message.cpp, 12 Nov 2026): the row reads unchecked with no checkClass - the
    //table's verbatim docQuote "Type: LONG The return value specifies the message time." is the whole Return value section,
    //so D51 step 2 reads no-check: the stamp is a time value and the page reserves nothing. AUDIT: 1 in-tree caller, so the
    //site-level read is owed before any token moves - none is landed.
    LONG User32::GetMessageTime(void)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, GetMessageTime);
        return Library::Call<LONG>(pFunc);
    }

    //Return policy (U2, table row GetQueueStatus / message.cpp, 12 Nov 2026): the row reads unchecked with no checkClass - the
    //table's verbatim docQuote "Type: DWORD The high-order word of the return value indicates the types of messages currently
    //in the queue. The low-order word indicates the types of messages that have been added to the queue and that are still in
    //the queue since the last call to the GetQueueStatus , GetMessage , or PeekMessage function." is an answer-shaped packed
    //state: a word that is zero reports that no such messages are queued, which is the answer the caller wants, so nothing
    //is reserved and keying CheckNonZero< DWORD > would report a legal empty queue as a failure. AUDIT: 1 in-tree caller,
    //so the site-level read is owed before any token moves - none is landed.
    DWORD User32::GetQueueStatus(UINT flags)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, GetQueueStatus);
        return Library::Call<DWORD, UINT>(pFunc, flags);
    }

    //Return policy (U2, table row InSendMessage / message.cpp, 12 Nov 2026): the row reads unchecked with no checkClass - the
    //table's verbatim docQuote "Type: BOOL If the window procedure is processing a message sent to it from another thread
    //using the SendMessage function, the return value is nonzero. If the window procedure is not processing a message sent
    //to it from another thread using the SendMessage function, the return value is zero." is a question about the message
    //being processed, not a status: the ordinary case is a window procedure handling a local message, which answers zero
    //legitimately, so nothing is reserved. AUDIT: 1 in-tree caller, so the site-level read is owed before any token moves -
    //none is landed.
    BOOL User32::InSendMessage(void)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, InSendMessage);
        return Library::Call<BOOL>(pFunc);
    }

    //Return policy (U2, table row InSendMessageEx / message.cpp, 12 Nov 2026): the row reads unchecked with no checkClass - the
    //table's verbatim docQuote "Type: DWORD If the message was not sent, the return value is ISMEX_NOSEND (0x00000000).
    //Otherwise, the return value is one or more of the following values. ISMEX_CALLBACK 0x00000004 ... ISMEX_NOTIFY
    //0x00000002 ... ISMEX_REPLIED 0x00000008 ... ISMEX_SEND 0x00000001 ..." is an answer-shaped flag set with a legal zero:
    //the return reports which SendMessage-family call produced the message the window procedure is handling, and a window
    //procedure handling its own local message answers ISMEX_NOSEND (zero) legitimately, so nothing is reserved and keying
    //CheckNonZero< DWORD > would report a legal local message as a failure. AUDIT: 1 in-tree caller, so the site-level read
    //is owed before any token moves - none is landed.
    DWORD User32::InSendMessageEx(void)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, InSendMessageEx);
        return Library::Call<DWORD, LPVOID>(pFunc, nullptr);
    }

    //Return policy (U2, table row PeekMessage / message.cpp, 12 Nov 2026): the row reads unchecked with no checkClass - the
    //table's verbatim docQuote "Type: BOOL If a message is available, the return value is nonzero. If no messages are
    //available, the return value is zero." is a question about the queue, not a status: the zero answers the caller's
    //question about its message queue, an empty queue being a legal state, so nothing is reserved; the Remarks put the
    //message itself in the MSGPLAYWINDOW structure the caller passes and note that the filter parameters' high word is
    //reserved for the system. The row's sdkRetType is NO-PROTOTYPE, so the base would be the published BOOL. 0 in-tree
    //callers for the exact stem (D14 published surface), so the row is change-safe and no site audit is owed; the page is
    //reached through the suffixed ANSI-twin slug nf-winuser-peekmessagea.
    BOOL User32::PeekMessageT(LPMSG lpMsg, HWND hWnd, UINT wMsgFilterMin, UINT wMsgFilterMax, UINT wRemoveMsg)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapiAW(user32, PeekMessage);
        return Library::Call<BOOL, LPMSG, HWND, UINT, UINT, UINT>(pFunc, lpMsg, hWnd, wMsgFilterMin, wMsgFilterMax, wRemoveMsg);
    }

    //Return policy (U2, table row PostMessage / message.cpp, 12 Nov 2026): the row reads checked with checkClass BoolCheck, and the
    //table's verbatim docQuote "Type: BOOL If the function succeeds, the return value is nonzero. If the function fails, the return
    //value is zero. To get extended error information, call GetLastError ." - the message being posted is a parameter and the MSGPLAYWINDOW
    //structure the caller fills is an out-parameter, so the BOOL carries no answer of its own and zero is reserved; the page is reached
    //through the suffixed ANSI-twin slug nf-winuser-postmessagea. The row's sdkRetType is NO-PROTOTYPE, so the base is the published
    //BOOL, which the preprocessed TU shows as typedef int BOOL (_tmp_msg_pp_final_s23.txt:145773), so BoolCheck (returncheck.h:94)
    //covers it - no new check class. 0 in-tree callers for the exact stem (D14 published surface), so nothing built today reaches
    //this site; G6 keeps the check live on every call, so a failed post now raises a continuable issue the Win32ErrorHandler above
    //reports rather than travelling as a silent FALSE.
    BOOL User32::PostMessageT(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapiAW(user32, PostMessage);
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call<BOOL, HWND, UINT, WPARAM, LPARAM>(pFunc, hWnd, Msg, wParam, lParam);
        return bResult;
    }

    //VOID in the SDK, so voidCall and nothing to test - the exit code is posted and the answer is not returned.
    void User32::PostQuitMessage(int nExitCode)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, PostQuitMessage);
        return Library::voidCall<int>(pFunc, nExitCode);
    }

    //Return policy (U2, table row PostThreadMessage / message.cpp, 12 Nov 2026): the row reads checked with checkClass BoolCheck, and the
    //table's verbatim docQuote "Type: BOOL If the function succeeds, the return value is nonzero. If the function fails, the return
    //value is zero. To get extended error information, call GetLastError . GetLastError returns ERROR_INVALID_THREAD_ID if idThread is
    //not a valid thread identifier, or if the thread specified by idThread does not have a message queue. GetLastError returns
    //ERROR_NOT_ENOUGH_QUOTA when the message limit is hit." - the failure conditions are named on the page, the message and the thread
    //id are parameters, so the BOOL carries no answer and zero is reserved; the page is reached through the suffixed ANSI-twin slug
    //nf-winuser-postthreadmessagea. sdkRetType NO-PROTOTYPE so the base is the published BOOL; BoolCheck (returncheck.h:94) is already
    //in the library - no new check class. 0 in-tree callers for the exact stem (D14 published surface), so nothing built today reaches
    //this site; G6 keeps the check live on every call, so a refused post now raises a continuable issue the Win32ErrorHandler above
    //reports rather than travelling as a silent FALSE.
    BOOL User32::PostThreadMessageT(DWORD idThread, UINT Msg, WPARAM wParam, LPARAM lParam)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapiAW(user32, PostThreadMessage);
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call<BOOL, DWORD, UINT, WPARAM, LPARAM>(pFunc, idThread, Msg, wParam, lParam);
        return bResult;
    }

    //Return policy (U2, table row RegisterWindowMessage / message.cpp, 12 Nov 2026): the row reads checked with checkClass
    //TRangeCheck< UINT, 0xC000, 0xFFFF > - docQuote "Type: UINT If the message is successfully registered, the return value is a
    //message identifier in the range 0xC000 through 0xFFFF. If the function fails, the return value is zero." so the page states the
    //success range as bounds: zero is reserved and the identifier is what the caller hands straight to its message handlers, so it is
    //never legitimately outside the stated range. TRangeCheck (returncheck.h:232, Test is (value >= Min && value <= Max) ? true :
    //false) is the tighter of the two classes that fit - CheckNonZero< UINT > is the weaker one and would let a value below 0xC000
    //through. The Remarks keep the private-message range WM_USER through 0x7FFF out of this stem's answer: that range is what the
    //caller registers by hand, not what this call returns. sdkRetType NO-PROTOTYPE so the base is the published UINT, and the page is
    //reached through the suffixed ANSI-twin slug nf-winuser-registerwindowmessagea. 0 in-tree callers for the exact stem (D14
    //published surface), so nothing built today reaches this site; G6 keeps the check live on every call, so an identifier outside
    //the documented range now raises a continuable issue the Win32ErrorHandler above reports rather than travelling as a silent value.
    UINT User32::RegisterWindowMessageT(LPCTSTR lpString)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapiAW(user32, RegisterWindowMessage);
        CheckReturn< UINT, TRangeCheck< UINT, 0xC000, 0xFFFF > >::TType uResult = Library::Call<UINT, LPCTSTR>(pFunc, lpString);
        return uResult;
    }

    //Return policy (U2, table row ReplyMessage / message.cpp, 12 Nov 2026): the row reads unchecked with no checkClass - the
    //table's verbatim docQuote "Type: BOOL If the calling thread was processing a message sent from another thread or process,
    //the return value is nonzero. If the calling thread was not processing a message sent from another thread or process,
    //the return value is zero." is a question about the message being handled, not a status: the Remarks make zero the
    //ordinary outcome ("If the message was not sent through SendMessage or if the message was sent by the same thread,
    //ReplyMessage has no effect"), so zero is a legal answer and keying BoolCheck would report a legal same-thread reply
    //as a failure. AUDIT: 1 in-tree caller, so the site-level read is owed before any token moves - none is landed.
    BOOL User32::ReplyMessage(LRESULT lResult)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, ReplyMessage);
        return Library::Call<BOOL, LRESULT>(pFunc, lResult);
    }

    //Return policy (U2, table row SendMessageCallback / message.cpp, 12 Nov 2026): the row reads checked with checkClass BoolCheck, and the
    //table's verbatim docQuote "Type: BOOL If the function succeeds, the return value is nonzero. If the function fails, the return
    //value is zero. To get extended error information, call GetLastError ." - the message, the window and the callback address are
    //parameters, so the BOOL carries no answer and zero is reserved; the page is reached through the suffixed ANSI-twin slug
    //nf-winuser-sendmessagecallbacka. sdkRetType NO-PROTOTYPE so the base is the published BOOL, which the preprocessed TU shows as
    //typedef int BOOL (_tmp_msg_pp_final_s23.txt:145773); BoolCheck (returncheck.h:94) is already in the library - no new check class.
    //0 in-tree callers for the exact stem (D14 published surface), so nothing built today reaches this site; G6 keeps the check live
    //on every call, so a send that did not queue now raises a continuable issue the Win32ErrorHandler above reports rather than
    //travelling as a silent FALSE.
    BOOL User32::SendMessageCallbackT(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam, ::SENDASYNCPROC lpResultCallBack, ULONG_PTR dwData)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapiAW(user32, SendMessageCallback);
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call<BOOL, HWND, UINT, WPARAM, LPARAM, ::SENDASYNCPROC, ULONG_PTR>(pFunc, hWnd, Msg, wParam, lParam, lpResultCallBack, dwData);
        return bResult;
    }

    //Return policy (U2, table row SendMessageTimeout / message.cpp, 12 Nov 2026): the row reads checked with checkClass
    //CheckNonZero< LRESULT > - docQuote "Type: LRESULT If the function succeeds, the return value is nonzero. SendMessageTimeout
    //does not provide information about individual windows timing out if HWND_BROADCAST is used. If the function fails or times out,
    //the return value is 0. Note that the function does not always call SetLastError on failure. If the reason for failure is
    //important to you, call SetLastError(ERROR_SUCCESS) before calling SendMessageTimeout. If the function returns 0, and GetLastError
    //returns ERROR_SUCCESS, then treat it as a generic failure." so zero is reserved for both failure states and is never a legal
    //answer here - a timed-out window answers through the HWND_BROADCAST line rather than through zero - and the page gives its own
    //reading rule for the generic case. The class base is the published LRESULT, so BoolCheck (returncheck.h:94, base BOOL) is ruled
    //out by type and CheckNonZero (returncheck.h:187, template< typename TNumeric > : public AutoErrorCheck< TNumeric >) fits. The
    //page is reached through the suffixed ANSI-twin slug nf-winuser-sendmessagetimeouta. 0 in-tree callers for the exact stem (D14
    //published surface), so nothing built today reaches this site; G6 keeps the check live on every call, so a failed or timed-out
    //send now raises a continuable issue the Win32ErrorHandler above reports rather than travelling as a silent 0.
    LRESULT User32::SendMessageTimeoutT(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam, UINT fuFlags, UINT uTimeout, PDWORD_PTR lpdwResult)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapiAW(user32, SendMessageTimeout);
        CheckReturn< LRESULT, CheckNonZero< LRESULT > >::TType lResult = Library::Call<LRESULT, HWND, UINT, WPARAM, LPARAM, UINT, UINT, PDWORD_PTR>(pFunc, hWnd, Msg, wParam, lParam, fuFlags, uTimeout, lpdwResult);
        return lResult;
    }

    //Return policy (U2, table row SendMessage / message.cpp, 12 Nov 2026): the row reads unchecked with no checkClass - value
    //answer with nothing reserved. The table's verbatim docQuote "The return value specifies the result of the message
    //processing; it depends on the message sent." is the whole Return value section, so the return is whatever the window
    //procedure that handled the message chose to answer with and every value is a legal answer; the page names no failure
    //value at all, so D51 step 2 reads no-check. The Remarks add only that a message blocked by UIPI sets GetLastError to 5
    //and that HWND_BROADCAST callers should use RegisterWindowMessage - neither reserves a return value. Deliberately unlike
    //the SendMessageTimeout row above, whose page does reserve 0 for failure and timeout and is therefore checked with
    //CheckNonZero< LRESULT >. 0 in-tree callers for the exact stem (D14 published surface), so nothing built today reaches
    //this site; the AW row's published type is LRESULT, so a check here would key at LRESULT. No token is landed.

    LRESULT User32::SendMessageT(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapiAW(user32, SendMessage);
        return Library::Call<LRESULT, HWND, UINT, WPARAM, LPARAM>(pFunc, hWnd, Msg, wParam, lParam);
    }

    //Return policy (U2, table row SendNotifyMessage / message.cpp, 12 Nov 2026): the row reads checked with checkClass BoolCheck, and the
    //table's verbatim docQuote "Type: BOOL If the function succeeds, the return value is nonzero. If the function fails, the return
    //value is zero. To get extended error information, call GetLastError ." - the message and the window are parameters, so the BOOL
    //carries no answer and zero is reserved; the page is reached through the suffixed ANSI-twin slug nf-winuser-sendnotifymessagea.
    //sdkRetType NO-PROTOTYPE so the base is the published BOOL, which the preprocessed TU shows as typedef int BOOL
    //(_tmp_msg_pp_final_s23.txt:145773); BoolCheck (returncheck.h:94) is already in the library - no new check class. 0 in-tree
    //callers for the exact stem (D14 published surface), so nothing built today reaches this site; G6 keeps the check live on
    //every call, so a send that did not queue now raises a continuable issue the Win32ErrorHandler above reports rather than
    //travelling as a silent FALSE.
    BOOL User32::SendNotifyMessageT(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapiAW(user32, SendNotifyMessage);
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call<BOOL, HWND, UINT, WPARAM, LPARAM>(pFunc, hWnd, Msg, wParam, lParam);
        return bResult;
    }

    //Return policy (U2, table row SetMessageExtraInfo / message.cpp, 12 Nov 2026): the row reads unchecked with no checkClass - a
    //value answer with no documented failure value. The table's verbatim docQuote "The return value is the previous value
    //associated with the current thread." is the whole Return value section, so D51 step 2 (an API that reserves no return
    //value for errors) reads no-check; the call reports the extra-info value the caller itself chose, and for a thread that
    //never set one that previous value is 0 - a legal answer - so keying CheckNonZero would report a legal first call as a
    //failure. The row's sdkRetType column is blank and its pubRetType is LPARAM, so the class base would have been the
    //published LPARAM had the row been checked. AUDIT: 1 in-tree caller, so the site-level read is owed before any token moves
    //against that base - none is landed.

    LPARAM User32::SetMessageExtraInfo(LPARAM lParam)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, SetMessageExtraInfo);
        return Library::Call<LPARAM, LPARAM>(pFunc, lParam);
    }

    //Return policy (U2, table row TranslateMessage / message.cpp, 12 Nov 2026): the row reads unchecked with no checkClass - a
    //question about the translation, not a status. The table's verbatim docQuote "If the message is translated (that is, a
    //character message is posted to the thread's message queue), the return value is nonzero. If the message is WM_KEYDOWN ,
    //WM_KEYUP , WM_SYSKEYDOWN , or WM_SYSKEYUP , the return value is nonzero, regardless of the translation. If the message is
    //not translated (that is, a character message is not posted to the thread's message queue), the return value is zero." - a
    //message that posts no character message is a legal state answering zero, so nothing is reserved and keying BoolCheck would
    //report a legal untranslated message as a failure; same shape as the InSendMessage row above. AUDIT: 1 in-tree caller, so
    //the site-level read is owed before any token moves - none is landed.

    BOOL User32::TranslateMessage(const MSG* lpMsg)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, TranslateMessage);
        return Library::Call<BOOL, const MSG*>(pFunc, lpMsg);
    }

    //Return policy (U2, table row WaitMessage / message.cpp, 12 Nov 2026): the row reads checked with checkClass BoolCheck, and the
    //table's verbatim docQuote "Type: BOOL If the function succeeds, the return value is nonzero. If the function fails, the return
    //value is zero. To get extended error information, call GetLastError ." - the message the thread waits for arrives through the
    //MSGPLAYWINDOW structure the caller passes, so the BOOL carries no answer and zero is reserved; the Remarks only qualify when
    //the call returns ("Note that WaitMessage does not return for unprocessed messages reported by a previous function which checks
    //the queue"), they name no legal zero. sdkRetType BOOL so the base is BOOL and BoolCheck (returncheck.h:94) is already in the
    //library - no new check class. AUDIT: **1** in-tree caller, re-read today at
    //`ui/gui/view/handlers/messagehandler.cpp:251` - `return User32::WaitMessage() ? true : false;` inside MessageHandler::Wait()
    //(:249-252), the file compiled (gui/CMakeLists.txt:60) but MessageHandler::Wait() has no in-tree invocation anywhere in src
    //re-measured today (the only MessageHandler match besides the definition itself), so nothing built today observes the new
    //issue; under G6 a failed wait now raises a continuable issue the Win32ErrorHandler above reports rather than reaching the
    //handler as a silent false that the ternary already flattens.
    BOOL User32::WaitMessage(void)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, WaitMessage);
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call<BOOL>(pFunc);
        return bResult;
    }

}}}//qor::win::api
