// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"

#include <system_error>
# include <WinSock2.h>
# include <WS2tcpip.h>
# include <MSWSock.h>
# include <Windows.h>

#include "sendtoop.h"
#include "src/platform/os/windows/io/network/addresshelpers.h"

namespace qor { namespace io { namespace async { namespace win {

	bool socket_send_to_operation_impl::try_start( win32_overlapped_operation_base& operation) noexcept
	{
		// Need to read this flag before starting the operation, otherwise
		// it may be possible that the operation will complete immediately
		// on another thread and then destroy the socket before we get a
		// chance to read it.
		const bool skipCompletionOnSuccess = true;//TODO: m_socket.skip_completion_on_success();

		SOCKADDR_STORAGE destinationAddress;
		const int destinationLength = qor::io::network::win::ip_endpoint_to_sockaddr(m_destination, std::ref(destinationAddress));

		DWORD numberOfBytesSent = 0;
		int result = ::WSASendTo(
			m_socket.m_socket,
			reinterpret_cast<WSABUF*>(&m_buffer),
			1, // buffer count
			&numberOfBytesSent,
			0, // flags
			reinterpret_cast<const SOCKADDR*>(&destinationAddress),
			destinationLength,
			operation.get_overlapped(),
			nullptr);
		if (result == SOCKET_ERROR)
		{
			int errorCode = ::WSAGetLastError();
			if (errorCode != WSA_IO_PENDING)
			{
				// Failed synchronously.
				operation.m_errorCode = static_cast<DWORD>(errorCode);
				operation.m_numberOfBytesTransferred = numberOfBytesSent;
				return false;
			}
		}
		else if (skipCompletionOnSuccess)
		{
			// Completed synchronously, no completion event will be posted to the IOCP.
			operation.m_errorCode = ERROR_SUCCESS;
			operation.m_numberOfBytesTransferred = numberOfBytesSent;
			return false;
		}

		// Operation will complete asynchronously.
		return true;
	}

	void socket_send_to_operation_impl::cancel(win32_overlapped_operation_base& operation) noexcept
	{
		(void)::CancelIoEx(
			reinterpret_cast<HANDLE>(m_socket.m_handle),
			operation.get_overlapped());
	}

}}}}//qor::io::async::win
