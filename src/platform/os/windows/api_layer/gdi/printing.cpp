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

    int GDI32::AbortDoc(HDC hdc)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(gdi32, AbortDoc);
        return Library::Call< int, HDC >(pFunc, hdc);
    }

    int GDI32::EndDoc(HDC hdc)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(gdi32, EndDoc);
        return Library::Call< int, HDC >(pFunc, hdc);
    }

    int GDI32::EndPage(HDC hdc)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(gdi32, EndPage);
        return Library::Call< int, HDC >(pFunc, hdc);
    }

    int GDI32::Escape(HDC hdc, int nEscape, int cbInput, LPCSTR lpvInData, LPVOID lpvOutData)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(gdi32, Escape);
        return Library::Call< int, HDC, int, int, LPCSTR, LPVOID >(pFunc, hdc, nEscape, cbInput, lpvInData, lpvOutData);
    }

    int GDI32::ExtEscape(HDC hdc, int nEscape, int cbInput, LPCSTR lpszInData, int cbOutput, LPSTR lpszOutData)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(gdi32, ExtEscape);
        return Library::Call< int, HDC, int, int, LPCSTR, int, LPSTR >(pFunc, hdc, nEscape, cbInput, lpszInData, cbOutput, lpszOutData);
    }

    int GDI32::SetAbortProc(HDC hdc, ::ABORTPROC lpAbortProc)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(gdi32, SetAbortProc);
        return Library::Call< int, HDC, ::ABORTPROC >(pFunc, hdc, lpAbortProc);
    }

    int GDI32::StartDocT(HDC hdc, CONST::DOCINFO* lpdi)
    {
        qor_pp_fcontext;
        qor_pp_useswinapiAW(gdi32, StartDoc);
        return Library::Call< int, HDC, CONST ::DOCINFO* >(pFunc, hdc, lpdi);
    }

    int GDI32::StartPage(HDC hDC)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(gdi32, StartPage);
        return Library::Call< int, HDC >(pFunc, hDC);
    }

}}}//qor::win::api
