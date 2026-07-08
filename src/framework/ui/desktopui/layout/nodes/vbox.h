// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_FRAMEWORK_DESKTOPUI_LAYOUT_VBOX
#define QOR_PP_H_FRAMEWORK_DESKTOPUI_LAYOUT_VBOX

#include "../node.h"
#include "detail/anyargs.h"

namespace qor{ namespace ui{ namespace layout { 

    class qor_pp_module_interface(QOR_DESKTOPUI) VBox : public Node 
    {
    public:

        explicit VBox(Elements children) : Node(std::move(children)) {}

    private:        
    
        void ComputeRequirement() override;
        void SetBox(Box box) override;
    };

    TAKE_ANY_ARGS(vbox)

    //A container displaying elements vertically one by one.
    //qor_pp_module_interface(QOR_DESKTOPUI) Element vbox(Elements children);

}}}//qor::ui::layout

#endif//QOR_PP_H_FRAMEWORK_DESKTOPUI_LAYOUT_VBOX

