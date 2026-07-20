
// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_FRAMEWORK_UI_LAYOUT_ARRANGABLE
#define QOR_PP_H_FRAMEWORK_UI_LAYOUT_ARRANGABLE

#include <type_traits>
#include "src/qor/memory/reference/newref.h"
#include "requirement.h"
#include "box.h"
#include "../compound.h"

namespace qor{ namespace ui{ namespace layout {
    
    //Has what is required to be placed in a position    
    class IArrangeable : public virtual Item
    {
    public:
        
        struct Status
        {
            int iteration = 0;
            bool need_iteration = false;
        };

        IArrangeable() = default;
        virtual ~IArrangeable() = default;        

        virtual void ComputeRequirement()
        {
            //Propagate ComputeRequirment to children
            if(Item::Is<Compound>())
            {
                Compound* compound = dynamic_cast<Compound*>(this);
                compound->ApplyIf<IArrangeable>([](IArrangeable* child)
                {
                    child->ComputeRequirement();
                });

                auto firstChild = compound->ChildAt(0)->As<IArrangeable>();
                if(firstChild)
                {
                    m_requirement = firstChild->GetRequirement();
                }
            }
            
            // By default, the requirement is the one of the first child.
        }

        Requirement GetRequirement()
        {
            return m_requirement;
        }

        virtual void SetBox(Box box)
        {
            m_box = box;
        }

        virtual void Check(Status* status)
        {
            //Propagate Check to children
            if(Item::Is<Compound>())
            {
                Compound* compound = dynamic_cast<Compound*>(this);
                compound->ApplyIf<IArrangeable>([status](IArrangeable* child)
                {
                    child->Check(status);
                });
            }
            status->need_iteration |= (status->iteration == 0);
        }

    protected:
        
        Requirement m_requirement;
        Box m_box;
    };

}}}//qor::ui::layout

#endif//QOR_PP_H_FRAMEWORK_UI_LAYOUT_ARRANGABLE
