// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_OS_WINDOWS_FRAMEWORK_ASYNCIOSERVICE_IOCP_RECVOP
#define QOR_PP_H_OS_WINDOWS_FRAMEWORK_ASYNCIOSERVICE_IOCP_RECVOP

#include <system_error>
#include <coroutine>
#include "overlappedoperationcancellable.h"
#include "overlappedoperation.h"
#include "src/framework/parallel/task/cancellation/cancellationtoken.h"
#include "src/framework/io/network/socket.h"

namespace qor { namespace io { namespace async { namespace win {

	class SocketRecvOperationImpl
	{
	public:

		SocketRecvOperationImpl(io::Descriptor* s,void* buffer,std::size_t byteCount) noexcept
			: m_socket(s)
			, m_buffer(buffer, byteCount)
		{ }

		bool try_start(win32_overlapped_operation_base& operation) noexcept;
		void cancel(win32_overlapped_operation_base& operation) noexcept;

	private:

		io::Descriptor* m_socket;
		wsabuf m_buffer;

	};

	class SocketRecvOperation : public win32_overlapped_operation<SocketRecvOperation>
	{
		friend class win32_overlapped_operation<SocketRecvOperation>;

	public:

		SocketRecvOperation(io::Descriptor* s,void* buffer,std::size_t byteCount) noexcept
			: m_impl(s, buffer, byteCount)
		{ }

		virtual ~SocketRecvOperation() = default;

	private:

		bool try_start() noexcept { return m_impl.try_start(*this); }

		SocketRecvOperationImpl m_impl;

	};

	class socket_recv_operation_cancellable : public win32_overlapped_operation_cancellable<socket_recv_operation_cancellable>
	{
		friend class win32_overlapped_operation_cancellable<socket_recv_operation_cancellable>;

	public:

		socket_recv_operation_cancellable(
			io::Descriptor* s,
			void* buffer,
			std::size_t byteCount,
			CancellationToken&& ct) noexcept
			: win32_overlapped_operation_cancellable<socket_recv_operation_cancellable>(std::move(ct))
			, m_impl(s, buffer, byteCount)
		{ }

	private:

		bool try_start() noexcept { return m_impl.try_start(*this); }
		void cancel() noexcept { m_impl.cancel(*this); }

		SocketRecvOperationImpl m_impl;

	};

}}}}//qor::io::async::win

#endif//QOR_PP_H_OS_WINDOWS_FRAMEWORK_ASYNCIOSERVICE_IOCP_RECVOP
