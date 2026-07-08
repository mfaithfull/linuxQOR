// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_FRAMEWORK_DESKTOPUI_LAYOUT_NODE
#define QOR_PP_H_FRAMEWORK_DESKTOPUI_LAYOUT_NODE

#include <memory>
#include <vector>
#include <string>
#include <functional>

#include "src/qor/essentials/current/currentthread.h"
#include "src/qor/memory/injection/typeidentity.h"
#include "src/qor/memory/reference/newref.h"

#include "box.h"
#include "requirement.h"
#include "functional/layout.h"

namespace qor{ namespace ui{ namespace layout { 
/*
    struct Dimensions
    {
        int dimx;
        int dimy;
    };
*/
    // Node is the base class for all elements in the DOM tree.
    // It typically contains child elements, which are also instances of Node.
    // Derive from this class to create custom elements.
    class qor_pp_module_interface(QOR_DESKTOPUI) Node 
    {
    public:
        Node();
        explicit Node(Elements children);
        Node(const Node&) = delete;
        Node(const Node&&) = delete;
        Node& operator=(const Node&) = delete;
        Node& operator=(const Node&&) = delete;

        virtual ~Node();

        // Step 1: Compute layout requirement. Tell parent what dimensions this element wants to be. Propagated from Children to Parents.
        virtual void ComputeRequirement();
        Requirement requirement() { return requirement_; }

        // Step 2: Assign this element its final dimensions. Propagated from Parents to Children.
        virtual void SetBox(Box box);

        // Step 3: Visit this element. Propagate things from Parents to Children, Focus, Selection etc, transients that affect appearance
        virtual void Visit(std::function<void (Node*)> operation);

        // Layout may not resolve within a single iteration for some elements. This
        // allows them to request additionnal iterations. This signal must be
        // forwarded to children at least once.
        struct Status 
        {
            int iteration = 0;
            bool need_iteration = false;
        };

        virtual void Check(Status* status);

        //Override this this for things that need rendering
        virtual bool IsVisible()
        {
            return false;
        }

        virtual void Express(std::function<void(Box&)> realize)
        {
            realize(box_);
        }
        
    protected:

        Elements children_;
        Requirement requirement_;
        Box box_;
    };

}}}//qor::ui::layout

#endif//QOR_PP_H_FRAMEWORK_DESKTOPUI_LAYOUT_NODE

