// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

//Derived from HippoMocks
//Copyright (C) 2008, Bas van Tiel, Christian Rexwinkel, Mike Looijmans, Peter Bindels
//under GNU LGPL v2.1

#ifndef QOR_PP_H_TESTMOCK_ASSIGNER
#define QOR_PP_H_TESTMOCK_ASSIGNER

namespace qor{ namespace mock{
    
    template <int index, int limit, typename ArgTuple, typename Tuple>
    struct assigner
    {
        static void Assign(ArgTuple& arg, Tuple& t) 
        {
            assignTo(std::get<index>(arg), std::get<index>(t));
            assigner<index + 1, limit, ArgTuple&, Tuple&>::Assign(arg, t);
        }
    };

    template <int limit, typename ArgTuple, typename Tuple>
    struct assigner<limit, limit, ArgTuple, Tuple>
    {
        static void Assign(ArgTuple&, Tuple&){}
    };

}}//qor::mock

#endif//QOR_PP_H_TESTMOCK_ASSIGNER
