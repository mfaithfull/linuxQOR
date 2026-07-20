// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"

#include "image.h"

namespace qor{ namespace components{ namespace tui {

    namespace 
    {
        Pixel& dev_null_pixel() 
        {
            static Pixel pixel;
            return pixel;
        }
    }//

    Image::Image(int dimx, int dimy) : 
        stencil{0, dimx - 1, 0, dimy - 1}, 
        dimx_(dimx), 
        dimy_(dimy), 
        pixels_(dimy, std::vector<Pixel>(dimx)) 
    {        
    }

    std::string& Image::at(int x, int y) 
    {
        return PixelAt(x, y).character;
    }

    const std::string& Image::at(int x, int y) const 
    {
        return PixelAt(x, y).character;
    }

    Pixel& Image::PixelAt(int x, int y) 
    {
        return stencil.Contain(x, y) ? pixels_[y][x] : dev_null_pixel();
    }

    const Pixel& Image::PixelAt(int x, int y) const 
    {
        return stencil.Contain(x, y) ? pixels_[y][x] : dev_null_pixel();
    }
    
    void Image::Clear() 
    {
        for (auto& line : pixels_) 
        {
            for (auto& cell : line) 
            {
                cell = Pixel();
            }
        }
    }

}}}//qor::components::tui
