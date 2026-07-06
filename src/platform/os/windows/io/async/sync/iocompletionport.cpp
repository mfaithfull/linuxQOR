// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"

#include <system_error>
#include "iocompletionport.h"
#include "src/framework/io/iodescriptor.h"
#include "src/platform/os/windows/api_layer/kernel/kernel32.h"

namespace qor { namespace io { namespace async { namespace win {

    IOCompletionPort::IOCompletionPort(std::uint32_t concurrencyHint) : m_handle(::CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, concurrencyHint))
    {
		if (m_handle.IsInvalid())
		{
			DWORD errorCode = ::GetLastError();
			throw std::system_error
			{
				static_cast<int>(errorCode),
				std::system_category(),
				"Error creating io_service: CreateIoCompletionPort"
			};
		}
    }

    IOCompletionPort::~IOCompletionPort() = default;

    bool IOCompletionPort::PostQueuedCompletionStatus(unsigned long numberOfBytesTransferred, unsigned long long completionKey, overlapped* pOverlapped)
    {
        return ::PostQueuedCompletionStatus(m_handle.Use(), numberOfBytesTransferred, completionKey, reinterpret_cast<LPOVERLAPPED>(pOverlapped));
    }

    bool IOCompletionPort::GetQueuedCompletionStatus(unsigned long& numberOfBytesTransferred, unsigned long long& completionKey, overlapped*& pOverlapped, unsigned long timeoutms)
    {
        return ::GetQueuedCompletionStatus(m_handle.Use(), &numberOfBytesTransferred, &completionKey, reinterpret_cast<LPOVERLAPPED*>(&pOverlapped), timeoutms);
    }

    bool IOCompletionPort::Enroll(io::Descriptor& ioDescriptor) const
    {
        const HANDLE result = ::CreateIoCompletionPort((HANDLE)ioDescriptor.m_handle, m_handle.Use(), 0, 0);

        if (result == nullptr)
		{
		    const DWORD errorCode = ::GetLastError();
			throw std::system_error( static_cast<int>(errorCode), std::system_category(), "Error creating io completion port: Enroll");
		}
        return true;
    }

    const platform::win::Handle& IOCompletionPort::Handle()
    {
        return m_handle;
    }

}}}}//qor::io::async::win
