// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_FUNCTION_TEF_TYPE_TRAITS
#define QOR_PP_H_FUNCTION_TEF_TYPE_TRAITS

#include <memory>
#include <tuple>
#include <type_traits>

namespace qor { namespace tef {

    // Forward-declaration of functions.
    template <typename FuncPtr>
    class Function;

    // Forward-declaration of member functions.
    template <typename FuncPtr>
    class MemberFunction;

    namespace internal {

        template <typename T>
        struct IsFunctionImpl : public std::false_type {};

        template <typename T>
        struct IsFunctionImpl<Function<T>> : public std::true_type {};

        template <typename T>
        struct IsMemberFunctionImpl : public std::false_type {};

        template <typename T>
        struct IsMemberFunctionImpl<MemberFunction<T>> : public std::true_type {};

        template <typename T>
        struct IsUniquePtrImpl : public std::false_type {};

        template <typename T, typename Deleter>
        struct IsUniquePtrImpl<std::unique_ptr<T, Deleter>> : public std::true_type {};

        template <typename T>
        struct IsSharedPtrImpl : public std::false_type {};

        template <typename T>
        struct IsSharedPtrImpl<std::shared_ptr<T>> : public std::true_type {};

    } // namespace internal

    // Tells if a provided type is a tef::Function.
    template <typename T>
    using IsFunction = internal::IsFunctionImpl<std::remove_cv_t<std::decay_t<T>>>;

    // Tells if a provided type is a tef::MemberFunction.
    template <typename T>
    using IsMemberFunction = internal::IsMemberFunctionImpl<std::remove_cv_t<std::decay_t<T>>>;

    // Tells if a provided type is a std::unique_ptr.
    template <typename T>
    using IsUniquePtr = internal::IsUniquePtrImpl<std::remove_cv_t<std::decay_t<T>>>;

    // Tells if a provided type is a std::shared_ptr.
    template <typename T>
    using IsSharedPtr = internal::IsSharedPtrImpl<std::remove_cv_t<std::decay_t<T>>>;

    // Tells if a provided type is a free function pointer.
    template <typename T>
    using IsFunctionPointer =
        std::integral_constant<bool,
            // It's a function pointer if it's a pointer and...
            std::is_pointer<std::remove_reference_t<T>>::value &&
            // After removing cv qualifications and a pointer it becomes a function.
            std::is_function<std::remove_pointer_t<std::remove_cv_t<std::remove_reference_t<T>>>>::value>;

}}//qor::tef

#endif//QOR_PP_H_FUNCTION_TEF_TYPE_TRAITS
