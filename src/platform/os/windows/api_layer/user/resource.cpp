// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"
#include "src/qor/module/module.h"
#include "src/qor/flyers/interception/functioncontext.h"
#include "src/qor/flyers/error/error.h"

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

    HANDLE User32::CopyImage(HANDLE hImage, UINT uType, int cxDesired, int cyDesired, UINT fuFlags)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, CopyImage);
        return Library::Call< HANDLE, HANDLE, UINT, int, int, UINT >(pFunc, hImage, uType, cxDesired, cyDesired, fuFlags);
    }

    HANDLE User32::LoadImageT(HINSTANCE hinst, LPCTSTR lpszName, UINT uType, int cxDesired, int cyDesired, UINT fuLoad)
    {
        qor_pp_fcontext;
        qor_pp_useswinapiAW(user32, LoadImage);
        return Library::Call< HANDLE, HINSTANCE, LPCTSTR, UINT, int, int, UINT >(pFunc, hinst, lpszName, uType, cxDesired, cyDesired, fuLoad);
    }

}}}//qor::win::api
