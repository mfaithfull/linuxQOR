// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_OS_WINDOWS_FRAMEWORK_ASYNCIOSERVICE_IOCP_OVERLAPPEDOPERATIONBASE
#define QOR_PP_H_OS_WINDOWS_FRAMEWORK_ASYNCIOSERVICE_IOCP_OVERLAPPEDOPERATIONBASE

#include <system_error>
#include <coroutine>
#include "../iostate.h"
#include "../wsabuffer.h"

namespace qor { namespace io { namespace async { namespace win {

	class win32_overlapped_operation_base : protected io_state
	{
	public:

		win32_overlapped_operation_base( io_state::callback_type* callback) noexcept : io_state(callback)
			, m_errorCode(0)
			, m_numberOfBytesTransferred(0)
		{ }

		win32_overlapped_operation_base( void* pointer, io_state::callback_type* callback) noexcept
			: io_state(pointer, callback)
			, m_errorCode(0)
			, m_numberOfBytesTransferred(0)
		{ }

		win32_overlapped_operation_base( std::uint64_t offset, io_state::callback_type* callback) noexcept : io_state(offset, callback)
			, m_errorCode(0)
			, m_numberOfBytesTransferred(0)
		{ }

		virtual ~win32_overlapped_operation_base() = default;

		_OVERLAPPED* get_overlapped() noexcept
		{
			return reinterpret_cast<_OVERLAPPED*>(static_cast<overlapped*>(this));
		}

		std::size_t get_result()
		{
			if (m_errorCode != 0)
			{
				throw std::system_error{
					static_cast<int>(m_errorCode),
					std::system_category()
				};
			}

			return m_numberOfBytesTransferred;
		}

		dword_t m_errorCode;
		dword_t m_numberOfBytesTransferred;

	};

}}}}//qor::io::async::win

#endif//QOR_PP_H_OS_WINDOWS_FRAMEWORK_ASYNCIOSERVICE_IOCP_OVERLAPPEDOPERATIONBASE
