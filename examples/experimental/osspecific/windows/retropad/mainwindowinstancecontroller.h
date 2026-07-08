

#ifndef QOR_PP_H_RETROPAD_MAINWINDOWINSTANCECONTROLLER
#define QOR_PP_H_RETROPAD_MAINWINDOWINSTANCECONTROLLER

#include "src/platform/os/windows/ui/gui/window.h"
#include "src/platform/os/windows/ui/gui/view/controllers/toplevel.h"
#include "src/platform/os/windows/ui/gui/view/controllers/interactive/draganddrop.h"
#include "src/platform/os/windows/ui/gui/view/controllers/rendered/ncrender.h"
#include "src/platform/os/windows/ui/gui/view/controllers/interactive/focus.h"
#include "viewmodel.h"

class MainWindowFocusController : public qor::platform::win::gui::view::FocusHandler
{
public:

    MainWindowFocusController(ViewState* viewState);
    ~MainWindowFocusController() noexcept(true) = default;

    virtual void OnSetFocus(qor::platform::win::Window& window, qor::platform::win::Window& otherWindow);

private:

    ViewState* m_view;
};

class MainWindowNonClientRenderController : public qor::platform::win::gui::view::NonClientRenderingHandler
{
public:

    MainWindowNonClientRenderController(ViewState* viewState);

    virtual void OnNCSize(qor::platform::win::Window& window, long long& lResult, unsigned long long wParam, long long lParam);

private:

    ViewState* m_view;
};

class MainWindowDragAndDropController : public qor::platform::win::gui::view::DragAndDropHandler
{
public:

    MainWindowDragAndDropController(ViewState* viewState);

    virtual void OnDropFiles(qor::platform::win::Window& window, void* hDrop);

private:

    ViewState* m_view;
};

class MainWindowMenuController : public qor::platform::win::gui::view::MenuHandler
{
public:

    MainWindowMenuController(ViewState* viewState);

    void OnInitMenuPopup(qor::platform::win::Window& window, qor::platform::win::Menu& menu, unsigned short wPos, bool bWindowMenu);

private:

    ViewState* m_view;
};

class MainWindowInstanceController : public qor::platform::win::gui::view::TopLevelWindowHandler
{
public:

    MainWindowInstanceController();
    virtual ~MainWindowInstanceController();

    virtual long long OnDestroy(qor::platform::win::Window& window, unsigned int uMsg, unsigned long long wParam, long long lParam);
    virtual void OnClose(qor::platform::win::Window& window);
    virtual void OnCommand(qor::platform::win::Window& window, unsigned short wNotify, unsigned short wID, long long lParam);

private:

    void* m_instance;
    unsigned int m_findMsg;
    ViewState* m_view;

    void HandleCommand(qor::platform::win::Window& window, unsigned short wNotify, unsigned short wID, long long lParam);

};

#endif//QOR_PP_H_RETROPAD_MAINWINDOWINSTANCECONTROLLER
