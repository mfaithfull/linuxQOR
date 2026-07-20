// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"

#include "vbox.h"
#include "nodes/detail/boxhelper.h"

namespace qor{ namespace ui{ namespace layout {

    VBox::VBox() = default;
    VBox::~VBox() = default;

    void VBox::ComputeRequirement()
    {
        m_requirement = Requirement{};

        Compound* compound = dynamic_cast<Compound*>(this);
        compound->ApplyIf<IArrangeable>([this](IArrangeable* child)
        {
            child->ComputeRequirement();

            // Extend the min_x and min_y to contain all the children
            m_requirement.min_y += child->GetRequirement().min_y;
            m_requirement.min_x = std::max(m_requirement.min_x, child->GetRequirement().min_x);
        });
    }

    void VBox::SetBox(Box box)
    {
        IArrangeable::SetBox(box);

        std::vector<detail::boxhelper::Element> elements(m_children.size());
        for (size_t i = 0; i < m_children.size(); ++i) 
        {
            auto& element = elements[i];
            auto arrangeable = m_children[i]->As<IArrangeable>();
            if(arrangeable)
            {
                const auto& requirement = arrangeable->GetRequirement();
                element.min_size = requirement.min_y;
                element.flex_grow = requirement.flex_grow_y;
                element.flex_shrink = requirement.flex_shrink_y;
            }            
        }

        const int target_size = box.y_max - box.y_min + 1;
        detail::boxhelper::Compute(&elements, target_size);

        int y = box.y_min;
        for (size_t i = 0; i < m_children.size(); ++i) 
        {
            box.y_min = y;
            box.y_max = y + elements[i].size - 1;
            auto arrangeable = m_children[i]->As<IArrangeable>();
            if(arrangeable)
            {
                arrangeable->SetBox(box);
            }
            y = box.y_max + 1;
        }
    }

}}}//qor::ui::layout
