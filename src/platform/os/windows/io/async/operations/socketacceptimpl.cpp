// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"

#include <system_error>
#include "acceptop.h"

#include "src/platform/os/windows/api_layer/winsock2/ws2.h"
#include "src/platform/os/windows/api_layer/kernel/kernel32.h"

using namespace qor::win::api;

namespace qor { namespace io { namespace async { namespace win {

	bool SocketAcceptOperationImpl::try_start( win32_overlapped_operation_base& operation) noexcept
	{
		static_assert(
			(sizeof(m_addressBuffer) / 2) >= (16 + sizeof(SOCKADDR_IN)) &&
			(sizeof(m_addressBuffer) / 2) >= (16 + sizeof(SOCKADDR_IN6)),
			"AcceptEx requires address buffer to be at least 16 bytes more than largest address.");

		// Need to read this flag before starting the operation, otherwise
		// it may be possible that the operation will complete immediately
		// on another thread and then destroy the socket before we get a
		// chance to read it.
		const bool skipCompletionOnSuccess = true;//TODO: m_listeningSocket.skip_completion_on_success();

		DWORD bytesReceived = 0;
		BOOL ok = WS2::AcceptEx(
			m_listeningSocket->m_socket,
			m_acceptingSocket->m_socket,
			m_addressBuffer,
			0,
			sizeof(m_addressBuffer) / 2,
			sizeof(m_addressBuffer) / 2,
			&bytesReceived,
			operation.get_overlapped());

		if (!ok)
		{
			int errorCode = WS2::WSAGetLastError();
			if (errorCode != ERROR_IO_PENDING)
			{
				operation.m_errorCode = static_cast<DWORD>(errorCode);
				return false;
			}
		}
		else if (skipCompletionOnSuccess)
		{
			operation.m_errorCode = ERROR_SUCCESS;
			return false;
		}

		return true;
	}

	void SocketAcceptOperationImpl::cancel( win32_overlapped_operation_base& operation) noexcept
	{
		(void)Kernel32::CancelIoEx( reinterpret_cast<HANDLE>(m_listeningSocket->m_handle), operation.get_overlapped());
	}

	int SocketAcceptOperationImpl::get_result( win32_overlapped_operation_base& operation)
	{
		if (operation.m_errorCode != ERROR_SUCCESS)
		{
			throw std::system_error{
				static_cast<int>(operation.m_errorCode),
				std::system_category(),
				"Accepting a connection failed: AcceptEx"
			};
		}

		sockaddr* localSockaddr = nullptr;
		sockaddr* remoteSockaddr = nullptr;

		INT localSockaddrLength;
		INT remoteSockaddrLength;

		WS2::GetAcceptExSockaddrs(
			m_addressBuffer,
			0,
			sizeof(m_addressBuffer) / 2,
			sizeof(m_addressBuffer) / 2,
			&localSockaddr,
			&localSockaddrLength,
			&remoteSockaddr,
			&remoteSockaddrLength);

		//TODO:m_acceptingSocket.m_localEndPoint = sockaddr_to_ip_endpoint(*localSockaddr);
		//TODO:m_acceptingSocket.m_remoteEndPoint = sockaddr_to_ip_endpoint(*remoteSockaddr);

		{
			// Need to set SO_UPDATE_ACCEPT_CONTEXT after the accept completes
			// to ensure that ::shutdown() and ::setsockopt() calls work on the
			// accepted socket.
			SOCKET listenSocket = m_listeningSocket->m_socket;
			const int result = WS2::setsockopt(
				m_acceptingSocket->m_socket,
				SOL_SOCKET,
				SO_UPDATE_ACCEPT_CONTEXT,
				(const char*)&listenSocket,
				sizeof(SOCKET));

			if (result == SOCKET_ERROR)
			{
				const int errorCode = WS2::WSAGetLastError();
				throw std::system_error{
					errorCode,
					std::system_category(),
					"Socket accept operation failed: setsockopt(SO_UPDATE_ACCEPT_CONTEXT)"
				};
			}
		}
		return operation.m_errorCode;
	}

}}}}//qor::io::async::win
