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
    //All eight caret entry points are single user32.dll exports: winuser.h:9456-9506 declares each one directly
    //with no A/W family and no stem -> stemW macro for any of them, so the bare stem is the name GetProcAddress
    //is given. cl /EP on this TU resolves the eight bindings to exactly those eight strings, and they are the
    //eight - and only eight - *Caret* names in the user32.dll export table, so the section is fully covered.
    //CreateCaret's hBitmap is _In_opt_ (NULL means a solid system caret); the bitmap itself stays owned by the
    //caller and must outlive the caret, which ends at DestroyCaret.
    //
    //Addendum, 12 Nov 2026 (U2 Small TU batch S5, item caret sites): the paragraphs above are the review's record and
    //stand superseded - the endpoint table reads all 8 rows of this TU checked, so this TU lands 8 CheckReturn sites
    //(7 BoolCheck over the BOOL rows and 1 CheckNonZero< UINT > over GetCaretBlinkTime) and gained its one
    //../returncheck.h include (0 -> 1). The decision is the table's, not re-derived here: the seven BOOL pages state
    //"If the function succeeds, the return value is nonzero. If the function fails, the return value is zero", so zero
    //is the reserved failure answer and the test reads value != 0; GetCaretBlinkTime answers "A return value is zero
    //indicates that the function has failed" while its INFINITE is the caret-does-not-blink answer, which a nonzero
    //test passes - so 0 is reserved and INFINITE is a legal value the check lets through. The pass-through this TU
    //had before is exactly what the checks leave in place; what changes is that a reserved answer now raises a
    //continuable issue the Win32ErrorHandler above reports instead of being handed over as an answer. Every one of the
    //8 stems has in-tree callers (1 each, 2 for HideCaret / ShowCaret), so each call site was read before its token
    //moved - see the per-site note below. Each row's verbatim docQuote and class sits at the site below.

    //Return policy (U2, table row CreateCaret / caret.cpp, 12 Nov 2026): the row reads checked with checkClass BoolCheck,
    //and the table's verbatim docQuote is "Type: BOOL If the function succeeds, the return value is nonzero. If the
    //function fails, the return value is zero. To get extended error information, call GetLastError ." - the caret
    //answers through the window's own state, so the BOOL is that status word and zero is reserved for failure. BoolCheck
    //(returncheck.h:94, Test at :99 is value ? true : false) fails on zero only, so the session-27 soundness test holds
    //at every value. 1 in-tree caller, re-read before this token moved: ui/gui/controllers/a_h/caret.cpp:37 returns this
    //BOOL as its own bool, so the new throw replaces that caller's false path with a continuable issue the Win32ErrorHandler
    //above reports; the wrapper still forwards hBitmap as the caller wrote it, _In_opt_ NULL included.
    BOOL User32::CreateCaret(HWND hWnd, HBITMAP hBitmap, int nWidth, int nHeight)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, CreateCaret);
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call< BOOL, HWND, HBITMAP, int, int >(pFunc, hWnd, hBitmap, nWidth, nHeight);
        return bResult;
    }

    //Return policy (U2, table row DestroyCaret / caret.cpp, 12 Nov 2026): the row reads checked with checkClass BoolCheck,
    //and the table's verbatim docQuote is the same status wording as the Create twin - "Type: BOOL If the function
    //succeeds, the return value is nonzero. If the function fails, the return value is zero. To get extended error
    //information, call GetLastError ." - the caret being destroyed is named by nothing here, so the BOOL is call status
    //and zero is reserved. BoolCheck (returncheck.h:94, Test at :99 is value ? true : false) fails on zero only. 1 in-tree
    //caller, re-read before this token moved: ui/gui/controllers/a_h/caret.cpp:42 already reads the BOOL as
    //"? true : false", so the new throw replaces that caller's false path with a continuable issue.
    BOOL User32::DestroyCaret(void)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, DestroyCaret);
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call< BOOL >(pFunc);
        return bResult;
    }

    //Return policy (U2, table row GetCaretBlinkTime / caret.cpp, 12 Nov 2026): the row reads checked with checkClass
    //CheckNonZero< UINT >, and the table's verbatim docQuote is "Type: UINT If the function succeeds, the return value
    //is the blink time, in milliseconds. A return value of INFINITE indicates that the caret does not blink. A return
    //value is zero indicates that the function has failed. To get extended error information, call GetLastError ." -
    //this UINT answers, and the page reserves exactly one of its values: 0 is the failure answer, while INFINITE
    //carries the real answer "the caret does not blink" and a nonzero test passes it, which is what the row wants.
    //CheckNonZero (returncheck.h:187, Test at :192 is (value == 0) ? false : true) fails on 0 only, so the session-27
    //soundness test holds at every value - no blink time is 0 milliseconds. 1 in-tree caller, re-read before this token
    //moved: ui/gui/controllers/a_h/caret.cpp:47 returns this UINT untested as its own UINT, so the new throw replaces
    //that caller's silent 0 with a continuable issue while INFINITE still reaches it unchanged.
    UINT User32::GetCaretBlinkTime(void)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, GetCaretBlinkTime);
        CheckReturn< UINT, CheckNonZero< UINT > >::TType uResult = Library::Call< UINT >(pFunc);
        return uResult;
    }

    //Return policy (U2, table row GetCaretPos / caret.cpp, 12 Nov 2026): the row reads checked with checkClass BoolCheck,
    //and the table's verbatim docQuote is "Type: BOOL If the function succeeds, the return value is nonzero. If the
    //function fails, the return value is zero. To get extended error information, call GetLastError ." - the position
    //answers through the _Out_ LPPOINT, so the BOOL is call status and zero is reserved for failure. BoolCheck
    //(returncheck.h:94, Test at :99 is value ? true : false) fails on zero only, so the session-27 soundness test holds
    //at every value. 1 in-tree caller, re-read before this token moved: ui/gui/controllers/a_h/caret.cpp:57 passes a
    //reinterpret_cast<LPPOINT>(&pt) of a local POINT and reads the BOOL as "? true : false", so the new throw replaces
    //that caller's false path while the out argument keeps its spelling.
    BOOL User32::GetCaretPos(LPPOINT lpPoint)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, GetCaretPos);
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call< BOOL, LPPOINT >(pFunc, lpPoint);
        return bResult;
    }

    //Return policy (U2, table row HideCaret / caret.cpp, 12 Nov 2026): the row reads checked with checkClass BoolCheck,
    //and the table's verbatim docQuote is "Type: BOOL If the function succeeds, the return value is nonzero. If the
    //function fails, the return value is zero. To get extended error information, call GetLastError ." - hiding the
    //caret answers through the window's own state, so the BOOL is call status and zero is reserved for failure. BoolCheck
    //(returncheck.h:94, Test at :99 is value ? true : false) fails on zero only, so the session-27 soundness test holds
    //at every value. 2 in-tree callers, both re-read before this token moved: ui/gui/controllers/a_h/caret.cpp:72 and
    //ui/gui/controllers/windowcontroller.cpp:663, the latter inside bool WindowController::HideCaret() at :661, read the
    //BOOL as "? true : false", so both callers' false paths become a continuable issue the Win32ErrorHandler above reports.
    BOOL User32::HideCaret(HWND hWnd)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, HideCaret);
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call< BOOL, HWND >(pFunc, hWnd);
        return bResult;
    }

    //Return policy (U2, table row SetCaretBlinkTime / caret.cpp, 12 Nov 2026): the row reads checked with checkClass
    //BoolCheck, and the table's verbatim docQuote is "Type: BOOL If the function succeeds, the return value is nonzero.
    //If the function fails, the return value is zero. To get extended error information, call GetLastError ." - the
    //blink time is an _In_ argument, so nothing answers through the return and zero is reserved for failure; the
    //UINT uMSeconds the caller passes is not a value this wrapper reads, and the SDK keeps its own floor for it. BoolCheck
    //(returncheck.h:94, Test at :99 is value ? true : false) fails on zero only, so the session-27 soundness test holds
    //at every value. 1 in-tree caller, re-read before this token moved: ui/gui/controllers/a_h/caret.cpp:52 returns this
    //BOOL as its own bool, so the caller's false path becomes a continuable issue the Win32ErrorHandler above reports.
    BOOL User32::SetCaretBlinkTime(UINT uMSeconds)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, SetCaretBlinkTime);
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call< BOOL, UINT >(pFunc, uMSeconds);
        return bResult;
    }

    //Return policy (U2, table row SetCaretPos / caret.cpp, 12 Nov 2026): the row reads checked with checkClass BoolCheck,
    //and the table's verbatim docQuote is "Type: BOOL If the function succeeds, the return value is nonzero. If the
    //function fails, the return value is zero. To get extended error information, call GetLastError ." - the position is
    //two _In_ coordinates, so the BOOL is pure call status and zero is reserved for failure. BoolCheck
    //(returncheck.h:94, Test at :99 is value ? true : false) fails on zero only, so the session-27 soundness test holds
    //at every value - no coordinate pair answers through the return. 1 in-tree caller, re-read before this token moved:
    //ui/gui/controllers/a_h/caret.cpp:62 reads the BOOL as "? true : false", so that caller's false path becomes a
    //continuable issue while both coordinates keep their spelling.
    BOOL User32::SetCaretPos(int X, int Y)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, SetCaretPos);
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call< BOOL, int, int >(pFunc, X, Y);
        return bResult;
    }

    //Return policy (U2, table row ShowCaret / caret.cpp, 12 Nov 2026): the row reads checked with checkClass BoolCheck,
    //and the table's verbatim docQuote is "Type: BOOL If the function succeeds, the return value is nonzero. If the
    //function fails, the return value is zero. To get extended error information, call GetLastError ." - showing the
    //caret answers through the window's own state, so the BOOL is call status and zero is reserved for failure. BoolCheck
    //(returncheck.h:94, Test at :99 is value ? true : false) fails on zero only, so the session-27 soundness test holds
    //at every value. 2 in-tree callers, both re-read before this token moved: ui/gui/controllers/a_h/caret.cpp:67 and
    //ui/gui/controllers/windowcontroller.cpp:668, the latter inside bool WindowController::ShowCaret() at :666, read the
    //BOOL as "? true : false", so both callers' false paths become a continuable issue the Win32ErrorHandler above reports.
    BOOL User32::ShowCaret(HWND hWnd)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, ShowCaret);
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call< BOOL, HWND >(pFunc, hWnd);
        return bResult;
    }

}}}//qor::win::api
