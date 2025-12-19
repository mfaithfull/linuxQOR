

#include "src/configuration/configuration.h"
#include "mainwindowinstancecontroller.h"
#include "src/platform/os/windows/gui/gui.h"
#include "resource.h"

using namespace qor::platform::nswindows;

MainWindowFocusController::MainWindowFocusController(ViewState* viewState) : m_view(viewState)
{
}
    
void MainWindowFocusController::OnSetFocus(qor::platform::nswindows::Window& window, qor::platform::nswindows::Window& otherWindow)
{
    if (m_view->hwndEdit)
    {
         m_view->hwndEdit->SetFocus();
    }
}

MainWindowNonClientRenderController::MainWindowNonClientRenderController(ViewState* viewState) : m_view(viewState)
{
}

void MainWindowNonClientRenderController::OnNCSize(qor::platform::nswindows::Window& window, long long& lResult, unsigned long long wParam, long long lParam)
{
    m_view->UpdateLayout();
    m_view->UpdateStatusBar();   
}

MainWindowMenuController::MainWindowMenuController(ViewState* viewState) : m_view(viewState)
{
}

void MainWindowMenuController::OnInitMenuPopup(qor::platform::nswindows::Window& window, qor::platform::nswindows::Menu& menu, unsigned short wPos, bool bWindowMenu)
{
    m_view->UpdateMenuStates();
}

MainWindowDragAndDropController::MainWindowDragAndDropController(ViewState* viewState) : m_view(viewState)
{
}

void MainWindowDragAndDropController::OnDropFiles(qor::platform::nswindows::Window& window, void* drop)
{
    HDROP hDrop = (HDROP)drop;
    stdstring path;
    path.reserve(MAX_PATH_BUFFER);
    if (DragQueryFile(hDrop, 0, path.data(), MAX_PATH_BUFFER)) 
    {
        if (m_view->PromptSaveChanges()) 
        {
            m_view->m_model->LoadDocumentFromPath(path);
        }
    }
    DragFinish(hDrop);    
}

MainWindowInstanceController::MainWindowInstanceController()
{
    m_dragAndDrop = qor::new_ref<MainWindowDragAndDropController>(m_view);
    m_menu = qor::new_ref<MainWindowMenuController>(m_view);
    m_ncRendering = qor::new_ref<MainWindowNonClientRenderController>(m_view);
}

MainWindowInstanceController::~MainWindowInstanceController()
{
}

long long MainWindowInstanceController::OnDestroy(qor::platform::nswindows::Window& window, unsigned int uMsg, unsigned long long wParam, long long lParam)
{
    GUI::Quit(0);
    return 0;
}

void MainWindowInstanceController::OnClose(qor::platform::nswindows::Window& window)
{
    if (m_view->PromptSaveChanges()) 
    {
        window.Destroy();
    }
}

void MainWindowInstanceController::OnCommand(qor::platform::nswindows::Window& Window, unsigned short wNotify, unsigned short wID, long long lParam)
{
    if (wNotify == EN_CHANGE && (HWND)lParam == (HWND)(m_view->hwndEdit->GetHandle().Use())) 
    {
        if(m_view->hwndEdit->ProcessMessage(EM_GETMODIFY, 0, 0) != 0)
        {
            int length = m_view->hwndEdit->GetTextLength() + 1;
            m_view->m_model->Document().SetCapacity(length);
            size_t lengthRequest = (size_t)length;
            byte* space = m_view->m_model->Document().WriteRequest(lengthRequest);                
            int lengthUsed;
            m_view->GetEditText((TCHAR*)space, &lengthUsed);
            size_t lengthAcknowledge = (size_t)(lengthUsed);
            m_view->m_model->Document().WriteAcknowledge(lengthAcknowledge);
            m_view->m_model->SetModified();
        }
        m_view->UpdateTitle();
        m_view->UpdateStatusBar();
    } 
    else if (wNotify == EN_UPDATE && (HWND)lParam == (HWND)(m_view->hwndEdit->GetHandle().Use())) 
    {
        m_view->UpdateStatusBar();
    }
    HandleCommand(Window, wNotify, wID, lParam);    
}

