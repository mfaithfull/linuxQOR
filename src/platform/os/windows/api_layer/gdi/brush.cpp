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

    HBRUSH GDI32::CreateBrushIndirect(CONST::LOGBRUSH* lplb)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(gdi32, CreateBrushIndirect);
        return Library::Call< HBRUSH, const ::LOGBRUSH* >(pFunc, lplb);
    }

    HBRUSH GDI32::CreateDIBPatternBrush(HGLOBAL hglbDIBPacked, UINT fuColorSpec)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(gdi32, CreateDIBPatternBrush);
        return Library::Call< HBRUSH, HGLOBAL, UINT >(pFunc, hglbDIBPacked, fuColorSpec);
    }

    HBRUSH GDI32::CreateHatchBrush(int fnStyle, COLORREF clrref)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(gdi32, CreateHatchBrush);
        return Library::Call< HBRUSH, int, COLORREF >(pFunc, fnStyle, clrref);
    }

    HBRUSH GDI32::CreatePatternBrush(HBITMAP hbmp)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(gdi32, CreatePatternBrush);
        return Library::Call< HBRUSH, HBITMAP >(pFunc, hbmp);
    }

    HBRUSH GDI32::CreateSolidBrush(COLORREF crColor)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(gdi32, CreateSolidBrush);
        return Library::Call< HBRUSH, COLORREF >(pFunc, crColor);
    }

    BOOL GDI32::GetBrushOrgEx(HDC hdc, ::LPPOINT lppt)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(gdi32, GetBrushOrgEx);
        return Library::Call< BOOL, HDC, ::LPPOINT >(pFunc, hdc, lppt);
    }

    BOOL GDI32::PatBlt(HDC hdc, int nXLeft, int nYLeft, int nWidth, int nHeight, DWORD dwRop)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(gdi32, PatBlt);
        return Library::Call< BOOL, HDC, int, int, int, int, DWORD >(pFunc, hdc, nXLeft, nYLeft, nWidth, nHeight, dwRop);
    }

    BOOL GDI32::SetBrushOrgEx(HDC hdc, int nXOrg, int nYOrg, ::LPPOINT lppt)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(gdi32, SetBrushOrgEx);
        return Library::Call< BOOL, HDC, int, int, ::LPPOINT >(pFunc, hdc, nXOrg, nYOrg, lppt);
    }

    HBRUSH GDI32::CreateDIBPatternBrushPt(CONST VOID* lpPackedDIB, UINT iUsage)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(gdi32, CreateDIBPatternBrushPt);
        return Library::Call< HBRUSH, CONST VOID*, UINT >(pFunc, lpPackedDIB, iUsage);
    }

    BOOL GDI32::FixBrushOrgEx(HDC hdc, int x, int y, LPPOINT ptl)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(gdi32, FixBrushOrgEx);
        return Library::Call< BOOL, HDC, int, int, LPPOINT >(pFunc, hdc, x, y, ptl);
    }

}}}//qor::win::api

