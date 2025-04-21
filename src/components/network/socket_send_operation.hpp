///////////////////////////////////////////////////////////////////////////////
// Copyright (c) Lewis Baker
// Licenced under MIT license. See LICENSE.txt for details.
///////////////////////////////////////////////////////////////////////////////
#ifndef CPPCORO_NET_SOCKET_SEND_OPERATION_HPP_INCLUDED
#define CPPCORO_NET_SOCKET_SEND_OPERATION_HPP_INCLUDED

#include <cppcoro/config.hpp>
#include <cppcoro/CancellationToken.hpp>

#include <cstdint>

#if CPPCORO_OS_WINNT
# include <cppcoro/detail/win32.hpp>
# include <cppcoro/detail/win32_overlapped_operation.hpp>

namespace net
{
	class socket;

	class socket_send_operation_impl
	{
	public:

		socket_send_operation_impl(
			socket& s,
			const void* buffer,
			std::size_t byteCount) noexcept
			: m_socket(s)
			, m_buffer(const_cast<void*>(buffer), byteCount)
		{}

		bool try_start(detail::win32_overlapped_operation_base& operation) noexcept;
		void cancel(detail::win32_overlapped_operation_base& operation) noexcept;

	private:

		socket& m_socket;
		detail::win32::wsabuf m_buffer;

	};

	class socket_send_operation
		: public detail::win32_overlapped_operation<socket_send_operation>
	{
	public:

		socket_send_operation(
			socket& s,
			const void* buffer,
			std::size_t byteCount) noexcept
			: m_impl(s, buffer, byteCount)
		{}

	private:

		friend class detail::win32_overlapped_operation<socket_send_operation>;

		bool try_start() noexcept { return m_impl.try_start(*this); }

		socket_send_operation_impl m_impl;

	};

	class socket_send_operation_cancellable
		: public detail::win32_overlapped_operation_cancellable<socket_send_operation_cancellable>
	{
	public:

		socket_send_operation_cancellable(
			socket& s,
			const void* buffer,
			std::size_t byteCount,
			CancellationToken&& ct) noexcept
			: detail::win32_overlapped_operation_cancellable<socket_send_operation_cancellable>(std::move(ct))
			, m_impl(s, buffer, byteCount)
		{}

	private:

		friend class detail::win32_overlapped_operation_cancellable<socket_send_operation_cancellable>;

		bool try_start() noexcept { return m_impl.try_start(*this); }
		void cancel() noexcept { return m_impl.cancel(*this); }

		socket_send_operation_impl m_impl;

	};

}

#endif // CPPCORO_OS_WINNT

#endif
