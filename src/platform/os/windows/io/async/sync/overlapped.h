// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_OS_WINDOWS_FRAMEWORK_ASYNCIOSERVICE_IOCP_OVERLAPPED
#define QOR_PP_H_OS_WINDOWS_FRAMEWORK_ASYNCIOSERVICE_IOCP_OVERLAPPED

#include <system_error>
#include <coroutine>
#include <utility>
#include <cstdint>

struct _OVERLAPPED;

namespace qor { namespace io { namespace async { namespace win {

#if (qor_pp_compiler == qor_pp_compiler_msvc)
#	pragma warning(push)
#	pragma warning(disable : 4201) // Non-standard anonymous struct/union
#endif

	using handle_t = void*;
	using ulongptr_t = std::uintptr_t;
	using longptr_t = std::intptr_t;
	using dword_t = unsigned long;
	using socket_t = std::uintptr_t;
	using ulong_t = unsigned long;

	/// Structure needs to correspond exactly to the builtin
	/// _OVERLAPPED structure from Windows.h.
	struct overlapped
	{
		ulongptr_t Internal;
		ulongptr_t InternalHigh;
		union
		{
			struct
			{
				dword_t Offset;
				dword_t OffsetHigh;
			};
			void* Pointer;
		};
		handle_t hEvent;
	};

#if (qor_pp_compiler == qor_pp_compiler_msvc)
#	pragma warning(pop)
#endif

}}}}//qor::io::async::win

#endif//QOR_PP_H_OS_WINDOWS_FRAMEWORK_ASYNCIOSERVICE_IOCP_OVERLAPPED
