#include "src/configuration/configuration.h"

#include "src/platform/os/windows/gui/windowclassregistration.h"
#include "src/platform/os/windows/gui/window.h"
#include "src/platform/os/windows/api_layer/kernel/kernel32.h"
#include <commdlg.h>
#include <commctrl.h>
#include <shellapi.h>

#include "controller.h"
#include "resource.h"

using namespace qor::nswindows::api;
using namespace qor::platform::nswindows;

LRESULT MainWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

RetroPadController::RetroPadController(void* instance) : m_instance(instance),
m_icon(LoadIcon((HINSTANCE)(m_instance), MAKEINTRESOURCE(IDI_RETROPAD))),
m_cursor(LoadCursor(NULL, IDC_IBEAM)),
m_backgroundBrush(COLOR_BACKGROUND)
{
    //Initialize the model
    m_model.SetEncoding(TextEncoding::ENC_UTF8);
    m_model.SetFindFlags(FR_DOWN);    

    //Initialize the OS interaction
    m_findMsg = RegisterWindowMessage(FINDMSGSTRING);
    m_windowClass = new WindowClass;
    m_windowClass->SetStyle( CS_HREDRAW | CS_VREDRAW );
    m_windowClass->SetMessageFunction((messageFunc)MainWndProc);
    m_windowClass->SetInstance((HINSTANCE)(m_instance));
    m_windowClass->SetIcon(m_icon);
    m_windowClass->SetSmallIcon(m_icon);
    m_windowClass->SetCursor(m_cursor);
    m_windowClass->SetBackgroundBrush(m_backgroundBrush);
    m_windowClass->SetName(TEXT("RETROPAD_WINDOW"));
    m_windowClass->SetMenuName((const TCHAR*)1000);
    m_wcReg = new WindowClassRegistration(*m_windowClass);    
}

RetroPadController::~RetroPadController()
{
    delete m_wcReg;
}

int RetroPadController::Run()
{
    m_view = ViewState::Create(m_instance, m_windowClass->Name(), m_menu, &m_model, this);    
    m_view->Show();
    int result = m_view->Run();
    delete m_view;
    return result;
}

static INT_PTR CALLBACK GoToDlgProc(HWND dlg, UINT msg, WPARAM wParam, LPARAM lParam) 
{
    ViewState* view = reinterpret_cast<ViewState*>(lParam);

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
                    MessageBox(dlg, TEXT("Enter a valid line number."), APP_TITLE.c_str(), MB_ICONWARNING);
                    return true;
                }
                int maxLine = (int) view->hwndEdit->ProcessMessage(EM_GETLINECOUNT, 0, 0);
                if ((int)line > maxLine) line = (UINT)maxLine;
                int charIndex = (int) view->hwndEdit->ProcessMessage(EM_LINEINDEX, line - 1, 0);
                if (charIndex >= 0) 
                {
                    view->hwndEdit->ProcessMessage(EM_SETSEL, charIndex, charIndex);
                    view->hwndEdit->ProcessMessage(EM_SCROLLCARET, 0, 0);
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

static INT_PTR CALLBACK AboutDlgProc(HWND dlg, UINT msg, WPARAM wParam, LPARAM lParam) 
{
    switch (msg) {
    case WM_INITDIALOG:
        return TRUE;
    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) 
        {
            EndDialog(dlg, LOWORD(wParam));
            return TRUE;
        }
        break;
    }
    return FALSE;
}

