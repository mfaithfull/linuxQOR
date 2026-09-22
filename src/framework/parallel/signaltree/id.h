// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_PARALLEL_SIGNALTREE_ID
#define QOR_PP_H_PARALLEL_SIGNALTREE_ID

#include <compare>
#include <cstddef>
#include <cstdint>
#include <functional>
#include <limits>

#include "nodetraits.h"
#include "processsharing.h"
#include "detail/synchronizationmode.h"

namespace qor{ namespace par{

    template <size_t tree_depth, synchronization_mode mode, process_sharing sharing> class signal_set;

    class signal_id
    {
    public:

        using value_type = uint64_t;

        constexpr signal_id() noexcept = default;
        constexpr explicit signal_id(value_type value) noexcept : value_(value) {}

        constexpr bool valid() const noexcept { return value_ != std::numeric_limits<value_type>::max(); }
        constexpr explicit operator value_type() const noexcept { return value_; }
        constexpr auto operator <=>(signal_id const &) const noexcept = default;

        static constexpr signal_id invalid() noexcept { return {}; }

    private:

        template <size_t N>
        constexpr size_t lane() const noexcept
        {
            using traits = node_traits<N>;
            constexpr auto mask = (value_type{1} << traits::hint_width) - value_type{1};
            return (value_ >> traits::hint_offset) & mask;
        }

        template <size_t N>
        constexpr signal_id with_lane(size_t lane) const noexcept
        {
            using traits = node_traits<N>;
            constexpr auto mask = traits::level_mask;
            return signal_id{(value_ & ~mask) | (lane << traits::hint_offset)};
        }

        value_type value_ = std::numeric_limits<value_type>::max();

        template <size_t, synchronization_mode, process_sharing> friend class signal_set;
    };

}}//qor::par

template <>
struct std::hash<qor::par::signal_id>
{
    size_t operator()(qor::par::signal_id id) const noexcept
    {
        return std::hash<qor::par::signal_id::value_type>{}(static_cast<qor::par::signal_id::value_type>(id));
    }
};

#endif//QOR_PP_H_PARALLEL_SIGNALTREE_ID