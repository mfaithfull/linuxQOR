// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"

#include "hbox.h"
#include "nodes/detail/boxhelper.h"

namespace qor{ namespace ui{ namespace layout {

    HBox::HBox() = default;
    HBox::~HBox() = default;

    void HBox::ComputeRequirement()
    {
        m_requirement = Requirement{};

        Compound* compound = dynamic_cast<Compound*>(this);
        compound->ApplyIf<IArrangeable>([this](IArrangeable* child)
        {
            child->ComputeRequirement();
            m_requirement.min_x += child->GetRequirement().min_x;
            m_requirement.min_y = std::max(m_requirement.min_y, child->GetRequirement().min_y);
        });
    }

    void HBox::SetBox(Box box)
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
                element.min_size = requirement.min_x;
                element.flex_grow = requirement.flex_grow_x;
                element.flex_shrink = requirement.flex_shrink_x;
            }
        }

        const int target_size = box.x_max - box.x_min + 1;
        detail::boxhelper::Compute(&elements, target_size);

        int x = box.x_min;
        for (size_t i = 0; i < m_children.size(); ++i) 
        {
            box.x_min = x;
            box.x_max = x + elements[i].size - 1;
            auto arrangeable = m_children[i]->As<IArrangeable>();
            if(arrangeable)
            {
                arrangeable->SetBox(box);
            }
            x = box.x_max + 1;
        }
    }

}}}//qor::ui::layout
