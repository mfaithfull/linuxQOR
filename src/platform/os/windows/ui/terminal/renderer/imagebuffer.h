// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_WIN_UI_TERMINAL_IMAGEBUFFER
#define QOR_PP_H_WIN_UI_TERMINAL_IMAGEBUFFER

#include <vector>
#include "src/framework/ui/layout/box.h"
#include "pixel.h"

namespace qor{ namespace ui{ namespace tui { namespace win{

    class ImageBuffer
    {
    public:

        ImageBuffer() = delete;
        ImageBuffer(int dimx, int dimy);
        virtual ~ImageBuffer() = default;

        std::string& at(int x, int y);// Access a character in the grid at a given position.
        const std::string& at(int x, int y) const;
        Pixel& PixelAt(int x, int y);// Access a cell (Pixel) in the grid at a given position.
        const Pixel& PixelAt(int x, int y) const;

        int dimx() const { return dimx_; }
        int dimy() const { return dimy_; }

        void Clear();// Fill the image with space and default style
        void ApplyShader();
        std::string ToString() const;// Produce a std::string that can be used to print the Screen on the terminal.

        layout::Box stencil;

    protected:
    
        int dimx_;
        int dimy_;
        std::vector<std::vector<Pixel>> pixels_;
        Terminal* m_terminal{nullptr};
    };    
}}}}//qor::ui::tui::win

#endif//QOR_PP_H_WIN_UI_TERMINAL_IMAGEBUFFER