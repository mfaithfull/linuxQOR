// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"

#include "label.h"
#include "../renderer/string.h"
#include "../renderer/renderer.h"

using namespace qor::ui::tty;

namespace qor{ namespace ui{ namespace widgets { namespace tty {

    Label::Label(const std::string& text) : widgets::Label(text) { }
    Label::~Label() = default;

    void Label::ComputeRequirement()
    {
        m_requirement.min_x = string_width(m_text);
        m_requirement.min_y = 1;
    }
    
    void Label::Render(qor::ui::Renderer* renderer)
    {
        auto ttyRenderer = dynamic_cast<qor::ui::tty::Renderer*>(renderer);

        if(!ttyRenderer)
        {
            return;
        }

        int x = m_box.x_min;
        const int y = m_box.y_min;

        if (y > m_box.y_max) 
        {
            return;
        }

        for (const auto& cell : Utf8ToGlyphs(m_text)) 
        {
            if (x > m_box.x_max) 
            {
                break;
            }
            if (cell == "\n") 
            {
                continue;
            }
            ttyRenderer->GetSurface()->PixelAt(x, y).character = cell;

            //TODO: Deal with states e.g. focus, selection
            ++x;
        }
    }
}}}}//qor::ui::widgets::tty
