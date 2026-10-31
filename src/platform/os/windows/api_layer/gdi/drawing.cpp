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

    BOOL GDI32::AngleArc(HDC hdc, int X, int Y, DWORD dwRadius, FLOAT eStartAngle, FLOAT eSweepAngle)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(gdi32, AngleArc);
        return Library::Call< BOOL, HDC, int, int, DWORD, FLOAT, FLOAT >(pFunc, hdc, X, Y, dwRadius, eStartAngle, eSweepAngle);
    }

    BOOL GDI32::Arc(HDC hdc, int nLeftRect, int nTopRect, int nRightRect, int nBottomRect, int nXStartArc, int nYStartArc, int nXEndArc, int nYEndArc)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(gdi32, Arc);
        return Library::Call< BOOL, HDC, int, int, int, int, int, int, int, int >(
            pFunc, hdc, nLeftRect, nTopRect, nRightRect, nBottomRect, nXStartArc, nYStartArc, nXEndArc, nYEndArc);
    }

    BOOL GDI32::ArcTo(HDC hdc, int nLeftRect, int nTopRect, int nRightRect, int nBottomRect, int nXRadial1, int nYRadial1, int nXRadial2, int nYRadial2)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(gdi32, ArcTo);
        return Library::Call< BOOL, HDC, int, int, int, int, int, int, int, int >(
            pFunc, hdc, nLeftRect, nTopRect, nRightRect, nBottomRect, nXRadial1, nYRadial1, nXRadial2, nYRadial2);
    }

    int GDI32::GetArcDirection(HDC hdc)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(gdi32, GetArcDirection);
        return Library::Call< int, HDC >(pFunc, hdc);
    }

    BOOL GDI32::LineDDA(int nXStart, int nYStart, int nXEnd, int nYEnd, ::LINEDDAPROC lpLineFunc, LPARAM lpData)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(gdi32, LineDDA);
        return Library::Call< BOOL, int, int, int, int, ::LINEDDAPROC, LPARAM >(pFunc, nXStart, nYStart, nXEnd, nYEnd, lpLineFunc, lpData);
    }

    BOOL GDI32::LineTo(HDC hdc, int nXEnd, int nYEnd)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(gdi32, LineTo);
        return Library::Call< BOOL, HDC, int, int >(pFunc, hdc, nXEnd, nYEnd);
    }

    BOOL GDI32::MoveToEx(HDC hdc, int X, int Y, ::LPPOINT lpPoint)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(gdi32, MoveToEx);
        return Library::Call< BOOL, HDC, int, int, ::LPPOINT >(pFunc, hdc, X, Y, lpPoint );
    }

    BOOL GDI32::PolyBezier(HDC hdc, CONST::POINT* lppt, DWORD cPoints)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(gdi32, PolyBezier);
        return Library::Call< BOOL, HDC, CONST ::POINT*, DWORD >(pFunc, hdc, lppt, cPoints);
    }

    BOOL GDI32::PolyBezierTo(HDC hdc, CONST::POINT* lppt, DWORD cCount)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(gdi32, PolyBezierTo);
        return Library::Call< BOOL, HDC, CONST ::POINT*, DWORD >(pFunc, hdc, lppt, cCount);
    }

    BOOL GDI32::PolyDraw(HDC hdc, CONST::POINT* lppt, CONST BYTE* lpbTypes, int cCount)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(gdi32, PolyDraw);
        return Library::Call< BOOL, HDC, CONST ::POINT*, CONST BYTE*, int >(pFunc, hdc, lppt, lpbTypes, cCount);
    }

    BOOL GDI32::Polyline(HDC hdc, CONST::POINT* lppt, int cPoints)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(gdi32, Polyline);
        return Library::Call< BOOL, HDC, CONST ::POINT*, int >(pFunc, hdc, lppt, cPoints);
    }

    BOOL GDI32::PolylineTo(HDC hdc, CONST::POINT* lppt, DWORD cCount)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(gdi32, PolylineTo);
        return Library::Call< BOOL, HDC, CONST ::POINT*, DWORD >(pFunc, hdc, lppt, cCount);
    }

    BOOL GDI32::PolyPolyline(HDC hdc, CONST::POINT* lppt, CONST DWORD* lpdwPolyPoints, DWORD cCount)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(gdi32, PolyPolyline);
        return Library::Call< BOOL, HDC, CONST ::POINT*, CONST DWORD*, DWORD >(pFunc, hdc, lppt, lpdwPolyPoints, cCount);
    }

    int GDI32::SetArcDirection(HDC hdc, int ArcDirection)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(gdi32, SetArcDirection);
        return Library::Call< int, HDC, int >(pFunc, hdc, ArcDirection);
    }

    BOOL GDI32::Chord(HDC hdc, int nLeftRect, int nTopRect, int nRightRect, int nBottomRect, int nXRadial1, int nYRadial1, int nXRadial2, int nYRadial2)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(gdi32, Chord);
        return Library::Call< BOOL, HDC, int, int, int, int, int, int, int, int >(
            pFunc, hdc, nLeftRect, nTopRect, nRightRect, nBottomRect, nXRadial1, nYRadial1, nXRadial2, nYRadial2);
    }

    BOOL GDI32::Ellipse(HDC hdc, int nLeftRect, int nTopRect, int nRightRect, int nBottomRect)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(gdi32, Ellipse);
        return Library::Call< BOOL, HDC, int, int, int, int >(pFunc, hdc, nLeftRect, nTopRect, nRightRect, nBottomRect);
    }

    BOOL GDI32::Pie(HDC hdc, int nLeftRect, int nTopRect, int nRightRect, int nBottomRect, int nXRadial1, int nYRadial1, int nXRadial2, int nYRadial2)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(gdi32, Pie);
        return Library::Call< BOOL, HDC, int, int, int, int, int, int, int, int >(
            pFunc, hdc, nLeftRect, nTopRect, nRightRect, nBottomRect, nXRadial1, nYRadial1, nXRadial2, nYRadial2);
    }

    BOOL GDI32::Polygon(HDC hdc, CONST::POINT* lpPoints, int nCount)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(gdi32, Polygon);
        return Library::Call< BOOL, HDC, CONST ::POINT*, int >(pFunc, hdc, lpPoints, nCount);
    }

    BOOL GDI32::PolyPolygon(HDC hdc, CONST::POINT* lpPoints, CONST INT* lpPolyCounts, int nCount)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(gdi32, PolyPolygon);
        return Library::Call< BOOL, HDC, CONST ::POINT*, CONST INT*, int >(pFunc, hdc, lpPoints, lpPolyCounts, nCount);
    }

    BOOL GDI32::Rectangle(HDC hdc, int nLeftRect, int nTopRect, int nRightRect, int nBottomRect)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(gdi32, Rectangle);
        return Library::Call< BOOL, HDC, int, int, int, int >(pFunc, hdc, nLeftRect, nTopRect, nRightRect, nBottomRect);
    }

    BOOL GDI32::RoundRect(HDC hdc, int nLeftRect, int nTopRect, int nRightRect, int nBottomRect, int nWidth, int nHeight)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(gdi32, RoundRect);
        return Library::Call< BOOL, HDC, int, int, int, int, int, int >(
            pFunc, hdc, nLeftRect, nTopRect, nRightRect, nBottomRect, nWidth, nHeight);
    }

}}}//qor::win::api
