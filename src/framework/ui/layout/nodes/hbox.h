// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_FRAMEWORK_DESKTOPUI_LAYOUT_HBOX
#define QOR_PP_H_FRAMEWORK_DESKTOPUI_LAYOUT_HBOX

#include "../node.h"
#include "detail/anyargs.h"

namespace qor{ namespace ui{ namespace layout { 

    class qor_pp_module_interface(QOR_DESKTOPUI) HBox : public Node 
    {
    public:
        explicit HBox(Elements children) : Node(std::move(children)) {}

    private:
        void ComputeRequirement() override;
        void SetBox(Box box) override;

    };

    TAKE_ANY_ARGS(hbox)

    //qor_pp_module_interface(QOR_DESKTOPUI) Element hbox(Elements children);

}}}//qor::ui:layout

#endif//QOR_PP_H_FRAMEWORK_DESKTOPUI_LAYOUT_HBOX

