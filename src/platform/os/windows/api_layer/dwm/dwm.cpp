// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"
#include "src/qor/module/module.h"
#include "src/qor/flyers/interception/functioncontext.h"
#include "src/qor/flyers/error/error.h"

//Windows specific headers must be last to prevent contaminating generic headers with Windows specific types and definitions
#include "dwm.h"
#include "../returncheck.h"
#include "../library.h"

namespace qor { namespace win { namespace api {

    BOOL DWM::DwmDefWindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam, LRESULT* plResult)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(dwmapi, DwmDefWindowProc);
        return Library::Call<BOOL, HWND, UINT, WPARAM, LPARAM, LRESULT*>(pFunc, hWnd, msg, wParam, lParam, plResult);
    }

    HRESULT DWM::EnableBlurBehindWindow(HWND hWnd, const DWM_BLURBEHIND* pBlurBehind)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(dwmapi, DwmEnableBlurBehindWindow);
        return Library::Call<HRESULT, HWND, const DWM_BLURBEHIND*>(pFunc, hWnd, pBlurBehind);
    }

    HRESULT DWM::EnableComposition(bool enable)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(dwmapi, DwmEnableComposition);
        return Library::Call<HRESULT, UINT>(pFunc, enable ? DWM_EC_ENABLECOMPOSITION : DWM_EC_DISABLECOMPOSITION);
    }

    HRESULT DWM::EnableMMCSS(bool enableMMCSS)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(dwmapi, DwmEnableMMCSS);
        return Library::Call<HRESULT, BOOL>(pFunc, enableMMCSS ? TRUE : FALSE);
    }

    HRESULT DWM::ExtendFrameIntoClientArea(HWND hWnd, const MARGINS* pMarInset)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(dwmapi, DwmExtendFrameIntoClientArea);
        return Library::Call<HRESULT, HWND, const MARGINS*>(pFunc, hWnd, pMarInset);
    }

    HRESULT DWM::Flush()
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(dwmapi, DwmFlush);
        return Library::Call<HRESULT>(pFunc);
    }

    HRESULT DWM::GetColorizationColor(DWORD* pcrColorization,BOOL* pfOpaqueBlend)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(dwmapi, DwmGetColorizationColor);
        return Library::Call<HRESULT, DWORD*, BOOL*>( pFunc, pcrColorization, pfOpaqueBlend);
    }

    HRESULT DWM::GetCompositionTimingInfo(DWM_TIMING_INFO *pTimingInfo)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(dwmapi, DwmGetCompositionTimingInfo);
        return Library::Call<HRESULT, HWND, DWM_TIMING_INFO*>(pFunc, NULL, pTimingInfo);
    }

    HRESULT DWM::GetTransportAttributes(BOOL* pfIsRemoting, BOOL* pfIsConnected, DWORD* pDwGeneration)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(dwmapi, DwmGetTransportAttributes);
        return Library::Call<HRESULT, BOOL*, BOOL*, DWORD*>(pFunc, pfIsRemoting, pfIsConnected, pDwGeneration);
    }

    HRESULT DWM::GetWindowAttribute(HWND hwnd,DWORD dwAttribute, PVOID pvAttribute, DWORD cbAttribute)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(dwmapi, DwmGetWindowAttribute);
        return Library::Call<HRESULT, HWND, DWORD, PVOID, DWORD>(pFunc, hwnd, dwAttribute, pvAttribute, cbAttribute);
    }

    HRESULT DWM::InvalidateIconicBitmaps(HWND hwnd)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(dwmapi, DwmInvalidateIconicBitmaps);
        return Library::Call<HRESULT, HWND>(pFunc, hwnd);
    }

    HRESULT DWM::IsCompositionEnabled(BOOL* pfEnabled)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(dwmapi, DwmIsCompositionEnabled);
        return Library::Call<HRESULT, BOOL*>(pFunc, pfEnabled);
    }

    HRESULT DWM::QueryThumbnailSourceSize(HTHUMBNAIL hThumbnail, PSIZE pSize)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(dwmapi, DwmQueryThumbnailSourceSize);
        return Library::Call<HRESULT, HTHUMBNAIL, PSIZE>(pFunc, hThumbnail, pSize);
    }

    HRESULT DWM::RegisterThumbnail(HWND hwndDestination, HWND hwndSource, PHTHUMBNAIL phThumbnailId)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(dwmapi, DwmRegisterThumbnail);
        return Library::Call<HRESULT, HWND, HWND, PHTHUMBNAIL>(pFunc, hwndDestination, hwndSource, phThumbnailId);
    }

    HRESULT DWM::RenderGesture(GESTURE_TYPE gt, UINT cContacts, const DWORD* pdwPointerID, const POINT* pPoints)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(dwmapi, DwmRenderGesture);
        return Library::Call<HRESULT, GESTURE_TYPE, UINT, const DWORD*, const POINT*>(pFunc, gt, cContacts, pdwPointerID, pPoints);
    }

    HRESULT DWM::SetIconicLivePreviewBitmap(HWND hwnd, HBITMAP hbmp, POINT* pptClient, DWORD dwSITFlags)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(dwmapi, DwmSetIconicLivePreviewBitmap);
        return Library::Call<HRESULT, HWND, HBITMAP, POINT*, DWORD>(pFunc, hwnd, hbmp, pptClient, dwSITFlags);
    }

    HRESULT DWM::SetIconicThumbnail(HWND hwnd, HBITMAP hbmp, DWORD dwSITFlags)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(dwmapi, DwmSetIconicThumbnail);
        return Library::Call<HRESULT, HWND, HBITMAP, DWORD>(pFunc, hwnd, hbmp, dwSITFlags);
    }

    HRESULT DWM::SetWindowAttribute(HWND hwnd, DWORD dwAttribute, LPCVOID pvAttribute, DWORD cbAttribute)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(dwmapi, DwmSetWindowAttribute);
        return Library::Call<HRESULT, HWND, DWORD, LPCVOID, DWORD>(pFunc, hwnd, dwAttribute, pvAttribute, cbAttribute);
    }

    HRESULT DWM::ShowContact(DWORD dwPointerID, DWM_SHOWCONTACT eShowContact)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(dwmapi, DwmShowContact);
        return Library::Call<HRESULT, DWORD, DWM_SHOWCONTACT>(pFunc, dwPointerID, eShowContact);
    }

    HRESULT DWM::TetherContact(DWORD dwPointerID, BOOL fEnable, POINT ptTether)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(dwmapi, DwmTetherContact);
        return Library::Call<HRESULT, DWORD, BOOL, POINT>(pFunc, dwPointerID, fEnable, ptTether);
    }

    HRESULT DWM::TransitionOwnedWindow(HWND hwnd, DWMTRANSITION_OWNEDWINDOW_TARGET target)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(dwmapi, DwmTransitionOwnedWindow);
        return Library::Call<HRESULT, HWND, DWMTRANSITION_OWNEDWINDOW_TARGET>(pFunc, hwnd, target);
    }

    HRESULT DWM::UnregisterThumbnail(HTHUMBNAIL hThumbnailId)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(dwmapi, DwmUnregisterThumbnail);
        return Library::Call<HRESULT, HTHUMBNAIL>(pFunc, hThumbnailId);
    }

    HRESULT DWM::UpdateThumbnailProperties(HTHUMBNAIL hThumbnailId, const DWM_THUMBNAIL_PROPERTIES* ptnProperties)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(dwmapi, DwmUpdateThumbnailProperties);
        return Library::Call<HRESULT, HTHUMBNAIL, const DWM_THUMBNAIL_PROPERTIES*>(pFunc, hThumbnailId, ptnProperties);
    }

}}}//qor::win::api
