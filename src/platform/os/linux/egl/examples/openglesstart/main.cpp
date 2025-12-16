#include <EGL/egl.h>        // EGL headers  
#include <GLES2/gl2.h>      // OpenGL ES 2.0 headers  
#include <X11/Xlib.h>       // X11 window system headers  
#include <stdio.h>  
 
int main() {  
    // ----------------------------  
    // Step 1: Create an X11 Window  
    // ----------------------------  
    Display* x11_display = XOpenDisplay(NULL);  
    if (!x11_display) {  
        fprintf(stderr, "Failed to open X11 display\n");  
        return 1;  
    }  
 
    int screen = DefaultScreen(x11_display);  
    Window root_window = RootWindow(x11_display, screen);  
 
    // Window attributes  
    XSetWindowAttributes xwa;  
    xwa.background_pixel = BlackPixel(x11_display, screen);  
    xwa.border_pixel = WhitePixel(x11_display, screen);  
    xwa.event_mask = StructureNotifyMask;  
 
    // Create a 800x600 window  
    Window x11_window = XCreateWindow(  
        x11_display, root_window,  
        0, 0, 800, 600, 0,  
        DefaultDepth(x11_display, screen),  
        InputOutput,  
        DefaultVisual(x11_display, screen),  
        CWBackPixel | CWBorderPixel | CWEventMask,  
        &xwa  
    );  
 
    XMapWindow(x11_display, x11_window);  // Show the window  
    XFlush(x11_display);  
 
    // Wait for the window to be mapped (visible)  
    XEvent event;  
    while (1) {  
        XNextEvent(x11_display, &event);  
        if (event.type == MapNotify) break;  
    }  
 
 
    // ----------------------------  
    // Step 2: Initialize EGL  
    // ----------------------------  
    // Get the EGL display (connect to X11 via EGL)  
    EGLDisplay egl_display = eglGetDisplay((EGLNativeDisplayType)x11_display);  
    if (egl_display == EGL_NO_DISPLAY) {  
        fprintf(stderr, "Failed to get EGL display\n");  
        return 1;  
    }  
 
    // Initialize EGL  
    EGLint major, minor;  
    if (!eglInitialize(egl_display, &major, &minor)) {  
        fprintf(stderr, "Failed to initialize EGL\n");  
        return 1;  
    }  
    printf("EGL initialized with version %d.%d\n", major, minor);  
 
 
    // ----------------------------  
    // Step 3: Choose an EGL Config  
    // ----------------------------  
    // An EGL "Config" defines surface properties (e.g., colour depth, alpha channel)  
    EGLConfig egl_config;  
    EGLint num_configs;  
    EGLint config_attribs[] = {  
        EGL_SURFACE_TYPE, EGL_WINDOW_BIT,  // Surface type: window  
        EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,  // Support OpenGL ES 2.0  
        EGL_BLUE_SIZE, 8,                  // 8 bits for blue channel  
        EGL_GREEN_SIZE, 8,                 // 8 bits for green  
        EGL_RED_SIZE, 8,                   // 8 bits for red  
        EGL_ALPHA_SIZE, 8,                 // 8 bits for alpha (transparency)  
        EGL_DEPTH_SIZE, 16,                // 16-bit depth buffer  
        EGL_NONE                           // Terminate list  
    };  
 
    if (!eglChooseConfig(egl_display, config_attribs, &egl_config, 1, &num_configs) || num_configs == 0) {  
        fprintf(stderr, "Failed to choose EGL config\n");  
        return 1;  
    }  
 
 
    // ----------------------------  
    // Step 4: Create EGL Surface  
    // ----------------------------  
    // Wrap the X11 window in an EGL window surface  
    EGLSurface egl_surface = eglCreateWindowSurface(  
        egl_display,  
        egl_config,  
        (EGLNativeWindowType)x11_window,  
        NULL  // No additional surface attributes  
    );  
    if (egl_surface == EGL_NO_SURFACE) {  
        fprintf(stderr, "Failed to create EGL surface\n");  
        return 1;  
    }  
 
 
    // ----------------------------  
    // Step 5: Create EGL Context  
    // ----------------------------  
    // Define OpenGL ES context attributes (version 2.0)  
    EGLint context_attribs[] = {  
        EGL_CONTEXT_CLIENT_VERSION, 2,  // Request OpenGL ES 2.0  
        EGL_NONE  
    };  
 
    EGLContext egl_context = eglCreateContext(  
        egl_display,  
        egl_config,  
        EGL_NO_CONTEXT,  // No shared context  
        context_attribs  
    );  
    if (egl_context == EGL_NO_CONTEXT) {  
        fprintf(stderr, "Failed to create EGL context\n");  
        return 1;  
    }  
 
 
    // ----------------------------  
    // Step 6: Make Context Current  
    // ----------------------------  
    // Bind the context and surface to the current thread  
    if (!eglMakeCurrent(egl_display, egl_surface, egl_surface, egl_context)) {  
        fprintf(stderr, "Failed to make EGL context current\n");  
        return 1;  
    }  
 
 
    // ----------------------------  
    // Step 7: Render with OpenGL ES  
    // ----------------------------  
    // Clear the screen to a blue colour  
    glClearColor(0.2f, 0.4f, 0.8f, 1.0f);  // RGBA: light blue  
    glClear(GL_COLOR_BUFFER_BIT);  
 
    // Swap buffers to display the result (synchronize with vsync)  
    eglSwapBuffers(egl_display, egl_surface);  
 
 
    // ----------------------------  
    // Step 8: Wait for User Input  
    // ----------------------------  
    printf("Rendered successfully! Press Enter to exit...\n");  
    getchar();  
 
 
    // ----------------------------  
    // Step 9: Cleanup  
    // ----------------------------  
    eglMakeCurrent(egl_display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);  
    eglDestroySurface(egl_display, egl_surface);  
    eglDestroyContext(egl_display, egl_context);  
    eglTerminate(egl_display);  
    XDestroyWindow(x11_display, x11_window);  
    XCloseDisplay(x11_display);  
 
    return 0;  
}