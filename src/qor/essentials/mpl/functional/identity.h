// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_MPL_FUNCTIIONAL_IDENTITY
#define QOR_PP_H_MPL_FUNCTIIONAL_IDENTITY

namespace qor { namespace mpl {

    //results in the dynamic input. Use to break recursive chains or forward args /
    // identity is one of the core concepts of cmpl. It is used as the default continuation and
    // when reached it breaks the chain as it has not continuation it self. it also comes in
    // handy as a place holder in each if one only wants to modify some of the arguments. It is
    // also used as a default predicate in algorithms like all where a predicate may be
    // unneeded.
    struct identity 
    {
        template <typename T>
        using f = T;
    };

    namespace eager 
    {
        template <typename T>
        using identity = T;
    }

}}//qor::mpl

#endif//QOR_PP_H_MPL_FUNCTIIONAL_IDENTITY