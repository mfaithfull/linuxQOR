// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_FUNCTION_TEF_FUNCTIONTRAITS
#define QOR_PP_H_FUNCTION_TEF_FUNCTIONTRAITS

#include <tuple>
#include <type_traits>

#include "type_traits.h"

namespace qor { namespace tef { namespace internal {

    // Forward declaration of FunctionTraitsImpl.
    template <typename FuncPtr>
    struct FunctionTraitsImpl;

    // Specialization for function types.
    template <typename Return_, typename... Args_>
    struct FunctionTraitsImpl<Return_(Args_...)> 
    {
        using Return = Return_;
        using Args = std::tuple<Args_...>;
        using FunctionType = Return_(Args_...);
        using FunctionPointerType = Return_(*)(Args_...);

        template <template <typename> class Filter>
        using FilteredArgs = std::tuple<Filter<Args_>...>;
        using DecayedArgs = decltype(std::make_tuple(std::declval<Args_>()...));

        enum : size_t { kNumArgs = sizeof...(Args_) };
    };

    // Specialization for function pointer types.
    template <typename Return_, typename... Args_>
    struct FunctionTraitsImpl<Return_ (*)(Args_...)> 
    {
        using Return = Return_;
        using Args = std::tuple<Args_...>;
        using FunctionType = Return_(Args_...);
        using FunctionPointerType = Return_(*)(Args_...);

        template <template <typename> class Filter>
        using FilteredArgs = std::tuple<Filter<Args_>...>;
        using DecayedArgs = decltype(std::make_tuple(std::declval<Args_>()...));

        enum : size_t { kNumArgs = sizeof...(Args_) };
    };

    // Specialization for member function types.
    template <typename Class_, typename Return_, typename... Args_>
    struct FunctionTraitsImpl<Return_ (Class_::*)(Args_...)> 
    {
        using Return = Return_;
        using Args = std::tuple<Args_...>;
        using Class = Class_;
        using FunctionType = Return_(Args_...);
        using FunctionPointerType = Return_ (Class_::*)(Args_...);
        using TypeErasedCallType = Return_(*)(void*, Args_...);

        template <template <typename> class Filter>
        using FilteredArgs = std::tuple<Filter<Args_>...>;
        using DecayedArgs = decltype(std::make_tuple(std::declval<Args_>()...));

        enum : size_t { kNumArgs = sizeof...(Args_) };

        enum : bool 
        {
            kIsConst = false,
            kIsVolatile = false,
        };
    };

    // Specialization for member const function types.
    template <typename Class_, typename Return_, typename... Args_>
    struct FunctionTraitsImpl<Return_ (Class_::*)(Args_...) const> 
    {
        using Return = Return_;
        using Args = std::tuple<Args_...>;
        using Class = const Class_;
        using FunctionType = Return_(Args_...);
        using FunctionPointerType = Return_ (Class_::*)(Args_...) const;
        using TypeErasedCallType = Return_(*)(const void*, Args_...);

        template <template <typename> class Filter>
        using FilteredArgs = std::tuple<Filter<Args_>...>;
        using DecayedArgs = decltype(std::make_tuple(std::declval<Args_>()...));

        enum : size_t { kNumArgs = sizeof...(Args_) };
        enum : bool 
        {
            kIsConst = true,
            kIsVolatile = false,
        };
    };

    // Specialization for member volatile function types.
    template <typename Class_, typename Return_, typename... Args_>
    struct FunctionTraitsImpl<Return_ (Class_::*)(Args_...) volatile> 
    {
        using Return = Return_;
        using Args = std::tuple<Args_...>;
        using Class = volatile Class_;
        using FunctionType = Return_(Args_...);
        using FunctionPointerType = Return_ (Class_::*)(Args_...) volatile;
        using TypeErasedCallType = Return_(*)(volatile void*, Args_...);

        template <template <typename> class Filter>
        using FilteredArgs = std::tuple<Filter<Args_>...>;
        using DecayedArgs = decltype(std::make_tuple(std::declval<Args_>()...));

        enum : size_t { kNumArgs = sizeof...(Args_) };
        enum : bool 
        {
            kIsConst = false,
            kIsVolatile = true,
        };
    };

    // Specialization for member const volatile function types.
    template <typename Class_, typename Return_, typename... Args_>
    struct FunctionTraitsImpl<Return_ (Class_::*)(Args_...) const volatile> 
    {
        using Return = Return_;
        using Args = std::tuple<Args_...>;
        using Class = const volatile Class_;
        using FunctionType = Return_(Args_...);
        using FunctionPointerType = Return_ (Class_::*)(Args_...) const volatile;
        using TypeErasedCallType = Return_(*)(const volatile void*, Args_...);

        template <template <typename> class Filter>
        using FilteredArgs = std::tuple<Filter<Args_>...>;
        using DecayedArgs = decltype(std::make_tuple(std::declval<Args_>()...));

        enum : size_t { kNumArgs = sizeof...(Args_) };
        enum : bool 
        {
            kIsConst = true,
            kIsVolatile = true,
        };
    };

    template <typename T, typename CV>
    struct CopyCVImpl 
    {
    private:
        using U = std::conditional_t<std::is_const<CV>::value,
            std::add_const_t<T>, T>;

    public:
        using type = std::conditional_t<std::is_volatile<CV>::value,
            std::add_volatile_t<U>, U>;
    };

    // Note: using std::add_const and std::add_volatile with function types does not
    // produce the same results and does not work for overload disambiguation.
    template <typename Return, typename... Args, typename CV>
    struct CopyCVImpl<Return(Args...), CV> 
    {
        using type = std::conditional_t<
            std::is_const<CV>::value && std::is_volatile<CV>::value,
            Return(Args...) const volatile,
                std::conditional_t<std::is_const<CV>::value, Return(Args...) const,
                    std::conditional_t<std::is_volatile<CV>::value,
                        Return(Args...) volatile, Return(Args...)>>>;
    };

    }  // namespace internal

    // Provides information for a type representing a function, a pointer to a
    // function or a pointer to a member function.
    template <typename T>
    using FunctionTraits = internal::FunctionTraitsImpl<std::remove_reference_t<T>>;

    // Copies the const-volatile qualifiers of a type to another.
    template <typename T, typename CV>
    using CopyCV = typename internal::CopyCVImpl<std::remove_cv_t<T>, CV>::type;

    // Returns the member function pointer type of a type's parenthesis operator
    // if there is exactly one. If there is none or there are many this will fail.
    template <typename Callable>
    using CallableType = std::enable_if_t<std::is_member_function_pointer<
        decltype(&std::remove_reference_t<Callable>::operator())>::value,
        decltype(&std::remove_reference_t<Callable>::operator())>;

}}//qor::tef

#endif//QOR_PP_H_FUNCTION_TEF_FUNCTIONTRAITS
