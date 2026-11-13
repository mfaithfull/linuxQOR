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

    //The four WinEvent entry points are single user32.dll exports - winuser.h declares them directly with no
    //A/W family and no stem -> stemW macro, and user32.h does not #undef these stems, so the bare stem is the
    //name GetProcAddress is given.

    //The callback is invoked by the system with the CALLBACK (__stdcall) convention, so the caller must supply a
    //function pointer of that convention; WINEVENTPROC is the SDK type (winuser.h:13734).
    //Return policy (U2, table row SetWinEventHook / accessibility.cpp, 12 Nov 2026): the row reads checked with checkClass
    //CheckNonZeroHandle< HWINEVENTHOOK >, and the table's verbatim docQuote is "Type: HWINEVENTHOOK If successful, returns an
    //HWINEVENTHOOK value that identifies this event hook instance. Applications save this return value to use it with the
    //UnhookWinEvent function. If unsuccessful, returns zero." - the page reserves zero for failure, and the answer this member
    //hands back is the hook handle the caller keeps to pass to UnhookWinEvent, for which the page gives no legal zero, so the
    //session-27 soundness test (a checked-failure site is only sound where the SDK's failure value is not also a legal success
    //value) passes at every value. The row names sdkRetType HWINEVENTHOOK (winuser.h:13746) and the binding is bare, so the
    //class base is that SDK type. CheckNonZeroHandle (returncheck.h:136, Test at :144 is (value == 0x00000000) ? false : true)
    //is the handle template the vocabulary already offers - no new check class. 0 in-tree callers for the exact stem (D14
    //published surface), so nothing built today reaches this site; G6 keeps the check live on every call, so an unsuccessful
    //registration now raises a continuable issue the Win32ErrorHandler above reports instead of travelling as a zero handle the
    //caller would hand to UnhookWinEvent (status.md U7).
    HWINEVENTHOOK User32::SetWinEventHook(UINT eventMin, UINT eventMax, HMODULE hmodWinEventProc, WINEVENTPROC pfnWinEventProc, DWORD idProcess, DWORD idThread, UINT dwFlags)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, SetWinEventHook);
        CheckReturn< HWINEVENTHOOK, CheckNonZeroHandle< HWINEVENTHOOK > >::TType hResult = Library::Call<HWINEVENTHOOK, UINT, UINT, HMODULE, WINEVENTPROC, DWORD, DWORD, UINT>(pFunc, eventMin, eventMax, hmodWinEventProc, pfnWinEventProc, idProcess, idThread, dwFlags);
        return hResult;
    }

    //BOOL success flag for the withdrawal: FALSE means the hook named by hWinEventHook is still installed.
    //Return policy (U2, table row UnhookWinEvent / accessibility.cpp, 12 Nov 2026): the row reads checked with checkClass
    //BoolCheck, and the table's verbatim docQuote is "Type: BOOL If successful, returns TRUE ; otherwise, returns FALSE .
    //Three common errors cause this function to fail: The hWinEventHook parameter is NULL or not valid. The event hook
    //specified by hWinEventHook was already removed. UnhookWinEvent is called from a thread that is different from the
    //original call to SetWinEventHook ." - FALSE is reserved for those three documented errors and this member answers nothing
    //other than the withdrawal's success (the handle is an in-parameter), so the session-27 soundness test passes at every
    //value and BOOL + zero-means-failure takes the BoolCheck the library already has (returncheck.h:94, value ? true : false)
    //- no new check class. 0 in-tree callers for the exact stem (D14 published surface); G6 keeps the check live on every call,
    //so a failed withdrawal now raises a continuable issue the Win32ErrorHandler above reports rather than travelling as a
    //silent success (status.md U7).
    BOOL User32::UnhookWinEvent(HWINEVENTHOOK hWinEventHook)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, UnhookWinEvent);
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call<BOOL, HWINEVENTHOOK>(pFunc, hWinEventHook);
        return bResult;
    }

    //A question, not a flag: TRUE when the event named by event already has a hook, FALSE when it has none.
    //Return policy (U2, table row IsWinEventHookInstalled / accessibility.cpp, 12 Nov 2026): the row reads unchecked with no
    //checkClass, and the table's verbatim docQuote is "Type: BOOL If there is a hook to be notified of the specified event,
    //the return value is TRUE . If there are no hooks to be notified of the specified event, the return value is FALSE ." -
    //the FALSE answers the question the caller asked about the event's hook coverage; it does not report a failure, so the
    //session-27 test fails at that value before any check is considered. 0 in-tree callers for the exact stem, and the row is
    //unchecked on the page's own wording rather than on that count; no token from this row and the answer comes back exactly
    //as the OS wrote it.
    BOOL User32::IsWinEventHookInstalled(DWORD event)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, IsWinEventHookInstalled);
        return Library::Call<BOOL, DWORD>(pFunc, event);
    }

    //void both in the SDK (winuser.h:13728) and here, so Library::voidCall matches and there is no return to inspect;
    //the row's verdict is void (page carries no Return value section) and this row adds neither a token nor a reason.
    //1 in-tree caller for the stem, and the call's own success is unobservable through this member either way (U7).
    void User32::NotifyWinEvent(DWORD event, HWND hwnd, LONG idObject, LONG idChild)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, NotifyWinEvent);
        return Library::voidCall<DWORD, HWND, LONG, LONG>(pFunc, event, hwnd, idObject, idChild);
    }

}}}//qor::win::api
