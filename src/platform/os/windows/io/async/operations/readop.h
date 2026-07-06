// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_OS_WINDOWS_FRAMEWORK_ASYNCIOSERVICE_IOCP_READOP
#define QOR_PP_H_OS_WINDOWS_FRAMEWORK_ASYNCIOSERVICE_IOCP_READOP

#include <system_error>
#include <coroutine>
#include "overlappedoperationcancellable.h"
#include "overlappedoperation.h"
#include "src/framework/parallel/task/cancellation/cancellationtoken.h"
#include "src/framework/io/network/socket.h"

namespace qor { namespace io { namespace async { namespace win {

	class ReadOperationImpl
	{
	public:

		ReadOperationImpl(io::Descriptor* s, void* buffer, size_t byteCount, long offset) noexcept
			: m_file(s)
			, m_buffer(buffer, byteCount){ }

		bool try_start(win32_overlapped_operation_base& operation) noexcept;
		void cancel(win32_overlapped_operation_base& operation) noexcept;

	private:

		io::Descriptor* m_file;
		wsabuf m_buffer;

	};

	class ReadOperation : public win32_overlapped_operation<ReadOperation>
	{
		friend class win32_overlapped_operation<ReadOperation>;

	public:

		ReadOperation(io::Descriptor* s, void* buffer, size_t byteCount, long offset) noexcept
			: m_impl(s, buffer, byteCount, offset){ }

	private:

		bool try_start() noexcept { return m_impl.try_start(*this); }

		ReadOperationImpl m_impl;

	};

	class ReadOperationCancellable : public win32_overlapped_operation_cancellable<ReadOperationCancellable>
	{
		friend class win32_overlapped_operation_cancellable<ReadOperationCancellable>;

	public:

		ReadOperationCancellable(io::Descriptor* s, void* buffer, std::size_t byteCount, long offset,
			CancellationToken&& ct) noexcept
			: win32_overlapped_operation_cancellable<ReadOperationCancellable>(std::move(ct))
			, m_impl(s, buffer, byteCount, offset) { }

	private:

		bool try_start() noexcept { return m_impl.try_start(*this); }
		void cancel() noexcept { return m_impl.cancel(*this); }

		ReadOperationImpl m_impl;
	};

}}}}//qor::io::async::win

#endif//QOR_PP_H_OS_WINDOWS_FRAMEWORK_ASYNCIOSERVICE_IOCP_READOP
