// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"

#include <string>
#include <utility>
#include <cstddef>

#include "node.h"

namespace qor{ namespace ui{ namespace layout {

    Node::Node() = default;
    
    Node::Node(Elements children) : children_(std::move(children)) {}

    Node::~Node() = default;

    //Compute how much space a node needs.
    void Node::ComputeRequirement() 
    {    
        if (children_.empty()) 
        {
            return;
        }
        for (auto& child : children_) 
        {
            if(child)
            {
                child->ComputeRequirement();
            }
        }

        // By default, the requirement is the one of the first child.
        if(children_[0])
        {
            requirement_ = children_[0]->requirement();
        }

        // Propagate additional requirements to children.
        for (size_t i = 1; i < children_.size(); ++i) 
        {
            /*
            if (!requirement_.focused.enabled && children_[i]->requirement().focused.enabled) 
            {
                requirement_.focused = children_[i]->requirement().focused;
            }
            */
        }
    }

    //Assign a position and a dimension to a node for drawing.
    void Node::SetBox(Box box) 
    {
        box_ = box;
    }

    //Visit a node
    void Node::Visit(std::function<void (Node*)> operation) 
    {
        operation(this);
        for (auto& child : children_) 
        {
            if(child)
            {
                child->Visit(operation);
            }
        }
    }

    void Node::Check(Status* status) 
    {
        for (auto& child : children_) 
        {
            if(child)
            {
                child->Check(status);
            }
        }
        status->need_iteration |= (status->iteration == 0);
    }

}}}//qor::ui::layout
