// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"

#include "reflect.h"
#include "../functional/flexbox.h"
#include "detail/anyargs.h"

namespace qor{ namespace ui{ namespace layout {

    Decorator reflect(Box& box) 
    {
        return [&](Element child) -> Element 
        {
            return new_ref<Reflect>(std::move(child), &box);
        };
    }

    Reflect::Reflect(Element child, Box* box) : Node(detail::unpack(std::move(child))), reflected_box_(*box) 
    {
    }

    void Reflect::ComputeRequirement()
    {
        Node::ComputeRequirement();
        requirement_ = children_[0]->requirement();
    }

    void Reflect::SetBox(Box box)
    {
        reflected_box_ = box;
        Node::SetBox(box);
        children_[0]->SetBox(box);
    }

}}}//qor::ui::layout
