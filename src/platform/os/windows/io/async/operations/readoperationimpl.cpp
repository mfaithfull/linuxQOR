// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"
#include "readop.h"

#include "src/platform/os/windows/api_layer/kernel/kernel32.h"
#include <system_error>

namespace qor { namespace io { namespace async { namespace win {

	bool ReadOperationImpl::try_start(win32_overlapped_operation_base& operation) noexcept
	{
        const DWORD numberOfBytesToRead = m_buffer.len <= 0xFFFFFFFF ? static_cast<DWORD>(m_buffer.len) : DWORD(0xFFFFFFFF);

		DWORD numberOfBytesRead = 0;
        BOOL ok = qor::win::api::Kernel32::ReadFile(
            m_file->m_handle,
            m_buffer.buf,
            numberOfBytesToRead,
            &numberOfBytesRead,
            operation.get_overlapped());
        DWORD errorCode = ok ? ERROR_SUCCESS : qor::win::api::Kernel32::GetLastError();
		if (errorCode != ERROR_IO_PENDING)
		{
            // Completed synchronously.
            //
            // We are assuming that the file-handle has been set to the
            // mode where synchronous completions do not post a completion
            // event to the I/O completion port and thus can return without
            // suspending here.

            operation.m_errorCode = errorCode;
            operation.m_numberOfBytesTransferred = numberOfBytesRead;

            return false;
		}

		// Operation will complete asynchronously.
		return true;
	}

	void ReadOperationImpl::cancel(win32_overlapped_operation_base& operation) noexcept
	{
        qor::win::api::Kernel32::CancelIoEx(reinterpret_cast<HANDLE>(m_file->m_handle), operation.get_overlapped());
	}

}}}}//qor::io::async::win
