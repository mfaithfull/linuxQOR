// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_MPL_SEQUENCE_MAKESEQUENCE
#define QOR_PP_H_MPL_SEQUENCE_MAKESEQUENCE

#include "../functional/call.h"
#include "../int.h"
#include "../list.h"

namespace qor { namespace mpl { namespace detail {

        constexpr unsigned next_number(unsigned current, unsigned end) 
        {
            return ((end - 2 * current) < 2) ?
                            end :
                            next_number(current, end / 2); // note that unsigned/2 always rounds down
        }

        constexpr unsigned next_state(unsigned current, unsigned end) 
        {
            return ((end - current) < 2) ? end - current :
                                            (2 + (next_number(current, end) - 2 * current));
        }

        template <unsigned State, typename C>
        struct make_seq_impl;

        template <typename C>
        struct make_seq_impl<0, C> 
        { // done
            template <unsigned End, unsigned... Is>
            using f = typename C::template f<uint_<Is>...>;
        };

        template <typename C>
        struct make_seq_impl<1, C> 
        { // done +1
            template <unsigned End, unsigned... Is>
            using f = typename C::template f<uint_<Is>..., uint_<End - 1>>;
        };

        template <typename C>
        struct make_seq_impl<2, C> 
        { // double
            template <unsigned End, unsigned... Is>
            using f = typename make_seq_impl<next_state(2 * sizeof...(Is), End),
                                            C>::template f<End, Is..., (Is + sizeof...(Is))...>;
        };

        template <typename C>
        struct make_seq_impl<3, C> 
        { // double +1
            template <unsigned End, unsigned... Is>
            using f = typename make_seq_impl<next_state(2 * sizeof...(Is) + 1, End),
                                                C>::template f<End, Is..., (Is + sizeof...(Is))...,
                                                            (2 * sizeof...(Is))>;
        };

    } // namespace detail

    //creates a sequence of uint_
    template <typename C = listify>
    struct make_int_sequence 
    {
        template <typename N>
        using f = typename detail::make_seq_impl<detail::next_state(0, N::value), C>::template f<N::value>;
    };

    namespace eager 
    {
        template <typename N>
        using make_int_sequence = call<make_int_sequence<>, N>;
    }
}}//qor::mpl

#endif//QOR_PP_H_MPL_SEQUENCE_MAKESEQUENCE
