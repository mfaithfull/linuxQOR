

#include "src/configuration/configuration.h"
#include "windowclasscontroller.h"
#include "src/platform/os/windows/gui/gui.h"

using namespace qor::platform::nswindows;

WindowClassController::WindowClassController()
{
}

WindowClassController::~WindowClassController()
{
}

long WindowClassController::OnCreate(Window& window, gui::view::CreateStruct* pCreateStruct)
{
    struct InitCommonControlsEx icc = { sizeof(icc), ICC_BAR_CLASSES };
    GUI::InitCommonControlsEx(icc);    
    GUI::DragAcceptFiles(window, true);
    return 0;
}

long long WindowClassController::OnDestroy(Window& window, unsigned int uMsg, unsigned long long wParam, long long lParam)
{
    GUI::Quit(0);
    return 0;
}
