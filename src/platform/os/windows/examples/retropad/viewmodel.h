
#ifndef QOR_PP_H_RETROPAD_VIEWMODEL
#define QOR_PP_H_RETROPAD_VIEWMODEL

#include "src/platform/os/windows/common/structures.h"
#include "src/platform/os/windows/gui/window.h"
#include "src/platform/os/windows/gui/font.h"
#include "model.h"

struct ViewState 
{
    void* m_instance;
    ModelState* model;
    qor::platform::nswindows::Window* hwndMain;
    qor::platform::nswindows::Window* hwndEdit;
    qor::platform::nswindows::Window* hwndStatus;
    qor::platform::nswindows::Font* hFont;
    bool wordWrap;
    bool statusVisible;
    bool statusBeforeWrap;
    FINDREPLACE find;
    qor::platform::nswindows::Window* hFindDlg;
    qor::platform::nswindows::Window* hReplaceDlg;

    static ViewState* Create(void* instance, const TCHAR* windowClass, const qor::platform::nswindows::Menu& menu, ModelState* model, void* param);
    void Show();
    void CreateEditControl();
    void UpdateLayout();
    void ApplyFontToEdit();
    void UpdateMenuStates();
    void ToggleStatusBar(bool visible);
    void UpdateStatusBar();
    void SetWordWrap(bool enabled);
    void UpdateTitle();
    bool GetEditText(TCHAR* buffer, int* lengthOut);
    void ShowFindDialog();
    void ShowReplaceDialog();
    void DoFileNew();
    bool PromptSaveChanges();
    void DoSelectFont();
    bool DoFindNext(bool reverse);
    void HandleFindReplace(LPFINDREPLACE lpfr);
    bool DoFileOpen();
    bool OpenFileDialog(TCHAR *pathOut, unsigned long pathLen);
    bool SaveFileDialog(TCHAR *pathOut, unsigned long pathLen);
    bool DoFileSave(bool saveAs);
    int Run();
};

#endif//QOR_PP_H_RETROPAD_VIEWMODEL