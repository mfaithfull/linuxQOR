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

    int GDI32::ExcludeClipRect(HDC hdc, int nLeftRect, int nTopRect, int nRightRect, int nBottomRect)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(gdi32, ExcludeClipRect);
        return Library::Call< int, HDC, int, int, int, int >(pFunc, hdc, nLeftRect, nTopRect, nRightRect, nBottomRect);
    }

    int GDI32::ExtSelectClipRgn(HDC hdc, HRGN hrgn, int fnMode)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(gdi32, ExtSelectClipRgn);
        return Library::Call< int, HDC, HRGN, int >(pFunc, hdc, hrgn, fnMode);
    }

    int GDI32::GetClipBox(HDC hdc, ::LPRECT lprc)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(gdi32, GetClipBox);
        return Library::Call< int, HDC, ::LPRECT >(pFunc, hdc, lprc);
    }

    int GDI32::GetClipRgn(HDC hdc, HRGN hrgn)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(gdi32, GetClipRgn);
        return Library::Call< int, HDC, HRGN >(pFunc, hdc, hrgn);
    }

    int GDI32::GetMetaRgn(HDC hdc, HRGN hrgn)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(gdi32, GetMetaRgn);
        return Library::Call< int, HDC, HRGN >(pFunc, hdc, hrgn);
    }

    int GDI32::SetMetaRgn(HDC hdc)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(gdi32, SetMetaRgn);
        return Library::Call< int, HDC >(pFunc, hdc);
    }

    int GDI32::GetRandomRgn(HDC hdc, HRGN hrgn, INT iNum)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(gdi32, GetRandomRgn);
        return Library::Call< int, HDC, HRGN, INT >(pFunc, hdc, hrgn, iNum);
    }

    int GDI32::IntersectClipRect(HDC hdc, int nLeftRect, int nTopRect, int nRightRect, int nBottomRect)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(gdi32, IntersectClipRect);
        return Library::Call< int, HDC, int, int, int, int >(pFunc, hdc, nLeftRect, nTopRect, nRightRect, nBottomRect);
    }

    int GDI32::OffsetClipRgn(HDC hdc, int nXOffset, int nYOffset)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(gdi32, OffsetClipRgn);
        return Library::Call< int, HDC, int, int >(pFunc, hdc, nXOffset, nYOffset);
    }

    BOOL GDI32::PtVisible(HDC hdc, int X, int Y)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(gdi32, PtVisible);
        return Library::Call< BOOL, HDC, int, int >(pFunc, hdc, X, Y);
    }

    BOOL GDI32::RectVisible(HDC hdc, CONST::RECT* lprc)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(gdi32, RectVisible);
        return Library::Call< BOOL, HDC, CONST ::RECT* >(pFunc, hdc, lprc);
    }

    BOOL GDI32::SelectClipPath(HDC hdc, int iMode)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(gdi32, SelectClipPath);
        return Library::Call< int, HDC, int >(pFunc, hdc, iMode);
    }

    int GDI32::SelectClipRgn(HDC hdc, HRGN hrgn)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(gdi32, SelectClipRgn);
        return Library::Call< int, HDC, HRGN >(pFunc, hdc, hrgn);
    }

}}}//qor::win::api
