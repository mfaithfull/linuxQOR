// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"

#include "hbox.h"
#include "detail/boxhelper.h"

namespace qor{ namespace ui{ namespace layout {

    Element hbox(Elements children)
    {
        return new_ref<HBox>(std::move(children));
    }

    void HBox::ComputeRequirement()
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
                requirement_.focused.box.Shift(requirement_.min_x, 0);
            }
            */
            // Extend the min_x and min_y to contain all the children
            requirement_.min_x += child->requirement().min_x;
            requirement_.min_y =
            std::max(requirement_.min_y, child->requirement().min_y);
        }
    }

    void HBox::SetBox(Box box)
    {
        Node::SetBox(box);

        std::vector<detail::boxhelper::Element> elements(children_.size());
        for (size_t i = 0; i < children_.size(); ++i) 
        {
            auto& element = elements[i];
            const auto& requirement = children_[i]->requirement();
            element.min_size = requirement.min_x;
            element.flex_grow = requirement.flex_grow_x;
            element.flex_shrink = requirement.flex_shrink_x;
        }
        const int target_size = box.x_max - box.x_min + 1;
        detail::boxhelper::Compute(&elements, target_size);

        int x = box.x_min;
        for (size_t i = 0; i < children_.size(); ++i) 
        {
            box.x_min = x;
            box.x_max = x + elements[i].size - 1;
            children_[i]->SetBox(box);
            x = box.x_max + 1;
        }
    }

}}}//qor::ui::layout
