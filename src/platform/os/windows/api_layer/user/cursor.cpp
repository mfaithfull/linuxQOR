// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"
#include "src/qor/module/module.h"
#include "src/qor/flyers/interception/functioncontext.h"
#include "src/qor/flyers/error/error.h"

//Windows specific headers must be last to prevent contaminating generic headers with Windows specific types and definitions
#include "user32.h"
#include "../returncheck.h"
#include "../library.h"

namespace qor { namespace win { namespace api {

    BOOL User32::ClipCursor(const RECT* lpRect)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, ClipCursor);
        return Library::Call<BOOL, const RECT*>(pFunc, lpRect);
    }

    HCURSOR User32::CopyCursorT(HCURSOR pcur)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, CopyIcon);
        return Library::Call< HCURSOR, HCURSOR >(pFunc, pcur);
    }

    HCURSOR User32::CreateCursor(HINSTANCE hInst, int xHotSpot, int yHotSpot, int nWidth, int nHeight, const VOID* pvANDPlane, const VOID* pvXORPlane)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, CreateCursor);
        return Library::Call< HCURSOR, HINSTANCE, int, int, int, int, const VOID*, const VOID* >(pFunc, hInst, xHotSpot, yHotSpot, nWidth, nHeight, pvANDPlane, pvXORPlane);
    }

    BOOL User32::DestroyCursor(HCURSOR hCursor)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, DestroyCursor);
        return Library::Call< BOOL, HCURSOR >(pFunc, hCursor);
    }

    BOOL User32::GetClipCursor(LPRECT lpRect)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, GetClipCursor);
        return Library::Call< BOOL, LPRECT >(pFunc, lpRect);
    }

    HCURSOR User32::GetCursor(void)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, GetCursor);
        return Library::Call< HCURSOR >(pFunc);
    }

    BOOL User32::GetCursorInfo(PCURSORINFO pci)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, GetCursorInfo);
        return Library::Call< BOOL, PCURSORINFO >(pFunc, pci);
    }

    BOOL User32::GetCursorPos(LPPOINT lpPoint)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, GetCursorPos);
        return Library::Call< BOOL, LPPOINT >(pFunc, lpPoint);
    }

    BOOL User32::GetPhysicalCursorPos(LPPOINT lpPoint)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, GetPhysicalCursorPos);
        return Library::Call< BOOL, LPPOINT >(pFunc, lpPoint);
    }

    HCURSOR User32::LoadCursorT(HINSTANCE hInstance, LPCTSTR lpCursorName)
    {
        qor_pp_fcontext;
        qor_pp_useswinapiAW(user32, LoadCursor);
        return Library::Call< HCURSOR, HINSTANCE, LPCTSTR >(pFunc, hInstance, lpCursorName);
    }

    HCURSOR User32::LoadCursorFromFileT(LPCTSTR lpFileName)
    {
        qor_pp_fcontext;
        qor_pp_useswinapiAW(user32, LoadCursorFromFile);
        return Library::Call< HCURSOR, LPCTSTR >(pFunc, lpFileName);
    }

    BOOL User32::SetCursorPos(int X, int Y)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, SetCursorPos);
        return Library::Call< BOOL, int, int >(pFunc, X, Y);
    }

    BOOL User32::SetPhysicalCursorPos(int X, int Y)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, SetPhysicalCursorPos);
        return Library::Call< BOOL, int, int >(pFunc, X, Y);
    }

    int User32::ShowCursor(BOOL bShow)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, ShowCursor);
        return Library::Call< int, BOOL >(pFunc, bShow);
    }

    HCURSOR User32::SetCursor(HCURSOR hCursor)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, SetCursor);
        return Library::Call< HCURSOR, HCURSOR >(pFunc, hCursor);
    }

    BOOL User32::SetSystemCursor(HCURSOR hcur, DWORD id)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, SetSystemCursor);
        return Library::Call< BOOL, HCURSOR, DWORD >(pFunc, hcur, id);
    }

}}}//qor::win::api
