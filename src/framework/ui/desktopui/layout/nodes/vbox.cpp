// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"

#include "vbox.h"
#include "detail/boxhelper.h"

namespace qor{ namespace ui{ namespace layout {

    //A container displaying elements vertically one by one.
    Element vbox(Elements children)
    {
        return new_ref<VBox>(std::move(children));
    }

    void VBox::ComputeRequirement()
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
                requirement_.focused.box.Shift(0, requirement_.min_y);
            }
            */

            // Extend the min_x and min_y to contain all the children
            requirement_.min_y += child->requirement().min_y;
            requirement_.min_x = std::max(requirement_.min_x, child->requirement().min_x);
        }
    }

    void VBox::SetBox(Box box)
    {
        Node::SetBox(box);

        std::vector<detail::boxhelper::Element> elements(children_.size());
        for (size_t i = 0; i < children_.size(); ++i) 
        {
            auto& element = elements[i];
            const auto& requirement = children_[i]->requirement();
            element.min_size = requirement.min_y;
            element.flex_grow = requirement.flex_grow_y;
            element.flex_shrink = requirement.flex_shrink_y;
        }
        const int target_size = box.y_max - box.y_min + 1;
        detail::boxhelper::Compute(&elements, target_size);

        int y = box.y_min;
        for (size_t i = 0; i < children_.size(); ++i) 
        {
            box.y_min = y;
            box.y_max = y + elements[i].size - 1;
            children_[i]->SetBox(box);
            y = box.y_max + 1;
        }
    }

}}}//qor::ui::layout
