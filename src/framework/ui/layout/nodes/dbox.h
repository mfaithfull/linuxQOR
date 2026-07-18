// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_FRAMEWORK_DESKTOPUI_LAYOUT_DBOX
#define QOR_PP_H_FRAMEWORK_DESKTOPUI_LAYOUT_DBOX

#include "../node.h"
#include "../box.h"
#include "detail/anyargs.h"

namespace qor{ namespace ui{ namespace layout { 

    class qor_pp_module_interface(QOR_DESKTOPUI) DBox : public Node 
    {
    public:
        explicit DBox(Elements children) : Node(std::move(children)) {}

        void ComputeRequirement() override 
        {
            requirement_ = Requirement{};
            for (auto& child : children_) 
            {
                child->ComputeRequirement();

                // Propagate the focused requirement.
                /*
                if (requirement_.focused.Prefer(child->requirement().focused)) 
                {
                    requirement_.focused = child->requirement().focused;
                }
                */

                // Extend the min_x and min_y to contain all the children
                requirement_.min_x = std::max(requirement_.min_x, child->requirement().min_x);
                requirement_.min_y = std::max(requirement_.min_y, child->requirement().min_y);
            }
        }

        void SetBox(Box box) override 
        {
            Node::SetBox(box);

            for (auto& child : children_) 
            {
                child->SetBox(box);
            }
        }
    };

    TAKE_ANY_ARGS(dbox)

    //Stack several element on top of each other.
    //children_ The input element.
    //qor_pp_module_interface(QOR_DESKTOPUI) Element dbox(Elements children_);

}}}//qor::ui::layout

#endif//QOR_PP_H_FRAMEWORK_DESKTOPUI_LAYOUT_DBOX

