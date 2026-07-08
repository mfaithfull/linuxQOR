// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_DELEGATE_BIND_MEMBER
#define QOR_PP_H_DELEGATE_BIND_MEMBER

#include <functional>

namespace qor 
{
    //create a std::function out of a member function and an object
    template <typename R, typename T, typename... Args>
    std::function<R(Args...)> bindMemberFunction(R(T::* func)(Args...), T *t)
    {
        return [func, t] (Args... args)
        {
            return (t->*func)(args...);
        };
    }
}

#endif//QOR_PP_H_DELEGATE_BIND_MEMBER