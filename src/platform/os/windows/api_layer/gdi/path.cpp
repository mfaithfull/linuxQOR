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

    BOOL GDI32::AbortPath(HDC hdc)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(gdi32, AbortPath);
        return Library::Call< BOOL, HDC >(pFunc, hdc);
    }

    BOOL GDI32::BeginPath(HDC hdc)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(gdi32, BeginPath);
        return Library::Call< BOOL, HDC >(pFunc, hdc);
    }

    BOOL GDI32::CloseFigure(HDC hdc)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(gdi32, CloseFigure);
        return Library::Call< BOOL, HDC >(pFunc, hdc);
    }

    BOOL GDI32::EndPath(HDC hdc)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(gdi32, EndPath);
        return Library::Call< BOOL, HDC >(pFunc, hdc);
    }

    BOOL GDI32::FillPath(HDC hdc)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(gdi32, FillPath);
        return Library::Call< BOOL, HDC >(pFunc, hdc);
    }

    BOOL GDI32::FlattenPath(HDC hdc)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(gdi32, FlattenPath);
        return Library::Call< BOOL, HDC >(pFunc, hdc);
    }

    BOOL GDI32::GetMiterLimit(HDC hdc, PFLOAT peLimit)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(gdi32, GetMiterLimit);
        return Library::Call< BOOL, HDC, PFLOAT >(pFunc, hdc, peLimit);
    }

    int GDI32::GetPath(HDC hdc, ::LPPOINT lpPoints, LPBYTE lpTypes, int nSize)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(gdi32, GetPath);
        return Library::Call< int, HDC, ::LPPOINT, LPBYTE, int >(pFunc, hdc, lpPoints, lpTypes, nSize);
    }

    HRGN GDI32::PathToRegion(HDC hdc)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(gdi32, PathToRegion);
        return Library::Call< HRGN, HDC >(pFunc, hdc);
    }

    BOOL GDI32::SetMiterLimit(HDC hdc, FLOAT eNewLimit, PFLOAT peOldLimit)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(gdi32, SetMiterLimit);
        return Library::Call< BOOL, HDC, FLOAT, PFLOAT >(pFunc, hdc, eNewLimit, peOldLimit);
    }

    BOOL GDI32::StrokeAndFillPath(HDC hdc)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(gdi32, StrokeAndFillPath);
        return Library::Call< BOOL, HDC >(pFunc, hdc);
    }

    BOOL GDI32::StrokePath(HDC hdc)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(gdi32, StrokePath);
        return Library::Call< BOOL, HDC >(pFunc, hdc);
    }

    BOOL GDI32::WidenPath(HDC hdc)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(gdi32, WidenPath);
        return Library::Call< BOOL, HDC >(pFunc, hdc);
    }

}}}//qor::win::api
