// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_MPL_SEQUENCE_TAKE
#define QOR_PP_H_MPL_SEQUENCE_TAKE

#include "drop.h"
#include "../algorithm/rotate.h"

namespace qor { namespace mpl {

    template <typename N, typename C = listify>
    struct take 
    {
        template <typename... Ts>
        using f = typename dcall<rotate<N, drop<uint_<(sizeof...(Ts)-N::value)>, C>>, sizeof...(Ts)>::template f<Ts...>;
    };

    namespace eager 
    {
        template <typename List, unsigned N>
        using take = call<unpack<mpl::take<mpl::uint_<N>>>, List>;
    }

}}//qor::mpl

#endif//QOR_PP_H_MPL_SEQUENCE_TAKE
