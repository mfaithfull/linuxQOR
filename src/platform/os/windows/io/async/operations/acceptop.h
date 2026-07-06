// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_OS_WINDOWS_FRAMEWORK_ASYNCIOSERVICE_IOCP_ACCEPTOP
#define QOR_PP_H_OS_WINDOWS_FRAMEWORK_ASYNCIOSERVICE_IOCP_ACCEPTOP

#include <system_error>
#include <coroutine>
#include "overlappedoperationcancellable.h"
#include "overlappedoperation.h"
#include "src/framework/parallel/task/cancellation/cancellationtoken.h"
#include "src/framework/io/network/socket.h"

namespace qor { namespace io { namespace async { namespace win {

	class SocketAcceptOperationImpl
	{
	public:

		SocketAcceptOperationImpl(io::Descriptor* listeningSocket, io::network::Socket* acceptingSocket) noexcept
			: m_listeningSocket(listeningSocket)
			, m_acceptingSocket(acceptingSocket)
		{ }

		bool try_start(win32_overlapped_operation_base& operation) noexcept;
		void cancel(win32_overlapped_operation_base& operation) noexcept;
		int get_result(win32_overlapped_operation_base& operation);

	private:

#if (qor_pp_compiler == qor_pp_compiler_msvc)
#	pragma warning(push)
#	pragma warning(disable : 4324) // Structure padded due to alignment
#endif

		io::Descriptor* m_listeningSocket;
		io::network::Socket* m_acceptingSocket;
		alignas(8) std::uint8_t m_addressBuffer[88];

#if (qor_pp_compiler == qor_pp_compiler_msvc)
#	pragma warning(pop)
#endif

	};

	class SocketAcceptOperation : public win32_overlapped_operation<SocketAcceptOperation>
	{
		friend class win32_overlapped_operation<SocketAcceptOperation>;

	public:

		SocketAcceptOperation(io::Descriptor* listeningSocket, io::network::Socket* acceptingSocket) noexcept
			: m_impl(listeningSocket, acceptingSocket)
		{ }

	private:

		bool try_start() noexcept { return m_impl.try_start(*this); }
		int get_result() { return m_impl.get_result(*this); }

		SocketAcceptOperationImpl m_impl;
	};

	class SocketAcceptOperationCancellable : public win32_overlapped_operation_cancellable<SocketAcceptOperationCancellable>
	{
		friend class win32_overlapped_operation_cancellable<SocketAcceptOperationCancellable>;

	public:

		SocketAcceptOperationCancellable(
			io::Descriptor* listeningSocket,
			io::network::Socket* acceptingSocket,
			CancellationToken&& ct) noexcept :
			win32_overlapped_operation_cancellable<SocketAcceptOperationCancellable>(std::move(ct)), m_impl(listeningSocket, acceptingSocket)
		{ }

	private:

		bool try_start() noexcept { return m_impl.try_start(*this); }
		void cancel() noexcept { m_impl.cancel(*this); }
		int get_result() { m_impl.get_result(*this); }

		SocketAcceptOperationImpl m_impl;

	};

}}}}//qor::io::async::win

#endif//QOR_PP_H_OS_WINDOWS_FRAMEWORK_ASYNCIOSERVICE_IOCP_ACCEPTOP
