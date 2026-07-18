// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_FRAMEWORK_DESKTOPUI_LAYOUT_NODEDECORATOR
#define QOR_PP_H_FRAMEWORK_DESKTOPUI_LAYOUT_NODEDECORATOR

#include "../node.h"
#include "vbox.h"
#include "hbox.h"

namespace qor{ namespace ui{ namespace layout { 

    class qor_pp_module_interface(QOR_DESKTOPUI) NodeDecorator : public Node 
    {
    public:
        explicit NodeDecorator(Element child);
        void ComputeRequirement() override;
        void SetBox(Box box) override;
    };

}}}//qor::ui::layout

#endif//QOR_PP_H_FRAMEWORK_DESKTOPUI_LAYOUT_NODEDECORATOR

