// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_FRAMEWORK_DESKTOPUI_WIDGETS_WIDGET
#define QOR_PP_H_FRAMEWORK_DESKTOPUI_WIDGETS_WIDGET

#include "../layout/arrangeable.h"
#include "../common/renderable.h"
#include "../common/contained.h"

namespace qor{ namespace ui {
    
    class Widget : public Compound, public virtual Contained, public virtual layout::IArrangeable, public virtual IRenderable
    {
    public:

        Widget() = default;
        virtual ~Widget() = default;
    };

}}//qor::ui

#endif//QOR_PP_H_FRAMEWORK_DESKTOPUI_WIDGETS_WIDGET

