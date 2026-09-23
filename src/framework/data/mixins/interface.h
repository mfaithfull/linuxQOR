// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_FRAMEWORK_MIXINS_INTERFACE
#define QOR_PP_H_FRAMEWORK_MIXINS_INTERFACE

#include "foreach.h"

namespace qor{ namespace mix{

    struct ringable{ };//just an empty tag

    //interface mixin
    template< typename T >
    struct bells : T 
    {
        void ring(){
            for_each(this, ability< ringable >, 
                [ a = access_to( this ) ]( auto & m )
                { 
                    m.ring(a); 
                }
            );
            //mixin uses a to find what it needs
            //for_each could be execute if there is always one or
            //call_on(this, predicate<my_selector>, \[\](...))
        }
    };

    //interface2<interface1<protect<access<composition<Ts...>>>>>
}}//qor::mix

#endif//QOR_PP_H_FRAMEWORK_MIXINS_INTERFACE