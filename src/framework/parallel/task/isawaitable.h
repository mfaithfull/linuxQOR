// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_FRAMEWORK_TASK_IS_AWAITABLE
#define QOR_PP_H_FRAMEWORK_TASK_IS_AWAITABLE

#include <type_traits>

#include "detail/getawaiter.h"

namespace qor
{
	template<typename T, typename = std::void_t<>>
	struct is_awaitable : std::false_type {};

	template<typename T>
	struct is_awaitable<T, std::void_t<decltype(detail::GetAwaiter(std::declval<T>()))>> : std::true_type {};

	template<typename T>
	constexpr bool is_awaitable_v = is_awaitable<T>::value;
}//qor

#endif//QOR_PP_H_FRAMEWORK_TASK_IS_AWAITABLE
