// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_OS_WINDOWS_FRAMEWORK_ASYNCIOSERVICE_IOCP_OVERLAPPEDOPERATION
#define QOR_PP_H_OS_WINDOWS_FRAMEWORK_ASYNCIOSERVICE_IOCP_OVERLAPPEDOPERATION

#include <system_error>
#include <coroutine>
#include "src/platform/compiler/compiler.h"
#include "overlappedoperationbase.h"

namespace qor { namespace io { namespace async { namespace win {

	template<typename OPERATION>
	class win32_overlapped_operation : protected win32_overlapped_operation_base
	{
	protected:

		win32_overlapped_operation() noexcept : win32_overlapped_operation_base( &win32_overlapped_operation::on_operation_completed)
		{ }

		win32_overlapped_operation(void* pointer) noexcept : win32_overlapped_operation_base(
			pointer,
			&win32_overlapped_operation::on_operation_completed)
		{ }

		win32_overlapped_operation(std::uint64_t offset) noexcept : win32_overlapped_operation_base(
			offset,
			&win32_overlapped_operation::on_operation_completed)
		{ }

		virtual ~win32_overlapped_operation() = default;

	public:

		bool await_ready() const noexcept { return false; }

		qor_pp_noinline bool await_suspend(std::coroutine_handle<> awaitingCoroutine)
		{
			static_assert(std::is_base_of_v<win32_overlapped_operation, OPERATION>);

			m_awaitingCoroutine = awaitingCoroutine;
			return static_cast<OPERATION*>(this)->try_start();
		}

		decltype(auto) await_resume()
		{
			return static_cast<OPERATION*>(this)->get_result();
		}

	private:

		static void on_operation_completed(
			io_state* ioState,
			dword_t errorCode,
			dword_t numberOfBytesTransferred,
			[[maybe_unused]] ulongptr_t completionKey) noexcept
		{
			auto* operation = static_cast<win32_overlapped_operation*>(ioState);
			operation->m_errorCode = errorCode;
			operation->m_numberOfBytesTransferred = numberOfBytesTransferred;
			if(operation->m_errorCode != 0)
			{
				//early error catch
				operation->m_errorCode = operation->m_errorCode;
			}
			if((uint64_t)(operation->m_awaitingCoroutine.address()) != 0xdddddddddddddddd)
			{
				operation->m_awaitingCoroutine.resume();
			}
			else
			{
				operation->m_awaitingCoroutine = operation->m_awaitingCoroutine;
			}
		}

		std::coroutine_handle<> m_awaitingCoroutine;

	};

}}}}//qor::io::async::win

#endif//QOR_PP_H_OS_WINDOWS_FRAMEWORK_ASYNCIOSERVICE_IOCP_OVERLAPPEDOPERATION
