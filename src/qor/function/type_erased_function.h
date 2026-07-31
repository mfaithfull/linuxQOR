// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_FUNCTION_TEF_TYPE_ERASED_FUNCTION
#define QOR_PP_H_FUNCTION_TEF_TYPE_ERASED_FUNCTION

#include "port.h"
#include "type_erased_object.h"
#include "type_id.h"

namespace qor { namespace tef {

    // Type-erased base function type.
    class TypeErasedFunction 
    {
    public:
        // Creates an empty, uninitialized type-erased function.
        // The object has no type yet and can be assigned to any other.
        // Trying to function_cast it to another object will fail.
        inline TypeErasedFunction() noexcept;

        // Default copy and move constructors.
        inline TypeErasedFunction(const TypeErasedFunction&) = default;
        inline TypeErasedFunction(TypeErasedFunction&&);

        // Copies another function into the current object.
        //
        // If this object has a type set, the other function must have the same type
        // or this call will raise an assertion failure.
        inline TypeErasedFunction& operator =(const TypeErasedFunction& function);

        // Moves another function into the current object.
        //
        // If this object has a type set, the other function must have the same type
        // or this call will raise an assertion failure.
        inline TypeErasedFunction& operator =(TypeErasedFunction&& function);

        // Tells if the object point to a valid function or not.
        explicit operator bool() const noexcept { return func_ptr_ != nullptr; }

        bool operator ==(std::nullptr_t) const noexcept 
        {
            return func_ptr_ == nullptr;
        }

        bool operator !=(std::nullptr_t) const noexcept 
        {
            return func_ptr_ != nullptr;
        }

        // Provides a unique integer representation of the type this object is
        // encapsulating. Type ids can be uninitialized (with a value of 0), but once
        // initialized they cannot change. This prevents from copying or moving two
        // incompatible objects at the type-erased function level.
        TypeId type_id() const noexcept { return type_id_; }

        // Returns a pointer to the object associated to this function if any.
        void* GetObject() const noexcept { return object_.GetObject(); }

        // Resets the function.
        inline TypeErasedFunction& operator =(std::nullptr_t);

    protected:
        // Type-erased versions of functions.
        using TypeErasedFuncPtr = void (*)();

        // Constructor used by derived types.
        inline TypeErasedFunction(TypeId type_id,
        TypeErasedFuncPtr func_ptr = nullptr) noexcept;

        // Type-erased version of the object associated with the function, if any.
        // Goes intentionally first because it can have alignment requirements.
        TypeErasedObject object_;

        // Type-erased function pointer.
        // Points to an auxiliary function that restores the actual type and performs
        // the appropriate call.
        TypeErasedFuncPtr func_ptr_;

        // Runtime representation of the type managed by this object.
        TypeId type_id_;
    };

}}//qor::tef

#include "type_erased_function.hpp"

#endif //QOR_PP_H_FUNCTION_TEF_TYPE_ERASED_FUNCTION
