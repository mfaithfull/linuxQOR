// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"

#include <string.h>
#include "when.h"

namespace qor {

    When::When()
    {
        m_timePoint = std::chrono::high_resolution_clock::now();
    }

    When::When(const When& src)
    {
        *this = src;
    }

    When::When(const ref_of<When>::type& src)
    {
        m_timePoint = src->m_timePoint;
    }

    When::~When() noexcept { }

    When& When::operator = (const When& src)
    {
        if (&src != this)
        {
            m_timePoint = src.m_timePoint;
        }
        return *this;
    }

    std::chrono::time_point<std::chrono::high_resolution_clock, std::chrono::nanoseconds> When::TimePoint(void) const
    {
        return m_timePoint;
    }

}//qor
