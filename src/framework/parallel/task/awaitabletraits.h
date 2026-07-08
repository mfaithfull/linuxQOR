// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_FRAMEWORK_TASK_AWAITABLE_TRAITS
#define QOR_PP_H_FRAMEWORK_TASK_AWAITABLE_TRAITS

#include <type_traits>
#include "detail/getawaiter.h"

namespace qor{

    template<typename T, typename = void>
	struct awaitable_of {};

	template<typename T>
	struct awaitable_of<T, std::void_t<decltype(qor::detail::GetAwaiter(std::declval<T>()))>>
	{
		using awaiter_t = decltype(qor::detail::GetAwaiter(std::declval<T>()));

		using await_result_t = decltype(std::declval<awaiter_t>().await_resume());
	};

}//qor

#endif//QOR_PP_H_FRAMEWORK_TASK_AWAITABLE_TRAITS
