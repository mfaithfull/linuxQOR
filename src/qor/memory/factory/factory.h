// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_FACTORY
#define QOR_PP_H_FACTORY

//factory_of type trait. Use to override the defualt use of InternalFactory
//for types that have an ExternalFactory. i.e. their factory must be looked
//up at runtime

namespace qor{

    template< class T > class InternalFactory;

    template<typename T>
    struct factory_of
    {
        typedef InternalFactory< T > type;
    };

}//qor

//Preprocessor macro shorthand for declaring a factory_of specialisation
#   define qor_pp_declare_factory_of(_CLASS,_FACTORY)\
template<> struct factory_of< _CLASS >\
{\
    typedef _FACTORY< _CLASS > type;\
};

//Example: qor_pp_declare_factory_of(SomeFeature, ExternalFactory);

#endif//QOR_PP_H_FACTORY
