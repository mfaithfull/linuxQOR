
#include "src/platform/os/windows/api_layer/kernel/kernel32.h"
#include "resource.h"

static INT_PTR CALLBACK GoToDlgProc(HWND dlg, UINT msg, WPARAM wParam, LPARAM lParam) 
{
    switch (msg) 
    {
    case WM_INITDIALOG: 
        {
            SetDlgItemInt(dlg, IDC_GOTO_EDIT, 1, FALSE);
            HWND edit = GetDlgItem(dlg, IDC_GOTO_EDIT);
            SendMessageW(edit, EM_SETLIMITTEXT, 10, 0);
            return true;
        }
    case WM_COMMAND:
        switch (LOWORD(wParam)) 
        {
        case IDOK: 
            {
                BOOL ok = FALSE;
                UINT line = GetDlgItemInt(dlg, IDC_GOTO_EDIT, &ok, FALSE);
                if (!ok || line == 0) 
                {
                    MessageBox(dlg, TEXT("Enter a valid line number."), APP_TITLE, MB_ICONWARNING);
                    return true;
                }
                int maxLine = (int)SendMessage(g_app.hwndEdit, EM_GETLINECOUNT, 0, 0);
                if ((int)line > maxLine) line = (UINT)maxLine;
                int charIndex = (int)SendMessage(g_app.hwndEdit, EM_LINEINDEX, line - 1, 0);
                if (charIndex >= 0) 
                {
                    SendMessage(g_app.hwndEdit, EM_SETSEL, charIndex, charIndex);
                    SendMessage(g_app.hwndEdit, EM_SCROLLCARET, 0, 0);
                }
                EndDialog(dlg, IDOK);
                return true;
            }
        case IDCANCEL:
            EndDialog(dlg, IDCANCEL);
            return true;
        }
        break;
    }
    return false;
}
