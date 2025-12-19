

#ifndef QOR_PP_H_RETROPAD_WINDOWCLASSCONTROLLER
#define QOR_PP_H_RETROPAD_WINDOWCLASSCONTROLLER

#include "src/platform/os/windows/gui/window.h"
#include "src/platform/os/windows/gui/view/controllers/toplevel.h"

class WindowClassController : public qor::platform::nswindows::gui::view::TopLevelWindowController
{
public:

    WindowClassController();
    virtual ~WindowClassController();

    virtual long OnCreate(qor::platform::nswindows::Window& window, qor::platform::nswindows::gui::view::CreateStruct* pCreateStruct);
    virtual long long OnDestroy(qor::platform::nswindows::Window& window, unsigned int uMsg, unsigned long long wParam, long long lParam);

};

#endif//QOR_PP_H_RETROPAD_WINDOWCLASSCONTROLLER
