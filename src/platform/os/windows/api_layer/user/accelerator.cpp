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

    //The accelerator families are A/W exports only - user32.dll exports CopyAcceleratorTableA/W,
    //CreateAcceleratorTableA/W, LoadAcceleratorsA/W and TranslateAcceleratorA/W but no plain stem for the
    //first three (TranslateAccelerator is exported as an alias of TranslateAcceleratorA). Binding the bare
    //stem only works while the SDK's A/W macro is still in scope at the call site, where it silently rewrites
    //the name before _ATXT() stringizes it; the explicit AW form spells the name that GetProcAddress is
    //actually given, so the binding survives any #undef of the stem (user32.h:9-17 #undef's nine such stems).

    int User32::CopyAcceleratorTableT(HACCEL hAccelSrc, LPACCEL lpAccelDst, int cAccelEntries)
    {
        qor_pp_fcontext;
        qor_pp_useswinapiAW(user32, CopyAcceleratorTable);
        return Library::Call<int, HACCEL, LPACCEL, int>(pFunc, hAccelSrc, lpAccelDst, cAccelEntries);
    }

    HACCEL User32::CreateAcceleratorTableT(LPACCEL lpaccl, int cEntries)
    {
        qor_pp_fcontext;
        qor_pp_useswinapiAW(user32, CreateAcceleratorTable);
        return Library::Call<HACCEL, LPACCEL, int>(pFunc, lpaccl, cEntries);
    }

    //DestroyAcceleratorTable has no A/W variants - it is a single user32.dll export, so the bare stem is
    //the exported name and there is no variant to select.
    BOOL User32::DestroyAcceleratorTable(HACCEL hAccel)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, DestroyAcceleratorTable);
        return Library::Call<BOOL, HACCEL>(pFunc, hAccel);
    }

    HACCEL User32::LoadAcceleratorsT(HINSTANCE hInstance, LPCTSTR lpTableName)
    {
        qor_pp_fcontext;
        qor_pp_useswinapiAW(user32, LoadAccelerators);
        return Library::Call<HACCEL, HINSTANCE, LPCTSTR>(pFunc, hInstance, lpTableName);
    }

    int User32::TranslateAcceleratorT(HWND hWnd, HACCEL hAccTable, LPMSG lpMsg)
    {
        qor_pp_fcontext;
        qor_pp_useswinapiAW(user32, TranslateAccelerator);
        return Library::Call<int, HWND, HACCEL, LPMSG>(pFunc, hWnd, hAccTable, lpMsg);
    }

}}}//qor::win::api
