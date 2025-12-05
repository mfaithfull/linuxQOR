#include "src/configuration/configuration.h"

#include "src/platform/os/windows/api_layer/kernel/kernel32.h"
#include <commdlg.h>
#include <commctrl.h>
#include <shellapi.h>
#include <strsafe.h>
#include <tchar.h>

#include "viewmodel.h"
#include "model.h"
#include "src/platform/os/windows/api_layer/kernel/kernel32.h"
#include "resource.h"

constexpr int DEFAULT_WIDTH  = 640;
constexpr int DEFAULT_HEIGHT = 480;

ViewState* ViewState::Create(void* instance, const TCHAR* windowClass, const qor::platform::nswindows::Menu& menu, ModelState* model, void* param)
{
    ViewState* view = new ViewState;
    view->m_instance = instance;
    view->model = model;
    view->wordWrap = false;
    view->statusVisible = true;
    view->statusBeforeWrap = true;

    view->hwndMain = new qor::platform::nswindows::Window(windowClass, APP_TITLE, WS_OVERLAPPEDWINDOW, 0, CW_USEDEFAULT, CW_USEDEFAULT, DEFAULT_WIDTH, DEFAULT_HEIGHT, NULL, menu, (HINSTANCE)(view->m_instance), param);
    view->Show();

    return view;
}

void ViewState::Show()
{
    hwndMain->Show(SW_SHOWNORMAL);
    hwndMain->Update(); 
}

void ViewState::CreateEditControl()
{
    if (hwndEdit) 
    {
        hwndEdit->Destroy();
    }

    unsigned long style = WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_MULTILINE | ES_AUTOVSCROLL | ES_WANTRETURN | ES_NOHIDESEL;
    if (!wordWrap) 
    {
        style |= WS_HSCROLL | ES_AUTOHSCROLL;
    }
    
    qor::platform::nswindows::Menu menu(1);
    hwndEdit = new qor::platform::nswindows::Window( TEXT("EDIT"), nullptr, style, WS_EX_CLIENTEDGE, 0, 0, 0, 0, hwndMain, menu, m_instance, nullptr);
    
    if (hwndEdit && hFont) 
    {
        ApplyFontToEdit();
    }
    SendMessage((HWND)(hwndEdit->GetHandle().Use()), EM_SETLIMITTEXT, 0, 0); // allow large files
    UpdateLayout();
}

void ViewState::UpdateLayout() 
{
    qor::platform::nswindows::Rect rc;
    hwndMain->GetClientRect(rc);

    int statusHeight = 0;
    if (statusVisible && hwndStatus) 
    {
        SendMessage((HWND)(hwndStatus->GetHandle().Use()), WM_SIZE, 0, 0);
        qor::platform::nswindows::Rect sbrc;
        hwndStatus->GetWindowRect(sbrc);
        statusHeight = sbrc.m_bottom - sbrc.m_top;
        hwndStatus->Move(0, rc.m_bottom - statusHeight, rc.m_right, statusHeight, true);
    }

    if (hwndEdit) 
    {
        hwndEdit->Move(0, 0, rc.m_right, rc.m_bottom - statusHeight, true);
    }
}

void ViewState::ApplyFontToEdit()
{
    SendMessage((HWND)(hwndEdit->GetHandle().Use()), WM_SETFONT, (WPARAM)hFont, TRUE);
}

void ViewState::UpdateMenuStates() 
{
    qor::platform::nswindows::Menu menu(hwndMain->GetMenu());    

    UINT wrapState = wordWrap ? MF_CHECKED : MF_UNCHECKED;
    UINT statusState = statusVisible ? MF_CHECKED : MF_UNCHECKED;
    menu.CheckItem(IDM_FORMAT_WORD_WRAP, MF_BYCOMMAND | wrapState);
    menu.CheckItem(IDM_FORMAT_WORD_WRAP, MF_BYCOMMAND | wrapState);
    menu.CheckItem(IDM_VIEW_STATUS_BAR, MF_BYCOMMAND | statusState);

    bool canGoTo = !wordWrap;
    menu.EnableItem(IDM_EDIT_GOTO, MF_BYCOMMAND | (canGoTo ? MF_ENABLED : MF_GRAYED));
    if (wordWrap) 
    {
        menu.EnableItem(IDM_VIEW_STATUS_BAR, MF_BYCOMMAND | MF_GRAYED);
    } 
    else 
    {
        menu.EnableItem(IDM_VIEW_STATUS_BAR, MF_BYCOMMAND | MF_ENABLED);
    }

    bool modified = (SendMessage((HWND)(hwndEdit->GetHandle().Use()), EM_GETMODIFY, 0, 0) != 0);
    menu.EnableItem(IDM_FILE_SAVE, MF_BYCOMMAND | (modified ? MF_ENABLED : MF_GRAYED));
}

