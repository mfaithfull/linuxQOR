
#ifndef QOR_PP_H_RETROPAD_CONTROLLER
#define QOR_PP_H_RETROPAD_CONTROLLER

#include <vector>

#include "src/platform/os/windows/gui/window.h"
#include "src/platform/os/windows/gui/windowclassregistration.h"
#include "src/platform/os/windows/gui/icon.h"
#include "src/platform/os/windows/gui/cursor.h"
#include "src/platform/os/windows/gui/brush.h"

#include "model.h"
#include "viewmodel.h"

class RetroPadController
{
public:
    RetroPadController(void* instance);
    virtual ~RetroPadController();

    int Run();

    LRESULT MainWindowHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
    
private:

    void* m_instance;
    unsigned int m_findMsg;
    ViewState* m_view;
    ModelState m_model;
    qor::platform::nswindows::Icon m_icon;
    qor::platform::nswindows::Cursor m_cursor;
    qor::platform::nswindows::Brush m_backgroundBrush;
    qor::platform::nswindows::Menu m_menu;
    qor::platform::nswindows::WindowClass* m_windowClass;
    qor::platform::nswindows::WindowClassRegistration* m_wcReg;

    void HandleCommand(HWND hwnd, WPARAM wParam, LPARAM lParam);    

};

#endif//QOR_PP_H_RETROPAD_CONTROLLER
