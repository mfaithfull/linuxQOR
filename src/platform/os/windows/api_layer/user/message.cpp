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

    long User32::BroadcastSystemMessageT(DWORD dwFlags, LPDWORD lpdwRecipients, UINT uiMessage, WPARAM wParam, LPARAM lParam)
    {
        qor_pp_fcontext;
        qor_pp_useswinapiAW(user32,BroadcastSystemMessage);
        return Library::Call<long, DWORD, LPDWORD, UINT, WPARAM, LPARAM>(pFunc, dwFlags, lpdwRecipients, uiMessage, wParam, lParam);
    }

    long User32::BroadcastSystemMessageExT(DWORD dwFlags, LPDWORD lpdwRecipients, UINT uiMessage, WPARAM wParam, LPARAM lParam, ::PBSMINFO pBSMInfo)
    {
        qor_pp_fcontext;
        qor_pp_useswinapiAW(user32,BroadcastSystemMessageEx);
        return Library::Call<long, DWORD, LPDWORD, UINT, WPARAM, LPARAM, ::PBSMINFO>(pFunc, dwFlags, lpdwRecipients, uiMessage, wParam, lParam, pBSMInfo);
    }

    LRESULT User32::DispatchMessageT(const MSG* lpmsg)
    {
        qor_pp_fcontext;
        qor_pp_useswinapiAW(user32, DispatchMessage);
        return Library::Call<LRESULT, const MSG*>(pFunc, lpmsg);
    }

    BOOL User32::GetInputState(void)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, GetInputState);
        return Library::Call<BOOL>(pFunc);
    }

    BOOL User32::GetMessageT(LPMSG lpMsg, HWND hWnd, UINT wMsgFilterMin, UINT wMsgFilterMax)
    {
        qor_pp_fcontext;
        qor_pp_useswinapiAW(user32, GetMessage);
        return Library::Call<BOOL, LPMSG, HWND, UINT, UINT>(pFunc, lpMsg, hWnd, wMsgFilterMin, wMsgFilterMax);
    }

    LPARAM User32::GetMessageExtraInfo(void)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, GetMessageExtraInfo);
        return Library::Call<LPARAM>(pFunc);
    }

    DWORD User32::GetMessagePos(void)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, GetMessagePos);
        return Library::Call<DWORD>(pFunc);
    }

    LONG User32::GetMessageTime(void)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32,GetMessageTime);
        return Library::Call<LONG>(pFunc);
    }

    DWORD User32::GetQueueStatus(UINT flags)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, GetQueueStatus);
        return Library::Call<DWORD, UINT>(pFunc, flags);
    }

    BOOL User32::InSendMessage(void)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, InSendMessage);
        return Library::Call<BOOL>(pFunc);
    }

    DWORD User32::InSendMessageEx()
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, InSendMessageEx);
        return Library::Call<DWORD, LPVOID>(pFunc, nullptr);
    }

    BOOL User32::PeekMessageT(LPMSG lpMsg, HWND hWnd, UINT wMsgFilterMin, UINT wMsgFilterMax, UINT wRemoveMsg)
    {
        qor_pp_fcontext;
        qor_pp_useswinapiAW(user32, PeekMessage);
        return Library::Call<BOOL, LPMSG, HWND, UINT, UINT, UINT>(pFunc, lpMsg, hWnd, wMsgFilterMin, wMsgFilterMax, wRemoveMsg);
    }

    BOOL User32::PostMessageT(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
    {
        qor_pp_fcontext;
        qor_pp_useswinapiAW(user32, PostMessage);
        return Library::Call<BOOL, HWND, UINT, WPARAM, LPARAM>(pFunc, hWnd, Msg, wParam, lParam);
    }

    void User32::PostQuitMessage(int nExitCode)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, PostQuitMessage);
        return Library::voidCall<int>(pFunc, nExitCode);
    }

    BOOL User32::PostThreadMessageT(DWORD idThread, UINT Msg, WPARAM wParam, LPARAM lParam)
    {
        qor_pp_fcontext;
        qor_pp_useswinapiAW(user32, PostThreadMessage);
        return Library::Call<BOOL, DWORD, UINT, WPARAM, LPARAM>(pFunc, idThread, Msg, wParam, lParam);
    }

    UINT User32::RegisterWindowMessageT(LPCTSTR lpString)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, RegisterWindowMessage);
        return Library::Call<UINT, LPCTSTR>(pFunc, lpString);
    }

    BOOL User32::ReplyMessage(LRESULT lResult)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, ReplyMessage);
        return Library::Call<BOOL, LRESULT>(pFunc, lResult);
    }

    BOOL User32::SendMessageCallbackT(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam, ::SENDASYNCPROC lpCallBack, ULONG_PTR dwData)
    {
        qor_pp_fcontext;
        qor_pp_useswinapiAW(user32, SendMessageCallback);
        return Library::Call<BOOL, HWND, UINT, WPARAM, LPARAM, ::SENDASYNCPROC, ULONG_PTR>(pFunc, hWnd, Msg, wParam, lParam, lpCallBack, dwData);
    }

    LRESULT User32::SendMessageTimeoutT(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam, UINT fuFlags, UINT uTimeout, PDWORD_PTR lpdwResult)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, SendMessageTimeout);
        return Library::Call<LRESULT, HWND, UINT, WPARAM, LPARAM, UINT, UINT, PDWORD_PTR>(pFunc, hWnd, Msg, wParam, lParam, fuFlags, uTimeout, lpdwResult);
    }

    LRESULT User32::SendMessageT(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, SendMessage);
        return Library::Call<LRESULT, HWND, UINT, WPARAM, LPARAM>(pFunc, hWnd, Msg, wParam, lParam);
    }

    BOOL User32::SendNotifyMessageT(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, SendNotifyMessage);
        return Library::Call<BOOL, HWND, UINT, WPARAM, LPARAM>(pFunc, hWnd, Msg, wParam, lParam);
    }

    LPARAM User32::SetMessageExtraInfo(LPARAM lParam)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, SetMessageExtraInfo);
        return Library::Call<LPARAM, LPARAM>(pFunc, lParam);
    }

    BOOL User32::TranslateMessage(const MSG* lpMsg)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, TranslateMessage);
        return Library::Call<BOOL, const MSG*>(pFunc, lpMsg);
    }

    BOOL User32::WaitMessage(void)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, WaitMessage);
        return Library::Call<BOOL>(pFunc);
    }

}}}//qor::win::api
