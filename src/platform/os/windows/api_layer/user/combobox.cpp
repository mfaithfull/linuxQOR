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


    int User32::DlgDirListComboBox(HWND hDlg, LPTSTR lpPathSpec, int nIDComboBox, int nIDStaticPath, UINT uFiletype)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(User32, DlgDirListComboBox);
        return Library::Call<int, HWND, LPTSTR, int, int, UINT>(pFunc, hDlg, lpPathSpec, nIDComboBox, nIDStaticPath, uFiletype);
    }

    BOOL User32::DlgDirSelectComboBoxEx(HWND hDlg, LPTSTR lpString, int nCount, int nIDComboBox)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(User32, DlgDirSelectComboBoxEx);
        return Library::Call<BOOL, HWND, LPTSTR, int, int>(pFunc, hDlg, lpString, nCount, nIDComboBox);
    }

    BOOL User32::GetComboBoxInfo(HWND hwndCombo, PCOMBOBOXINFO pcbi)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(User32, GetComboBoxInfo);
        return Library::Call<BOOL, HWND, PCOMBOBOXINFO>(pFunc, hwndCombo, pcbi);
    }

}}}//qor::win::api
