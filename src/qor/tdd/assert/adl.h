// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

//Derived from assertcc
//Copyright 2021 Sean Nash
//under BSD 3 clause license

#ifndef QOR_PP_H_ASSERT_ADL
#define QOR_PP_H_ASSERT_ADL

namespace qor{ namespace assert{

    /**
        This class serves for the compiler to locate the assert_that_internal
       functions using argument dependent lookup.
    
        Without this currently Clang 10 will not compile the code. GCC 10 will
       compile the code due to a bug.
    
        https://clang.llvm.org/compatibility.html#dep_lookup
    */
    class Adl {};
    
}}//qor::assert

#endif//QOR_PP_H_ASSERT_ADL