static long long __stdcall AboutDlgProc(void* dlg, unsigned int msg, unsigned long long wParam, long long lParam) 
{
    switch (msg) {
    case WM_INITDIALOG:
        return TRUE;
    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) 
        {
            EndDialog((HWND)(dlg), LOWORD(wParam));
            return TRUE;
        }
        break;
    }
    return FALSE;
}

static long long __stdcall GoToDlgProc(void* dlg, unsigned int msg, unsigned long long wParam, long long lParam) 
{
    ViewState* view = reinterpret_cast<ViewState*>(lParam);

    switch (msg) 
    {
    case WM_INITDIALOG: 
        {
            SetDlgItemInt((HWND)(dlg), IDC_GOTO_EDIT, 1, FALSE);
            HWND edit = GetDlgItem((HWND)(dlg), IDC_GOTO_EDIT);
            SendMessageW(edit, EM_SETLIMITTEXT, 10, 0);
            return true;
        }
    case WM_COMMAND:
        switch (LOWORD(wParam)) 
        {
        case IDOK: 
            {
                BOOL ok = FALSE;
                UINT line = GetDlgItemInt((HWND)(dlg), IDC_GOTO_EDIT, &ok, FALSE);
                if (!ok || line == 0) 
                {
                    MessageBox((HWND)(dlg), TEXT("Enter a valid line number."), APP_TITLE.c_str(), MB_ICONWARNING);
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
                EndDialog((HWND)(dlg), IDOK);
                return true;
            }
        case IDCANCEL:
            EndDialog((HWND)(dlg), IDCANCEL);
            return true;
        }
        break;
    }
    return false;
}

void MainWindowInstanceController::HandleCommand(qor::platform::nswindows::Window& Window, unsigned short wNotify, unsigned short wID, long long lParam) 
{
    switch(wNotify) 
    {
    case IDM_FILE_NEW:
        m_view->m_model->DoFileNew();
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
        MessageBox((HWND)(Window.GetHandle().Use()), TEXT("Printing is not implemented in Retropad."), APP_TITLE.c_str(), MB_ICONINFORMATION);
        break;
    case IDM_FILE_EXIT:
        PostMessage((HWND)(Window.GetHandle().Use()), WM_CLOSE, 0, 0);
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
            MessageBox((HWND)(Window.GetHandle().Use()), TEXT("Go To is unavailable when Word Wrap is on."), APP_TITLE.c_str(), MB_ICONINFORMATION);
        } 
        else 
        {
            DialogBoxParam((HINSTANCE)(m_instance), MAKEINTRESOURCE(IDD_GOTO), (HWND)(Window.GetHandle().Use()), (DLGPROC)(GoToDlgProc), (long long)(m_view));
        }
        break;
    case IDM_EDIT_SELECT_ALL:
        m_view->hwndEdit->ProcessMessage(EM_SETSEL, 0, -1);
        break;
    case IDM_EDIT_TIME_DATE:
        m_view->m_model->InsertTimeDate();
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
        MessageBox((HWND)(Window.GetHandle().Use()), TEXT("No help file is available for Retropad."), APP_TITLE.c_str(), MB_ICONINFORMATION);
        break;
    case IDM_HELP_ABOUT:
        DialogBox((HINSTANCE)(m_instance), MAKEINTRESOURCE(IDD_ABOUT), (HWND)(Window.GetHandle().Use()), (DLGPROC)[](HWND dlg, unsigned int msg, unsigned long long wParam, long long lParam) -> long long 
        {
            switch (msg) 
            {
            case WM_INITDIALOG:
                return 1;
            case WM_COMMAND:
                if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) 
                {
                    EndDialog((HWND)(dlg), LOWORD(wParam));
                    return 1;
                }
                break;
            }
            return 0;
        });
        break;
    }
}