void ViewState::ToggleStatusBar(bool visible)
{
    statusVisible = visible;
    if (visible) 
    {
        if (!hwndStatus) 
        {
            hwndStatus = new qor::platform::nswindows::Window(CreateStatusWindow(WS_CHILD | SBARS_SIZEGRIP, TEXT(""), (HWND)(hwndMain->GetHandle().Use()), 2));
        }
        hwndStatus->Show(SW_SHOW);
    } 
    else if (hwndStatus) 
    {
        hwndStatus->Show(SW_HIDE);
    }
    UpdateLayout();
    UpdateStatusBar();
}

void ViewState::UpdateStatusBar()
{
    if (!statusVisible || !hwndStatus) return;
    DWORD selStart = 0, selEnd = 0;
    SendMessage((HWND)(hwndEdit->GetHandle().Use()), EM_GETSEL, (WPARAM)&selStart, (LPARAM)&selEnd);
    int line = (int)SendMessage((HWND)(hwndEdit->GetHandle().Use()), EM_LINEFROMCHAR, selStart, 0) + 1;
    int col = (int)(selStart - SendMessage((HWND)(hwndEdit->GetHandle().Use()), EM_LINEINDEX, line - 1, 0)) + 1;
    int lines = (int)SendMessage((HWND)(hwndEdit->GetHandle().Use()), EM_GETLINECOUNT, 0, 0);

    TCHAR status[128];
    StringCchPrintf(status, ARRAYSIZE(status), TEXT("Ln %d, Col %d    Lines: %d"), line, col, lines);
    SendMessage((HWND)(hwndStatus->GetHandle().Use()), SB_SETTEXT, 0, (LPARAM)status);
}

bool ViewState::GetEditText(TCHAR* buffer, int* lengthOut) 
{
    int length = hwndEdit->GetTextLength();    
    if (!buffer) return false;
    hwndEdit->GetText(buffer, length + 1);
    if (lengthOut) *lengthOut = length;
    return true;
}

void ViewState::SetWordWrap(bool enabled) 
{
    if (wordWrap == enabled) 
    {
        return;
    }
    wordWrap = enabled;
    
    TCHAR* text = nullptr;
    int len = 0;
    if (!GetEditText(text, &len)) 
    {
        return;
    }
    DWORD start = 0, end = 0;
    hwndEdit->ProcessMessage(EM_GETSEL, (WPARAM)&start, (LPARAM)&end);

    CreateEditControl();
    hwndEdit->SetText(text);    
    
    hwndEdit->ProcessMessage(EM_SETSEL, start, end);
    
    HeapFree(GetProcessHeap(), 0, text);

    if (enabled) 
    {
        statusBeforeWrap = statusVisible;
        ToggleStatusBar(false);
        hwndMain->GetMenu().EnableItem(IDM_VIEW_STATUS_BAR, MF_BYCOMMAND | MF_GRAYED);
        hwndMain->GetMenu().EnableItem(IDM_EDIT_GOTO, MF_BYCOMMAND | MF_GRAYED);
    } else {
        ToggleStatusBar(statusBeforeWrap);
        hwndMain->GetMenu().EnableItem(IDM_VIEW_STATUS_BAR, MF_BYCOMMAND | MF_ENABLED);
        hwndMain->GetMenu().EnableItem(IDM_EDIT_GOTO, MF_BYCOMMAND | MF_ENABLED);
    }
    UpdateTitle();
    UpdateStatusBar();
}

