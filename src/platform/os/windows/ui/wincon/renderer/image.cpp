// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"
#include <utility>
#include "image.h"

namespace qor { namespace ui{ namespace win {

    namespace 
    {
        Pixel& dev_null_pixel() 
        {
            static Pixel pixel;
            return pixel;
        }
    }//

    Image::Image(int width, int height) : 
        m_stencil{0, width - 1, 0, height - 1}, 
        m_width(width), 
        m_height(height), 
        m_pixels(m_height, std::vector<Pixel>(m_width)) 
    { }

    Image::~Image() = default;

    int Image::Width() const { return m_width; }
    int Image::Height() const { return m_height; }

    wchar_t Image::at(int x, int y) 
    {
        return PixelAt(x, y).Char.UnicodeChar;
    }

    const wchar_t Image::at(int x, int y) const 
    {
        return PixelAt(x, y).Char.UnicodeChar;
    }

    Pixel& Image::PixelAt(int x, int y) 
    {
        return m_stencil.Contain(x, y) ? m_pixels[y][x] : dev_null_pixel();
    }

    const Pixel& Image::PixelAt(int x, int y) const 
    {
        return m_stencil.Contain(x, y) ? m_pixels[y][x] : dev_null_pixel();
    }
    
    void Image::Clear() 
    {
        for (auto& line : m_pixels) 
        {
            for (auto& cell : line) 
            {
                cell = Pixel();
            }
        }
    }

}}}//qor::ui::win