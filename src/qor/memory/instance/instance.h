// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_INSTANCE
#define QOR_PP_H_INSTANCE

//Instancer trait class. Used to imbue a type with instance limits. 
//To make it's instances pooled or a singleton
namespace qor{

    class DefaultInstancer;

    template<typename T>
    struct instancer_of
    {
        typedef DefaultInstancer type;
    };

}//qor

//Preprocessor macro shorthand for declaring a instancer_of specialisation
#   define qor_pp_declare_instancer_of(_CLASS,_INSTANCER)\
template<> struct instancer_of< _CLASS >\
{\
    typedef _INSTANCER type;\
};
//Example: qor_pp_declare_instancer_of(LimitedResource, PoolInstancer);

#endif//QOR_PP_H_INSTANCE
