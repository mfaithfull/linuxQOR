// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_OS_WINDOWS_FRAMEWORK_ASYNCIOSERVICE_WAITABLETIMEREVENT
#define QOR_PP_H_OS_WINDOWS_FRAMEWORK_ASYNCIOSERVICE_WAITABLETIMEREVENT

#include "src/platform/os/windows/common/handles/handle.h"

#include <cstdint>

#include "../sync/overlapped.h"
#include "src/platform/os/windows/common/structures.h"

namespace qor { namespace io { namespace async { namespace win {

	class WaitableTimerEvent
    {
    public:

        WaitableTimerEvent();
        ~WaitableTimerEvent();

        const platform::win::Handle& Handle();

        bool Set( platform::win::LARGE_INTEGER dueTime, long period, bool resumeFromSuspend);

    private:

        platform::win::Handle m_handle;
    };

}}}}//qor::io::async::win

#endif//QOR_PP_H_OS_WINDOWS_FRAMEWORK_ASYNCIOSERVICE_WAITABLETIMEREVENT
