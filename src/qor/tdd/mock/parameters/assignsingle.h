// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

//Derived from HippoMocks
//Copyright (C) 2008, Bas van Tiel, Christian Rexwinkel, Mike Looijmans, Peter Bindels
//under GNU LGPL v2.1

#ifndef QOR_PP_H_TESTMOCK_ASSIGNSINGLE
#define QOR_PP_H_TESTMOCK_ASSIGNSINGLE

namespace qor{ namespace mock{

    template <typename A, typename B>
    struct assign_single
    {
        void operator()(A, B){}
    };

    template <typename A, typename B>
    struct assign_single<InParam<A>&, B&>
    {
        void operator()(InParam<A> a, B b)
        {
            *a.value = b;
        }
    };

    template <typename A, typename B>
    struct assign_single<OutParam<A>&, B&>
    {
        void operator()(OutParam<A> a, B& b) 
        {
            b = a.value;
        }
    };

    template <typename A, typename B>
    struct assign_single<OutParam<A>&, B*&>
    {
        void operator()(OutParam<A> a, B*& b) 
        {
            *b = a.value;
        }
    };

    template <typename A, typename B>
    void assignTo(A& a, B& b)
    {
        assign_single<A&, B&>()(a, b);
    }

}}//qor::mock

#endif//QOR_PP_H_TESTMOCK_ASSIGNSINGLE
