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

    HANDLE CopyImage(HANDLE hImage, UINT uType, int cxDesired, int cyDesired, UINT fuFlags)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, CopyImage);
        return Library::Call< HANDLE, HANDLE, UINT, int, int , UINT >(pFunc, hImage, uType, cxDesired, cyDesired, fuFlags);
    }

    HANDLE LoadImageT(HINSTANCE hinst, LPCTSTR lpszName, UINT uType, int cxDesired, int cyDesired, UINT fuLoad)
    {
        qor_pp_fcontext;
        qor_pp_useswinapiAW(user32, LoadImage);
        return Library::Call< HANDLE, HINSTANCE, LPCTSTR, UINT, int, int, UINT >( pFunc, hinst, lpszName, uType, cxDesired, cyDesired, fuLoad);
    }

}}}//qor::win::api
