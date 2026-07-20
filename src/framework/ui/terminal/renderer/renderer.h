// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_FRAMEWORK_UI_TERMINAL_RENDERER
#define QOR_PP_H_FRAMEWORK_UI_TERMINAL_RENDERER

//A renderer for the the terminal. Triangles, quads, text, effects, etc

namespace qor{ namespace ui{ 
    
    class qor_pp_module_interface(QOR_TERMINAL) Terminal;

    namespace tty{

    class Renderer
    {
    public:

        Renderer(Terminal* display);
        ~Renderer();

        void SwapBuffers();
        void Present();

    private:

        Terminal* m_display;
        unsigned char* m_buffer;

    };

}}}//qor::ui::tty

#endif//QOR_PP_H_FRAMEWORK_UI_TERMINAL_RENDERER
