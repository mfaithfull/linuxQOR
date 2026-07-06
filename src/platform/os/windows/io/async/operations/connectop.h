// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_OS_WINDOWS_FRAMEWORK_ASYNCIOSERVICE_IOCP_CONNECTOP
#define QOR_PP_H_OS_WINDOWS_FRAMEWORK_ASYNCIOSERVICE_IOCP_CONNECTOP

#include <system_error>
#include <coroutine>
#include "overlappedoperationcancellable.h"
#include "overlappedoperation.h"
#include "src/framework/parallel/task/cancellation/cancellationtoken.h"
#include "src/framework/io/network/socket.h"
#include "src/framework/io/network/ipendpoint.h"

namespace qor { namespace io {namespace async { namespace win {

	class socket_connect_operation_impl
	{
	public:

		socket_connect_operation_impl( io::network::Socket& socket, const io::network::IPEndpoint& remoteEndPoint) noexcept
			: m_socket(socket)
			, m_remoteEndPoint(remoteEndPoint)
		{ }

		bool try_start(win32_overlapped_operation_base& operation) noexcept;
		void cancel(win32_overlapped_operation_base& operation) noexcept;
		void get_result(win32_overlapped_operation_base& operation);

	private:

		io::network::Socket& m_socket;
		io::network::IPEndpoint m_remoteEndPoint;

	};

	class socket_connect_operation : public win32_overlapped_operation<socket_connect_operation>
	{
		friend class win32_overlapped_operation<socket_connect_operation>;

	public:

		socket_connect_operation( io::network::Socket& socket, const io::network::IPEndpoint& remoteEndPoint) noexcept
			: m_impl(socket, remoteEndPoint)
		{ }

	private:

		bool try_start() noexcept { return m_impl.try_start(*this); }
		decltype(auto) get_result() { return m_impl.get_result(*this); }

		socket_connect_operation_impl m_impl;

	};

	class socket_connect_operation_cancellable : public win32_overlapped_operation_cancellable<socket_connect_operation_cancellable>
	{
		friend class win32_overlapped_operation_cancellable<socket_connect_operation_cancellable>;

	public:

		socket_connect_operation_cancellable( io::network::Socket& socket, const io::network::IPEndpoint& remoteEndPoint, CancellationToken&& ct) noexcept
			: win32_overlapped_operation_cancellable<socket_connect_operation_cancellable>(std::move(ct))
			, m_impl(socket, remoteEndPoint)
		{ }

	private:

		bool try_start() noexcept { return m_impl.try_start(*this); }
		void cancel() noexcept { m_impl.cancel(*this); }
		void get_result() { m_impl.get_result(*this); }

		socket_connect_operation_impl m_impl;

	};

}}}}//qor::io::async::win

#endif//QOR_PP_H_OS_WINDOWS_FRAMEWORK_ASYNCIOSERVICE_IOCP_CONNECTOP
