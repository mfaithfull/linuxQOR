// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_PARALLEL_SIGNALTREE_NODETRAITS
#define QOR_PP_H_PARALLEL_SIGNALTREE_NODETRAITS

#include <bit>
#include <concepts>
#include <cstddef>
#include <cstdint>
#include <limits>

#include "lanetraits.h"

namespace qor{ namespace par{

    template <size_t Level> struct node_traits;

    template <>
    struct node_traits<0>
    {
        using value_type = uint64_t;

        static constexpr size_t level          = 0;
        static constexpr size_t node_width     = std::numeric_limits<value_type>::digits;
        static constexpr size_t lane_width     = lane_traits<0>::width;
        static constexpr size_t lane_capacity  = lane_traits<0>::capacity;
        static constexpr size_t lanes_per_node = node_width / lane_width;
        static constexpr size_t node_capacity  = lanes_per_node * lane_capacity;
        static constexpr size_t hint_width     = std::bit_width(lanes_per_node - 1ull);
        static constexpr size_t hint_offset    = 0ull;
        static constexpr size_t level_mask     = (1ull << (hint_offset + hint_width)) - 1ull;

        static constexpr bool is_leaf = true;
    };


    template <size_t Level>
    struct node_traits
    {
        using child = node_traits<Level - 1>;
        using lane = lane_traits<Level>;
        using value_type = typename child::value_type;

        static constexpr size_t level          = Level;
        static constexpr size_t node_width     = child::node_width;
        static constexpr size_t lane_width     = lane::width;
        static constexpr size_t lane_capacity  = child::node_capacity;
        static constexpr size_t lanes_per_node = node_width / lane_width;
        static constexpr size_t node_capacity  = lanes_per_node * lane_capacity;
        static constexpr size_t hint_width     = std::bit_width(lanes_per_node - 1ull);
        static constexpr size_t hint_offset    = child::hint_offset + child::hint_width;
        static constexpr size_t level_mask     = (1ull << (hint_offset + hint_width)) - 1ull;

        static constexpr bool is_leaf = false;
    };


    template <typename T>
    concept node_traits_concept = std::same_as<T, node_traits<T::level>>;


    template <typename T>
    concept leaf_node_traits_concept = node_traits_concept<T> && T::is_leaf;


    template <typename T>
    concept branch_node_traits_concept = node_traits_concept<T> && (not T::is_leaf);


}}//qor::par

#endif//QOR_PP_H_PARALLEL_SIGNALTREE_NODETRAITS
