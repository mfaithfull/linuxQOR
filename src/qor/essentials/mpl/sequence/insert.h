// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_MPL_SEQUENCE_INSERT
#define QOR_PP_H_MPL_SEQUENCE_INSERT

#include "../algorithm/rotate.h"
#include "../functional/call.h"
#include "pushfront.h"
#include "../int.h"

namespace qor { namespace mpl {

    template <typename Index, typename Input, typename C = listify>
    struct insert 
    {
        template <typename... Ts>
        using f = typename rotate<Index, push_front<Input, rotate<uint_<(1 + sizeof...(Ts)-Index::value)>, C>>>::template f<Ts...>;
    };

    namespace eager 
    {
        template <typename List, unsigned Index, typename Input>
        using insert = call<unpack<insert<uint_<Index>, Input>>, List>;
    }

}}//qor::mpl

#endif//QOR_PP_H_MPL_SEQUENCE_INSERT