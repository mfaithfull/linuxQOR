
#ifndef QOR_PP_H_RETROPAD_CONTROLLER
#define QOR_PP_H_RETROPAD_CONTROLLER

#include <vector>

#include "src/framework/event/signals/slotbase.h"
#include "src/platform/os/windows/ui/gui/window.h"
#include "src/platform/os/windows/ui/gui/windowclassregistration.h"
#include "src/platform/os/windows/ui/gui/icon.h"
#include "src/platform/os/windows/ui/gui/cursor.h"
#include "src/platform/os/windows/ui/gui/brush.h"
#include "src/platform/os/windows/ui/gui/view/controllers/rendered/ncrender.h"
#include "src/platform/os/windows/ui/gui/view/controllers/toplevel.h"

#include "model.h"
#include "viewmodel.h"

class RetroPadNonClientRenderingController : public qor::platform::win::gui::view::NonClientRenderingHandler
{
public:
    RetroPadNonClientRenderingController() : qor::platform::win::gui::view::NonClientRenderingHandler()
    {        
    }

    virtual void OnNCSize(qor::platform::win::Window& window, long long& lResult, unsigned long long wParam, long long lParam)
    {
        //Consider forwarding the OnNCSize as a signal and listening to it to trigger the relayout.
        //m_view->UpdateLayout();
        //m_view->UpdateStatusBar();
    }
};

class RetroPadTopLevelWindowController : public qor::platform::win::gui::view::TopLevelWindowHandler, public virtual qor::SlotBase
{
public:
    RetroPadTopLevelWindowController();
    virtual ~RetroPadTopLevelWindowController() noexcept(true);

    virtual long OnCreate(qor::platform::win::Window& window, qor::platform::win::gui::view::CreateStruct* pCreateStruct);

    virtual long long OnDestroy(qor::platform::win::Window& window, unsigned int uMsg, unsigned long long wParam, long long lParam)
    {
        PostQuitMessage(0);
        return 0;
    }

    void OnSetFocus(qor::platform::win::Window& window, qor::platform::win::Window& otherWindow)
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
    qor::platform::win::Icon m_icon;
    qor::platform::win::Cursor m_cursor;
    qor::platform::win::Brush m_backgroundBrush;
    qor::platform::win::Menu m_menu;
    qor::platform::win::WindowClass* m_windowClass;
    qor::platform::win::WindowClassRegistration* m_wcReg;

    void HandleCommand(HWND hwnd, WPARAM wParam, LPARAM lParam);    

};

#endif//QOR_PP_H_RETROPAD_CONTROLLER
