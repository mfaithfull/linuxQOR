
// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_UI_ITEM
#define QOR_PP_H_UI_ITEM

#include <memory>
#include <vector>
#include <cassert>

#include "src/qor/memory/reference/newref.h"

namespace qor{ namespace ui {
    
    class Item
    {
    public:

        virtual ~Item() {};
        template< typename U > constexpr bool Is() 
        { 
            U* test = dynamic_cast<U*>(this);
            return test != nullptr; 
        }        

        template< typename U > U* As()
        {
            U* as = dynamic_cast<U*>(this);
            return as;
        }
    };

}}//qor::ui

#endif//QOR_PP_H_UI_COMPOUND