void ViewState::UpdateTitle() 
{
    TCHAR name[MAX_PATH_BUFFER];
    if (model->GetCurrentPath()[0]) 
    {
        TCHAR *fileName = _tcsrchr(model->GetCurrentPath(), TEXT('\\'));
        fileName = fileName ? fileName + 1 : model->GetCurrentPath();
        StringCchCopy(name, MAX_PATH_BUFFER, fileName);
    } 
    else 
    {
        StringCchCopy(name, MAX_PATH_BUFFER, UNTITLED_NAME);
    }

    TCHAR title[MAX_PATH_BUFFER + 32];
    StringCchPrintf(title, ARRAYSIZE(title), TEXT("%s%s - %s"), (model->GetModified() ? TEXT("*") : TEXT("")), name, APP_TITLE);
    hwndMain->SetText(title);
}

void ViewState::ShowFindDialog()
{
    if (hFindDlg) 
    {
        hFindDlg->SetForeground();
        return;
    }

    ZeroMemory(&find, sizeof(find));
    find.lStructSize = sizeof(FINDREPLACE);
    find.hwndOwner = (HWND)(hwndMain->GetHandle().Use());
    find.lpstrFindWhat = model->GetFindText();
    find.wFindWhatLen = model->GetFindTextLength();
    find.Flags = model->GetFindFlags();
    hFindDlg = new qor::platform::nswindows::Window(FindText(&find));
}

void ViewState::ShowReplaceDialog() 
{
    if (hReplaceDlg) 
    {
        hReplaceDlg->SetForeground();
        return;
    }

    ZeroMemory(&find, sizeof(find));
    find.lStructSize = sizeof(FINDREPLACE);
    find.hwndOwner = (HWND)(hwndMain->GetHandle().Use());
    find.lpstrFindWhat = model->GetFindText();
    find.lpstrReplaceWith = model->GetReplaceText();
    find.wFindWhatLen = model->GetFindTextLength();
    find.wReplaceWithLen = model->GetReplaceTextLength();
    find.Flags = model->GetFindFlags();
    hReplaceDlg = new qor::platform::nswindows::Window(ReplaceText(&find));
}

void ViewState::DoFileNew() 
{
    if (!PromptSaveChanges()) return;
    hwndEdit->SetText(TEXT(""));
    hwndEdit->ProcessMessage(EM_SETMODIFY, FALSE, 0);
    UpdateTitle();
    UpdateStatusBar();
}

bool ViewState::PromptSaveChanges()
{
    if (!model->GetModified()) return true;

    TCHAR prompt[MAX_PATH_BUFFER + 64];
    const TCHAR* name = model->GetCurrentPath()[0] ? model->GetCurrentPath() : UNTITLED_NAME;
    StringCchPrintf(prompt, ARRAYSIZE(prompt), TEXT("Do you want to save changes to %s?"), name);
    int res = MessageBox((HWND)(hwndMain->GetHandle().Use()), prompt, APP_TITLE, MB_ICONQUESTION | MB_YESNOCANCEL);
    if (res == IDYES) 
    {
        return DoFileSave(false);
    }
    return res == IDNO;
}

bool ViewState::DoFileSave(bool saveAs)
{
    TCHAR path[MAX_PATH_BUFFER];
    if (saveAs || model->GetCurrentPath()[0] == L'\0') 
    {
        path[0] = TEXT('\0');
        if (model->GetCurrentPath()[0]) 
        {
            StringCchCopy(path, ARRAYSIZE(path), model->GetCurrentPath());
        }
        if (!SaveFileDialog(path, ARRAYSIZE(path))) 
        {
            return false;
        }
        StringCchCopy(model->GetCurrentPath(), model->GetCurrentPathLength(), path);
    } 
    else 
    {
        StringCchCopy(path, ARRAYSIZE(path), model->GetCurrentPath());
    }

    int len = hwndEdit->GetTextLength();
    TCHAR *buffer = (TCHAR *)HeapAlloc(GetProcessHeap(), 0, (len + 1) * sizeof(TCHAR));
    if (!buffer) 
    {
        return false;
    }
    hwndEdit->GetText(buffer, len + 1);

    bool ok = model->SaveTextFile(path, buffer, len, model->GetEncoding());
    HeapFree(GetProcessHeap(), 0, buffer);
    if (ok) 
    {
        hwndEdit->ProcessMessage(EM_SETMODIFY, FALSE, 0);
        model->ClearModified();
        UpdateTitle();
    }
    return ok;
}

