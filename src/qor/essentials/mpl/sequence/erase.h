// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_MPL_SEQUENCE_ERASE
#define QOR_PP_H_MPL_SEQUENCE_ERASE

#include "../algorithm/rotate.h"
#include "../functional/call.h"
#include "popfront.h"
#include "../int.h"

namespace qor { namespace mpl {

    template <typename Index, typename C = listify>
    struct erase 
    {
        template <typename... Ts>
        using f = typename rotate<Index, pop_front<rotate<mpl::uint_<(sizeof...(Ts)-Index::value - 1)>, C>>>:: template f<Ts...>;
    };

    namespace eager 
    {
        template <typename List, unsigned Index>
        using erase = call<unpack<mpl::erase<uint_<Index>>>, List>;
    }

}}//qor::mpl

#endif//QOR_PP_H_MPL_SEQUENCE_ERASE
