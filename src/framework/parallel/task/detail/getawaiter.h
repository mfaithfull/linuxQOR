// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_FRAMEWORK_TASK_DETAIL_GETAWAITER
#define QOR_PP_H_FRAMEWORK_TASK_DETAIL_GETAWAITER

#include <coroutine>

#include "isawaiter.h"
#include "src/qor/essentials/objectcontext/any.h"

namespace qor{	namespace detail{

    template<typename T>
    auto GetAwaiterImpl(T&& value, int) noexcept(noexcept(static_cast<T&&>(value).operator co_await())) -> decltype(static_cast<T&&>(value).operator co_await())
    {
        return static_cast<T&&>(value).operator co_await();
    }

    template<typename T>
    auto GetAwaiterImpl(T&& value, long) noexcept(noexcept(operator co_await(static_cast<T&&>(value)))) -> decltype(operator co_await(static_cast<T&&>(value)))
    {
        return operator co_await(static_cast<T&&>(value));
    }

    template<typename T, std::enable_if_t<qor::detail::is_awaiter<T&&>::value, int> = 0>
    T&& GetAwaiterImpl(T&& value, qor::detail::any) noexcept
    {
        return static_cast<T&&>(value);
    }

    template<typename T>
    auto GetAwaiter(T&& value) noexcept(noexcept(detail::GetAwaiterImpl(static_cast<T&&>(value), 123))) -> decltype(detail::GetAwaiterImpl(static_cast<T&&>(value), 123))
    {
        return detail::GetAwaiterImpl(static_cast<T&&>(value), 123);
    }

}}//qor::detail

#endif//QOR_PP_H_FRAMEWORK_TASK_DETAIL_GETAWAITER
