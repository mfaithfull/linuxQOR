// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"
#include "src/qor/module/module.h"
#include "src/qor/flyers/interception/functioncontext.h"
#include "src/qor/flyers/error/error.h"

//Windows specific headers must be last to prevent contaminating generic headers with Windows specific types and definitions
#include "gdi32.h"
#include "../returncheck.h"
#include "../library.h"

namespace qor { namespace win { namespace api {

    HPEN GDI32::CreatePen(int fnPenStyle, int nWidth, COLORREF crColor)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(gdi32, CreatePen);
        return Library::Call< HPEN, int, int, COLORREF >(pFunc, fnPenStyle, nWidth, crColor);
    }

    HPEN GDI32::CreatePenIndirect(CONST::LOGPEN* lplgpn)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(gdi32, CreatePenIndirect);
        return Library::Call< HPEN, CONST ::LOGPEN* >(pFunc, lplgpn);
    }

    HPEN GDI32::ExtCreatePen(DWORD dwPenStyle, DWORD dwWidth, CONST::LOGBRUSH* lplb, DWORD dwStyleCount, CONST DWORD* lpStyle)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(gdi32, ExtCreatePen);
        return Library::Call< HPEN, DWORD, DWORD, CONST ::LOGBRUSH*, DWORD, CONST DWORD* >(pFunc, dwPenStyle, dwWidth, lplb, dwStyleCount, lpStyle);
    }

}}}//qor::win::api