void ViewState::DoSelectFont()
{
    LOGFONT lf = {0};
    if (hFont) 
    {
        GetObject(hFont, sizeof(LOGFONT), &lf);
    } 
    else 
    {
        SystemParametersInfo(SPI_GETICONTITLELOGFONT, sizeof(LOGFONT), &lf, 0);
    }

    CHOOSEFONT cf = {0};
    cf.lStructSize = sizeof(cf);
    cf.hwndOwner = (HWND)(hwndMain->GetHandle().Use());
    cf.lpLogFont = &lf;
    cf.Flags = CF_SCREENFONTS | CF_INITTOLOGFONTSTRUCT;

    if (ChooseFont(&cf)) 
    {
        HFONT newFont = CreateFontIndirect(&lf);
        if (newFont) 
        {
            if (hFont) delete hFont;
            hFont = new qor::platform::nswindows::Font(newFont);
            ApplyFontToEdit();
            UpdateLayout();
        }
    }
}

bool ViewState::DoFindNext(bool reverse) 
{
    if (model->GetFindText()[0] == TEXT('\0')) 
    {
        ShowFindDialog();
        return false;
    }

    DWORD start = 0, end = 0;
    hwndEdit->ProcessMessage(EM_GETSEL, (WPARAM)&start, (LPARAM)&end);
    bool matchCase = (model->GetFindFlags() & FR_MATCHCASE) != 0;
    bool down = (model->GetFindFlags() & FR_DOWN) != 0;
    if (reverse) 
    {
        down = !down;
    }
    DWORD searchStart = down ? end : start;
    DWORD outStart = 0, outEnd = 0;
    if (model->FindInText(model->GetFindText(), matchCase, down, searchStart, &outStart, &outEnd)) 
    {
        hwndEdit->ProcessMessage(EM_SETSEL, outStart, outEnd);
        hwndEdit->ProcessMessage(EM_SCROLLCARET, 0, 0);
        return true;
    }
    MessageBox((HWND)(hwndMain->GetHandle().Use()), TEXT("Cannot find the text."), APP_TITLE, MB_ICONINFORMATION);
    return false;
}

