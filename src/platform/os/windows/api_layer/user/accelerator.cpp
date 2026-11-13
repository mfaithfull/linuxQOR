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

    //The accelerator families are A/W exports only - user32.dll exports CopyAcceleratorTableA/W,
    //CreateAcceleratorTableA/W, LoadAcceleratorsA/W and TranslateAcceleratorA/W but no plain stem for the
    //first three (TranslateAccelerator is exported as an alias of TranslateAcceleratorA). Binding the bare
    //stem only works while the SDK's A/W macro is still in scope at the call site, where it silently rewrites
    //the name before _ATXT() stringizes it; the explicit AW form spells the name that GetProcAddress is
    //actually given, so the binding survives any #undef of the stem (user32.h:9-17 #undef's nine such stems).

    //Return policy (U2, table row CopyAcceleratorTable / accelerator.cpp, 12 Nov 2026): the row reads unchecked with no
    //checkClass, and the table's verbatim docQuote is "Type: int If lpAccelDst is NULL , the return value specifies the
    //number of accelerator-table entries in the original table. Otherwise, it specifies the number of accelerator-table
    //entries that were copied." - the count is the answer the caller asks for (the page's own first line names the call's
    //purpose as obtaining the table data OR determining its size), so an empty accelerator table legitimately answers 0;
    //the page has no "If the function fails" branch and never tells the caller to call GetLastError, so D51 step 2 (an
    //API that reserves no return value for errors) reads no-check and the session-27 soundness test fails at that value -
    //keying CheckNonZero< int > here would report an empty table as a failure. The value comes back exactly as the SDK
    //produced it. 0 in-tree callers for the exact stem (D14 published surface).
    int User32::CopyAcceleratorTableT(HACCEL hAccelSrc, LPACCEL lpAccelDst, int cAccelEntries)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapiAW(user32, CopyAcceleratorTable);
        return Library::Call<int, HACCEL, LPACCEL, int>(pFunc, hAccelSrc, lpAccelDst, cAccelEntries);
    }

    //Return policy (U2, table row CreateAcceleratorTable / accelerator.cpp, 12 Nov 2026): the row reads checked with
    //checkClass CheckNonZeroHandle< HACCEL >, and the table's verbatim docQuote is "Type: HACCEL If the function succeeds,
    //the return value is the handle to the created accelerator table; otherwise, it is NULL . To get extended error
    //information, call GetLastError ." - NULL is reserved for the documented failure and the handle created here is what
    //the caller hands to TranslateAccelerator or DestroyAcceleratorTable, so a created table never answers NULL and the
    //session-27 soundness test passes at every value; sdkRetType is NO-PROTOTYPE so the class base is the published
    //HACCEL already in scope at user32.h:425, and CheckNonZeroHandle (returncheck.h:137, Test at :142 is (value ==
    //0x00000000) ? false : true) covers it at that type - the same template the HBITMAP and HANDLE rows take, so no new
    //check class. 0 in-tree callers for the exact stem, so nothing built today reaches this site; G6 keeps the check live
    //on every call, so a failed creation now raises a continuable issue the Win32ErrorHandler above reports rather than
    //travelling as a silent NULL (status.md U7).
    HACCEL User32::CreateAcceleratorTableT(LPACCEL paccel, int cAccel)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapiAW(user32, CreateAcceleratorTable);
        CheckReturn< HACCEL, CheckNonZeroHandle< HACCEL > >::TType hResult = Library::Call<HACCEL, LPACCEL, int>(pFunc, paccel, cAccel);
        return hResult;
    }

    //DestroyAcceleratorTable has no A/W variants - it is a single user32.dll export, so the bare stem is
    //the exported name and there is no variant to select.
    //Return policy (U2, table row DestroyAcceleratorTable / accelerator.cpp, 12 Nov 2026): the row reads unchecked with no
    //checkClass, and the table's verbatim docQuote is "Type: BOOL If the function succeeds, the return value is nonzero.
    //However, if the table has been loaded more than one call to LoadAccelerators , the function will return a nonzero
    //value only when DestroyAcceleratorTable has been called an equal number of times. If the function fails, the return
    //value is zero." - the page's middle clause puts zero at a NON-failure outcome: when a table was loaded N times and
    //destroyed fewer than N times the destroy succeeded and still answers zero (the table remains loaded), so zero is a
    //legal outcome the caller needs and keying BoolCheck would report a legal partial destroy as a failure - the session-27
    //soundness test fails at that value. The value comes back exactly as the SDK produced it. 1 in-tree caller for the
    //exact stem, audited here rather than deferred: the row lands unchecked, so no token moves and the caller sees no change.
    BOOL User32::DestroyAcceleratorTable(HACCEL hAccel)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, DestroyAcceleratorTable);
        return Library::Call<BOOL, HACCEL>(pFunc, hAccel);
    }

    //Return policy (U2, table row LoadAccelerators / accelerator.cpp, 12 Nov 2026): the row reads checked with
    //checkClass CheckNonZeroHandle< HACCEL >, and the table's verbatim docQuote is "Type: HACCEL If the function succeeds,
    //the return value is a handle to the loaded accelerator table. If the function fails, the return value is NULL . To
    //get extended error information, call GetLastError ." - the CreateAcceleratorTable row's wording: NULL is reserved for
    //the documented failure and the loaded-table handle is what the caller passes to TranslateAccelerator, so a loaded
    //table never answers NULL and the session-27 soundness test passes at every value; sdkRetType is NO-PROTOTYPE so the
    //class base is the published HACCEL already in scope at user32.h:427, and the CheckNonZeroHandle template (returncheck.h:137,
    //Test at :142 is (value == 0x00000000) ? false : true) covers it - no new check class. 0 in-tree callers for the exact
    //stem, so nothing built today reaches this site; G6 keeps the check live on every call, so a failed load now raises a
    //continuable issue the Win32ErrorHandler above reports rather than travelling as a silent NULL (status.md U7).
    HACCEL User32::LoadAcceleratorsT(HINSTANCE hInstance, LPCTSTR lpTableName)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapiAW(user32, LoadAccelerators);
        CheckReturn< HACCEL, CheckNonZeroHandle< HACCEL > >::TType hResult = Library::Call<HACCEL, HINSTANCE, LPCTSTR>(pFunc, hInstance, lpTableName);
        return hResult;
    }

    //Return policy (U2, table row TranslateAccelerator / accelerator.cpp, 12 Nov 2026): the row reads checked with
    //checkClass CheckNonZero< int >, and the table's verbatim docQuote is "Type: int If the function succeeds, the return
    //value is nonzero. If the function fails, the return value is zero. To get extended error information, call GetLastError
    //." - the call's subject is its three parameters (the window, the accelerator handle and the MSG structure), and the
    //message the call sends on behalf of the caller answers through that window procedure's own return rather than through
    //this row; the Remarks put the translation marker in the *sent* message ("the high-order word of the wParam parameter
    //of the WM_COMMAND or WM_SYSCOMMAND message contains the value 1") rather than in the return, so the return carries no
    //answer and zero is reserved - the session-27 soundness test passes at every value. sdkRetType is NO-PROTOTYPE so the
    //class base is the published int already in scope at user32.h:428, and the CheckNonZero template (returncheck.h:187,
    //Test at :192 is (value == 0) ? false : true) covers it - no new check class. 0 in-tree callers for the exact stem, so
    //nothing built today reaches this site; G6 keeps the check live on every call, so a failed translation now raises a
    //continuable issue the Win32ErrorHandler above reports rather than travelling as a silent zero (status.md U7).
    int User32::TranslateAcceleratorT(HWND hWnd, HACCEL hAccTable, LPMSG lpMsg)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapiAW(user32, TranslateAccelerator);
        CheckReturn< int, CheckNonZero< int > >::TType iResult = Library::Call<int, HWND, HACCEL, LPMSG>(pFunc, hWnd, hAccTable, lpMsg);
        return iResult;
    }

}}}//qor::win::api
