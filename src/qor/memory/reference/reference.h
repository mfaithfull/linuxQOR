// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_REFERENCE
#define QOR_PP_H_REFERENCE

#include "ref.h"
#include "flyerref.h"

//trait that attaches a smart reference to a type

namespace qor{

    template<typename T>
    struct ref_of
    {
        typedef Ref<T> type;
    };

}//qor

//Preprocessor macro shorthand for declaring a ref_of specialisation
#   define qor_pp_declare_ref_of(_CLASS,_REF)\
template<> struct ref_of< _CLASS >\
{\
    typedef _REF< _CLASS > type;\
};

//Example: qor_pp_declare_ref_of(ErrorHandler, flyerref);

#endif//QOR_PP_H_FACTORY
