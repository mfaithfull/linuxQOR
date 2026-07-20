
// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_FRAMEWORK_UI_LAYOUT_LEAF
#define QOR_PP_H_FRAMEWORK_UI_LAYOUT_LEAF

#include "arrangeable.h"
#include "../contained.h"

namespace qor{ namespace ui{ namespace layout {
    
    class Leaf : public Contained, public virtual IArrangeable
    {
    public:
        
        Leaf() = default;
        virtual ~Leaf() = default;        

    };

}}}//qor::ui::layout

#endif//QOR_PP_H_FRAMEWORK_UI_LAYOUT_LEAF
