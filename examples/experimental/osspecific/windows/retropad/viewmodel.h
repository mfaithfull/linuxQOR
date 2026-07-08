
#ifndef QOR_PP_H_RETROPAD_VIEWMODEL
#define QOR_PP_H_RETROPAD_VIEWMODEL

#include "src/platform/os/windows/common/structures.h"
#include "src/platform/os/windows/ui/gui/window.h"
#include "src/platform/os/windows/ui/gui/font.h"
#include "model.h"
//#include "src/platform/os/windows/api_layer/kernel/kernel32.h"
//#include <commdlg.h>

struct ViewState 
{
    ModelState* m_model;
    void* m_instance = nullptr;
    ModelState* model = nullptr;
    qor::platform::win::Window* hwndMain = nullptr;
    qor::platform::win::Window* hwndEdit = nullptr;
    qor::platform::win::Window* hwndStatus = nullptr;
    qor::platform::win::Font* hFont = nullptr;
    bool wordWrap;
    bool statusVisible;
    bool statusBeforeWrap;
    qor::platform::win::FindReplace find;
    qor::platform::win::Window* hFindDlg = nullptr;
    qor::platform::win::Window* hReplaceDlg = nullptr;

    static ViewState* Create(void* instance, const TCHAR* windowClass, const qor::platform::win::Menu& menu, ModelState* model, void* param);
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
    void HandleFindReplace(qor::platform::win::FindReplace* lpfr);
    bool DoFileOpen();
    bool OpenFileDialog(TCHAR *pathOut, unsigned long pathLen);
    bool SaveFileDialog(stdstring& path);
    bool DoFileSave(bool saveAs);
    int Run();
};

#endif//QOR_PP_H_RETROPAD_VIEWMODEL