// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_PARALLEL_SIGNALTREE_LANETRAITS
#define QOR_PP_H_PARALLEL_SIGNALTREE_LANETRAITS

#include <stddef.h>
#include <bit>

namespace qor { namespace par {

    template <size_t> struct node_traits;
    template <size_t> struct lane_traits;
    
    template <>
    struct lane_traits<0>
    {
        static constexpr size_t width    = 1;
        static constexpr size_t capacity = (1ull << width) - 1ull;
    };


    template <size_t Level>
    struct lane_traits
    {
        using child_node = node_traits<Level - 1>;

        static constexpr size_t width = std::bit_ceil(static_cast<unsigned>(std::bit_width(child_node::node_capacity)));
        static constexpr size_t capacity = (1ull << width) - 1ull;
    };

}}//qor::par

#endif//QOR_PP_H_PARALLEL_SIGNALTREE_LANETRAITS
