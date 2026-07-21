// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_FRAMEWORK_DESKTOPUI_WIDGETS_PUSHBUTTON
#define QOR_PP_H_FRAMEWORK_DESKTOPUI_WIDGETS_PUSHBUTTON

#include "widget.h"
#include "../common/activateable.h"
#include "../common/focusable.h"
#include "../common/selectable.h"
#include "../common/hoverable.h"

namespace qor{ namespace ui { namespace widgets {
    
    class PushButton : public Widget, 
        public virtual IActivatable, 
        public virtual IFocusable, 
        public virtual ISelectable, 
        public virtual IHoverable
    {
    public:
        
        PushButton(const std::string& text);
        virtual ~PushButton();

        virtual void OnSelected() {}//TODO: Raise a signal

    protected:
        std::string m_text;
    };

}}}//qor::ui::widgets

#endif//QOR_PP_H_FRAMEWORK_DESKTOPUI_WIDGETS_PUSHBUTTON

