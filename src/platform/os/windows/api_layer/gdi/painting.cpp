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

    BOOL GDI32::GdiFlush()
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(gdi32, GdiFlush);
        return Library::Call< BOOL >(pFunc);
    }

    DWORD GDI32::GdiGetBatchLimit()
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(gdi32, GdiGetBatchLimit);
        return Library::Call< DWORD >(pFunc);
    }

    DWORD GDI32::GdiSetBatchLimit(DWORD dwLimit)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(gdi32, GdiSetBatchLimit);
        return Library::Call< DWORD, DWORD >(pFunc, dwLimit);
    }

    COLORREF GDI32::GetBkColor(HDC hdc)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(gdi32, GetBkColor);
        return Library::Call< COLORREF, HDC >(pFunc, hdc);
    }

    int GDI32::GetBkMode(HDC hdc)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(gdi32, GetBkMode);
        return Library::Call< int, HDC >(pFunc, hdc);
    }

    UINT GDI32::GetBoundsRect(HDC hdc, ::LPRECT lprcBounds, UINT flags)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(gdi32, GetBoundsRect);
        return Library::Call< UINT, HDC, LPRECT, UINT >(pFunc, hdc, lprcBounds, flags);
    }

    int GDI32::GetROP2(HDC hdc)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(gdi32, GetROP2);
        return Library::Call< int, HDC >(pFunc, hdc);
    }

    COLORREF GDI32::SetBkColor(HDC hdc, COLORREF crColor)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(gdi32, SetBkColor);
        return Library::Call< COLORREF, HDC, COLORREF >(pFunc, hdc, crColor);
    }

    int GDI32::SetBkMode(HDC hdc, int iBkMode)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(gdi32, SetBkMode);
        return Library::Call< int, HDC, int >(pFunc, hdc, iBkMode);
    }

    UINT GDI32::SetBoundsRect(HDC hdc, CONST RECT* lprcBounds, UINT flags)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(gdi32, SetBoundsRect);
        return Library::Call< UINT, HDC, CONST RECT*, UINT >(pFunc, hdc, lprcBounds, flags);
    }

    int GDI32::SetROP2(HDC hdc, int fnDrawMode)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(gdi32, SetROP2);
        return Library::Call< int, HDC, int >(pFunc, hdc, fnDrawMode);
    }

    //GdiGradientFill is the spelling gdi32.dll exports; the bare GradientFill prototype in wingdi.h has
    //no export on the target system (tmp/_tmp_gdi_exports.txt).
    BOOL GDI32::GradientFill(HDC hdc, PTRIVERTEX pVertex, ULONG nVertex, PVOID pMesh, ULONG nMesh, ULONG ulMode)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(gdi32, GdiGradientFill);
        return Library::Call< BOOL, HDC, PTRIVERTEX, ULONG, PVOID, ULONG, ULONG >(pFunc, hdc, pVertex, nVertex, pMesh, nMesh, ulMode);
    }

}}}//qor::win::api

