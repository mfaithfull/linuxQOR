// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_OS_WINDOWS_FRAMEWORK_ASYNCIOSERVICE_IOCP_IOSTATE
#define QOR_PP_H_OS_WINDOWS_FRAMEWORK_ASYNCIOSERVICE_IOCP_IOSTATE

#include <system_error>
#include <coroutine>
#include "sync/overlapped.h"

namespace qor { namespace async { namespace win {

	struct io_state : overlapped
	{
		using callback_type = void( io_state* state, dword_t errorCode, dword_t numberOfBytesTransferred, ulongptr_t completionKey);

		io_state(callback_type* callback = nullptr) noexcept : io_state(std::uint64_t(0), callback)
		{
		}

		io_state(void* pointer, callback_type* callback) noexcept : m_callback(callback)
		{
			this->Internal = 0;
			this->InternalHigh = 0;
			this->Pointer = pointer;
			this->hEvent = nullptr;
		}

		io_state(std::uint64_t offset, callback_type* callback) noexcept : m_callback(callback)
		{
			this->Internal = 0;
			this->InternalHigh = 0;
			this->Offset = static_cast<dword_t>(offset);
			this->OffsetHigh = static_cast<dword_t>(offset >> 32);
			this->hEvent = nullptr;
		}

		callback_type* m_callback;
	};

}}}//qor::async::win

#endif//QOR_PP_H_OS_WINDOWS_FRAMEWORK_ASYNCIOSERVICE_IOCP_IOSTATE