void ViewState::HandleFindReplace(LPFINDREPLACE lpfr)
{
    if (lpfr->Flags & FR_DIALOGTERM) 
    {
        hFindDlg = NULL;
        hReplaceDlg = NULL;
        return;
    }

    model->SetFindFlags(lpfr->Flags);
    if (lpfr->lpstrFindWhat && lpfr->lpstrFindWhat[0]) 
    {
        StringCchCopy(model->GetFindText(), model->GetFindTextLength(), lpfr->lpstrFindWhat);
    }
    if (lpfr->lpstrReplaceWith) 
    {
        StringCchCopy(model->GetReplaceText(), model->GetReplaceTextLength(), lpfr->lpstrReplaceWith);
    }

    bool matchCase = (lpfr->Flags & FR_MATCHCASE) != 0;
    bool down = (lpfr->Flags & FR_DOWN) != 0;

    if (lpfr->Flags & FR_FINDNEXT) 
    {
        DWORD start = 0, end = 0;
        hwndEdit->ProcessMessage(EM_GETSEL, (WPARAM)&start, (LPARAM)&end);
        DWORD searchStart = down ? end : start;
        DWORD outStart = 0, outEnd = 0;
        if (model->FindInText(model->GetFindText(), matchCase, down, searchStart, &outStart, &outEnd))
        {
            hwndEdit->ProcessMessage(EM_SETSEL, outStart, outEnd);
            hwndEdit->ProcessMessage(EM_SCROLLCARET, 0, 0);
        } 
        else 
        {
            MessageBox((HWND)(hwndMain->GetHandle().Use()), TEXT("Cannot find the text."), APP_TITLE, MB_ICONINFORMATION);
        }
    } 
    else if (lpfr->Flags & FR_REPLACE) 
    {
        DWORD start = 0, end = 0;
        hwndEdit->ProcessMessage(EM_GETSEL, (WPARAM)&start, (LPARAM)&end);
        DWORD outStart = 0, outEnd = 0;
        if (model->FindInText(model->GetFindText(), matchCase, down, start, &outStart, &outEnd)) 
        {
            hwndEdit->ProcessMessage(EM_SETSEL, outStart, outEnd);
            hwndEdit->ProcessMessage(EM_REPLACESEL, TRUE, (LPARAM)model->GetReplaceText());
            hwndEdit->ProcessMessage(EM_SCROLLCARET, 0, 0);
            model->SetModified();
            UpdateTitle();
        } 
        else 
        {
            MessageBox((HWND)(hwndMain->GetHandle().Use()), TEXT("Cannot find the text."), APP_TITLE, MB_ICONINFORMATION);
        }
    } 
    else if (lpfr->Flags & FR_REPLACEALL) 
    {
        int replaced = model->ReplaceAllOccurrences(matchCase);
        TCHAR msg[64];
        StringCchPrintf(msg, ARRAYSIZE(msg), TEXT("Replaced %d occurrence%s."), replaced, replaced == 1 ? TEXT("") : TEXT("s"));
        MessageBox((HWND)(hwndMain->GetHandle().Use()), msg, APP_TITLE, MB_OK | MB_ICONINFORMATION);
    }
}

bool ViewState::DoFileOpen() 
{
    if (!PromptSaveChanges()) 
    {
        return false;
    }

    TCHAR path[MAX_PATH_BUFFER] = TEXT("");
    if (!OpenFileDialog(path, ARRAYSIZE(path))) 
    {
        return false;
    }
    return model->LoadDocumentFromPath(path);
}

bool ViewState::OpenFileDialog(TCHAR *pathOut, unsigned long pathLen)
{
    pathOut[0] = L'\0';
    OPENFILENAME ofn = {0};
    TCHAR filter[] = TEXT("Text Files (*.txt)\0*.txt\0All Files (*.*)\0*.*\0\0");
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = (HWND)(hwndMain->GetHandle().Use());
    ofn.lpstrFilter = filter;
    ofn.lpstrFile = pathOut;
    ofn.nMaxFile = pathLen;
    ofn.Flags = OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_PATHMUSTEXIST;
    ofn.lpstrDefExt = TEXT("txt");
    return GetOpenFileName(&ofn) ? true : false;
}

bool ViewState::SaveFileDialog(TCHAR *pathOut, unsigned long pathLen) 
{
    OPENFILENAME ofn = {0};
    TCHAR filter[] = TEXT("Text Files (*.txt)\0*.txt\0All Files (*.*)\0*.*\0\0");
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = (HWND)(hwndMain->GetHandle().Use());;
    ofn.lpstrFilter = filter;
    ofn.lpstrFile = pathOut;
    ofn.nMaxFile = pathLen;
    ofn.Flags = OFN_OVERWRITEPROMPT | OFN_PATHMUSTEXIST;
    ofn.lpstrDefExt = TEXT("txt");
    if (pathOut[0] == TEXT('\0')) 
    {
        StringCchCopy(pathOut, pathLen, TEXT("*.txt"));
    }
    return GetSaveFileName(&ofn) ? true : false;
}

int ViewState::Run()
{
    qor::platform::nswindows::AcceleratorTable accTable(LoadAccelerators((HINSTANCE)(m_instance), MAKEINTRESOURCE(IDC_RETROPAD)));
    qor::platform::nswindows::Message msg;

    while (GetMessage((MSG*)&msg, NULL, 0, 0)) 
    {        
        if (!accTable.GetHandle().Use() || !hwndMain->TranslateAccel(accTable, &msg)) 
        {
            TranslateMessage((MSG*)&msg);
            DispatchMessageW((MSG*)&msg);
        }
    }

    return (int)msg.wParam;
}