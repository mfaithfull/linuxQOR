// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_OS_WINDOWS_FRAMEWORK_ASYNCIOSERVICE_IOCP_RECVFROMOP
#define QOR_PP_H_OS_WINDOWS_FRAMEWORK_ASYNCIOSERVICE_IOCP_RECVFROMOP

#include <system_error>
#include <coroutine>
#include <tuple>
#include "overlappedoperationcancellable.h"
#include "overlappedoperation.h"
#include "src/framework/parallel/task/cancellation/cancellationtoken.h"
#include "src/framework/io/network/socket.h"
#include "src/framework/io/network/ipendpoint.h"

namespace qor { namespace io { namespace async { namespace win {

	class socket_recv_from_operation_impl
	{
	public:

		socket_recv_from_operation_impl( io::network::Socket& socket, void* buffer, std::size_t byteCount) noexcept
			: m_socket(socket)
			, m_buffer(buffer, byteCount)
		{ }

		bool try_start(win32_overlapped_operation_base& operation) noexcept;
		void cancel(win32_overlapped_operation_base& operation) noexcept;
		std::tuple<std::size_t, io::network::IPEndpoint> get_result(win32_overlapped_operation_base& operation);

	private:

		io::network::Socket& m_socket;
		wsabuf m_buffer;

		static constexpr std::size_t sockaddrStorageAlignment = 4;

		// Storage suitable for either SOCKADDR_IN or SOCKADDR_IN6
		alignas(sockaddrStorageAlignment) std::uint8_t m_sourceSockaddrStorage[28];
		int m_sourceSockaddrLength;

	};

	class socket_recv_from_operation
		: public win32_overlapped_operation<socket_recv_from_operation>
	{
		friend class win32_overlapped_operation<socket_recv_from_operation>;

	public:

		socket_recv_from_operation(
			io::network::Socket& socket,
			void* buffer,
			std::size_t byteCount) noexcept
			: m_impl(socket, buffer, byteCount)
		{
		}

	private:

		bool try_start() noexcept { return m_impl.try_start(*this); }
		decltype(auto) get_result() { return m_impl.get_result(*this); }

		socket_recv_from_operation_impl m_impl;

	};

	class socket_recv_from_operation_cancellable : public win32_overlapped_operation_cancellable<socket_recv_from_operation_cancellable>
	{
		friend class win32_overlapped_operation_cancellable<socket_recv_from_operation_cancellable>;

	public:

		socket_recv_from_operation_cancellable(
			io::network::Socket& socket,
			void* buffer,
			std::size_t byteCount,
			CancellationToken&& ct) noexcept
			: win32_overlapped_operation_cancellable<socket_recv_from_operation_cancellable>(std::move(ct))
			, m_impl(socket, buffer, byteCount)
		{
		}

	private:

		bool try_start() noexcept { return m_impl.try_start(*this); }
		void cancel() noexcept { m_impl.cancel(*this); }
		decltype(auto) get_result() { return m_impl.get_result(*this); }

		socket_recv_from_operation_impl m_impl;

	};

}}}}//qor::io::async::win

#endif//QOR_PP_H_OS_WINDOWS_FRAMEWORK_ASYNCIOSERVICE_IOCP_RECVFROMOP
