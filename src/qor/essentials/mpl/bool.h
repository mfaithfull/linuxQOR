// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_MPL_BOOL
#define QOR_PP_H_MPL_BOOL

namespace qor{ namespace mpl{

    // wrapper which represents bool types
    template <bool Value>
    struct bool_ 
    {
        static constexpr bool value = Value;
    };
    
    using true_ = bool_<true>;  // universal true type in the library
    
    using false_ = bool_<false>;// universal false type in the library

}}//qor::mpl

#endif//QOR_PP_H_MPL_BOOL