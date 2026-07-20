// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_OS_WINDOWS_UI_GRAPHCON_RENDERER_IMAGE
#define QOR_PP_H_OS_WINDOWS_UI_GRAPHCON_RENDERER_IMAGE

#include <string>
#include <vector>
#include "src/components/ui/layout/box.h"
#include "pixel.h"

namespace qor { namespace ui { namespace win {

    class Image
    {
    public:

        Image() = delete;
        Image(int width, int height);
        virtual ~Image();

        wchar_t at(int x, int y);// Access a character in the grid at a given position.
        const wchar_t at(int x, int y) const;
        Pixel& PixelAt(int x, int y);// Access a cell (Pixel) in the grid at a given position.
        const Pixel& PixelAt(int x, int y) const;
        int Width() const;
        int Height() const;
        void Clear();// Fill the image with space and default style

        components::ui::Box m_stencil;

    protected:
    
        int m_width;
        int m_height;
        std::vector<std::vector<Pixel>> m_pixels;

    };

}}}//qor::ui::win

#endif//QOR_PP_H_OS_WINDOWS_UI_GRAPHCON_RENDERER_IMAGE
