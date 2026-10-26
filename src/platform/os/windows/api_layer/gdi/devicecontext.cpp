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

    HDC GDI32::CreateCompatibleDC(HDC hdc)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(gdi32, CreateCompatibleDC);
        return Library::Call< HDC, HDC >(pFunc, hdc);
    }

    HDC GDI32::CreateDCT(LPCTSTR lpszDriver, LPCTSTR lpszDevice, LPCTSTR lpszOutput, CONST::DEVMODE* lpInitData)
    {
        qor_pp_fcontext;
        qor_pp_useswinapiAW(gdi32, CreateDC);
        return Library::Call< HDC, LPCTSTR, LPCTSTR, LPCTSTR, CONST ::DEVMODE* >(pFunc, lpszDriver, lpszDevice, lpszOutput, lpInitData);
    }

    BOOL GDI32::DeleteDC(HDC hdc)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(gdi32, DeleteDC);
        return Library::Call< BOOL, HDC >(pFunc, hdc);
    }

    BOOL GDI32::CancelDC(HDC hdc)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(gdi32, CancelDC);
        return Library::Call< BOOL, HDC >(pFunc, hdc);
    }

    HDC GDI32::CreateICT(LPCTSTR lpszDriver, LPCTSTR lpszDevice, LPCTSTR lpszOutput, CONST::DEVMODE* lpdvmInit)
    {
        qor_pp_fcontext;
        qor_pp_useswinapiAW(gdi32, CreateIC);
        return Library::Call< HDC, LPCTSTR, LPCTSTR, LPCTSTR, CONST ::DEVMODE* >(pFunc, lpszDriver, lpszDevice, lpszOutput, lpdvmInit);
    }

    BOOL GDI32::DeleteObject(HGDIOBJ hObject)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(gdi32, DeleteObject);
        return Library::Call< BOOL, HGDIOBJ >(pFunc, hObject);
    }

    int GDI32::DrawEscape(HDC hdc, int nEscape, int cbInput, LPCSTR lpszInData)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(gdi32, DrawEscape);
        return Library::Call< int, HDC, int, int, LPCSTR >(pFunc, hdc, nEscape, cbInput, lpszInData);
    }

    int GDI32::EnumObjects(HDC hdc, int nObjectType, GOBJENUMPROC lpObjectFunc, LPARAM lParam)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(gdi32, EnumObjects);
        return Library::Call< int, HDC, int, GOBJENUMPROC, LPARAM >(pFunc, hdc, nObjectType, lpObjectFunc, lParam);
    }

    HGDIOBJ GDI32::GetCurrentObject(HDC hdc, UINT uObjectType)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(gdi32, GetCurrentObject);
        return Library::Call< HGDIOBJ, HDC, UINT >(pFunc, hdc, uObjectType);
    }

    COLORREF GDI32::GetDCBrushColor(HDC hdc)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(gdi32, GetDCBrushColor);
        return Library::Call< COLORREF, HDC >(pFunc, hdc);
    }

    BOOL GDI32::GetDCOrgEx(HDC hdc, ::LPPOINT lpPoint)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(gdi32, GetDCOrgEx);
        return Library::Call< BOOL, HDC, ::LPPOINT >(pFunc, hdc, lpPoint);
    }

    COLORREF GDI32::GetDCPenColor(HDC hdc)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(gdi32, GetDCPenColor);
        return Library::Call< COLORREF, HDC >(pFunc, hdc);
    }

    int GDI32::GetDeviceCaps(HDC hdc, int nIndex)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(gdi32, GetDeviceCaps);
        return Library::Call< int, HDC, int >(pFunc, hdc, nIndex);
    }

    DWORD GDI32::GetLayout(HDC hdc)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(gdi32, GetLayout);
        return Library::Call< DWORD, HDC >(pFunc, hdc);
    }

    int GDI32::GetObjectT(HGDIOBJ hgdiobj, int cbBuffer, LPVOID lpvObject)
    {
        qor_pp_fcontext;
        qor_pp_useswinapiAW(gdi32, GetObject);
        return Library::Call< int, HGDIOBJ, int, LPVOID >(pFunc, hgdiobj, cbBuffer, lpvObject);
    }

    DWORD GDI32::GetObjectType(HGDIOBJ h)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(gdi32, GetObjectType);
        return Library::Call< DWORD, HGDIOBJ >(pFunc, h);
    }

    HGDIOBJ GDI32::GetStockObject(int fnObject)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(gdi32, GetStockObject);
        return Library::Call< HGDIOBJ, int >(pFunc, fnObject);
    }

    HDC GDI32::ResetDCT(HDC hdc, CONST::DEVMODE* lpInitData)
    {
        qor_pp_fcontext;
        qor_pp_useswinapiAW(gdi32, ResetDC);
        return Library::Call< HDC, HDC, CONST ::DEVMODE* >(pFunc, hdc, lpInitData);
    }

    BOOL GDI32::RestoreDC(HDC hdc, int nSavedDC)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(gdi32, RestoreDC);
        return Library::Call< BOOL, HDC, int >(pFunc, hdc, nSavedDC);
    }

    int GDI32::SaveDC(HDC hdc)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(gdi32, SaveDC);
        return Library::Call< int, HDC >(pFunc, hdc );
    }

    HGDIOBJ GDI32::SelectObject(HDC hdc, HGDIOBJ hgdiobj)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(gdi32, SelectObject);
        return Library::Call< HGDIOBJ, HDC, HGDIOBJ >(pFunc, hdc, hgdiobj);
    }

    COLORREF GDI32::SetDCBrushColor(HDC hdc, COLORREF crColour)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(gdi32, SetDCBrushColor);
        return Library::Call< COLORREF, HDC, COLORREF >(pFunc, hdc, crColour );
    }

    COLORREF GDI32::SetDCPenColor(HDC hdc, COLORREF crColour)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(gdi32, SetDCPenColor);
        return Library::Call< COLORREF, HDC, COLORREF >(pFunc, hdc, crColour);
    }

    DWORD GDI32::SetLayout(HDC hdc, DWORD dwLayout)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(gdi32, SetLayout);
        return Library::Call< DWORD, HDC, DWORD >(pFunc, hdc, dwLayout);
    }

    BOOL GDI32::AlphaBlend(HDC hdcDest, int xoriginDest, int yoriginDest, int wDest, int hDest, HDC hdcSrc, int xoriginSrc, int yoriginSrc, int wSrc, int hSrc, BLENDFUNCTION ftn)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(gdi32, AlphaBlend);
        return Library::Call< BOOL, HDC, int, int, int, int, HDC, int, int, int, int, BLENDFUNCTION >(
            pFunc, hdcDest, xoriginDest, yoriginDest, wDest, hDest, hdcSrc, xoriginSrc, yoriginSrc, wSrc, hSrc, ftn);
    }

}}}//qor::win::api