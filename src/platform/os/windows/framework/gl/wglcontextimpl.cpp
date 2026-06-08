
#include "src/configuration/configuration.h"
#include "src/platform/os/windows/common/structures.h"

#include "contextimpl.h"

#include "src/platform/os/windows/api_layer/user/user32.h"
#include "src/platform/os/windows/api_layer/gdi/gdi32.h"

namespace qor{ namespace framework{ namespace nswindows{ namespace gl {

    // Initialize WGL
    bool ContextImpl::InitWGL()
    {
        PIXELFORMATDESCRIPTOR pfd;
        HGLRC prc, rc;
        HDC pdc, dc;

        if (_glfw.wgl.instance)
        {
            return true;
        }

        _glfw.wgl.instance = _glfwPlatformLoadModule("opengl32.dll");
        if (!_glfw.wgl.instance)
        {
            _glfwInputErrorWin32(GLFW_PLATFORM_ERROR, "WGL: Failed to load opengl32.dll");
            return false;
        }

        // NOTE: A dummy context has to be created for opengl32.dll to load the
        //       OpenGL ICD, from which we can then query WGL extensions
        // NOTE: This code will accept the Microsoft GDI ICD; accelerated context
        //       creation failure occurs during manual pixel format enumeration

        dc = GetDC(_glfw.win32.helperWindowHandle);

        ZeroMemory(&pfd, sizeof(pfd));
        pfd.nSize = sizeof(pfd);
        pfd.nVersion = 1;
        pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
        pfd.iPixelType = PFD_TYPE_RGBA;
        pfd.cColorBits = 24;

        if (!SetPixelFormat(dc, ChoosePixelFormat(dc, &pfd), &pfd))
        {
            _glfwInputErrorWin32(GLFW_PLATFORM_ERROR, "WGL: Failed to set pixel format for dummy context");
            return false;
        }

        rc = wglCreateContext(dc);
        if (!rc)
        {
            _glfwInputErrorWin32(GLFW_PLATFORM_ERROR, "WGL: Failed to create dummy context");
            return false;
        }

        pdc = wglGetCurrentDC();
        prc = wglGetCurrentContext();

        if (!wglMakeCurrent(dc, rc))
        {
            _glfwInputErrorWin32(GLFW_PLATFORM_ERROR, "WGL: Failed to make dummy context current");
            wglMakeCurrent(pdc, prc);
            wglDeleteContext(rc);
            return false;
        }

        // NOTE: Functions must be loaded first as they're needed to retrieve the extension string that tells us whether the functions are supported
        _glfw.wgl.GetExtensionsStringEXT = (PFNWGLGETEXTENSIONSSTRINGEXTPROC) wglGetProcAddress("wglGetExtensionsStringEXT");
        _glfw.wgl.GetExtensionsStringARB = (PFNWGLGETEXTENSIONSSTRINGARBPROC) wglGetProcAddress("wglGetExtensionsStringARB");
        _glfw.wgl.CreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC) wglGetProcAddress("wglCreateContextAttribsARB");
        _glfw.wgl.SwapIntervalEXT = (PFNWGLSWAPINTERVALEXTPROC) wglGetProcAddress("wglSwapIntervalEXT");
        _glfw.wgl.GetPixelFormatAttribivARB = (PFNWGLGETPIXELFORMATATTRIBIVARBPROC) wglGetProcAddress("wglGetPixelFormatAttribivARB");


        wglMakeCurrent(pdc, prc);
        wglDeleteContext(rc);
        return true;
    }

}}}}//qor::framework::nswindow::gl
