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

    BOOL User32::CopyRect(::LPRECT lprcDst, CONST::RECT* lprcSrc)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, CopyRect);
        return Library::Call< BOOL, ::LPRECT, CONST ::RECT* >(pFunc, lprcDst, lprcSrc );
    }

    BOOL User32::EqualRect(CONST::RECT* lprc1, CONST::RECT* lprc2)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, EqualRect);
        return Library::Call< BOOL, CONST ::RECT*, CONST RECT* >(pFunc, lprc1, lprc2);
    }

    BOOL User32::InflateRect(::LPRECT lprc, int dx, int dy)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, InflateRect);
        return Library::Call< BOOL, ::LPRECT, int, int >(pFunc, lprc, dx, dy);
    }

    BOOL User32::IntersectRect(::LPRECT lprcDst, CONST::RECT* lprcSrc1, CONST::RECT* lprcSrc2)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, IntersectRect);
        return Library::Call< BOOL, ::LPRECT, CONST ::RECT*, CONST ::RECT* >(pFunc, lprcDst, lprcSrc1, lprcSrc2);
    }

    BOOL User32::IsRectEmpty(CONST::RECT* lprc)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, IsRectEmpty);
        return Library::Call< BOOL, CONST ::RECT* >(pFunc, lprc);
    }

    BOOL User32::OffsetRect(::LPRECT lprc, int dx, int dy)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, OffsetRect);
        return Library::Call< BOOL, ::LPRECT, int, int >(pFunc, lprc, dx, dy);
    }

    BOOL User32::PtInRect(CONST::RECT* lprc, POINT pt)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, PtInRect);
        return Library::Call< BOOL, CONST ::RECT*, POINT >(pFunc, lprc, pt);
    }

    BOOL User32::SetRect(::LPRECT lprc, int xLeft, int yTop, int xRight, int yBottom)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, SetRect);
        return Library::Call< BOOL, ::LPRECT, int, int , int, int >(pFunc, lprc, xLeft, yTop, xRight, yBottom);
    }

    BOOL User32::SetRectEmpty(::LPRECT lprc)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, SetRectEmpty);
        return Library::Call< BOOL, ::LPRECT >(pFunc, lprc);
    }

    BOOL User32::SubtractRect(::LPRECT lprcDst, CONST::RECT* lprcSrc1, CONST::RECT* lprcSrc2)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, SubtractRect);
        return Library::Call< BOOL, ::LPRECT, CONST ::RECT*, CONST ::RECT* >(pFunc, lprcDst, lprcSrc1, lprcSrc2);
    }

    BOOL User32::UnionRect(::LPRECT lprcDst, CONST::RECT* lprcSrc1, CONST::RECT* lprcSrc2)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, UnionRect);
        return Library::Call< BOOL, ::LPRECT, CONST ::RECT*, CONST RECT* >(pFunc, lprcDst, lprcSrc1, lprcSrc2);
    }

}}}//qor::win::api
