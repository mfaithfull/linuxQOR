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

    int GDI32::CombineRgn(HRGN hrgnDest, HRGN hrgnSrc1, HRGN hrgnSrc2, int fnCombineMode)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(gdi32, CombineRgn);
        return Library::Call< int, HRGN, HRGN, HRGN, int >(pFunc, hrgnDest, hrgnSrc1, hrgnSrc2, fnCombineMode);
    }

    HRGN GDI32::CreateEllipticRgn(int nLeftRect, int nTopRect, int nRightRect, int nBottomRect)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(gdi32, CreateEllipticRgn);
        return Library::Call< HRGN, int, int, int, int >(pFunc, nLeftRect, nTopRect, nRightRect, nBottomRect);
    }

    HRGN GDI32::CreateEllipticRgnIndirect(CONST::RECT* lprc)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(gdi32, CreateEllipticRgnIndirect);
        return Library::Call< HRGN, CONST ::RECT* >(pFunc, lprc);
    }

    HRGN GDI32::CreatePolygonRgn(CONST::POINT* lppt, int cPoints, int fnPolyFillMode)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(gdi32, CreatePolygonRgn);
        return Library::Call< HRGN, CONST ::POINT*, int, int >(pFunc, lppt, cPoints, fnPolyFillMode);
    }

    HRGN GDI32::CreatePolyPolygonRgn(CONST::POINT* lppt, CONST INT* lpPolyCounts, int nCount, int fnPolyFillMode)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(gdi32, CreatePolyPolygonRgn);
        return Library::Call< HRGN, CONST ::POINT*, CONST INT*, int, int >(pFunc, lppt, lpPolyCounts, nCount, fnPolyFillMode);
    }

    HRGN GDI32::CreateRectRgn(int nLeftRect, int nTopRect, int nRightRect, int nBottomRect)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(gdi32, CreateRectRgn);
        return Library::Call< HRGN, int, int, int, int >(pFunc, nLeftRect, nTopRect, nRightRect, nBottomRect);
    }

    HRGN GDI32::CreateRectRgnIndirect(CONST::RECT* lprc)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(gdi32, CreateRectRgnIndirect);
        return Library::Call< HRGN, CONST ::RECT* >(pFunc, lprc);
    }

    HRGN GDI32::CreateRoundRectRgn(int nLeftRect, int nTopRect, int nRightRect, int nBottomRect, int nWidthEllipse, int nHeightEllipse)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(gdi32, CreateRoundRectRgn);
        return Library::Call< HRGN, int, int, int, int, int, int >(pFunc, nLeftRect, nTopRect, nRightRect, nBottomRect, nWidthEllipse, nHeightEllipse);
    }

    BOOL GDI32::EqualRgn(HRGN hSrcRgn1, HRGN hSrcRgn2)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(gdi32, EqualRgn);
        return Library::Call< BOOL, HRGN, HRGN >(pFunc, hSrcRgn1, hSrcRgn2);
    }

    HRGN GDI32::ExtCreateRegion(CONST::XFORM* lpXform, DWORD nCount, CONST::RGNDATA* lpRgnData)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(gdi32, ExtCreateRegion);
        return Library::Call< HRGN, CONST ::XFORM*, DWORD, CONST ::RGNDATA* >(pFunc, lpXform, nCount, lpRgnData);
    }

    BOOL GDI32::FillRgn(HDC hdc, HRGN hrgn, HBRUSH hbr)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(gdi32, FillRgn);
        return Library::Call< BOOL, HDC, HRGN, HBRUSH >(pFunc, hdc, hrgn, hbr);
    }

    BOOL GDI32::FrameRgn(HDC hdc, HRGN hrgn, HBRUSH hbr, int nWidth, int nHeight)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(gdi32, FrameRgn);
        return Library::Call< BOOL, HDC, HRGN, HBRUSH, int, int >(pFunc, hdc, hrgn, hbr, nWidth, nHeight);
    }

    int GDI32::GetPolyFillMode(HDC hdc)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(gdi32, GetPolyFillMode);
        return Library::Call< int, HDC >(pFunc, hdc);
    }

    DWORD GDI32::GetRegionData(HRGN hRgn, DWORD dwCount, ::LPRGNDATA lpRgnData)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(gdi32, GetRegionData);
        return Library::Call< DWORD, HRGN, DWORD, ::LPRGNDATA >(pFunc, hRgn, dwCount, lpRgnData);
    }

    int GDI32::GetRgnBox(HRGN hrgn, ::LPRECT lprc)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(gdi32, GetRgnBox);
        return Library::Call< int, HRGN, LPRECT >(pFunc, hrgn, lprc);
    }

    BOOL GDI32::InvertRgn(HDC hdc, HRGN hrgn)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(gdi32, InvertRgn);
        return Library::Call< BOOL, HDC, HRGN >(pFunc, hdc, hrgn);
    }

    int GDI32::OffsetRgn(HRGN hrgn, int nXOffset, int nYOffset)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(gdi32, OffsetRgn);
        return Library::Call< int, HRGN, int, int >(pFunc, hrgn, nXOffset, nYOffset);
    }

    BOOL GDI32::PtInRegion(HRGN hrgn, int X, int Y)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(gdi32, PtInRegion);
        return Library::Call< BOOL, HRGN, int, int >(pFunc, hrgn, X, Y);
    }

    BOOL GDI32::PaintRgn(HDC hdc, HRGN hrgn)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(gdi32, PaintRgn);
        return Library::Call< BOOL, HDC, HRGN >(pFunc, hdc, hrgn);
    }

    BOOL GDI32::RectInRegion(HRGN hrgn, CONST::RECT* lprc)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(gdi32, RectInRegion);
        return Library::Call< BOOL, HRGN, CONST ::RECT* >(pFunc, hrgn, lprc);
    }

    int GDI32::SetPolyFillMode(HDC hdc, int iPolyFillMode)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(gdi32, SetPolyFillMode);
        return Library::Call< int, HDC, int >(pFunc, hdc, iPolyFillMode);
    }

    BOOL GDI32::SetRectRgn(HRGN hrgn, int nLeftRect, int nTopRect, int nRightRect, int nBottomRect)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(gdi32, SetRectRgn);
        return Library::Call< BOOL, HRGN, int, int, int, int >(pFunc, hrgn, nLeftRect, nTopRect, nRightRect, nBottomRect);
    }

}}}//qor::win::api
