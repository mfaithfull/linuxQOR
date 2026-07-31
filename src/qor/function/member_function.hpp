// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_FUNCTION_TEF_MEMBER_FUNCTION_IMPL
#define QOR_PP_H_FUNCTION_TEF_MEMBER_FUNCTION_IMPL

#include "function.h"
#include "type_id.h"

namespace qor { namespace tef {

    // Default constructor.
    template <typename MemberFuncPtr>
    MemberFunction<MemberFuncPtr>::MemberFunction() noexcept : TypeErasedFunction(get_type_id<MemberFuncPtr>()){ }

    // Constructor used by factory methods taking member functions addresses.
    template <typename MemberFuncPtr>
    MemberFunction<MemberFuncPtr>::MemberFunction(TypeErasedFunction::TypeErasedFuncPtr member_func_ptr) noexcept
        : TypeErasedFunction(get_type_id<MemberFuncPtr>(), member_func_ptr){ }

    template <typename MemberFuncPtr>
    template <MemberFuncPtr member_func_ptr, typename>
    MemberFunction<MemberFuncPtr>
    MemberFunction<MemberFuncPtr>::FromMemberFunction() noexcept 
    {
        // We use the function type here to avoid having to deduce the argument pack
        // types from MemberFuncPtr. If we do, we also have to specialize for all
        // possible combinations of const and volatile qualifications of the function.
        // The extra reinterpret_cast to void* is to avoid a MSVC 2015 warning.
        return MemberFunction(
            reinterpret_cast<TypeErasedFunction::TypeErasedFuncPtr>(
            reinterpret_cast<void*>(&Function<FunctionType>::template CallMemberFuncAddress<MemberFuncPtr, member_func_ptr>)));
    }

    template <typename MemberFuncPtr>
    template <typename... CallArgs>
    typename MemberFunction<MemberFuncPtr>::ReturnType
    MemberFunction<MemberFuncPtr>::operator ()(ClassType& object, CallArgs&&... args) const 
    {
        MAGIC_FUNC_DCHECK(func_ptr_, Error::kInvalidFunction);
        using FuncCallPtr = typename Traits::TypeErasedCallType;
        return (*reinterpret_cast<FuncCallPtr>(this->func_ptr_))(&object, std::forward<CallArgs>(args)...);
    }

}}//qor::tef

#endif//QOR_PP_H_FUNCTION_TEF_MEMBER_FUNCTION_IMPL
