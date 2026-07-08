// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"

#include "size.h"
#include "../functional/flexbox.h"
#include "detail/anyargs.h"

namespace qor{ namespace ui{ namespace layout {

    Size::Size(Element child, WidthOrHeight direction, Constraint constraint, int value) : Node(detail::unpack(std::move(child))), direction_(direction), constraint_(constraint), value_(std::max(0, value)) 
    {                
    }
    //Apply a constraint on the size of an element. direction Whether the WIDTH or the HEIGHT of the element must be constrained.
    //constraint The type of constaint.
    Decorator size(WidthOrHeight direction, Constraint constraint, int value) 
    {
        return [=](Element e) 
        {
            return new_ref<Size>(std::move(e), direction, constraint, value);
        };
    }

    void Size::ComputeRequirement()
    {
        Node::ComputeRequirement();
        requirement_ = children_[0]->requirement();

        auto& value = direction_ == WIDTH ? requirement_.min_x : requirement_.min_y;

        switch (constraint_) 
        {
        case LESS_THAN:
            value = std::min(value, value_);
            break;
        case EQUAL:
            value = value_;
            break;
        case GREATER_THAN:
            value = std::max(value, value_);
            break;
        }

        if (direction_ == WIDTH) 
        {
            requirement_.flex_grow_x = 0;
            requirement_.flex_shrink_x = 0;
        } 
        else 
        {
            requirement_.flex_grow_y = 0;
            requirement_.flex_shrink_y = 0;
        }
    }

    void Size::SetBox(Box box)
    {
        Node::SetBox(box);

        if (direction_ == WIDTH) 
        {
            switch (constraint_) 
            {
                case LESS_THAN:
                case EQUAL:
                box.x_max = std::min(box.x_min + value_ + 1, box.x_max);
                break;
                case GREATER_THAN:
                break;
            }
        } 
        else 
        {
            switch (constraint_) 
            {
                case LESS_THAN:
                case EQUAL:
                box.y_max = std::min(box.y_min + value_ + 1, box.y_max);
                break;
                case GREATER_THAN:
                break;
            }
        }
        children_[0]->SetBox(box);
    }

}}}//qor::ui::layout
