// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_MPL_SEQUENCE_SIZE
#define QOR_PP_H_MPL_SEQUENCE_SIZE

#include "../functional/call.h"
#include "../functional/identity.h"
#include "../int.h"
#include "../list.h"

namespace qor { namespace mpl {

    template <typename C = identity>
    struct size 
    {
        template <typename... Ts>
        using f = typename C::template f<uint_<sizeof...(Ts)>>;
    };

    template <>
    struct size<identity> 
    {
        template <typename... Ts>
        using f = uint_<sizeof...(Ts)>;
    };

    /// offset provides the difference between the origional length of a list
    ///(provided as the input parameter) and the pack supplied to f
    template <typename T, typename C = identity>
    struct offset 
    {
        template <typename... Ts>
        using f = typename C::template f<uint_<(T::value - sizeof...(Ts))>>;
    };

    template <typename T>
    struct offset<T, identity> 
    {
        template <typename... Ts>
        using f = uint_<(T::value - sizeof...(Ts))>;
    };

    namespace eager 
    {
        /// get the size of a list, the returned type is convertible to some unspecified int type
        template <typename List>
        using size = call<unpack<mpl::size<>>, List>;
    }//eager

}}//qor::mpl

#endif//QOR_PP_H_MPL_SEQUENCE_SIZE