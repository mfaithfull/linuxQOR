// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_SYNC_LIGHTWEIGHTMANUALRESETEVENT
#define QOR_PP_H_SYNC_LIGHTWEIGHTMANUALRESETEVENT

# include <mutex>
# include <condition_variable>

//prefer a platform specific variant over this not so lightweight platfrom generic version

namespace qor{ namespace detail{

    class qor_pp_module_interface(QOR_SYNC) LightManualResetEvent
    {
    public:

        LightManualResetEvent() noexcept;
        LightManualResetEvent(bool initiallySet) noexcept;

        ~LightManualResetEvent();

        void Set() noexcept;
        void Reset() noexcept;
        void Wait() noexcept;

    private:

        // For platforms that don't have a native futex
        // or manual reset event we can just use a std::mutex
        // and std::condition_variable to perform the wait.
        // Not so lightweight, but should be portable to all platforms.
        std::mutex m_mutex;
        std::condition_variable m_cv;
        bool m_isSet;

    };

}}//qor::detail

#endif//QOR_PP_H_SYNC_LIGHTWEIGHTMANUALRESETEVENT
