// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_OS_WINDOWS_FRAMEWORK_ASYNCIOSERVICE_IOCP_SENDOP
#define QOR_PP_H_OS_WINDOWS_FRAMEWORK_ASYNCIOSERVICE_IOCP_SENDOP

#include <system_error>
#include <coroutine>
#include "overlappedoperationcancellable.h"
#include "overlappedoperation.h"
#include "src/framework/parallel/task/cancellation/cancellationtoken.h"
#include "src/framework/io/network/socket.h"

namespace qor { namespace io { namespace async { namespace win {

	class SocketSendOperationImpl
	{
	public:

		SocketSendOperationImpl(io::Descriptor* s,
			const void* buffer,
			std::size_t byteCount) noexcept
			: m_socket(s)
			, m_buffer(const_cast<void*>(buffer), byteCount)
		{ }

		bool try_start(win32_overlapped_operation_base& operation) noexcept;
		void cancel(win32_overlapped_operation_base& operation) noexcept;

	private:

		io::Descriptor* m_socket;
		wsabuf m_buffer;

	};

	class SocketSendOperation : public win32_overlapped_operation<SocketSendOperation>
	{
		friend class win32_overlapped_operation<SocketSendOperation>;

	public:

		SocketSendOperation(io::Descriptor* s, const void* buffer, std::size_t byteCount) noexcept
			: m_impl(s, buffer, byteCount)
		{
		}

	private:

		bool try_start() noexcept { return m_impl.try_start(*this); }

		SocketSendOperationImpl m_impl;

	};

	class SocketSendOperationCancellable : public win32_overlapped_operation_cancellable<SocketSendOperationCancellable>
	{
		friend class win32_overlapped_operation_cancellable<SocketSendOperationCancellable>;

	public:

		SocketSendOperationCancellable(io::Descriptor* s, const void* buffer,
			std::size_t byteCount,
			CancellationToken&& ct) noexcept
			: win32_overlapped_operation_cancellable<SocketSendOperationCancellable>(std::move(ct))
			, m_impl(s, buffer, byteCount)
		{ }

	private:

		bool try_start() noexcept { return m_impl.try_start(*this); }
		void cancel() noexcept { return m_impl.cancel(*this); }

		SocketSendOperationImpl m_impl;
	};

}}}}//qor::io::async::win

#endif//QOR_PP_H_OS_WINDOWS_FRAMEWORK_ASYNCIOSERVICE_IOCP_SENDOP
