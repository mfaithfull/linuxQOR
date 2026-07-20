// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"

#include "contained.h"
#include "compound.h"

namespace qor{ namespace ui{

    constexpr bool Contained::Is(const Contained*) const { return true; }

    Contained::Contained() = default;
    
    Contained::~Contained() = default;

    Compound* Contained::Parent() const
    {
        return m_parent;
    }

    void Contained::SetParent(Compound* parent)
    {
        m_parent = parent;
    }

    int Contained::Index() const
    {
        if (m_parent == nullptr) 
        {
            return -1;
        }
        return m_parent->IndexOf(this);
    }

    void Contained::Detach()
    {
        if (m_parent == nullptr) 
        {
            return;
        }
        m_parent->Detach(this);
        m_parent = nullptr;
    }

}}//qor::ui
