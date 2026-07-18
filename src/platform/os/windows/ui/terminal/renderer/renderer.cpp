// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"

#include <iostream>
#include <sstream>
#include "renderer.h"

namespace qor{ namespace ui{ namespace tui { namespace win {

    // Print the Screen to the terminal.
    void Renderer::Print() const 
    {
        std::cout << m_image_buffer->ToString() << '\0' << std::flush;
    }

    void Renderer::ApplyShader() 
    {
        m_image_buffer->ApplyShader();
    }

    std::string Renderer::ResetPosition(bool clear) const 
    {
        std::stringstream ss;
        if (clear) 
        {
            ss << "\r";       // MOVE_LEFT;
            ss << "\x1b[2K";  // CLEAR_SCREEN;
            for (int y = 1; y < dimy_; ++y) 
            {
                ss << "\x1B[1A";  // MOVE_UP;
                ss << "\x1B[2K";  // CLEAR_LINE;
            }
        } 
        else 
        {
            ss << "\r";  // MOVE_LEFT;
            for (int y = 1; y < dimy_; ++y) 
            {
                ss << "\x1B[1A";  // MOVE_UP;
            }
        }
        return ss.str();
    }

}}}}//qor::ui::tui::win
