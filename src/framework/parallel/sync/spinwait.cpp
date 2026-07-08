// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"

#include <thread>

#include "spinwait.h"

namespace{ namespace local {
        constexpr std::uint32_t yield_threshold = 10;
}}//::local
    
namespace qor{

    SpinWait::SpinWait() noexcept
    {
        Reset();
    }

    bool SpinWait::NextSpinWillYield() const noexcept
    {
        return m_count >= local::yield_threshold;
    }

    void SpinWait::Reset() noexcept
    {
        static const std::uint32_t initialCount = std::thread::hardware_concurrency() > 1 ? 0 : local::yield_threshold;
        m_count = initialCount;
    }

    void SpinWait::SpinOne() noexcept
    {
        if (NextSpinWillYield())
        {
            std::this_thread::yield();
        }

        ++m_count;
        if (m_count == 0)
        {
            // Don't wrap around to zero as this would go back to
            // busy-waiting.
            m_count = local::yield_threshold;
        }
    }

}//qor
