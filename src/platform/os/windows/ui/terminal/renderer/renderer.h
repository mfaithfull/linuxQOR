// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_WIN_UI_TERMINAL_RENDERER
#define QOR_PP_H_WIN_UI_TERMINAL_RENDERER

#include <cstdint>
#include <string>
#include <vector>

#include "imagebuffer.h"

namespace qor{ namespace ui{ namespace tui { namespace win{

    struct Cursor 
    {
        int x = 0;
        int y = 0;

        enum Shape 
        {
            Hidden = 0,
            BlockBlinking = 1,
            Block = 2,
            UnderlineBlinking = 3,
            Underline = 4,
            BarBlinking = 5,
            Bar = 6,
        };
        Shape shape = Hidden;
    };

    class Renderer 
    {
    public:
    
        enum Palette 
        {
            Palette1,
            Palette16,
            Palette256,
            TrueColour,
        };

        void Print() const;// Print the Screen on to the terminal.
        void Clear();// Fill the screen with space and reset any screen state, like hyperlinks, and cursor
        std::string ResetPosition(bool clear = false) const;// Move the terminal cursor n-lines up with n = dimy().
        void ApplyShader();
        static Palette ColourSupport();
        static void SetColourSupport(Palette palette);
        Cursor cursor() const { return cursor_; }
        void SetCursor(Cursor cursor) { cursor_ = cursor; }

    protected:
        Cursor cursor_;
        ImageBuffer* m_image_buffer{nullptr};
    };

}}}}//qor::ui::tui::win

#endif//QOR_PP_H_WIN_UI_TERMINAL_RENDERER