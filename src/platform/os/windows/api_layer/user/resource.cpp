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

    // Two wrappers, the whole "//Resource functions" section: user32.h:442 label, members :443 CopyImage and
    // :444 LoadImageT, both HANDLE-returning, both under #if(WINVER >= 0x0400) in the SDK (winuser.h:10883).
    // Both definitions carry the User32:: qualifier since 3 Nov 2026 (review session 29, U11). Before that they were
    // free functions in qor::win::api, so the two static members had no definition anywhere (U4 x2) and both
    // bindings sat outside any member definition - the folder's only such pair, and the reason the wrong-stem scan
    // flagged both of its bindings for years.
    // Bindings: CopyImage is a single export with no A/W family, so the bare qor_pp_useswinapi below is its D13
    // spelling; LoadImage exists only as LoadImageA/LoadImageW behind #define LoadImage LoadImageW, so
    // qor_pp_useswinapiAW is required and resolves to "LoadImageW". Neither wrapper inspects the HANDLE it returns:
    // a NULL failure is silent here, which is the settled policy for this layer (no CheckReturn, no handler).
    //Addendum, 11 Nov 2026 (U2 Small TU batch S1, item resource sites): the "no CheckReturn" sentence above is that
    //session's record and stands superseded - the endpoint table now reads BOTH rows checked, CopyImage and LoadImage
    //each with checkClass CheckNonZeroHandle< HANDLE >, so both wrappers here carry a CheckReturn and this TU gained its
    //one ../returncheck.h include (0 -> 2 CheckReturn sites in this TU). The decision is the table's, not re-derived
    //here; each row's verbatim docQuote and class are stated at the site below.

    //Return policy (U2, table row CopyImage / resource.cpp, 11 Nov 2026): the row reads checked with checkClass
    //CheckNonZeroHandle< HANDLE >, and the table's verbatim docQuote is "Type: HANDLE If the function succeeds, the
    //return value is the handle to the newly created image. If the function fails, the return value is NULL . To get
    //extended error information, call GetLastError ." - the row's note records "failure value NULL means no image
    //handle, never a legal answer", so the session-27 soundness test (a checked-failure site is only sound where the
    //SDK's failure value is not also a legal success value) passes at every value and the handle row takes the
    //CheckNonZeroHandle template (returncheck.h:137, Test at :144 is (value == 0x00000000) ? false : true) the
    //vocabulary already offers - no new check class. 0 in-tree callers (D14 published surface), so the new throw is
    //confined to that surface; the wrapper still forwards all five arguments exactly as the caller wrote them.
    HANDLE User32::CopyImage(HANDLE h, UINT type, int cx, int cy, UINT flags)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, CopyImage);
        CheckReturn< HANDLE, CheckNonZeroHandle< HANDLE > >::TType hResult = Library::Call< HANDLE, HANDLE, UINT, int, int, UINT >(pFunc, h, type, cx, cy, flags);
        return hResult;
    }

    //Return policy (U2, table row LoadImage / resource.cpp, 11 Nov 2026): the row reads checked with checkClass
    //CheckNonZeroHandle< HANDLE >, and the table's verbatim docQuote is "Type: HANDLE If the function succeeds, the
    //return value is the handle of the newly loaded image. If the function fails, the return value is NULL . To get
    //extended error information, call GetLastError ." - the row's note records "failure value NULL means no image
    //handle, never a legal answer", so the session-27 soundness test (a checked-failure site is only sound where the
    //SDK's failure value is not also a legal success value) passes at every value and the handle row takes the
    //CheckNonZeroHandle template (returncheck.h:137, Test at :144 is (value == 0x00000000) ? false : true) the
    //vocabulary already offers - no new check class, and the AW binding that reaches LoadImageW is unchanged. 0 in-tree
    //callers (D14 published surface), so the new throw is confined to that surface.
    HANDLE User32::LoadImageT(HINSTANCE hInst, LPCTSTR name, UINT type, int cx, int cy, UINT fuLoad)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapiAW(user32, LoadImage);
        CheckReturn< HANDLE, CheckNonZeroHandle< HANDLE > >::TType hResult = Library::Call< HANDLE, HINSTANCE, LPCTSTR, UINT, int, int, UINT >(pFunc, hInst, name, type, cx, cy, fuLoad);
        return hResult;
    }

}}}//qor::win::api
