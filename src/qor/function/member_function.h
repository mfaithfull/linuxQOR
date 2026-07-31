// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_FUNCTION_TEF_MEMBER_FUNCTION
#define QOR_PP_H_FUNCTION_TEF_MEMBER_FUNCTION

#include <type_traits>

#include "port.h"
#include "type_erased_function.h"

namespace qor { namespace tef {

// Class encapsulating a member function of an object.
// MemberFuncPtr must be a member function pointer type.
template <typename MemberFuncPtr>
class MemberFunction : public TypeErasedFunction 
{
public:
    static_assert(std::is_member_function_pointer<MemberFuncPtr>::value, "Template argument must be a member function pointer.");

    // Auxiliary type for traits of the function pointer.
    using Traits = FunctionTraits<MemberFuncPtr>;

    // The function type of this member function pointer type.
    // For example, for void (Class::*)(int) is void(int).
    using FunctionType = typename Traits::FunctionType;

    // The member function pointer type of this MemberFunction.
    using FunctionPointerType = typename Traits::FunctionPointerType;

    // The return type of the member function.
    using ReturnType = typename Traits::Return;

    // A tuple with the argument types of the member function.
    using ArgTypes = typename Traits::Args;

    // The class the member function belongs to.
    using ClassType = typename Traits::Class;

    // Flag indicating the qualifications of the member function.
    enum : bool 
    {
        kIsConst = std::is_const<ClassType>::value,
        kIsVolatile = std::is_volatile<ClassType>::value,
    };

    // Creates an empty typed MemberFunction.
    MemberFunction() noexcept;

    // Factory method for member function addresses.
    template <MemberFuncPtr member_func_ptr, typename = std::enable_if_t<std::is_member_function_pointer<MemberFuncPtr>::value>>
    static MemberFunction<MemberFuncPtr> FromMemberFunction() noexcept;

    // Invokes the function returning its result.
    //
    // Provided arguments must be convertible to the arguments of the
    // MemberFunction. Similarly, the object must have compatible qualifications
    // (const and volatile) to the targeted member function.
    //
    // This method uses universal references, so provided arguments do not need
    // to be rvalue references. These can be lvalue or rvalue references depending
    // on what is provided for each argument.
    //
    // Note that ClassType has the same qualifications as the member function.
    // This makes the object become a const reference if the function is const.
    template <typename... CallArgs>
    ReturnType operator ()(ClassType& object, CallArgs&&... args) const;

private:
    // For accessing func_ptr_.
    template <typename FuncType>
    friend class Function;

    explicit MemberFunction(TypeErasedFunction::TypeErasedFuncPtr member_func_ptr) noexcept;
};

}}//qor::tef

#include "member_function.hpp"

#endif//QOR_PP_H_FUNCTION_TEF_MEMBER_FUNCTION
