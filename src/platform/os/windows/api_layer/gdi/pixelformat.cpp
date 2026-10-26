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

    //Pixel format functions - the six entry points wingdi.h declares for GDI pixel format selection.
    //All six are exported by gdi32.dll (tmp/_tmp_gdi_exports.txt) and none is #defined to an A/W pair,
    //so they bind unsuffixed through qor_pp_useswinapi.

    int GDI32::ChoosePixelFormat(HDC hdc, CONST::PIXELFORMATDESCRIPTOR* ppfd)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(gdi32, ChoosePixelFormat);
        return Library::Call< int, HDC, CONST ::PIXELFORMATDESCRIPTOR* >(pFunc, hdc, ppfd);
    }

    int GDI32::DescribePixelFormat(HDC hdc, int iPixelFormat, UINT nBytes, ::LPPIXELFORMATDESCRIPTOR ppfd)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(gdi32, DescribePixelFormat);
        return Library::Call< int, HDC, int, UINT, ::LPPIXELFORMATDESCRIPTOR >(pFunc, hdc, iPixelFormat, nBytes, ppfd);
    }

    int GDI32::GetPixelFormat(HDC hdc)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(gdi32, GetPixelFormat);
        return Library::Call< int, HDC >(pFunc, hdc);
    }

    BOOL GDI32::SetPixelFormat(HDC hdc, int iPixelFormat, CONST::PIXELFORMATDESCRIPTOR* ppfd)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(gdi32, SetPixelFormat);
        return Library::Call< BOOL, HDC, int, CONST ::PIXELFORMATDESCRIPTOR* >(pFunc, hdc, iPixelFormat, ppfd);
    }

    BOOL GDI32::SwapBuffers(HDC hdc)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(gdi32, SwapBuffers);
        return Library::Call< BOOL, HDC >(pFunc, hdc);
    }

    UINT GDI32::GetEnhMetaFilePixelFormat(HENHMETAFILE hemf, DWORD cbBuffer, CONST::PIXELFORMATDESCRIPTOR* ppfd)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(gdi32, GetEnhMetaFilePixelFormat);
        return Library::Call< UINT, HENHMETAFILE, DWORD, CONST ::PIXELFORMATDESCRIPTOR* >(pFunc, hemf, cbBuffer, ppfd);
    }

}}}//qor::win::api
