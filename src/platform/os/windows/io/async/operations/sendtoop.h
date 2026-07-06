// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_OS_WINDOWS_FRAMEWORK_ASYNCIOSERVICE_IOCP_SENDTOOP
#define QOR_PP_H_OS_WINDOWS_FRAMEWORK_ASYNCIOSERVICE_IOCP_SENDTOOP

#include <system_error>
#include <coroutine>
#include "overlappedoperationcancellable.h"
#include "overlappedoperation.h"
#include "src/framework/parallel/task/cancellation/cancellationtoken.h"
#include "src/framework/io/network/socket.h"
#include "src/framework/io/network/ipendpoint.h"

namespace qor { namespace io { namespace async { namespace win {

	class socket_send_to_operation_impl
	{
	public:

		socket_send_to_operation_impl( io::network::Socket& s,
			const io::network::IPEndpoint& destination,
			const void* buffer,
			std::size_t byteCount) noexcept
			: m_socket(s)
			, m_destination(destination)
			, m_buffer(const_cast<void*>(buffer), byteCount)
		{ }

		bool try_start(win32_overlapped_operation_base& operation) noexcept;
		void cancel(win32_overlapped_operation_base& operation) noexcept;

	private:

		io::network::Socket& m_socket;
		io::network::IPEndpoint m_destination;
		wsabuf m_buffer;

	};

	class socket_send_to_operation : public win32_overlapped_operation<socket_send_to_operation>
	{
		friend class win32_overlapped_operation<socket_send_to_operation>;

	public:

		socket_send_to_operation(
			io::network::Socket& s,
			const io::network::IPEndpoint& destination,
			const void* buffer,
			std::size_t byteCount) noexcept
			: m_impl(s, destination, buffer, byteCount){ }

	private:

		bool try_start() noexcept { return m_impl.try_start(*this); }

		socket_send_to_operation_impl m_impl;

	};

	class socket_send_to_operation_cancellable : public win32_overlapped_operation_cancellable<socket_send_to_operation_cancellable>
	{
		friend class win32_overlapped_operation_cancellable<socket_send_to_operation_cancellable>;

	public:

		socket_send_to_operation_cancellable(
			io::network::Socket& s,
			const io::network::IPEndpoint& destination,
			const void* buffer,
			std::size_t byteCount,
			CancellationToken&& ct) noexcept
			: win32_overlapped_operation_cancellable<socket_send_to_operation_cancellable>(std::move(ct))
			, m_impl(s, destination, buffer, byteCount){ }

	private:

		bool try_start() noexcept { return m_impl.try_start(*this); }
		void cancel() noexcept { return m_impl.cancel(*this); }

		socket_send_to_operation_impl m_impl;

	};

}}}}//qor::io::async::win

#endif//QOR_PP_H_OS_WINDOWS_FRAMEWORK_ASYNCIOSERVICE_IOCP_SENDTOOP
