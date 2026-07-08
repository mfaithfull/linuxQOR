

#ifndef QOR_PP_H_RETROPAD_WINDOWCLASSCONTROLLER
#define QOR_PP_H_RETROPAD_WINDOWCLASSCONTROLLER

#include "src/platform/os/windows/ui/gui/window.h"
#include "src/platform/os/windows/ui/gui/view/controllers/toplevel.h"

class WindowClassController : public qor::platform::win::gui::view::TopLevelWindowHandler
{
public:

    WindowClassController();
    virtual ~WindowClassController();

    virtual long OnCreate(qor::platform::win::Window& window, qor::platform::win::gui::view::CreateStruct* pCreateStruct);
    virtual long long OnDestroy(qor::platform::win::Window& window, unsigned int uMsg, unsigned long long wParam, long long lParam);

};

#endif//QOR_PP_H_RETROPAD_WINDOWCLASSCONTROLLER
