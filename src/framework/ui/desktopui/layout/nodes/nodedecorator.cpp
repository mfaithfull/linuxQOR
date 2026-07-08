// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"

#include "nodedecorator.h"
#include "../functional/flexbox.h"
#include "detail/anyargs.h"

namespace qor{ namespace ui{ namespace layout {

    NodeDecorator::NodeDecorator(Element child) : Node(detail::unpack(std::move(child))) 
    {
    }

    void NodeDecorator::ComputeRequirement() 
    {
        Node::ComputeRequirement();
        requirement_ = children_[0]->requirement();
    }

    void NodeDecorator::SetBox(Box box) 
    {
        Node::SetBox(box);
        children_[0]->SetBox(box);
    }
    
}}}//qor::ui::layout
