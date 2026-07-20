// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"

#include "dbox.h"
#include "nodes/detail/boxhelper.h"

namespace qor{ namespace ui{ namespace layout {

    DBox::DBox() = default;
    DBox::~DBox() = default;

    void DBox::ComputeRequirement()
    {
        m_requirement = Requirement{};
        Compound* compound = dynamic_cast<Compound*>(this);
        compound->ApplyIf<IArrangeable>([this](IArrangeable* child)
        {
            child->ComputeRequirement();
            m_requirement.min_x = std::max(m_requirement.min_x, child->GetRequirement().min_x);
            m_requirement.min_y = std::max(m_requirement.min_y, child->GetRequirement().min_y);
        });
    }

    void DBox::SetBox(Box box)
    {
        Arranger::SetBox(box);
        for (size_t i = 0; i < m_children.size(); ++i) 
        {            
            auto arrangeable = m_children[i]->As<IArrangeable>();
            if(arrangeable)
            {
                arrangeable->SetBox(box);
            }
        }
    }

}}}//qor::ui::layout
