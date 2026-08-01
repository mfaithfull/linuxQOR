// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_FUNCTION_TEF_CAST
#define QOR_PP_H_FUNCTION_TEF_CAST

#include <type_traits>

#include "function.h"
#include "function_traits.h"
#include "member_function.h"
#include "type_erased_function.h"
#include "type_id.h"
#include "type_traits.h"

namespace qor { namespace tef {

    // Casts from a type-erased function to a function when a function type is
    // provided.
    template <typename T, typename = std::enable_if_t<std::is_function<T>::value>>
    Function<T>& function_cast(TypeErasedFunction& function) 
    {
        check(function.type_id() == get_type_id<T>(), Error::kInvalidCast);
        return static_cast<Function<T>&>(function);
    }

    // Const version of the above.
    template <typename T, typename = std::enable_if_t<std::is_function<T>::value>>
    const Function<T>& function_cast(const TypeErasedFunction& function) 
    {
        check(function.type_id() == get_type_id<T>(), Error::kInvalidCast);
        return static_cast<const Function<T>&>(function);
    }

    // Casts from a type-erased function to a function when a function pointer type
    // is provided. Allows using the function_cast<decltype(func)> syntax.
    template <typename T, typename = std::enable_if_t<IsFunctionPointer<T>::value>>
    Function<typename FunctionTraits<T>::FunctionType>& function_cast(
    TypeErasedFunction& function) 
    {
        using FunctionType = typename FunctionTraits<T>::FunctionType;
        check(function.type_id() == get_type_id<FunctionType>(),
        Error::kInvalidCast);
        return static_cast<Function<FunctionType>&>(function);
    }

    // Const version of the above.
    template <typename T, typename = std::enable_if_t<IsFunctionPointer<T>::value>>
    const Function<typename FunctionTraits<T>::FunctionType>& function_cast(
    const TypeErasedFunction& function) 
    {
        using FunctionType = typename FunctionTraits<T>::FunctionType;
        check(function.type_id() == get_type_id<FunctionType>(),
        Error::kInvalidCast);
        return static_cast<const Function<FunctionType>&>(function);
    }

    // Casts from a type-erased function to a member function when a member function
    // pointer type is provided.
    template <
    typename T,
    typename = std::enable_if_t<std::is_member_function_pointer<T>::value>>
    MemberFunction<T>& function_cast(TypeErasedFunction& function) 
    {
        check(function.type_id() == get_type_id<T>(), Error::kInvalidCast);
        return static_cast<MemberFunction<T>&>(function);
    }

    // Const version of the above.
    template <
    typename T,
    typename = std::enable_if_t<std::is_member_function_pointer<T>::value>>
    const MemberFunction<T>& function_cast(const TypeErasedFunction& function) 
    {
        check(function.type_id() == get_type_id<T>(), Error::kInvalidCast);
        return static_cast<const MemberFunction<T>&>(function);
    }

}}//qor::tef

#endif//QOR_PP_H_FUNCTION_TEF_CAST
