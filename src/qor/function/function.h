// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_FUNCTION_TEF_FUNCTION
#define QOR_PP_H_FUNCTION_TEF_FUNCTION

#include <tuple>

#include "function_traits.h"
#include "port.h"
#include "type_erased_function.h"
#include "type_traits.h"

namespace qor { namespace tef {

    // Forward declaration.
    template <typename FuncPtr>
    class MemberFunction;

    // Type encapsulating callable functions of a given type.
    //
    // \tparam Func A function type or a function pointer type.
    template <typename FuncPtr>
    class Function;

    // Specialization for function types.
    template <typename Return, typename... Args>
    class Function<Return(Args...)> : public TypeErasedFunction 
    {
        public:
        using FunctionType = Return(Args...);
        using FunctionPointerType = Return (*)(Args...);
        using ReturnType = Return;
        using ArgTypes = std::tuple<Args...>;
        enum : size_t 
        { 
            kNumArgs = sizeof...(Args) 
        };

        // Creates an empty typed Function.
        Function() noexcept;

        // Creates a new Function from the address of a free or static function.
        // The use of the qor_pp_make_function macro is recommended to deduce the type of
        // the function and simplify the syntax of call. For example:
        //
        // void Foo(int x);
        // auto function = qor_pp_make_function(&Foo); // Function<void(int)>.
        template <FunctionPointerType func_ptr>
        static Function FromFunction() noexcept;

        // Creates a new Function by binding a member function to an object pointer.
        // The caller must ensure the validity of the provided object pointer at the
        // time of any call. No ownership of the object is taken.
        //
        // Raises an error if the provided object pointer is nullptr.
        //
        // The use of the qor_pp_make_function macro is recommended to deduce the type of
        // the function and simplify the syntax of call. For example:
        //
        // struct Object {
        //   void Foo(int x);
        //   void Bar(int x) const;
        // };
        //
        // // Deduced as Function<void(int)>.
        // Object object;
        // auto function = qor_pp_make_function(&Object::Foo, &object);
        //
        // // Deduced as Function<void(int)> too. Objects must respect constness.
        // const Object const_object;
        // auto const_function1 = qor_pp_make_function(&Object::Bar, &object);
        // auto const_function2 = qor_pp_make_function(&Object::Bar, &const_object);
        //
        // However, qor_pp_make_function does not work when provided with an overloaded
        // function because it does not know which one to use. In these cases the full
        // function syntax must be used to disambiguate. For example:
        //
        // struct Object {
        //   int Foo() { return 1; }
        //   int Foo(int) { return 2; }
        //
        //   int Bar() { return 3; }
        //   int Bar() const { return 4; }
        // };
        //
        // Object object;
        //
        // // Overloads with a different signature type.
        // auto function_foo_1 = Function<void()>::FromMemberFunction<
        //     Object, &Object::Foo>(&object); // Returns 1 if invoked.
        //
        // auto function_foo_2 = Function<void(int)>::FromMemberFunction<
        //     Object, &Object::Foo>(&object); // Returns 2 if invoked.
        //
        // // Overloads with different const and volatile qualifiers.
        // auto function_bar_1 = Function<void()>::FromMemberFunction<
        //     Object, &Object::Bar>(&object); // Returns 3 if invoked.
        //
        // auto function_bar_2 = Function<void()>::FromMemberFunction<
        //     const Object, &Object::Bar>(&object); // Returns 4 if invoked.
        //
        // Note that both the Function signature and the Object qualifiers must match
        // in order for a member function to be found.
        template <typename Object, CopyCV<FunctionType, Object> Object::*func_ptr>
        static Function FromMemberFunction(Object* object);

        // Creates a new Function by binding a member function to an object shared
        // pointer. The Function will hold a copy of the shared pointer, keeping the
        // object alive during the Function's lifetime. Copying the Function will
        // create additional copies of the object shared pointer. This is the case
        // even when the Function object is type-erased.
        //
        // Raises an error if the provided shared pointer is null.
        //
        // The use of the qor_pp_make_function macro is recommended to deduce the type of
        // the function and simplify the syntax of call. For example:
        //
        // struct Object {
        //   void Foo(int x);
        //   void Bar(int x) const;
        // };
        //
        // // Deduced as Function<void(int)>.
        // auto object = std::make_shared<Object>();
        // auto function = qor_pp_make_function(&Object::Foo, object);
        //
        // // Deduced as Function<void(int)> too. Objects must respect constness.
        // auto const_object = std::make_shared<const Object>();
        // auto const_function_1 = qor_pp_make_function(&Object::Bar, object);
        // auto const_function_2 = qor_pp_make_function(&Object::Bar, const_object);
        //
        // However, qor_pp_make_function does not work when provided with an overloaded
        // function because it does not know which one to use. In these cases the full
        // function syntax must be used to disambiguate. For example:
        //
        // struct Object {
        //   int Foo() { return 1; }
        //   int Foo(int) { return 2; }
        //
        //   int Bar() { return 3; }
        //   int Bar() const { return 4; }
        // };
        //
        // auto object = std::make_shared<Object>();
        //
        // // Overloads with a different signature type.
        // auto function_foo_1 = Function<void()>::FromMemberFunction<
        //     Object, &Object::Foo>(object); // Returns 1 if invoked.
        //
        // auto function_foo_2 = Function<void(int)>::FromMemberFunction<
        //     Object, &Object::Foo>(object); // Returns 2 if invoked.
        //
        // // Overloads with different const and volatile qualifiers.
        // auto function_bar_1 = Function<void()>::FromMemberFunction<
        //     Object, &Object::Bar>(object); // Returns 3 if invoked.
        //
        // auto function_bar_2 = Function<void()>::FromMemberFunction<
        //     const Object, &Object::Bar>(object); // Returns 4 if invoked.
        //
        // Note that both the Function signature and the Object qualifiers must match
        // in order for a member function to be found.
        template <typename Object, CopyCV<FunctionType, Object> Object::*func_ptr>
        static Function FromMemberFunction(const std::shared_ptr<Object>& object);

