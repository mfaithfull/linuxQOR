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

    BOOL GDI32::AnimatePalette(HPALETTE hpal, UINT iStartIndex, UINT cEntries, CONST::PALETTEENTRY* ppe)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(gdi32, AnimatePalette);
        return Library::Call< BOOL, HPALETTE, UINT, UINT, CONST ::PALETTEENTRY* >(pFunc, hpal, iStartIndex, cEntries, ppe);
    }

    HPALETTE GDI32::CreateHalftonePalette(HDC hdc)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(gdi32, CreateHalftonePalette);
        return Library::Call< HPALETTE, HDC >(pFunc, hdc);
    }

    HPALETTE GDI32::CreatePalette(CONST::LOGPALETTE* lplgpl)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(gdi32, CreatePalette);
        return Library::Call< HPALETTE, CONST ::LOGPALETTE* >(pFunc, lplgpl);
    }

    BOOL GDI32::GetColorAdjustment(HDC hdc, ::LPCOLORADJUSTMENT lpca)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(gdi32, GetColorAdjustment);
        return Library::Call< BOOL, HDC, ::LPCOLORADJUSTMENT >(pFunc, hdc, lpca);
    }

    COLORREF GDI32::GetNearestColor(HDC hdc, COLORREF crColor)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(gdi32, GetNearestColor);
        return Library::Call< COLORREF, HDC, COLORREF >(pFunc, hdc, crColor);
    }

    UINT GDI32::GetNearestPaletteIndex(HPALETTE hpal, COLORREF crColor)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(gdi32, GetNearestPaletteIndex);
        return Library::Call< UINT, HPALETTE, COLORREF >(pFunc, hpal, crColor);
    }

    UINT GDI32::GetPaletteEntries(HPALETTE hpal, UINT iStartIndex, UINT nEntries, ::LPPALETTEENTRY lppe)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(gdi32, GetPaletteEntries);
        return Library::Call< UINT, HPALETTE, UINT, UINT, ::LPPALETTEENTRY >(pFunc, hpal, iStartIndex, nEntries, lppe);
    }

    UINT GDI32::GetSystemPaletteEntries(HDC hdc, UINT iStartIndex, UINT nEntries, ::LPPALETTEENTRY lppe)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(gdi32, GetSystemPaletteEntries);
        return Library::Call< UINT, HDC, UINT, UINT, ::LPPALETTEENTRY >(pFunc, hdc, iStartIndex, nEntries, lppe);
    }

    UINT GDI32::GetSystemPaletteUse(HDC hdc)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(gdi32, GetSystemPaletteUse);
        return Library::Call< UINT, HDC >(pFunc, hdc);
    }

    UINT GDI32::RealizePalette(HDC hdc)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(gdi32, RealizePalette);
        return Library::Call< UINT, HDC >(pFunc, hdc);
    }

    BOOL GDI32::ResizePalette(HPALETTE hpal, UINT nEntries)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(gdi32, ResizePalette);
        return Library::Call< BOOL, HPALETTE, UINT >(pFunc, hpal, nEntries);
    }

    HPALETTE GDI32::SelectPalette(HDC hdc, HPALETTE hpal, BOOL bForceBackground)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(gdi32, SelectPalette);
        return Library::Call< HPALETTE, HDC, HPALETTE, BOOL >(pFunc, hdc, hpal, bForceBackground);
    }

    BOOL GDI32::SetColorAdjustment(HDC hdc, CONST::COLORADJUSTMENT* lpca)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(gdi32, SetColorAdjustment);
        return Library::Call< BOOL, HDC, CONST ::COLORADJUSTMENT* >(pFunc, hdc, lpca);
    }

    UINT GDI32::SetPaletteEntries(HPALETTE hpal, UINT iStart, UINT cEntries, CONST::PALETTEENTRY* lppe)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(gdi32, SetPaletteEntries);
        return Library::Call< UINT, HPALETTE, UINT, UINT, CONST ::PALETTEENTRY* >(pFunc, hpal, iStart, cEntries, lppe);
    }

    UINT GDI32::SetSystemPaletteUse(HDC hdc, UINT uUsage)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(gdi32, SetSystemPaletteUse);
        return Library::Call< UINT, HDC, UINT >(pFunc, hdc, uUsage);
    }

    BOOL GDI32::UnrealizeObject(HGDIOBJ hgdiobj)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(gdi32, UnrealizeObject);
        return Library::Call< BOOL, HGDIOBJ >(pFunc, hgdiobj);
    }

    BOOL GDI32::UpdateColors(HDC hdc)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(gdi32, UpdateColors);
        return Library::Call< BOOL, HDC >(pFunc, hdc);
    }

    BOOL GDI32::GetDeviceGammaRamp(HDC hDC, LPVOID lpRamp)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(gdi32, GetDeviceGammaRamp);
        return Library::Call< BOOL, HDC, LPVOID >(pFunc, hDC, lpRamp);
    }

    BOOL GDI32::SetDeviceGammaRamp(HDC hDC, LPVOID lpRamp)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(gdi32, SetDeviceGammaRamp);
        return Library::Call< BOOL, HDC, LPVOID >(pFunc, hDC, lpRamp);
    }

    int GDI32::EnumICMProfilesT(HDC hDC, ::ICMENUMPROC lpEnumICMProfilesFunc, LPARAM lParam)
    {
        qor_pp_fcontext;
        qor_pp_useswinapiAW(gdi32, EnumICMProfiles);
        return Library::Call< int, HDC, ::ICMENUMPROC, LPARAM >(pFunc, hDC, lpEnumICMProfilesFunc, lParam);
    }

    BOOL GDI32::GetICMProfileT(HDC hDC, LPDWORD lpcbName, LPTSTR lpszFilename)
    {
        qor_pp_fcontext;
        qor_pp_useswinapiAW(gdi32, GetICMProfile);
        return Library::Call< BOOL, HDC, LPDWORD, LPTSTR >(pFunc, hDC, lpcbName, lpszFilename);
    }

    int GDI32::SetICMMode(HDC hDC, int iEnableICM)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(gdi32, SetICMMode);
        return Library::Call< int, HDC, int >(pFunc, hDC, iEnableICM);
    }

    BOOL GDI32::SetICMProfileT(HDC hDC, LPTSTR lpFileName)
    {
        qor_pp_fcontext;
        qor_pp_useswinapiAW(gdi32, SetICMProfile);
        return Library::Call< BOOL, HDC, LPTSTR >(pFunc, hDC, lpFileName);
    }

    BOOL GDI32::CheckColorsInGamut(HDC hDC, LPVOID lpRGBTriples, LPVOID lpBuffer, UINT nCount)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(gdi32, CheckColorsInGamut);
        return Library::Call< BOOL, HDC, LPVOID, LPVOID, UINT >(pFunc, hDC, lpRGBTriples, lpBuffer, nCount);
    }

    BOOL GDI32::ColorCorrectPalette(HDC hDC, HPALETTE hPalette, DWORD dwFirstEntry, DWORD dwNumOfEntries)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(gdi32, ColorCorrectPalette);
        return Library::Call< BOOL, HDC, HPALETTE, DWORD, DWORD >(pFunc, hDC, hPalette, dwFirstEntry, dwNumOfEntries);
    }

    BOOL GDI32::ColorMatchToTarget(HDC hDC, HDC hdcTarget, DWORD uiAction)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(gdi32, ColorMatchToTarget);
        return Library::Call< BOOL, HDC, HDC, DWORD >(pFunc, hDC, hdcTarget, uiAction);
    }

    HCOLORSPACE GDI32::CreateColorSpaceT(::LPLOGCOLORSPACE lpLogColorSpace)
    {
        qor_pp_fcontext;
        qor_pp_useswinapiAW(gdi32, CreateColorSpace);
        return Library::Call< HCOLORSPACE, ::LPLOGCOLORSPACE >(pFunc, lpLogColorSpace);
    }

    BOOL GDI32::DeleteColorSpace(HCOLORSPACE hColorSpace)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(gdi32, DeleteColorSpace);
        return Library::Call< BOOL, HCOLORSPACE >(pFunc, hColorSpace);
    }

    HCOLORSPACE GDI32::GetColorSpace(HDC hDC)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(gdi32, GetColorSpace);
        return Library::Call< HCOLORSPACE, HDC >(pFunc, hDC);
    }

    BOOL GDI32::GetLogColorSpaceT(HCOLORSPACE hColorSpace, ::LPLOGCOLORSPACE lpBuffer, DWORD nSize)
    {
        qor_pp_fcontext;
        qor_pp_useswinapiAW(gdi32, GetLogColorSpace);
        return Library::Call< BOOL, HCOLORSPACE, ::LPLOGCOLORSPACE, DWORD >(pFunc, hColorSpace, lpBuffer, nSize);
    }

    HCOLORSPACE GDI32::SetColorSpace(HDC hDC, HCOLORSPACE hColorSpace)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(gdi32, SetColorSpace);
        return Library::Call< HCOLORSPACE, HDC, HCOLORSPACE >(pFunc, hDC, hColorSpace);
    }

    BOOL GDI32::SetMagicColors(HDC hdc, ULONG u1, ULONG u2)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(gdi32, SetMagicColors);
        return Library::Call< BOOL, HDC, ULONG, ULONG >(pFunc, hdc, u1, u2);
    }

}}}//qor::win::api
