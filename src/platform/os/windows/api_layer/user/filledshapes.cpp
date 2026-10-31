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

    int User32::FillRect(HDC hDC, CONST::RECT* lprc, HBRUSH hbr)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, FillRect);
        return Library::Call< int, HDC, CONST ::RECT*, HBRUSH >(pFunc, hDC, lprc, hbr);
    }

    int User32::FrameRect(HDC hDC, CONST::RECT* lprc, HBRUSH hbr)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, FrameRect);
        return Library::Call< int, HDC, CONST ::RECT*, HBRUSH >(pFunc, hDC, lprc, hbr);
    }

    BOOL User32::InvertRect(HDC hDC, CONST::RECT* lprc)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, InvertRect);
        return Library::Call< BOOL, HDC, CONST ::RECT* >(pFunc, hDC, lprc);
    }


}}}//qor::win::api
