// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_FRAMEWORK_DIO_VALUEADAPTER
#define QOR_PP_H_FRAMEWORK_DIO_VALUEADAPTER

#include "src/qor/essentials/traits/typetraits.h"

#include "valueadapters/streamio.h"
#include "valueadapters/readonly.h"
#include "valueadapters/readwrite.h"
#include "valueadapters/tostring.h"
#include "valueadapters/name.h"

namespace qor{ namespace dio{

    template <typename T, typename... Bases>
    struct ValueAdapter : public Bases...
    {
        using value_type = T;

        ValueAdapter(T& t) : Bases(std::forward<T&>(t))...
        { }        
    };

    template<template<class...> class P, class T, template<class> class... L>
    struct adapter_apply
    { 
        using type = P<T, L<T>... >;
    };

}}//qor::dio

#endif//QOR_PP_H_FRAMEWORK_DIO_VALUEADAPTER
