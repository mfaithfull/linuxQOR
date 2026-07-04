// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

//Derived from HippoMocks
//Copyright (C) 2008, Bas van Tiel, Christian Rexwinkel, Mike Looijmans, Peter Bindels
//under GNU LGPL v2.1

#ifndef QOR_PP_H_MOCKREVERSE
#define QOR_PP_H_MOCKREVERSE

namespace qor { namespace mock {

    template<typename T>
    struct reversed_container
    {
        T& container;
    };

    template <typename T>
    auto begin(reversed_container<T> container) -> decltype(container.container.rbegin()) 
    { 
        return container.container.rbegin(); 
    }

    template <typename T>
    auto end(reversed_container<T> container) -> decltype(container.container.rend()) 
    { 
        return container.container.rend(); 
    }

    template <typename T>
    reversed_container<T> reverse_order(T& container) 
    { 
        return { container }; 
    }

}}//qor::mock

#endif//QOR_PP_H_MOCKREVERSE
