// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"

#include <system_error>
# include <WinSock2.h>
# include <WS2tcpip.h>
# include <MSWSock.h>
# include <Windows.h>

#include "disconnectop.h"

namespace qor { namespace io { namespace async { namespace win {

	bool socket_disconnect_operation_impl::try_start( win32_overlapped_operation_base& operation) noexcept
	{
		// Lookup the address of the DisconnectEx function pointer for this socket.
		LPFN_DISCONNECTEX disconnectExPtr;
		{
			GUID disconnectExGuid = WSAID_DISCONNECTEX;
			DWORD byteCount = 0;
			const int result = ::WSAIoctl(
				m_socket.m_socket,
				SIO_GET_EXTENSION_FUNCTION_POINTER,
				static_cast<void*>(&disconnectExGuid),
				sizeof(disconnectExGuid),
				static_cast<void*>(&disconnectExPtr),
				sizeof(disconnectExPtr),
				&byteCount,
				nullptr,
				nullptr);
			if (result == SOCKET_ERROR)
			{
				operation.m_errorCode = static_cast<DWORD>(::WSAGetLastError());
				return false;
			}
		}

		// Need to read this flag before starting the operation, otherwise
		// it may be possible that the operation will complete immediately
		// on another thread and then destroy the socket before we get a
		// chance to read it.
		const bool skipCompletionOnSuccess = true;//TODO:  m_socket.skip_completion_on_success();

		// Need to add TF_REUSE_SOCKET to these flags if we want to allow reusing
		// a socket for subsequent connections once the disconnect operation
		// completes.
		const DWORD flags = 0;

		const BOOL ok = disconnectExPtr(
			m_socket.m_socket,
			operation.get_overlapped(),
			flags,
			0);
		if (!ok)
		{
			const int errorCode = ::WSAGetLastError();
			if (errorCode != ERROR_IO_PENDING)
			{
				// Failed synchronously.
				operation.m_errorCode = static_cast<DWORD>(errorCode);
				return false;
			}
		}
		else if (skipCompletionOnSuccess)
		{
			// Successfully completed synchronously and no completion event
			// will be posted to an I/O thread so we can return without suspending.
			operation.m_errorCode = ERROR_SUCCESS;
			return false;
		}

		return true;
	}

	void socket_disconnect_operation_impl::cancel( win32_overlapped_operation_base& operation) noexcept
	{
		(void)::CancelIoEx(
			reinterpret_cast<HANDLE>(m_socket.m_handle), operation.get_overlapped());
	}

	void socket_disconnect_operation_impl::get_result( win32_overlapped_operation_base& operation)
	{
		if (operation.m_errorCode != ERROR_SUCCESS)
		{
			if (operation.m_errorCode == ERROR_OPERATION_ABORTED)
			{
				throw operation_cancelled{};
			}

			throw std::system_error{
				static_cast<int>(operation.m_errorCode),
				std::system_category(),
				"Disconnect operation failed: DisconnectEx"
			};
		}
	}

}}}}//qor::io::async::win
