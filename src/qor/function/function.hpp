// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_FUNCTION_TEF_FUNCTION_IMPL
#define QOR_PP_H_FUNCTION_TEF_FUNCTION_IMPL

#include "type_id.h"

namespace qor { namespace tef {

    // Does an extra reinterpret cast to void* to avoid MSVC 2015 warnings.
    template <typename T, typename U>
    T reinterpret_func(U&& target) 
    {
        return reinterpret_cast<T>(reinterpret_cast<void*>(target));
    }

    // Default constructor.
    template <typename Return, typename... Args>
    Function<Return(Args...)>::Function() noexcept : TypeErasedFunction(get_type_id<FunctionType>()) { }

    // Factory method for function addresses.
    template <typename Return, typename... Args>
    template <Return (*func_ptr)(Args...)>
    Function<Return(Args...)> Function<Return(Args...)>::FromFunction() noexcept 
    {
        return Function(reinterpret_func<TypeErasedFuncPtr>(&CallFunctionAddress<func_ptr>));
    }

    // Factory function for member function addresses bound to an object pointer.
    template <typename Return, typename... Args>
    template <typename Object, CopyCV<Return(Args...), Object> Object::*func_ptr>
    Function<Return(Args...)> Function<Return(Args...)>::FromMemberFunction(Object* object) 
    {
        debug_check(object, Error::kInvalidObject);
        auto function = Function(reinterpret_func<TypeErasedFuncPtr>(&CallMemberFuncAddress<decltype(func_ptr), func_ptr>));
        function.object_.StorePointer(object);
        return function;
    }

    // Factory function for member function addresses bound to an object shared_ptr.
    template <typename Return, typename... Args>
    template <typename Object, CopyCV<Return(Args...), Object> Object::*func_ptr>
    Function<Return(Args...)> Function<Return(Args...)>::FromMemberFunction(const std::shared_ptr<Object>& object) 
    {
        debug_check(object, Error::kInvalidObject);
        auto function = Function(reinterpret_func<TypeErasedFuncPtr>(
            &CallMemberFuncAddress<decltype(func_ptr), func_ptr>));
        function.object_.StoreObject(object);
        return function;
    }

    // Constructor for MemberFunction objects bound to an object pointer.
    template <typename Return, typename... Args>
    template <typename MemberFuncPtr, typename Object, typename>
    Function<Return(Args...)>::Function(const MemberFunction<MemberFuncPtr>& member_function, Object* object) : 
        TypeErasedFunction(get_type_id<FunctionType>()) 
    {
        // Class is qualified as the member function and Object as the object.
        // This enforces const compatibility and produces more useful build errors.
        typename FunctionTraits<MemberFuncPtr>::Class* class_ptr = object;
        debug_check(class_ptr, Error::kInvalidObject);
        func_ptr_ = member_function.func_ptr_;
        object_.StorePointer(class_ptr);
    }

    // Constructor for MemberFunction objects bound to an object shared pointer.
    template <typename Return, typename... Args>
    template <typename MemberFuncPtr, typename Object, typename>
    Function<Return(Args...)>::Function(const MemberFunction<MemberFuncPtr>& member_function,const std::shared_ptr<Object>& object) : 
        TypeErasedFunction(get_type_id<FunctionType>()) 
    {
        // Class is qualified as the member function and Object as the object.
        // This enforces const compatibility and produces more useful build errors.
        using Class = typename FunctionTraits<MemberFuncPtr>::Class;
        const std::shared_ptr<Class>& class_object = object;
        debug_check(class_object, Error::kInvalidObject);
        func_ptr_ = member_function.func_ptr_;
        object_.StoreObject(class_object);
    }

    // Constructor for compatible callable objects.
    template <typename Return, typename... Args>
    template <typename Callable, typename>
    Function<Return(Args...)>::Function(Callable&& callable) : 
        TypeErasedFunction(get_type_id<FunctionType>(),reinterpret_func<TypeErasedFuncPtr>(&CallCallable<Callable>)) 
    {
        // Store the callable object within the function or owned by it in the heap.
        object_.StoreObject(std::forward<Callable>(callable));
    }

    // Auxiliary constructor for factory methods based on function addresses and
    // member function addresses bound to objects.
    template <typename Return, typename... Args>
    Function<Return(Args...)>::Function(TypeErasedFuncPtr func_ptr) noexcept : 
        TypeErasedFunction(get_type_id<FunctionType>(), func_ptr){ }

    // Assignment operator for compatible callable objects.
    template <typename Return, typename... Args>
    template <typename Callable, typename>
    Function<Return(Args...)>& Function<Return(Args...)>::operator =(Callable&& callable) 
    {
        //using T = std::remove_reference_t<Callable>;
        func_ptr_ = reinterpret_func<TypeErasedFuncPtr>(&CallCallable<Callable>);
        object_.StoreObject(std::forward<Callable>(callable));
        return *this;
    }

    // Assignment operator to nullptr.
    template <typename Return, typename... Args>
    Function<Return(Args...)>& Function<Return(Args...)>::operator =(std::nullptr_t null) 
    {
        TypeErasedFunction::operator =(null);
        return *this;
    }

    // Parenthesis operator for calling functions.
    template <typename Return, typename... Args>
    Return Function<Return(Args...)>::operator ()(Args... args) const 
    {
        debug_check(func_ptr_, Error::kInvalidFunction);

        // Invoke whatever helper function is set.
        // Each one will take care of undoing type erasure and calling.
        return (*reinterpret_func<Return (*)(void*, Args...)>(func_ptr_))(object_.GetObject(), std::forward<Args>(args)...);
    }

    // Auxiliary function to forward calls to function addresses provided as
    // template arguments.
    template <typename Return, typename... Args>
    template <Return (*func_ptr)(Args...)>
    Return Function<Return(Args...)>::CallFunctionAddress(void*, Args... args) 
    {
        return func_ptr(std::forward<Args>(args)...);
    }

    // Auxiliary function to recover from type erasure and call a member function
    // address with the provided object.
    template <typename Return, typename... Args>
    template <typename MemberFuncPtr, MemberFuncPtr func_ptr, typename>
    Return Function<Return(Args...)>::CallMemberFuncAddress(void* object, Args... args) 
    {
        debug_check(object, Error::kInvalidObject);
        using Class = typename FunctionTraits<MemberFuncPtr>::Class;
        return (reinterpret_cast<Class*>(object)->*func_ptr)(std::forward<Args>(args)...);
    }

    template <typename Return, typename... Args>
    template <typename Callable>
    Return Function<Return(Args...)>::CallCallable(void* object, Args... args) 
    {
        //TODO:if(!object){ continuable(tefError.at(Error::kInvalidObject)); }
        using Object = std::remove_reference_t<Callable>;
        return reinterpret_cast<Object*>(object)->operator()(std::forward<Args>(args)...);
    }

}}//qor::tef

#endif//QOR_PP_H_FUNCTION_TEF_FUNCTION_IMPL
