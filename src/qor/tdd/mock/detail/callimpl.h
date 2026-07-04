// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

//Derived from HippoMocks
//Copyright (C) 2008, Bas van Tiel, Christian Rexwinkel, Mike Looijmans, Peter Bindels
//under GNU LGPL v2.1

#ifndef QOR_PP_H_TESTMOCK_DETAIL_CALLIMPL
#define QOR_PP_H_TESTMOCK_DETAIL_CALLIMPL

namespace qor{ namespace mock{ namespace detail {

    template <typename F, typename Tuple, bool Done, int Total, int... N>
    struct call_impl
    {
        static typename F::result_type call(F f, Tuple&& t)
        {
            return call_impl<F, Tuple, Total == 1 + sizeof...(N), Total, N..., sizeof...(N)>::call(f, std::forward<Tuple>(t));
        }
    };

    template <typename F, typename Tuple, int Total, int... N>
    struct call_impl<F, Tuple, true, Total, N...>
    {        
        static typename F::result_type call(F f, Tuple&& t)
        {
            return f(std::get<N>(std::forward<Tuple>(t))...);
        }
    };

}}}//qor::mock::detail

#endif//QOR_PP_H_TESTMOCK_DETAIL_CALLIMPL
