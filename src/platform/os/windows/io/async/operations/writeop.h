// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_OS_WINDOWS_FRAMEWORK_ASYNCIOSERVICE_IOCP_WRITEOP
#define QOR_PP_H_OS_WINDOWS_FRAMEWORK_ASYNCIOSERVICE_IOCP_WRITEOP

#include <system_error>
#include <coroutine>
#include "overlappedoperationcancellable.h"
#include "overlappedoperation.h"
#include "src/framework/parallel/task/cancellation/cancellationtoken.h"
#include "src/framework/io/network/socket.h"

namespace qor { namespace io { namespace async { namespace win {

	class WriteOperationImpl
	{
	public:

		WriteOperationImpl(io::Descriptor* s, void* buffer, size_t byteCount, long offset) noexcept
			: m_file(s)
			, m_buffer(buffer, byteCount){ }

		bool try_start(win32_overlapped_operation_base& operation) noexcept;
		void cancel(win32_overlapped_operation_base& operation) noexcept;

	private:

		io::Descriptor* m_file;
		wsabuf m_buffer;

	};

	class WriteOperation : public win32_overlapped_operation<WriteOperation>
	{
		friend class win32_overlapped_operation<WriteOperation>;

	public:

		WriteOperation(io::Descriptor* s, void* buffer, size_t byteCount, long offset) noexcept
			: m_impl(s, buffer, byteCount, offset){ }

	private:

		bool try_start() noexcept { return m_impl.try_start(*this); }

		WriteOperationImpl m_impl;

	};

	class WriteOperationCancellable : public win32_overlapped_operation_cancellable<WriteOperationCancellable>
	{
		friend class win32_overlapped_operation_cancellable<WriteOperationCancellable>;

	public:

		WriteOperationCancellable(io::Descriptor* s, void* buffer, std::size_t byteCount, long offset,
			CancellationToken&& ct) noexcept
			: win32_overlapped_operation_cancellable<WriteOperationCancellable>(std::move(ct))
			, m_impl(s, buffer, byteCount, offset) { }

	private:

		bool try_start() noexcept { return m_impl.try_start(*this); }
		void cancel() noexcept { return m_impl.cancel(*this); }

		WriteOperationImpl m_impl;
	};

}}}}//qor::io::async::win

#endif//QOR_PP_H_OS_WINDOWS_FRAMEWORK_ASYNCIOSERVICE_IOCP_WRITEOP
