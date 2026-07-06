// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"

#include <system_error>
#include "waitabletimerevent.h"
#include "src/platform/os/windows/api_layer/kernel/kernel32.h"

namespace qor { namespace io { namespace async { namespace win {

    WaitableTimerEvent::WaitableTimerEvent() : m_handle(::CreateWaitableTimerW(nullptr, 0, nullptr))
    {
		if (m_handle.IsInvalid())
		{
			const DWORD errorCode = ::GetLastError();
			throw std::system_error
			{
				static_cast<int>(errorCode),
				std::system_category()
			};
		}
    }

    WaitableTimerEvent::~WaitableTimerEvent() = default;

    const platform::win::Handle& WaitableTimerEvent::Handle()
    {
        return m_handle;
    }

    bool WaitableTimerEvent::Set( platform::win::LARGE_INTEGER dueTime, long period, bool resumeFromSuspend)
    {
        return ::SetWaitableTimer(
            m_handle.Use(),
            reinterpret_cast<LARGE_INTEGER*>(&dueTime),
            period,
            nullptr,
            nullptr,
            resumeFromSuspend ? 1 : 0);
    }

}}}}//qor::io::async::win
