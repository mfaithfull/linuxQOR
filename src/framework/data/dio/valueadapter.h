// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_FRAMEWORK_DIO_VALUEADAPTER
#define QOR_PP_H_FRAMEWORK_DIO_VALUEADAPTER

#include "src/qor/essentials/traits/typetraits.h"

#include "valueoperations/streamio.h"
#include "valueoperations/readonly.h"
#include "valueoperations/readwrite.h"
#include "valueoperations/tostring.h"
#include "valueoperations/name.h"

namespace qor{ namespace dio{

    //Each of the Bases should be a valueoperation
    template <typename T, typename... Bases>
    struct ValueAdapter : public Bases...
    {
        using value_type = T;

        ValueAdapter(T& t) : Bases(std::forward<T&>(t))...
        { }        
    };

    //genertate a ValueAdapter type P templated on T with each member of the type list L as bases also templated on T
    template<template<class...> class P, class T, template<class> class... L>
    struct adapter_apply
    { 
        using type = P<T, L<T>... >;
    };

}}//qor::dio

#endif//QOR_PP_H_FRAMEWORK_DIO_VALUEADAPTER
