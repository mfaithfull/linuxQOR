// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"

#include <system_error>
#include "autoresetevent.h"
#include "src/platform/os/windows/api_layer/kernel/kernel32.h"

namespace qor { namespace io { namespace async { namespace win {

    AutoResetEvent::AutoResetEvent() : m_handle(::CreateEventW(nullptr, FALSE, FALSE, nullptr))
    {
		if (m_handle.IsInvalid())
		{
			const DWORD errorCode = qor::win::api::Kernel32::GetLastError();
			throw std::system_error
			{
				static_cast<int>(errorCode),
				std::system_category(),
				"Error creating manual reset event: CreateEventW"
			};
		}
    }

    AutoResetEvent::~AutoResetEvent() = default;

    const platform::win::Handle& AutoResetEvent::Handle()
    {
        return m_handle;
    }

    bool AutoResetEvent::Set()
    {
        return ::SetEvent(m_handle.Use());
    }

}}}}//qor::io::async::win
