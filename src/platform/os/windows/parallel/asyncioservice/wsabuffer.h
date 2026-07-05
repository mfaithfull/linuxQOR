// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_OS_WINDOWS_FRAMEWORK_ASYNCIOSERVICE_IOCP_WSABUFFER
#define QOR_PP_H_OS_WINDOWS_FRAMEWORK_ASYNCIOSERVICE_IOCP_WSABUFFER

#include <system_error>
#include <coroutine>
#include "sync/overlapped.h"

namespace qor { namespace async { namespace win {

	struct wsabuf
	{
		constexpr wsabuf() noexcept : len(0) , buf(nullptr) { }

		constexpr wsabuf(void* ptr, std::size_t size)
			: len(size <= ulong_t(-1) ? ulong_t(size) : ulong_t(-1))
			, buf(static_cast<char*>(ptr)){ }

		ulong_t len;
		char* buf;
	};

}}}//qor::async::win

#endif//QOR_PP_H_OS_WINDOWS_FRAMEWORK_ASYNCIOSERVICE_IOCP_WSABUFFER
