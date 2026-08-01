// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_FUNCTION_TEF_TYPE_ERASED_FUNCTION_IMPL
#define QOR_PP_H_FUNCTION_TEF_TYPE_ERASED_FUNCTION_IMPL

namespace qor { namespace tef {

    TypeErasedFunction::TypeErasedFunction() noexcept
        : func_ptr_(nullptr), type_id_(0) { }

    TypeErasedFunction::TypeErasedFunction(TypeId type_id, TypeErasedFuncPtr func_ptr) noexcept
        : func_ptr_(func_ptr), type_id_(type_id) { }

    TypeErasedFunction::TypeErasedFunction(TypeErasedFunction&& function)
        : func_ptr_(nullptr), type_id_(0) 
    {
        std::swap(object_, function.object_);
        std::swap(func_ptr_, function.func_ptr_);
        std::swap(type_id_, function.type_id_);
    }

    TypeErasedFunction& TypeErasedFunction::operator =(const TypeErasedFunction& function) 
    {
        if (this == &function)
        return *this;

        check(type_id_ == 0 || type_id_ == function.type_id_, Error::kIncompatibleType);
        object_ = function.object_;
        func_ptr_ = function.func_ptr_;
        type_id_ = function.type_id_;
        return *this;
    }

    TypeErasedFunction& TypeErasedFunction::operator =(TypeErasedFunction&& function) 
    {
        if (this == &function)
        return *this;

        check(type_id_ == 0 || type_id_ == function.type_id_, Error::kIncompatibleType);
        object_.Reset();
        func_ptr_ = nullptr;
        type_id_ = 0;

        std::swap(object_, function.object_);
        std::swap(func_ptr_, function.func_ptr_);
        std::swap(type_id_, function.type_id_);
        return *this;
    }

    TypeErasedFunction& TypeErasedFunction::operator =(std::nullptr_t) 
    {
        func_ptr_ = nullptr;
        object_.Reset();
        return *this;
    }

}}//qor::tef

#endif//QOR_PP_H_FUNCTION_TEF_TYPE_ERASED_FUNCTION_IMPL
