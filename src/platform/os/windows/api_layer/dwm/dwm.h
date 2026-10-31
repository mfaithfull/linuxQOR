// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_OS_WINDOWS_API_DWM
#define QOR_PP_H_OS_WINDOWS_API_DWM

#include "src/platform/os/windows/api_layer/user/user32.h"

#include <dwmapi.h>

namespace qor { namespace win { namespace api {

	class qor_pp_module_interface(QOR_WINAPIDWM) DWM
	{
	public:

        static BOOL DwmDefWindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam, LRESULT* plResult);
        static HRESULT EnableBlurBehindWindow(HWND hWnd, const DWM_BLURBEHIND* pBlurBehind);
        static HRESULT EnableComposition(bool enable);
        static HRESULT EnableMMCSS(bool enableMMCSS);
        static HRESULT ExtendFrameIntoClientArea(HWND hWnd, const MARGINS* pMarInset);
        static HRESULT Flush();
        static HRESULT GetColorizationColor(DWORD* pcrColorization,BOOL* pfOpaqueBlend);
        static HRESULT GetCompositionTimingInfo(DWM_TIMING_INFO *pTimingInfo);
        static HRESULT GetTransportAttributes(BOOL* pfIsRemoting, BOOL* pfIsConnected, DWORD* pDwGeneration);
        static HRESULT GetWindowAttribute(HWND hwnd,DWORD dwAttribute, PVOID pvAttribute, DWORD cbAttribute);
        static HRESULT InvalidateIconicBitmaps(HWND hwnd);
        static HRESULT IsCompositionEnabled(BOOL* pfEnabled);

        static HRESULT QueryThumbnailSourceSize(HTHUMBNAIL hThumbnail, PSIZE pSize);
        static HRESULT RegisterThumbnail(HWND hwndDestination, HWND hwndSource, PHTHUMBNAIL phThumbnailId);
        static HRESULT RenderGesture(GESTURE_TYPE gt, UINT cContacts, const DWORD* pdwPointerID, const POINT* pPoints);
        static HRESULT SetIconicLivePreviewBitmap(HWND hwnd, HBITMAP hbmp, POINT* pptClient, DWORD dwSITFlags);
        static HRESULT SetIconicThumbnail(HWND hwnd, HBITMAP hbmp, DWORD dwSITFlags);
        static HRESULT SetWindowAttribute(HWND hwnd, DWORD dwAttribute, LPCVOID pvAttribute, DWORD cbAttribute);
        static HRESULT ShowContact(DWORD dwPointerID, DWM_SHOWCONTACT eShowContact);
        static HRESULT TetherContact(DWORD dwPointerID, BOOL fEnable, POINT ptTether);
        static HRESULT TransitionOwnedWindow(HWND hwnd, DWMTRANSITION_OWNEDWINDOW_TARGET target);
        static HRESULT UnregisterThumbnail(HTHUMBNAIL hThumbnailId);
        static HRESULT UpdateThumbnailProperties(HTHUMBNAIL hThumbnailId, const DWM_THUMBNAIL_PROPERTIES* ptnProperties);
    };

}}}//qor::win::api

#endif//QOR_PP_H_OS_WINDOWS_API_DWM
