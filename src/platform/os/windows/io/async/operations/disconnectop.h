// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_OS_WINDOWS_FRAMEWORK_ASYNCIOSERVICE_IOCP_DISCONNECTOP
#define QOR_PP_H_OS_WINDOWS_FRAMEWORK_ASYNCIOSERVICE_IOCP_DISCONNECTOP

#include <system_error>
#include <coroutine>
#include "overlappedoperationcancellable.h"
#include "overlappedoperation.h"
#include "src/framework/parallel/task/cancellation/cancellationtoken.h"
#include "src/framework/io/network/socket.h"

namespace qor { namespace io {namespace async { namespace win {

	class socket_disconnect_operation_impl
	{
	public:

		socket_disconnect_operation_impl(io::network::Socket& socket) noexcept
			: m_socket(socket)
		{ }

		bool try_start(win32_overlapped_operation_base& operation) noexcept;
		void cancel(win32_overlapped_operation_base& operation) noexcept;
		void get_result(win32_overlapped_operation_base& operation);

	private:

		io::network::Socket& m_socket;

	};

	class socket_disconnect_operation : public win32_overlapped_operation<socket_disconnect_operation>
	{
		friend class win32_overlapped_operation<socket_disconnect_operation>;

	public:

		socket_disconnect_operation(io::network::Socket& socket) noexcept : m_impl(socket)
		{ }

	private:

		bool try_start() noexcept { return m_impl.try_start(*this); }
		void get_result() { m_impl.get_result(*this); }

		socket_disconnect_operation_impl m_impl;

	};

	class socket_disconnect_operation_cancellable : public win32_overlapped_operation_cancellable<socket_disconnect_operation_cancellable>
	{
		friend class win32_overlapped_operation_cancellable<socket_disconnect_operation_cancellable>;

	public:

		socket_disconnect_operation_cancellable(io::network::Socket& socket, CancellationToken&& ct) noexcept
			: win32_overlapped_operation_cancellable<socket_disconnect_operation_cancellable>(std::move(ct))
			, m_impl(socket)
		{ }

	private:

		bool try_start() noexcept { return m_impl.try_start(*this); }
		void cancel() noexcept { m_impl.cancel(*this); }
		void get_result() { m_impl.get_result(*this); }

		socket_disconnect_operation_impl m_impl;

	};

}}}}//qor::io::async::win

#endif//QOR_PP_H_OS_WINDOWS_FRAMEWORK_ASYNCIOSERVICE_IOCP_DISCONNECTOP
