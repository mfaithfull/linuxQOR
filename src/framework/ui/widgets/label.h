// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_FRAMEWORK_UI_WIDGETS_LABEL
#define QOR_PP_H_FRAMEWORK_UI_WIDGETS_LABEL

#include "widget.h"

namespace qor{ namespace ui { namespace widgets {
    
    class Label : public Widget
    {
    public:
        
        Label(const std::string& text) : Widget(), m_text(text){ }
        virtual ~Label() = default;

        virtual void ComputeRequirement()
        {
            //Defaults for fixed font, console rendering. Otherwise we need a Measure from the Renderer
            m_requirement.min_x = static_cast<int>(m_text.size());
            m_requirement.min_y = 1;
        }
        
    protected:

        std::string m_text;
    };

}}}//qor::ui::widgets

#endif//QOR_PP_H_FRAMEWORK_UI_WIDGETS_LABEL

