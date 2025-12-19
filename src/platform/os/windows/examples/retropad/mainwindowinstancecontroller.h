

#ifndef QOR_PP_H_RETROPAD_MAINWINDOWINSTANCECONTROLLER
#define QOR_PP_H_RETROPAD_MAINWINDOWINSTANCECONTROLLER

#include "src/platform/os/windows/gui/window.h"
#include "src/platform/os/windows/gui/view/controllers/toplevel.h"
#include "src/platform/os/windows/gui/view/controllers/interactive/draganddrop.h"
#include "src/platform/os/windows/gui/view/controllers/rendered/ncrender.h"
#include "src/platform/os/windows/gui/view/controllers/interactive/focus.h"
#include "viewmodel.h"

class MainWindowFocusController : public qor::platform::nswindows::gui::view::FocusController
{
public:

    MainWindowFocusController(ViewState* viewState);
    ~MainWindowFocusController() noexcept(true) = default;

    virtual void OnSetFocus(qor::platform::nswindows::Window& window, qor::platform::nswindows::Window& otherWindow);

private:

    ViewState* m_view;
};

class MainWindowNonClientRenderController : public qor::platform::nswindows::gui::view::NonClientRenderingController
{
public:

    MainWindowNonClientRenderController(ViewState* viewState);

    virtual void OnNCSize(qor::platform::nswindows::Window& window, long long& lResult, unsigned long long wParam, long long lParam);

private:

    ViewState* m_view;
};

class MainWindowDragAndDropController : public qor::platform::nswindows::gui::view::DragAndDropController
{
public:

    MainWindowDragAndDropController(ViewState* viewState);

    virtual void OnDropFiles(qor::platform::nswindows::Window& window, void* hDrop);

private:

    ViewState* m_view;
};

class MainWindowMenuController : public qor::platform::nswindows::gui::view::MenuController
{
public:

    MainWindowMenuController(ViewState* viewState);

    void OnInitMenuPopup(qor::platform::nswindows::Window& window, qor::platform::nswindows::Menu& menu, unsigned short wPos, bool bWindowMenu);

private:

    ViewState* m_view;
};

class MainWindowInstanceController : public qor::platform::nswindows::gui::view::TopLevelWindowController
{
public:

    MainWindowInstanceController();
    virtual ~MainWindowInstanceController();

    virtual long long OnDestroy(qor::platform::nswindows::Window& window, unsigned int uMsg, unsigned long long wParam, long long lParam);
    virtual void OnClose(qor::platform::nswindows::Window& window);
    virtual void OnCommand(qor::platform::nswindows::Window& window, unsigned short wNotify, unsigned short wID, long long lParam);

private:

    void* m_instance;
    unsigned int m_findMsg;
    ViewState* m_view;

    void HandleCommand(qor::platform::nswindows::Window& window, unsigned short wNotify, unsigned short wID, long long lParam);

};

#endif//QOR_PP_H_RETROPAD_MAINWINDOWINSTANCECONTROLLER
