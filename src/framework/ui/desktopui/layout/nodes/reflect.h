// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_FRAMEWORK_DESKTOPUI_LAYOUT_REFLECT
#define QOR_PP_H_FRAMEWORK_DESKTOPUI_LAYOUT_REFLECT

#include "../node.h"
#include "vbox.h"
#include "hbox.h"

namespace qor{ namespace ui{ namespace layout { 

    class qor_pp_module_interface(QOR_DESKTOPUI) Reflect : public Node 
    {
    public:
        Reflect(Element child, Box* box);
        void ComputeRequirement() final;
        void SetBox(Box box) final;

    private:
        Box& reflected_box_;
    };

    //qor_pp_module_interface(QOR_DESKTOPUI) Decorator reflect(Box& box);
}}}//qor::ui::layout

#endif//QOR_PP_H_FRAMEWORK_DESKTOPUI_LAYOUT_REFLECT

