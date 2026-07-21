// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_FRAMEWORK_UI_TERMINAL_RENDERER_IMAGE
#define QOR_PP_H_FRAMEWORK_UI_TERMINAL_RENDERER_IMAGE

#include <cstdint>
#include <string>
#include <vector>
#include "../../layout/box.h"
#include "pixel.h"

namespace qor{ namespace ui{ namespace tty {

    class qor_pp_module_interface(QOR_TERMINAL) Image 
    {
    public:

        Image() = delete;
        Image(int dimx, int dimy);
        virtual ~Image() = default;

        std::string& at(int x, int y);// Access a character in the grid at a given position.
        const std::string& at(int x, int y) const;
        Pixel& PixelAt(int x, int y);// Access a cell (Pixel) in the grid at a given position.
        const Pixel& PixelAt(int x, int y) const;

        int dimx() const { return dimx_; }
        int dimy() const { return dimy_; }

        void Clear();// Fill the image with space and default style

        layout::Box stencil;

    protected:
    
        int dimx_;
        int dimy_;
        std::vector<std::vector<Pixel>> pixels_;
    };

}}}//qor::ui::tty

#endif//QOR_PP_H_FRAMEWORK_UI_TERMINAL_RENDERER_IMAGE

