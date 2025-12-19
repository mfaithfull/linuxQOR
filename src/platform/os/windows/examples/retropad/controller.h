
#ifndef QOR_PP_H_RETROPAD_CONTROLLER
#define QOR_PP_H_RETROPAD_CONTROLLER

#include <vector>

#include "src/framework/signals/slotbase.h"
#include "src/platform/os/windows/gui/window.h"
#include "src/platform/os/windows/gui/windowclassregistration.h"
#include "src/platform/os/windows/gui/icon.h"
#include "src/platform/os/windows/gui/cursor.h"
#include "src/platform/os/windows/gui/brush.h"
#include "src/platform/os/windows/gui/view/controllers/rendered/ncrender.h"
#include "src/platform/os/windows/gui/view/controllers/toplevel.h"

#include "model.h"
#include "viewmodel.h"

class RetroPadNonClientRenderingController : public qor::platform::nswindows::gui::view::NonClientRenderingController
{
public:
    RetroPadNonClientRenderingController() : qor::platform::nswindows::gui::view::NonClientRenderingController()
    {        
    }

    virtual void OnNCSize(qor::platform::nswindows::Window& window, long long& lResult, unsigned long long wParam, long long lParam)
    {
        //Consider forwarding the OnNCSize as a signal and listening to it to trigger the relayout.
        //m_view->UpdateLayout();
        //m_view->UpdateStatusBar();
    }
};

class RetroPadTopLevelWindowController : public qor::platform::nswindows::gui::view::TopLevelWindowController, public virtual qor::SlotBase
{
public:
    RetroPadTopLevelWindowController();
    virtual ~RetroPadTopLevelWindowController() noexcept(true);

    virtual long OnCreate(qor::platform::nswindows::Window& window, qor::platform::nswindows::gui::view::CreateStruct* pCreateStruct);

    virtual long long OnDestroy(qor::platform::nswindows::Window& window, unsigned int uMsg, unsigned long long wParam, long long lParam)
    {
        PostQuitMessage(0);
        return 0;
    }

    void OnSetFocus(qor::platform::nswindows::Window& window, qor::platform::nswindows::Window& otherWindow)
    {

    }
};

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
