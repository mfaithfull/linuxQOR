
// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_FRAMEWORK_UI_LAYOUT_ARRANGER
#define QOR_PP_H_FRAMEWORK_UI_LAYOUT_ARRANGER

#include <type_traits>
#include "src/qor/memory/reference/newref.h"
#include "requirement.h"
#include "box.h"
#include "arrangeable.h"

namespace qor{ namespace ui{ namespace layout {
    
    class Arranger : public Compound, public virtual IArrangeable
    {
    public:
        
        Arranger() = default;
        virtual ~Arranger() = default;        

        virtual void SetBox(Box box)
        {
            IArrangeable::SetBox(box);
            for (size_t i = 0; i < m_children.size(); ++i) 
            {            
                auto arrangeable = m_children[i]->As<IArrangeable>();
                if(arrangeable)
                {
                    arrangeable->SetBox(box);
                }
            }
        }
    };

}}}//qor::ui::layout

#endif//QOR_PP_H_FRAMEWORK_UI_LAYOUT_ARRANGER
