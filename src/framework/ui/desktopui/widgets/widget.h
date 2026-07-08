// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_FRAMEWORK_DESKTOPUI_WIDGETS_WIDGET
#define QOR_PP_H_FRAMEWORK_DESKTOPUI_WIDGETS_WIDGET

#include "../layout/node.h"

namespace qor{ namespace ui {
    
    class qor_pp_module_interface(QOR_DESKTOPUI) Widget : public layout::Node
    {
    public:

        Widget() : layout::Node()
        {            
        }

        virtual ~Widget() = default;
    };

}}//qor::ui

#endif//QOR_PP_H_FRAMEWORK_DESKTOPUI_WIDGETS_WIDGET

