///////////////////////////////////////////////////////////////////////////////
// Copyright (c) Lewis Baker
// Licenced under MIT license. See LICENSE.txt for details.
///////////////////////////////////////////////////////////////////////////////
#ifndef CPPCORO_FILE_READ_OPERATION_HPP_INCLUDED
#define CPPCORO_FILE_READ_OPERATION_HPP_INCLUDED

#include <cppcoro/config.hpp>
#include <cppcoro/CancellationRegistration.hpp>
#include <cppcoro/CancellationToken.hpp>

#include <atomic>
#include <optional>
#include <experimental/coroutine>

#if CPPCORO_OS_WINNT
# include <cppcoro/detail/win32.hpp>
# include <cppcoro/detail/win32_overlapped_operation.hpp>

namespace cppcoro
{
	class file_read_operation_impl
	{
	public:

		file_read_operation_impl(
			detail::win32::handle_t fileHandle,
			void* buffer,
			std::size_t byteCount) noexcept
			: m_fileHandle(fileHandle)
			, m_buffer(buffer)
			, m_byteCount(byteCount)
		{}

		bool try_start(detail::win32_overlapped_operation_base& operation) noexcept;
		void cancel(detail::win32_overlapped_operation_base& operation) noexcept;

	private:

		detail::win32::handle_t m_fileHandle;
		void* m_buffer;
		std::size_t m_byteCount;

	};

	class file_read_operation
		: public detail::win32_overlapped_operation<file_read_operation>
	{
	public:

		file_read_operation(
			detail::win32::handle_t fileHandle,
			std::uint64_t fileOffset,
			void* buffer,
			std::size_t byteCount) noexcept
			: detail::win32_overlapped_operation<file_read_operation>(fileOffset)
			, m_impl(fileHandle, buffer, byteCount)
		{}

	private:

		friend class detail::win32_overlapped_operation<file_read_operation>;

		bool try_start() noexcept { return m_impl.try_start(*this); }

		file_read_operation_impl m_impl;

	};

	class file_read_operation_cancellable
		: public detail::win32_overlapped_operation_cancellable<file_read_operation_cancellable>
	{
	public:

		file_read_operation_cancellable(
			detail::win32::handle_t fileHandle,
			std::uint64_t fileOffset,
			void* buffer,
			std::size_t byteCount,
			CancellationToken&& cancellationToken) noexcept
			: detail::win32_overlapped_operation_cancellable<file_read_operation_cancellable>(
				fileOffset, std::move(cancellationToken))
			, m_impl(fileHandle, buffer, byteCount)
		{}

	private:

		friend class detail::win32_overlapped_operation_cancellable<file_read_operation_cancellable>;

		bool try_start() noexcept { return m_impl.try_start(*this); }
		void cancel() noexcept { m_impl.cancel(*this); }

		file_read_operation_impl m_impl;

	};

#endif
}

#endif
