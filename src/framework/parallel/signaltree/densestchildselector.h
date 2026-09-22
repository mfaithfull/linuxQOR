// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_PARALLEL_SIGNALTREE_DENSESTCHILDSELECTOR
#define QOR_PP_H_PARALLEL_SIGNALTREE_DENSESTCHILDSELECTOR

#include <cstddef>
#include <cstdint>
#include <type_traits>

#include "src/platform/compiler/compiler.h"
#include "nodetraits.h"

namespace qor{ namespace par{

    class densest_child_selector
    {
    public:

        template <size_t Capacity>
        constexpr explicit densest_child_selector(std::integral_constant<size_t, Capacity>) noexcept
        {
        }

        template <branch_node_traits_concept Traits>
        qor_pp_forceinline size_t select(Traits, uint64_t /*hint*/, uint64_t counters) const noexcept
        {
            return tournament<Traits, Traits::lanes_per_node>(counters);
        }

        uint64_t mask_ = 0;

    private:

        template <typename Traits, uint64_t L>
        qor_pp_forceinline static size_t tournament(uint64_t counters) noexcept
        {
            if constexpr (L == 1)
            {
                return 0;
            }
            else
            {
                constexpr uint64_t lane_width = Traits::lane_width;
                constexpr uint64_t half_counters = L / 2;
                constexpr uint64_t bits_per_half = half_counters * lane_width;
                constexpr uint64_t high_mask = ((uint64_t{1} << bits_per_half) - 1ull) << bits_per_half;
                constexpr uint64_t lane_mask = (uint64_t{1} << lane_width) - 1ull;

                auto const left = counters & ~high_mask;
                auto const right = counters >> bits_per_half;

                uint64_t leftSum = 0;
                uint64_t rightSum = 0;

                for(size_t i = 0; i < half_counters; ++i)
                {
                    leftSum += (left >> (i * lane_width)) & lane_mask;
                    rightSum += (right >> (i * lane_width)) & lane_mask;
                }

                return (rightSum > leftSum) ? half_counters + tournament<Traits, half_counters>(right)
                        : tournament<Traits, half_counters>(left);
            }
        }
    };

}}//qor::par

#endif//QOR_PP_H_PARALLEL_SIGNALTREE_DENSESTCHILDSELECTOR