void RetroPadController::HandleCommand(HWND hwnd, WPARAM wParam, LPARAM lParam) 
{
    switch (LOWORD(wParam)) 
    {
    case IDM_FILE_NEW:
        m_model.DoFileNew();
        m_view->DoFileNew();
        break;
    case IDM_FILE_OPEN:
        m_view->DoFileOpen();
        break;
    case IDM_FILE_SAVE:
        m_view->DoFileSave(FALSE);
        break;
    case IDM_FILE_SAVE_AS:
        m_view->DoFileSave(TRUE);
        break;
    case IDM_FILE_PAGE_SETUP:
    case IDM_FILE_PRINT:
        MessageBox(hwnd, TEXT("Printing is not implemented in Retropad."), APP_TITLE.c_str(), MB_ICONINFORMATION);
        break;
    case IDM_FILE_EXIT:
        PostMessage(hwnd, WM_CLOSE, 0, 0);
        break;
    case IDM_EDIT_UNDO:        
        m_view->hwndEdit->ProcessMessage(EM_UNDO, 0, 0);
        break;
    case IDM_EDIT_CUT:
        m_view->hwndEdit->ProcessMessage(WM_CUT, 0, 0);
        break;
    case IDM_EDIT_COPY:
        m_view->hwndEdit->ProcessMessage(WM_COPY, 0, 0);
        break;
    case IDM_EDIT_PASTE:
        m_view->hwndEdit->ProcessMessage(WM_PASTE, 0, 0);
        break;
    case IDM_EDIT_DELETE:
        m_view->hwndEdit->ProcessMessage(WM_CLEAR, 0, 0);
        break;
    case IDM_EDIT_FIND:
        m_view->ShowFindDialog();
        break;
    case IDM_EDIT_FIND_NEXT:
        m_view->DoFindNext(false);
        break;
    case IDM_EDIT_REPLACE:
        m_view->ShowReplaceDialog();
        break;
    case IDM_EDIT_GOTO:
        if (m_view->wordWrap) 
        {
            MessageBox(hwnd, TEXT("Go To is unavailable when Word Wrap is on."), APP_TITLE.c_str(), MB_ICONINFORMATION);
        } 
        else 
        {
            DialogBoxParam((HINSTANCE)(m_instance), MAKEINTRESOURCE(IDD_GOTO), hwnd, GoToDlgProc, (long long)(m_view));
        }
        break;
    case IDM_EDIT_SELECT_ALL:
        m_view->hwndEdit->ProcessMessage(EM_SETSEL, 0, -1);
        break;
    case IDM_EDIT_TIME_DATE:
        m_model.InsertTimeDate();
        break;

    case IDM_FORMAT_WORD_WRAP:
        m_view->SetWordWrap(m_view->wordWrap);
        break;
    case IDM_FORMAT_FONT:
        m_view->DoSelectFont();
        break;

    case IDM_VIEW_STATUS_BAR:
        m_view->ToggleStatusBar(m_view->statusVisible);
        break;

    case IDM_HELP_VIEW_HELP:
        MessageBox(hwnd, TEXT("No help file is available for Retropad."), APP_TITLE.c_str(), MB_ICONINFORMATION);
        break;
    case IDM_HELP_ABOUT:
        DialogBox((HINSTANCE)(m_instance), MAKEINTRESOURCE(IDD_ABOUT), hwnd, AboutDlgProc);
        break;
    }
}

LRESULT MainWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    Window w(hwnd);
    RetroPadController* controller = reinterpret_cast<RetroPadController*>(w.GetPointer(GWLP_USERDATA));
    if(controller == nullptr)
    {
        controller = reinterpret_cast<RetroPadController*>(lParam);
    }
    if(controller)
    {
        return controller->MainWindowHandler(hwnd, msg, wParam, lParam);
    }
    else
    {
        return ::DefWindowProc(hwnd, msg, wParam, lParam);
    }
}

LRESULT RetroPadController::MainWindowHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (msg == m_findMsg) 
    {
        m_view->HandleFindReplace((LPFINDREPLACE)lParam);
        return 0;
    }

    switch (msg) 
    {
    case WM_CREATE: 
    {
        INITCOMMONCONTROLSEX icc = { sizeof(icc), ICC_BAR_CLASSES };
        InitCommonControlsEx(&icc);
        DragAcceptFiles(hwnd, TRUE);
        return 0;
    }
    case WM_SETFOCUS:
        if (m_view->hwndEdit) m_view->hwndEdit->SetFocus();
        return 0;
    case WM_SIZE:
        m_view->UpdateLayout();
        m_view->UpdateStatusBar();
        return 0;
    case WM_DROPFILES: 
    {
        HDROP hDrop = (HDROP)wParam;
        stdstring path;
        path.reserve(MAX_PATH_BUFFER);
        if (DragQueryFile(hDrop, 0, path.data(), MAX_PATH_BUFFER)) 
        {
            if (m_view->PromptSaveChanges()) 
            {
                m_model.LoadDocumentFromPath(path);
            }
        }
        DragFinish(hDrop);
        return 0;
    }
    case WM_COMMAND:
        if (HIWORD(wParam) == EN_CHANGE && (HWND)lParam == (HWND)(m_view->hwndEdit->GetHandle().Use())) 
        {
            if(m_view->hwndEdit->ProcessMessage(EM_GETMODIFY, 0, 0) != 0)
            {
                int length = m_view->hwndEdit->GetTextLength() + 1;
                m_model.Document().SetCapacity(length);
                size_t lengthRequest = (size_t)length;
                byte* space = m_model.Document().WriteRequest(lengthRequest);                
                int lengthUsed;
                m_view->GetEditText((TCHAR*)space, &lengthUsed);
                size_t lengthAcknowledge = (size_t)(lengthUsed);
                m_model.Document().WriteAcknowledge(lengthAcknowledge);
                m_model.SetModified();
            }
            m_view->UpdateTitle();
            m_view->UpdateStatusBar();
            return 0;
        } 
        else if (HIWORD(wParam) == EN_UPDATE && (HWND)lParam == (HWND)(m_view->hwndEdit->GetHandle().Use())) 
        {
            m_view->UpdateStatusBar();
            return 0;
        }
        HandleCommand(hwnd, wParam, lParam);
        return 0;
    case WM_INITMENUPOPUP:
        m_view->UpdateMenuStates();
        return 0;
    case WM_CLOSE:
        if (m_view->PromptSaveChanges()) 
        {
            DestroyWindow(hwnd);
        }
        return 0;
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hwnd, msg, wParam, lParam);
}
