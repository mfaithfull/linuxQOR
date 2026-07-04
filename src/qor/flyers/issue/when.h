// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_ISSUE_WHEN
#define QOR_PP_H_ISSUE_WHEN

#include <chrono>
#include "src/qor/memory/reference/reference.h"

//When captures the TimePoint when an issue is raised

namespace qor{ 

    class qor_pp_module_interface(QOR_ISSUE) When
    {
    public:
        When();
        When(const When&);
        When(const ref_of<When>::type&);
        When& operator = (const When&);
        virtual ~When() noexcept = default;

        std::chrono::time_point<std::chrono::high_resolution_clock, std::chrono::nanoseconds> TimePoint() const;

    private:
        std::chrono::time_point<std::chrono::high_resolution_clock, std::chrono::nanoseconds> m_timePoint;
    };

}//qor

#endif//QOR_PP_H_ISSUE_WHEN