        // Creates a new Function that binds a MemberFunction to a pointer of an
        // externally managed object. The caller is responsible to ensure the pointer
        // is valid by the time any call is made. No object ownership is taken.
        //
        // Raises an error if the provided object pointer is nullptr.
        //
        // Example:
        // struct Object {
        //   void Foo(int x);
        //   void Bar(int x) const;
        // };
        //
        // Object object;
        // auto member_function = MakeFunction(&Object::Foo);
        // Function<void(int)> function(member_function, &object);
        //
        // const Object const_object;
        // auto const_member_function = MakeFunction(&Object::Bar);
        // Function<void(int)> function_const_1(const_member_function, &object);
        // Function<void(int)> function_const_2(const_member_function, &const_object);
        template <typename MemberFuncPtr, typename Object,
                typename = std::enable_if_t<std::is_same<FunctionType,
                    typename FunctionTraits<MemberFuncPtr>::FunctionType>::value>>
        Function(const MemberFunction<MemberFuncPtr>& function, Object* object);

        // Creates a new Function that binds a MemberFunction to an object shared
        // pointer. The Function makes a copy of the shared pointer, ensuring the
        // object is kept alive for as long the Function is.
        //
        // Raises an error if the provided shared pointer is null.
        //
        // Example:
        // struct Object {
        //   void Foo(int x);
        //   void Bar(int x) const;
        // };
        //
        // auto member_function = MakeFunction(&Object::Foo);
        // auto object = std::make_shared<Object>();
        // Function<void(int)> function(member_function, object);
        //
        // auto const_member_function = MakeFunction(&Object::Bar);
        // auto object_const = std::make_shared<const Object>();
        // Function<void(int)> function_const_1(const_member_function, object);
        // Function<void(int)> function_const_2(const_member_function, const_object);
        template <typename MemberFuncPtr, typename Object,
                typename = std::enable_if_t<std::is_same<FunctionType,
                    typename FunctionTraits<MemberFuncPtr>::FunctionType>::value>>
        Function(const MemberFunction<MemberFuncPtr>& function, const std::shared_ptr<Object>& object);

        // Universal reference constructor for callable objects, including lambdas.
        //
        // Callable objects are stored in the heap, either copied or moved depending
        // on how this method is invoked. The callable type must be copy-constructible
        // and implement an operator () that has argument and return types that are
        // convertible to the function ones.
        //
        // This method can also be used to take an std::function or the result of a
        // std::bind, but note that this does not bring any performance improvements.
        //
        // Since this method takes a universal reference, the callable object can be
        // a lvalue reference or a rvalue reference. Similarly the Callable type can
        // be a reference type.
        //
        // This constructor is intentionally non-explicit to allow the syntax in the
        // example below.
        //
        // Example:
        // Function<void(int)> function = [](int x) { std::cout << x << std::endl; };
        template <typename Callable,
                typename = std::enable_if_t<
                    !IsFunction<Callable>::value &&
                    !IsMemberFunction<Callable>::value &&
                    !std::is_base_of<TypeErasedFunction, Callable>::value>>
        Function(Callable&& callable);

        // Universal reference assignment operator for compatible callable objects.
        //
        // Callable objects are stored in the heap, either copied or moved depending
        // on how this method is invoked. The callable type must be copy-constructible
        // and implement an operator () that has argument and return types that are
        // convertible to the function ones.
        //
        // Since this method takes a universal reference, the callable object can be
        // a lvalue reference or a rvalue reference. Similarly the Callable type can
        // be a reference type.
        //
        // Example:
        // Function<void(int)> function;
        // function = [](int x) { std::cout << x << std::endl; };
        template <typename Callable,
                typename = std::enable_if_t<
                    !IsFunction<Callable>::value &&
                    !IsMemberFunction<Callable>::value &&
                    !std::is_base_of<TypeErasedFunction, Callable>::value>>
        Function& operator =(Callable&& callable);

        Function& operator =(std::nullptr_t null);          // Assignment to nullptr. Clears the function object.
        
        Return operator ()(Args... args) const;             // Invokes the function returning its result.

    private:

    
        template <typename FuncPtr>
        friend class MemberFunction;                            // For access to CallMemberFuncAddress.

        // Auxiliary constructor used as part of creating Functions from function
        // addresses and member function addresses bound to objects.
        explicit Function(TypeErasedFuncPtr func_ptr) noexcept;

        // Calls a function address provided as a template argument.
        template <FunctionPointerType func_ptr>
        static Return CallFunctionAddress(void* object, Args... args);

        // Calls a member function with its address as a template argument.
        // Also used by MemberFunction in order to avoid specializing qualified
        // member function pointers in order to get the Args variadic argument pack.
        template <typename MemberFuncPtr, MemberFuncPtr func_ptr,
                typename = std::enable_if_t<
                    std::is_member_function_pointer<MemberFuncPtr>::value, Return>>
        static Return CallMemberFuncAddress(void* object, Args... args);

        // Calls the appropriate operator () of a callable object.
        template <typename Callable>
        static Return CallCallable(void* object, Args... args);
    };

}}//qor::tef

#include "function.hpp"

#endif  // QOR_PP_H_FUNCTION_TEF_FUNCTION
