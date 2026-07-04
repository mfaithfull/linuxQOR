// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

//Derived from HippoMocks
//Copyright (C) 2008, Bas van Tiel, Christian Rexwinkel, Mike Looijmans, Peter Bindels
//under GNU LGPL v2.1

#ifndef QOR_PP_H_TESTMOCK_INVOKE
#define QOR_PP_H_TESTMOCK_INVOKE

namespace qor{ namespace mock{

    template <typename F, typename Tuple>
    typename F::result_type invoke(F f, Tuple&& t)
    {
        typedef typename std::decay<Tuple>::type ttype;
        return detail::call_impl<F, Tuple, 0 == std::tuple_size<ttype>::value, std::tuple_size<ttype>::value>::call(f, std::forward<Tuple>(t));
    }

}}//qor::mock

#endif//QOR_PP_H_TESTMOCK_INVOKE
