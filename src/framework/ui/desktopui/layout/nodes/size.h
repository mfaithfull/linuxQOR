// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_FRAMEWORK_DESKTOPUI_LAYOUT_SIZE
#define QOR_PP_H_FRAMEWORK_DESKTOPUI_LAYOUT_SIZE

#include "../node.h"
#include "vbox.h"
#include "hbox.h"

namespace qor{ namespace ui{ namespace layout { 

    class qor_pp_module_interface(QOR_DESKTOPUI) Size : public Node 
    {
    public:
        Size(Element child, WidthOrHeight direction, Constraint constraint, int value);
        void ComputeRequirement() override;
        void SetBox(Box box) override;

    private:
        WidthOrHeight direction_;
        Constraint constraint_;
        int value_;
    };

    //Apply a constraint on the size of an element. direction Whether the WIDTH or the HEIGHT of the element must be constrained.
    //constraint The type of constaint.
    //qor_pp_module_interface(QOR_DESKTOPUI) Decorator size(WidthOrHeight direction, Constraint constraint, int value);

}}}//qor::ui::layout

#endif//QOR_PP_H_FRAMEWORK_DESKTOPUI_LAYOUT_SIZE

