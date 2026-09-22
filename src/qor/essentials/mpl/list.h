
// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_MPL_LIST
#define QOR_PP_H_MPL_LIST

#include "functional/bind.h"

namespace qor { namespace mpl {

    template <typename... Ts>
    struct list {};

    namespace detail 
    {
        // recursive list, for internal use only
        template <typename Head, typename Tail>
        struct rlist {};

        using rlist_tail_of8 = rlist<
                list<>,
                rlist<list<>,
                        rlist<list<>,
                            rlist<list<>,
                                    rlist<list<>,
                                        rlist<list<>, rlist<list<>, rlist<list<>, void>>>>>>>>;
    }//detail

    using listify = cfe<list>;

    template <typename S>
    struct sequencify;

    template <template <typename...> class S, typename... Ts>
    struct sequencify<S<Ts...>> 
    {
        using type = cfe<S>;
    };

    template <typename... Ts>
    list<Ts...> make_list(Ts...) 
    {
        return {};
    }

}}//qor::mpl

#endif//QOR_PP_H_MPL_LIST