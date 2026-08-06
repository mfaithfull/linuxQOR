// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

// This test needs C++ exceptions thrown by MagicFunc exceptions to work.
// These exceptions are turned off in release builds that define NDEBUG.
#undef NDEBUG

#include "src/configuration/configuration.h"
#include <functional>
#include <type_traits>

#include "src/qor/tdd/tdd.h"

#include "src/qor/function/error.h"
#include "src/qor/function/function.h"
#include "src/qor/function/function_cast.h"
#include "src/qor/function/make_function.h"
#include "src/qor/function/type_id.h"

#include "test_common.h"

using namespace qor;
using namespace qor::tef;

// Some versions of llvm-libc++ are affected by a bug that prevents from
// creating a volatile shared pointer. This affects some tests here.
//
// https://llvm.org/bugs/show_bug.cgi?id=23647
//
// Define this macro in order to disable such tests.
//#define DISABLE_VOLATILE_SHARED_PTRS

struct TypeErasedFunctionTestSuite{};

qor_pp_test_suite_case(TypeErasedFunctionTestSuite, FunctionEmpty) 
{
    Function<void()> function;
    qor_pp_assert_that(!function);
    qor_pp_assert_that(nullptr == function.GetObject());
    qor_pp_assert_that(get_type_id<void()>() == function.type_id());

    // Test calling an empty function.
    try 
    {
        function();        
    } 
    catch(const Serious& error) 
    {
        qor_pp_assert_that(error.what().Content() == tefError.at(Error::kInvalidFunction)).isTrue();        
    }
}

qor_pp_test_suite_case(TypeErasedFunctionTestSuite, FreeFunctionCall) 
{
    // Try creating a function with its explicit type and calling it.
    Function<int(bool&, bool&&)> function = qor_pp_make_function(&FreeFunction);
    qor_pp_assert_that((std::is_same<decltype(&FreeFunction),
                typename decltype(function)::FunctionPointerType>::value));

    qor_pp_assert_that(!!function);
    qor_pp_assert_that(nullptr == function.GetObject());
    qor_pp_assert_that((get_type_id<int(bool&, bool&&)>()) == function.type_id());

    bool called = false;
    qor_pp_assert_that(42 == function(called, true));
    qor_pp_assert_that(called);

    // Try calling a copy of the function.
    auto function_copy = function;
    called = false;
    qor_pp_assert_that(42 == function_copy(called, true));
    qor_pp_assert_that(called);

    // Try calling a move of the function.
    auto function_move = std::move(function_copy);
    called = false;
    qor_pp_assert_that(42 == function_move(called, true));
    qor_pp_assert_that(called);

    // Reset the function.
    function = nullptr;
    qor_pp_assert_that(!function);
    qor_pp_assert_that(nullptr == function.GetObject());
    qor_pp_assert_that((get_type_id<int(bool&, bool&&)>()) == function.type_id());
}

qor_pp_test_suite_case(TypeErasedFunctionTestSuite, LambdaCall) 
{
    // Try creating a function directly from a lambda and calling it.
    int id = rand();
    auto lambda = [=](bool& called, bool&& value) 
    {
        called = std::move(value);
        return id;
    };

    Function<int(bool&, bool&&)> function(lambda);
    qor_pp_assert_that(!!function);
    qor_pp_assert_that(nullptr != function.GetObject());
    qor_pp_assert_that((get_type_id<int(bool&, bool&&)>()) == function.type_id());

    bool called = false;
    qor_pp_assert_that(id == function(called, true));
    qor_pp_assert_that(called);

    // Try explicitly assigning the lambda on function construction.
    Function<int(bool&, bool&&)> function_lambda_construct = lambda;

    called = false;
    qor_pp_assert_that(id == function_lambda_construct(called, true));
    qor_pp_assert_that(called);

    // Try usign copy assignment with the lambda.
    Function<int(bool&, bool&&)> function_lambda_copy;
    function_lambda_copy = lambda;

    called = false;
    qor_pp_assert_that(id == function_lambda_copy(called, true));
    qor_pp_assert_that(called);

    // Try usign move assignment with the lambda.
    Function<int(bool&, bool&&)> function_lambda_move;
    function_lambda_move = std::move(function_lambda_copy);

    called = false;
    qor_pp_assert_that(id == function_lambda_move(called, true));
    qor_pp_assert_that(called);

    // Try assigning a const lambda.
    const auto& const_lambda = lambda;
    Function<int(bool&, bool&&)> function_const_lambda;
    function_const_lambda = const_lambda;

    called = false;
    qor_pp_assert_that(id == function_const_lambda(called, true));
    qor_pp_assert_that(called);

    // Try calling a copy of the function.
    auto function_copy = function;
    called = false;
    qor_pp_assert_that(id == function_copy(called, true));
    qor_pp_assert_that(called);

    // Try calling a move of the function.
    auto function_move = std::move(function_copy);
    called = false;
    qor_pp_assert_that(id == function_move(called, true));
    qor_pp_assert_that(called);

    // Reset the function.
    function = nullptr;
    qor_pp_assert_that(!function);
    qor_pp_assert_that(nullptr == function.GetObject());
    qor_pp_assert_that((get_type_id<int(bool&, bool&&)>()) == function.type_id());
}

qor_pp_test_suite_case(TypeErasedFunctionTestSuite, LambdaConvertible) 
{
    // Test that functions can be initialized to lambdas as long as argument and
    // return types are convertible.
    auto lambda = [](const std::string& str, int i) { return str[i]; };

    qor_pp_assert_that((std::is_same<decltype(make_function(lambda)), Function<char(const std::string&, int)>>::value));

    {
        // Test that functions can be initialized to lambdas as long as they
        // argument and result types are convertible.
        Function<short(const char[], float)> function = lambda;
        qor_pp_assert_that((short)('e') == function("test", 1));
    }

    {
        // Test that functions can be assigned to lambdas as long as they argument
        // and result types are convertible.
        Function<short(const char[], float)> function;
        function = lambda;
        qor_pp_assert_that((short)('e') == function("test", 1));
    }
}

qor_pp_test_suite_case(TypeErasedFunctionTestSuite, LambdaMutable) 
{
    // Create a function for a mutable lambda.
    size_t call_count = 0;
    auto function = make_function([=]() mutable { return call_count++; });

    // Call it 50 times.
    for (size_t i = 0; i < 50; ++i)
    qor_pp_assert_that(i == function());

    // Copy the function. This should make a copy of the stored lambda object.
    auto function_copy = function;

    // Call the original 50 more times.
    for (size_t i = 0; i < 50; ++i)
    qor_pp_assert_that(50 + i == function());

    // If we call the copy, its state should be separate from the original.
    qor_pp_assert_that(100 == function());
    qor_pp_assert_that(50 == function_copy());

    // If we move the copy, we replace the state of the original.
    function = std::move(function_copy);
    qor_pp_assert_that(51 == function());
}

qor_pp_test_suite_case(TypeErasedFunctionTestSuite, CallableOverload) 
{
    // Test calling the non-qualified operator ().
    {
        int id = rand();
        OverloadedCallable callable(id);
        Function<int(CVQualification&)> function = callable;

        CVQualification cv = CVQualification::kUndefined;
        qor_pp_assert_that(id == function(cv));
        qor_pp_assert_that(CVQualification::kNonQualified == cv);
    }

    // Test calling the const operator ().
    {
        int id = rand();
        const OverloadedCallable callable(id);
        Function<int(CVQualification&)> function = callable;

        CVQualification cv = CVQualification::kUndefined;
        qor_pp_assert_that(id == function(cv));
        qor_pp_assert_that(CVQualification::kConstQualified == cv);
    }

    // Test calling the volatile operator ().
    {
        int id = rand();
        volatile OverloadedCallable callable(id);
        Function<int(CVQualification&)> function = callable;

        CVQualification cv = CVQualification::kUndefined;
        qor_pp_assert_that(id == function(cv));
        qor_pp_assert_that(CVQualification::kVolatileQualified == cv);
    }

    // Test calling the const volatile operator ().
    {
        int id = rand();
        const volatile OverloadedCallable callable(id);
        Function<int(CVQualification&)> function = callable;

        CVQualification cv = CVQualification::kUndefined;
        qor_pp_assert_that(id == function(cv));
        qor_pp_assert_that(CVQualification::kConstVolatileQualified == cv);
    }
}

qor_pp_test_suite_case(TypeErasedFunctionTestSuite, FromMemberFunctionAndObjectPointer) 
{
    // Create a MemberFunction pointing to a member function.
    auto member_function = qor_pp_make_function(&Object::Function);
    qor_pp_assert_that(IsMemberFunction<decltype(member_function)>::value);
    qor_pp_assert_that(!!member_function);
    qor_pp_assert_that(!decltype(member_function)::kIsConst);
    qor_pp_assert_that(!decltype(member_function)::kIsVolatile);
    qor_pp_assert_that(nullptr == member_function.GetObject());
    qor_pp_assert_that((get_type_id<decltype(&Object::Function)>()) == member_function.type_id());

    // Create a MemberFunction pointing to a const member function.
    auto const_member_function = qor_pp_make_function(&Object::ConstFunction);
    qor_pp_assert_that(IsMemberFunction<decltype(const_member_function)>::value);
    qor_pp_assert_that(!!const_member_function);
    qor_pp_assert_that(!!decltype(const_member_function)::kIsConst);
    qor_pp_assert_that(!decltype(const_member_function)::kIsVolatile);
    qor_pp_assert_that(nullptr == const_member_function.GetObject());
    qor_pp_assert_that((get_type_id<decltype(&Object::ConstFunction)>()) == const_member_function.type_id());

    // Create a MemberFunction pointing to a volatile member function.
    auto volatile_member_function = qor_pp_make_function(&Object::VolatileFunction);
    qor_pp_assert_that(IsMemberFunction<decltype(volatile_member_function)>::value);
    qor_pp_assert_that(!!volatile_member_function);
    qor_pp_assert_that(!decltype(volatile_member_function)::kIsConst);
    qor_pp_assert_that(!!decltype(volatile_member_function)::kIsVolatile);
    qor_pp_assert_that(nullptr == volatile_member_function.GetObject());
    qor_pp_assert_that((get_type_id<decltype(&Object::VolatileFunction)>()) == volatile_member_function.type_id());

    // Create a MemberFunction pointing to a const_volatile member function.
    auto const_volatile_member_function = qor_pp_make_function(&Object::ConstVolatileFunction);
    qor_pp_assert_that(IsMemberFunction<decltype(const_volatile_member_function)>::value);
    qor_pp_assert_that(!!const_volatile_member_function);
    qor_pp_assert_that(!!decltype(const_volatile_member_function)::kIsConst);
    qor_pp_assert_that(!!decltype(const_volatile_member_function)::kIsVolatile);
    qor_pp_assert_that(nullptr == const_volatile_member_function.GetObject());
    qor_pp_assert_that((get_type_id<decltype(&Object::ConstVolatileFunction)>()) == const_volatile_member_function.type_id());

    {
        // Test creating a Function from a MemberFunction pointing to a member
        // function and binding an object pointer to it.
        int id = rand();
        Object object(id);
        Function<int(bool&, bool&&)> function(member_function, &object);

        qor_pp_assert_that(!!function);
        qor_pp_assert_that(&object == function.GetObject());
        qor_pp_assert_that((get_type_id<int(bool&, bool&&)>()) == function.type_id());

        bool called = false;
        qor_pp_assert_that(id == function(called, true));
        qor_pp_assert_that(called);
    }

    {
        // Test creating a Function from a MemberFunction pointing to a member const
        // function and binding an object pointer to it.
        int id = rand();
        Object object(id);
        Function<int(bool&, bool&&)> function(const_member_function, &object);

        qor_pp_assert_that(!!function);
        qor_pp_assert_that(&object == function.GetObject());
        qor_pp_assert_that((get_type_id<int(bool&, bool&&)>()) == function.type_id());

        bool called = false;
        qor_pp_assert_that(id == function(called, true));
        qor_pp_assert_that(called);
    }

    {
        // Test creating a Function from a MemberFunction pointing to a member
        // volatile function and binding an object pointer to it.
        int id = rand();
        Object object(id);
        Function<int(bool&, bool&&)> function(volatile_member_function, &object);

        qor_pp_assert_that(!!function);
        qor_pp_assert_that(&object == function.GetObject());
        qor_pp_assert_that((get_type_id<int(bool&, bool&&)>()) == function.type_id());

        bool called = false;
        qor_pp_assert_that(id == function(called, true));
        qor_pp_assert_that(called);
    }

    {
        // Test creating a Function from a MemberFunction pointing to a member const
        // volatile function and binding an object pointer to it.
        int id = rand();
        Object object(id);
        Function<int(bool&, bool&&)> function(
        const_volatile_member_function, &object);

        qor_pp_assert_that(!!function);
        qor_pp_assert_that(&object == function.GetObject());
        qor_pp_assert_that((get_type_id<int(bool&, bool&&)>()) == function.type_id());

        bool called = false;
        qor_pp_assert_that(id == function(called, true));
        qor_pp_assert_that(called);
    }

    {
        // Test creating a Function from a MemberFunction pointing to a member const
        // function and binding a const object pointer to it.
        int id = rand();
        const Object object(id);
        Function<int(bool&, bool&&)> function(const_member_function, &object);

        qor_pp_assert_that(!!function);
        qor_pp_assert_that(&object == function.GetObject());
        qor_pp_assert_that((get_type_id<int(bool&, bool&&)>()) == function.type_id());

        bool called = false;
        qor_pp_assert_that(id == function(called, true));
        qor_pp_assert_that(called);
    }

    {
        // Test creating a Function from a MemberFunction pointing to a member const
        // volatile function and binding a const object pointer to it.
        int id = rand();
        const Object object(id);
        Function<int(bool&, bool&&)> function(
        const_volatile_member_function, &object);

        qor_pp_assert_that(!!function);
        qor_pp_assert_that(&object == function.GetObject());
        qor_pp_assert_that((get_type_id<int(bool&, bool&&)>()) == function.type_id());

        bool called = false;
        qor_pp_assert_that(id == function(called, true));
        qor_pp_assert_that(called);
    }

    {
        // Test creating a Function from a MemberFunction pointing to a member
        // volatile function and binding a volatile object pointer to it.
        int id = rand();
        volatile Object object(id);
        Function<int(bool&, bool&&)> function(volatile_member_function, &object);

        qor_pp_assert_that(!!function);
        qor_pp_assert_that(&object == function.GetObject());
        qor_pp_assert_that((get_type_id<int(bool&, bool&&)>()) == function.type_id());

        bool called = false;
        qor_pp_assert_that(id == function(called, true));
        qor_pp_assert_that(called);
    }

    {
        // Test creating a Function from a MemberFunction pointing to a member
        // const volatile function and binding a const volatile object pointer.
        int id = rand();
        const volatile Object object(id);
        Function<int(bool&, bool&&)> function(
        const_volatile_member_function, &object);

        qor_pp_assert_that(!!function);
        qor_pp_assert_that(&object == function.GetObject());
        qor_pp_assert_that((get_type_id<int(bool&, bool&&)>()) == function.type_id());

        bool called = false;
        qor_pp_assert_that(id == function(called, true));
        qor_pp_assert_that(called);
    }

    {
        // Test calling after copying and moving the function.
        int id = rand();
        Object object(id);
        Function<int(bool&, bool&&)> function(member_function, &object);

        auto function_copy = function;
        qor_pp_assert_that(!!function_copy);
        qor_pp_assert_that(&object == function_copy.GetObject());
        qor_pp_assert_that((get_type_id<int(bool&, bool&&)>()) == function_copy.type_id());

        bool called = false;
        qor_pp_assert_that(id == function_copy(called, true));
        qor_pp_assert_that(called);

        auto function_move = std::move(function_copy);
        qor_pp_assert_that(!!function_move);
        qor_pp_assert_that(&object == function_move.GetObject());
        qor_pp_assert_that((get_type_id<int(bool&, bool&&)>()) == function_move.type_id());

        called = false;
        qor_pp_assert_that(id == function_move(called, true));
        qor_pp_assert_that(called);
    }

    {
        // Test creating a function with an invalid object.
        try 
        {
            Object* object = nullptr;
            Function<int(bool&, bool&&)> function(member_function, object);            
        } 
        catch(const Serious& error) 
        {
            qor_pp_assert_that(error.what().Content() == tefError.at(Error::kInvalidObject)).isTrue();
        }
    }
}

qor_pp_test_suite_case(TypeErasedFunctionTestSuite, FromMemberFunctionAndObjectPointerVirtual) 
{
    auto member_function = qor_pp_make_function(&Object::VirtualFunction);
    qor_pp_assert_that(IsMemberFunction<decltype(member_function)>::value);

    qor_pp_assert_that(!!member_function);
    qor_pp_assert_that(nullptr == member_function.GetObject());
    qor_pp_assert_that((get_type_id<decltype(&Object::VirtualFunction)>()) == member_function.type_id());

    {
        // Test calling a virtual function using a base object.
        int id = rand();
        Object object(id);
        Function<int(bool&, bool&&, bool*)> function(member_function, &object);
        qor_pp_assert_that(!!function);
        qor_pp_assert_that(&object == function.GetObject());
        qor_pp_assert_that((get_type_id<int(bool&, bool&&, bool*)>()) == function.type_id());

        bool called = false;
        bool derived = false;
        qor_pp_assert_that(id == function(called, true, &derived));
        qor_pp_assert_that(called);
        qor_pp_assert_that(!derived);
    }

    {
        // Test calling a virtual function using a derived object.
        int id = rand();
        DerivedObject derived_object(id);
        Function<int(bool&, bool&&, bool*)> function(member_function, &derived_object);
        qor_pp_assert_that(!!function);
        qor_pp_assert_that(&derived_object == function.GetObject());
        qor_pp_assert_that((get_type_id<int(bool&, bool&&, bool*)>()) == function.type_id());

        bool called = false;
        bool derived = false;
        qor_pp_assert_that(id == function(called, true, &derived));
        qor_pp_assert_that(called);
        qor_pp_assert_that(derived);
    }
}

qor_pp_test_suite_case(TypeErasedFunctionTestSuite, FromMemberFunctionAndSharedPointer) 
{
    // Create a MemberFunction pointing to a member function.
    auto member_function = qor_pp_make_function(&Object::Function);
    qor_pp_assert_that(IsMemberFunction<decltype(member_function)>::value);
    qor_pp_assert_that(!!member_function);
    qor_pp_assert_that(!decltype(member_function)::kIsConst);
    qor_pp_assert_that(!decltype(member_function)::kIsVolatile);
    qor_pp_assert_that(nullptr == member_function.GetObject());
    qor_pp_assert_that((get_type_id<decltype(&Object::Function)>()) == member_function.type_id());

    // Create a MemberFunction pointing to a const member function.
    auto const_member_function = qor_pp_make_function(&Object::ConstFunction);
    qor_pp_assert_that(IsMemberFunction<decltype(const_member_function)>::value);
    qor_pp_assert_that(!!const_member_function);
    qor_pp_assert_that(!!decltype(const_member_function)::kIsConst);
    qor_pp_assert_that(!decltype(const_member_function)::kIsVolatile);
    qor_pp_assert_that(nullptr == const_member_function.GetObject());
    qor_pp_assert_that((get_type_id<decltype(&Object::ConstFunction)>()) == const_member_function.type_id());

    // Create a MemberFunction pointing to a volatile member function.
    auto volatile_member_function = qor_pp_make_function(&Object::VolatileFunction);
    qor_pp_assert_that(IsMemberFunction<decltype(volatile_member_function)>::value);
    qor_pp_assert_that(!!volatile_member_function);
    qor_pp_assert_that(!decltype(volatile_member_function)::kIsConst);
    qor_pp_assert_that(!!decltype(volatile_member_function)::kIsVolatile);
    qor_pp_assert_that(nullptr == volatile_member_function.GetObject());
    qor_pp_assert_that((get_type_id<decltype(&Object::VolatileFunction)>()) == volatile_member_function.type_id());

    // Create a MemberFunction pointing to a const_volatile member function.
    auto const_volatile_member_function =
    qor_pp_make_function(&Object::ConstVolatileFunction);
    qor_pp_assert_that(IsMemberFunction<
    decltype(const_volatile_member_function)>::value);
    qor_pp_assert_that(!!const_volatile_member_function);
    qor_pp_assert_that(!!decltype(const_volatile_member_function)::kIsConst);
    qor_pp_assert_that(!!decltype(const_volatile_member_function)::kIsVolatile);
    qor_pp_assert_that(nullptr == const_volatile_member_function.GetObject());
    qor_pp_assert_that((get_type_id<decltype(&Object::ConstVolatileFunction)>()) == const_volatile_member_function.type_id());

    {
        // Test creating a Function from a MemberFunction pointing to a member
        // function and binding an object shared pointer to it.
        int id = rand();
        auto object = std::make_shared<Object>(id);
        Function<int(bool&, bool&&)> function(member_function, object);

        qor_pp_assert_that(!!function);
        qor_pp_assert_that(object.get() == function.GetObject());
        qor_pp_assert_that((get_type_id<int(bool&, bool&&)>()) == function.type_id());

        bool called = false;
        qor_pp_assert_that(id == function(called, true));
        qor_pp_assert_that(called);
    }

    {
        // Test creating a Function from a MemberFunction pointing to a member const
        // function and binding an object shared pointer to it.
        int id = rand();
        auto object = std::make_shared<Object>(id);
        Function<int(bool&, bool&&)> function(const_member_function, object);

        qor_pp_assert_that(!!function);
        qor_pp_assert_that(object.get() == function.GetObject());
        qor_pp_assert_that((get_type_id<int(bool&, bool&&)>()) == function.type_id());

        bool called = false;
        qor_pp_assert_that(id == function(called, true));
        qor_pp_assert_that(called);
    }

    {
        // Test creating a Function from a MemberFunction pointing to a member
        // volatile function and binding an object shared pointer to it.
        int id = rand();
        auto object = std::make_shared<Object>(id);
        Function<int(bool&, bool&&)> function(volatile_member_function, object);

        qor_pp_assert_that(!!function);
        qor_pp_assert_that(object.get() == function.GetObject());
        qor_pp_assert_that((get_type_id<int(bool&, bool&&)>()) == function.type_id());

        bool called = false;
        qor_pp_assert_that(id == function(called, true));
        qor_pp_assert_that(called);
    }

    {
        // Test creating a Function from a MemberFunction pointing to a member const
        // volatile function and binding an object shared pointer to it.
        int id = rand();
        auto object = std::make_shared<Object>(id);
        Function<int(bool&, bool&&)> function(
        const_volatile_member_function, object);

        qor_pp_assert_that(!!function);
        qor_pp_assert_that(object.get() == function.GetObject());
        qor_pp_assert_that((get_type_id<int(bool&, bool&&)>()) == function.type_id());

        bool called = false;
        qor_pp_assert_that(id == function(called, true));
        qor_pp_assert_that(called);
    }

    {
        // Test creating a Function from a MemberFunction pointing to a member const
        // function and binding a const object shared pointer to it.
        int id = rand();
        auto object = std::make_shared<const Object>(id);
        Function<int(bool&, bool&&)> function(const_member_function, object);

        qor_pp_assert_that(!!function);
        qor_pp_assert_that(object.get() == function.GetObject());
        qor_pp_assert_that((get_type_id<int(bool&, bool&&)>()) == function.type_id());

        bool called = false;
        qor_pp_assert_that(id == function(called, true));
        qor_pp_assert_that(called);
    }

    {
        // Test creating a Function from a MemberFunction pointing to a member const
        // volatile function and binding a const object shared pointer to it.
        int id = rand();
        auto object = std::make_shared<const Object>(id);
        Function<int(bool&, bool&&)> function(
        const_volatile_member_function, object);

        qor_pp_assert_that(!!function);
        qor_pp_assert_that(object.get() == function.GetObject());
        qor_pp_assert_that((get_type_id<int(bool&, bool&&)>()) == function.type_id());

        bool called = false;
        qor_pp_assert_that(id == function(called, true));
        qor_pp_assert_that(called);
    }

#ifndef DISABLE_VOLATILE_SHARED_PTRS
    {
        // Test creating a Function from a MemberFunction pointing to a member
        // volatile function and binding a volatile object shared pointer to it.
        int id = rand();
        auto object = std::make_shared<volatile Object>(id);
        Function<int(bool&, bool&&)> function(volatile_member_function, object);

        qor_pp_assert_that(!!function);
        qor_pp_assert_that(object.get() == function.GetObject());
        qor_pp_assert_that((get_type_id<int(bool&, bool&&)>()) == function.type_id());

        bool called = false;
        qor_pp_assert_that(id == function(called, true));
        qor_pp_assert_that(called);
    }

    {
        // Test creating a Function from a MemberFunction pointing to a member const
        // volatile function and binding a const volatile object shared pointer.
        int id = rand();
        auto object = std::make_shared<const volatile Object>(id);
        Function<int(bool&, bool&&)> function(
        const_volatile_member_function, object);

        qor_pp_assert_that(!!function);
        qor_pp_assert_that(object.get() == function.GetObject());
        qor_pp_assert_that((get_type_id<int(bool&, bool&&)>()) == function.type_id());

        bool called = false;
        qor_pp_assert_that(id == function(called, true));
        qor_pp_assert_that(called);
    }
#endif  // DISABLE_VOLATILE_SHARED_PTRS

    {
        // Test calling after copying and moving the function.
        int id = rand();
        auto object = std::make_shared<Object>(id);
        auto member_function = qor_pp_make_function(&Object::Function);
        Function<int(bool&, bool&&)> function(member_function, object);

        auto function_copy = function;
        qor_pp_assert_that(!!function_copy);
        qor_pp_assert_that(object.get() == function_copy.GetObject());
        qor_pp_assert_that((get_type_id<int(bool&, bool&&)>()) == function_copy.type_id());

        bool called = false;
        qor_pp_assert_that(id == function_copy(called, true));
        qor_pp_assert_that(called);

        auto function_move = std::move(function_copy);
        qor_pp_assert_that(!!function_move);
        qor_pp_assert_that(object.get() == function_move.GetObject());
        qor_pp_assert_that((get_type_id<int(bool&, bool&&)>()) == function_move.type_id());

        called = false;
        qor_pp_assert_that(id  == function_move(called, true));
        qor_pp_assert_that(called);
    }

    {
        // Test creating a function with an invalid object.
        try 
        {
            std::shared_ptr<Object> object;
            Function<int(bool&, bool&&)> function(member_function, object);            
        } 
        catch(const Serious& error) 
        {
            qor_pp_assert_that(error.what().Content() == tefError.at(Error::kInvalidObject)).isTrue();
        }
    }

    {
        // Check that the functions keep the object alive.
        auto object = std::make_shared<Object>(0);
        Function<int(bool&, bool&&)> function(member_function, object);

        std::weak_ptr<Object> weak_ptr = object;
        object.reset();
        qor_pp_assert_that(!weak_ptr.expired());
        function = nullptr;
        qor_pp_assert_that(weak_ptr.expired());
    }
}

qor_pp_test_suite_case(TypeErasedFunctionTestSuite, FromMemberFunctionAndSharedPointerVirtual) 
{
    auto member_function = qor_pp_make_function(&Object::VirtualFunction);
    qor_pp_assert_that(IsMemberFunction<decltype(member_function)>::value);

    qor_pp_assert_that(!!member_function);
    qor_pp_assert_that(nullptr == member_function.GetObject());
    qor_pp_assert_that((get_type_id<decltype(&Object::VirtualFunction)>()) == member_function.type_id());

    {
        // Test calling a virtual function using a base object.
        int id = rand();
        auto object = std::make_shared<Object>(id);
        Function<int(bool&, bool&&, bool*)> function(member_function, object);
        qor_pp_assert_that(!!function);
        qor_pp_assert_that(object.get() == function.GetObject());
        qor_pp_assert_that((get_type_id<int(bool&, bool&&, bool*)>()) == function.type_id());

        bool called = false;
        bool derived = false;
        qor_pp_assert_that(id == function(called, true, &derived));
        qor_pp_assert_that(called);
        qor_pp_assert_that(!derived);
    }

    {
        // Test calling a virtual function using a derived object.
        int id = rand();
        auto derived_object = std::make_shared<DerivedObject>(id);
        Function<int(bool&, bool&&, bool*)> function(member_function, derived_object);
        qor_pp_assert_that(!!function);
        qor_pp_assert_that(derived_object.get() == function.GetObject());
        qor_pp_assert_that((get_type_id<int(bool&, bool&&, bool*)>()) == function.type_id());

        bool called = false;
        bool derived = false;
        qor_pp_assert_that(id == function(called, true, &derived));
        qor_pp_assert_that(called);
        qor_pp_assert_that(derived);
    }
}

qor_pp_test_suite_case(TypeErasedFunctionTestSuite, FromFunctionAddressAndObjectPointer) 
{
    {
        // Test creating a Function from a member function address binding and
        // object pointer to it.
        int id = rand();
        Object object(id);
        auto function = qor_pp_make_function(&Object::Function, &object);

        qor_pp_assert_that(!!function);
        qor_pp_assert_that(&object == function.GetObject());
        qor_pp_assert_that((get_type_id<int(bool&, bool&&)>()) == function.type_id());

        bool called = false;
        qor_pp_assert_that(id == function(called, true));
        qor_pp_assert_that(called);
    }

    {
        // Test creating a Function from a member const function address and binding
        // an object pointer to it.
        int id = rand();
        Object object(id);
        auto function = qor_pp_make_function(&Object::ConstFunction, &object);

        qor_pp_assert_that(!!function);
        qor_pp_assert_that(&object == function.GetObject());
        qor_pp_assert_that((get_type_id<int(bool&, bool&&)>()) == function.type_id());

        bool called = false;
        qor_pp_assert_that(id == function(called, true));
        qor_pp_assert_that(called);
    }

    {
        // Test creating a Function from a member volatile function address and
        // binding an object pointer to it.
        int id = rand();
        Object object(id);
        auto function = qor_pp_make_function(&Object::VolatileFunction, &object);

        qor_pp_assert_that(!!function);
        qor_pp_assert_that(&object == function.GetObject());
        qor_pp_assert_that((get_type_id<int(bool&, bool&&)>()) == function.type_id());

        bool called = false;
        qor_pp_assert_that(id == function(called, true));
        qor_pp_assert_that(called);
    }

    {
        // Test creating a Function from a member const volatile function address
        // and binding an object pointer to it.
        int id = rand();
        Object object(id);
        auto function = qor_pp_make_function(&Object::ConstVolatileFunction, &object);

        qor_pp_assert_that(!!function);
        qor_pp_assert_that(&object == function.GetObject());
        qor_pp_assert_that((get_type_id<int(bool&, bool&&)>()) == function.type_id());

        bool called = false;
        qor_pp_assert_that(id == function(called, true));
        qor_pp_assert_that(called);
    }

    {
        // Test creating a Function from a member const function address and binding
        // a const object pointer to it.
        int id = rand();
        const Object object(id);
        auto function = qor_pp_make_function(&Object::ConstFunction, &object);

        qor_pp_assert_that(!!function);
        qor_pp_assert_that(&object == function.GetObject());
        qor_pp_assert_that((get_type_id<int(bool&, bool&&)>()) == function.type_id());

        bool called = false;
        qor_pp_assert_that(id == function(called, true));
        qor_pp_assert_that(called);
    }

    {
        // Test creating a Function from a member volatile function address and
        // binding a volatile object pointer to it.
        int id = rand();
        volatile Object object(id);
        auto function = qor_pp_make_function(&Object::VolatileFunction, &object);

        qor_pp_assert_that(!!function);
        qor_pp_assert_that(&object == function.GetObject());
        qor_pp_assert_that((get_type_id<int(bool&, bool&&)>()) == function.type_id());

        bool called = false;
        qor_pp_assert_that(id == function(called, true));
        qor_pp_assert_that(called);
    }

    {
        // Test creating a Function from a member const volatile function address
        // and binding a const volatile object pointer to it.
        int id = rand();
        const volatile Object object(id);
        auto function = qor_pp_make_function(&Object::ConstVolatileFunction, &object);

        qor_pp_assert_that(!!function);
        qor_pp_assert_that(&object == function.GetObject());
        qor_pp_assert_that((get_type_id<int(bool&, bool&&)>()) == function.type_id());

        bool called = false;
        qor_pp_assert_that(id == function(called, true));
        qor_pp_assert_that(called);
    }

    {
        // Test calling after copying and moving the function.
        int id = rand();
        Object object(id);
        auto function = qor_pp_make_function(&Object::Function, &object);

        auto function_copy = function;
        qor_pp_assert_that(!!function_copy);
        qor_pp_assert_that(&object == function_copy.GetObject());
        qor_pp_assert_that((get_type_id<int(bool&, bool&&)>()) == function_copy.type_id());

        bool called = false;
        qor_pp_assert_that(id == function_copy(called, true));
        qor_pp_assert_that(called);

        auto function_move = std::move(function_copy);
        qor_pp_assert_that(!!function_move);
        qor_pp_assert_that(&object == function_move.GetObject());
        qor_pp_assert_that((get_type_id<int(bool&, bool&&)>()) == function_move.type_id());

        called = false;
        qor_pp_assert_that(id == function_move(called, true));
        qor_pp_assert_that(called);
    }

    {
        // Test creating a function with an invalid object.
        try 
        {
            Object* object = nullptr;
            auto function = qor_pp_make_function(&Object::Function, object);            
        } 
        catch(const Serious& error) 
        {
            qor_pp_assert_that(error.what().Content() == tefError.at(Error::kInvalidObject)).isTrue();
        }
    }
}

qor_pp_test_suite_case(TypeErasedFunctionTestSuite, FromFunctionAddressAndObjectPointerVirtual) 
{
    {
        // Test calling a virtual function using a base object.
        int id = rand();
        Object object(id);
        auto function = qor_pp_make_function(&Object::VirtualFunction, &object);
        qor_pp_assert_that(!!function);
        qor_pp_assert_that(&object == function.GetObject());
        qor_pp_assert_that((get_type_id<int(bool&, bool&&, bool*)>()) == function.type_id());

        bool called = false;
        bool derived = false;
        qor_pp_assert_that(id == function(called, true, &derived));
        qor_pp_assert_that(called);
        qor_pp_assert_that(!derived);
    }

    {
        // Test calling a virtual function using a derived object.
        int id = rand();
        DerivedObject derived_object(id);
        auto function = qor_pp_make_function(&Object::VirtualFunction, &derived_object);
        qor_pp_assert_that(!!function);
        qor_pp_assert_that(&derived_object == function.GetObject());
        qor_pp_assert_that((get_type_id<int(bool&, bool&&, bool*)>()) == function.type_id());

        bool called = false;
        bool derived = false;
        qor_pp_assert_that(id == function(called, true, &derived));
        qor_pp_assert_that(called);
        qor_pp_assert_that(derived);
    }
}

qor_pp_test_suite_case(TypeErasedFunctionTestSuite, MemberFunctionAddressAndObjectPointerOverload) 
{
    // Test calling the non-qualified operator ().
    {
        int id = rand();
        Object object(id);
        auto function = Function<int(CVQualification&)>::FromMemberFunction<Object, &Object::Overloaded>(&object);

        CVQualification cv = CVQualification::kUndefined;
        qor_pp_assert_that(id == function(cv));
        qor_pp_assert_that(CVQualification::kNonQualified == cv);

        auto function_const = Function<int(CVQualification&)>::FromMemberFunction<const Object, &Object::Overloaded>(&object);
        qor_pp_assert_that(id == function_const(cv));
        qor_pp_assert_that(CVQualification::kConstQualified == cv);

        auto function_volatile = Function<int(CVQualification&)>::FromMemberFunction<volatile Object, &Object::Overloaded>(&object);
        qor_pp_assert_that(id == function_volatile(cv));
        qor_pp_assert_that(CVQualification::kVolatileQualified == cv);

        auto function_const_volatile = Function<int(CVQualification&)>::FromMemberFunction<const volatile Object, &Object::Overloaded>(&object);
        qor_pp_assert_that(id == function_const_volatile(cv));
        qor_pp_assert_that(CVQualification::kConstVolatileQualified == cv);
    }

    // Test calling the const operator ().
    {
        int id = rand();
        const Object object(id);
        auto function = Function<int(CVQualification&)>::FromMemberFunction<const Object, &Object::Overloaded>(&object);

        CVQualification cv = CVQualification::kUndefined;
        qor_pp_assert_that(id == function(cv));
        qor_pp_assert_that(CVQualification::kConstQualified == cv);

        auto function_const_volatile = Function<int(CVQualification&)>::FromMemberFunction<const volatile Object, &Object::Overloaded>(&object);
        qor_pp_assert_that(id == function_const_volatile(cv));
        qor_pp_assert_that(CVQualification::kConstVolatileQualified == cv);
    }

    // Test calling the volatile operator ().
    {
        int id = rand();
        volatile Object object(id);
        auto function = Function<int(CVQualification&)>::FromMemberFunction<decltype(object), &Object::Overloaded>(&object);

        CVQualification cv = CVQualification::kUndefined;
        qor_pp_assert_that(id == function(cv));
        qor_pp_assert_that(CVQualification::kVolatileQualified == cv);

        auto function_const_volatile = Function<int(CVQualification&)>::FromMemberFunction<const volatile Object, &Object::Overloaded>(&object);
        qor_pp_assert_that(id == function_const_volatile(cv));
        qor_pp_assert_that(CVQualification::kConstVolatileQualified == cv);
    }

    // Test calling the const volatile operator ().
    {
        int id = rand();
        const volatile Object object(id);
        auto function = Function<int(CVQualification&)>::FromMemberFunction<decltype(object), &Object::Overloaded>(&object);

        CVQualification cv = CVQualification::kUndefined;
        qor_pp_assert_that(id == function(cv));
        qor_pp_assert_that(CVQualification::kConstVolatileQualified == cv);
    }
}

qor_pp_test_suite_case(TypeErasedFunctionTestSuite, FromFunctionAddressAndSharedPointer) 
{
    {
        // Test creating a Function from a member function address and binding an
        // object shared pointer to it.
        int id = rand();
        auto object = std::make_shared<Object>(id);
        auto function = qor_pp_make_function(&Object::Function, object);

        qor_pp_assert_that(!!function);
        qor_pp_assert_that(object.get() == function.GetObject());
        qor_pp_assert_that((get_type_id<int(bool&, bool&&)>()) == function.type_id());

        bool called = false;
        qor_pp_assert_that(id == function(called, true));
        qor_pp_assert_that(called);
    }

    {
        // Test creating a Function from a member const function address and binding
        // an object shared pointer to it.
        int id = rand();
        auto object = std::make_shared<Object>(id);
        auto function = qor_pp_make_function(&Object::ConstFunction, object);

        qor_pp_assert_that(!!function);
        qor_pp_assert_that(object.get() == function.GetObject());
        qor_pp_assert_that((get_type_id<int(bool&, bool&&)>()) == function.type_id());

        bool called = false;
        qor_pp_assert_that(id == function(called, true));
        qor_pp_assert_that(called);
    }

    {
        // Test creating a Function from a member volatile function address and
        // binding an object shared pointer to it.
        int id = rand();
        auto object = std::make_shared<Object>(id);
        auto function = qor_pp_make_function(&Object::VolatileFunction, object);

        qor_pp_assert_that(!!function);
        qor_pp_assert_that(object.get() == function.GetObject());
        qor_pp_assert_that((get_type_id<int(bool&, bool&&)>()) == function.type_id());

        bool called = false;
        qor_pp_assert_that(id == function(called, true));
        qor_pp_assert_that(called);
    }

    {
        // Test creating a Function from a member const volatile function address
        // and binding an object shared pointer to it.
        int id = rand();
        auto object = std::make_shared<Object>(id);
        auto function = qor_pp_make_function(&Object::ConstVolatileFunction, object);

        qor_pp_assert_that(!!function);
        qor_pp_assert_that(object.get() == function.GetObject());
        qor_pp_assert_that((get_type_id<int(bool&, bool&&)>()) == function.type_id());

        bool called = false;
        qor_pp_assert_that(id == function(called, true));
        qor_pp_assert_that(called);
    }

    {
        // Test creating a Function from a member const function address and binding
        // a const object shared pointer to it.
        int id = rand();
        auto object = std::make_shared<const Object>(id);
        auto function = qor_pp_make_function(&Object::ConstFunction, object);

        qor_pp_assert_that(!!function);
        qor_pp_assert_that(object.get() == function.GetObject());
        qor_pp_assert_that((get_type_id<int(bool&, bool&&)>()) == function.type_id());

        bool called = false;
        qor_pp_assert_that(id == function(called, true));
        qor_pp_assert_that(called);
    }

#ifndef DISABLE_VOLATILE_SHARED_PTRS
    {
        // Test creating a Function from a member volatile function address and
        // binding a volatile object shared pointer to it.
        int id = rand();
        auto object = std::make_shared<volatile Object>(id);
        auto function = qor_pp_make_function(&Object::VolatileFunction, object);

        qor_pp_assert_that(!!function);
        qor_pp_assert_that(object.get() == function.GetObject());
        qor_pp_assert_that((get_type_id<int(bool&, bool&&)>()) == function.type_id());

        bool called = false;
        qor_pp_assert_that(id == function(called, true));
        qor_pp_assert_that(called);
    }

    {
        // Test creating a Function from a member const volatile function address
        // and binding a const volatile object shared pointer to it.
        int id = rand();
        auto object = std::make_shared<const volatile Object>(id);
        auto function = qor_pp_make_function(&Object::ConstVolatileFunction, object);

        qor_pp_assert_that(!!function);
        qor_pp_assert_that(object.get() == function.GetObject());
        qor_pp_assert_that((get_type_id<int(bool&, bool&&)>()) == function.type_id());

        bool called = false;
        qor_pp_assert_that(id == function(called, true));
        qor_pp_assert_that(called);
    }
#endif  // DISABLE_VOLATILE_SHARED_PTRS

    {
        // Test calling after copying and moving the function.
        int id = rand();
        auto object = std::make_shared<Object>(id);
        auto function = qor_pp_make_function(&Object::Function, object);

        auto function_copy = function;
        qor_pp_assert_that(!!function_copy);
        qor_pp_assert_that(object.get() == function_copy.GetObject());
        qor_pp_assert_that((get_type_id<int(bool&, bool&&)>()) == function_copy.type_id());

        bool called = false;
        qor_pp_assert_that(id == function_copy(called, true));
        qor_pp_assert_that(called);

        auto function_move = std::move(function_copy);
        qor_pp_assert_that(!!function_move);
        qor_pp_assert_that(object.get() == function_move.GetObject());
        qor_pp_assert_that((get_type_id<int(bool&, bool&&)>()) == function_move.type_id());

        called = false;
        qor_pp_assert_that(id == function_move(called, true));
        qor_pp_assert_that(called);
    }

    {
        // Test creating a function with an invalid object.
        try 
        {
            std::shared_ptr<Object> object;
            auto function = qor_pp_make_function(&Object::Function, object);            
        }
        catch(const Serious& error) 
        {
            qor_pp_assert_that(error.what().Content() == tefError.at(Error::kInvalidObject)).isTrue();
        }
    }

    {
        // Check that the functions keep the object alive.
        auto object = std::make_shared<Object>(0);
        auto function = qor_pp_make_function(&Object::Function, object);

        std::weak_ptr<Object> weak_ptr = object;
        object.reset();
        qor_pp_assert_that(!weak_ptr.expired());
        function = nullptr;
        qor_pp_assert_that(weak_ptr.expired());
    }
}

qor_pp_test_suite_case(TypeErasedFunctionTestSuite, FromFunctionAddressAndSharedPointerVirtual) 
{
    {
        // Test calling a virtual function using a base object.
        int id = rand();
        auto object = std::make_shared<Object>(id);
        auto function = qor_pp_make_function(&Object::VirtualFunction, object);
        qor_pp_assert_that(!!function);
        qor_pp_assert_that(object.get() == function.GetObject());
        qor_pp_assert_that((get_type_id<int(bool&, bool&&, bool*)>()) == function.type_id());

        bool called = false;
        bool derived = false;
        qor_pp_assert_that(id == function(called, true, &derived));
        qor_pp_assert_that(called);
        qor_pp_assert_that(!derived);
    }

    {
        // Test calling a virtual function using a derived object.
        int id = rand();
        auto derived_object = std::make_shared<DerivedObject>(id);
        auto function = qor_pp_make_function(&Object::VirtualFunction, derived_object);
        qor_pp_assert_that(!!function);
        qor_pp_assert_that(derived_object.get() == function.GetObject());
        qor_pp_assert_that((get_type_id<int(bool&, bool&&, bool*)>()) == function.type_id());

        bool called = false;
        bool derived = false;
        qor_pp_assert_that(id == function(called, true, &derived));
        qor_pp_assert_that(called);
        qor_pp_assert_that(derived);
    }
}

qor_pp_test_suite_case(TypeErasedFunctionTestSuite, MemberFunctionAddressAndSharedPointerOverload) 
{
    // Test calling the non-qualified operator ().
    {
        int id = rand();
        auto object = std::make_shared<Object>(id);
        auto function = Function<int(CVQualification&)>::FromMemberFunction<Object, &Object::Overloaded>(object);

        CVQualification cv = CVQualification::kUndefined;
        qor_pp_assert_that(id == function(cv));
        qor_pp_assert_that(CVQualification::kNonQualified == cv);

        auto function_const = Function<int(CVQualification&)>::FromMemberFunction<const Object, &Object::Overloaded>(object);
        qor_pp_assert_that(id == function_const(cv));
        qor_pp_assert_that(CVQualification::kConstQualified == cv);

        auto function_volatile = Function<int(CVQualification&)>::FromMemberFunction<volatile Object, &Object::Overloaded>(object);
        qor_pp_assert_that(id == function_volatile(cv));
        qor_pp_assert_that(CVQualification::kVolatileQualified == cv);

        auto function_const_volatile = Function<int(CVQualification&)>::FromMemberFunction<const volatile Object, &Object::Overloaded>(object);
        qor_pp_assert_that(id == function_const_volatile(cv));
        qor_pp_assert_that(CVQualification::kConstVolatileQualified == cv);
    }

    // Test calling the const operator ().
    {
        int id = rand();
        auto object = std::make_shared<const Object>(id);
        auto function = Function<int(CVQualification&)>::FromMemberFunction<
        const Object, &Object::Overloaded>(object);

        CVQualification cv = CVQualification::kUndefined;
        qor_pp_assert_that(id == function(cv));
        qor_pp_assert_that(CVQualification::kConstQualified == cv);

        auto function_const_volatile = Function<int(CVQualification&)>::FromMemberFunction<const volatile Object, &Object::Overloaded>(object);
        qor_pp_assert_that(id == function_const_volatile(cv));
        qor_pp_assert_that(CVQualification::kConstVolatileQualified == cv);
    }

#ifndef DISABLE_VOLATILE_SHARED_PTRS
    // Test calling the volatile operator ().
    {
        int id = rand();
        auto object = std::make_shared<volatile Object>(id);
        auto function = Function<int(CVQualification&)>::FromMemberFunction<volatile Object, &Object::Overloaded>(object);

        CVQualification cv = CVQualification::kUndefined;
        qor_pp_assert_that(id == function(cv));
        qor_pp_assert_that(CVQualification::kVolatileQualified == cv);

        auto function_const_volatile = Function<int(CVQualification&)>::FromMemberFunction<const volatile Object, &Object::Overloaded>(object);
        qor_pp_assert_that(id == function_const_volatile(cv));
        qor_pp_assert_that(CVQualification::kConstVolatileQualified == cv);
    }

    // Test calling the const volatile operator ().
    {
        int id = rand();
        auto object = std::make_shared<const volatile Object>(id);
        auto function = Function<int(CVQualification&)>::FromMemberFunction<
        const volatile Object, &Object::Overloaded>(object);
        CVQualification cv = CVQualification::kUndefined;
        qor_pp_assert_that(id == function(cv));
        qor_pp_assert_that(CVQualification::kConstVolatileQualified == cv);
    }
#endif  // DISABLE_VOLATILE_SHARED_PTRS
}

qor_pp_test_suite_case(TypeErasedFunctionTestSuite, StdFunction) 
{
    // Test convertibility between tef::Function and std::function / std::bind
    // when pointing to a free function.
    {
        int a = rand();
        Function<int(int, int)> func1 = qor_pp_make_function(&Sum);
        Function<int(int, int)> func2 = std::function<int(int, int)>(func1);
        Function<int(int)> func3 = std::bind(func2, a, std::placeholders::_1);

        qor_pp_assert_that(!!func1);
        qor_pp_assert_that(!!func2);
        qor_pp_assert_that(!!func3);

        int b = rand();
        qor_pp_assert_that(a + b == func1(a, b));
        qor_pp_assert_that(a + b == func2(a, b));
        qor_pp_assert_that(a + b == func3(b));
    }

    // Test convertibility between tef::Function and std::function / std::bind
    // when pointing to a lambda.
    {
        int a = rand();
        Function<int(int, int)> func1 = [](int x, int y) { return x + y; };
        Function<int(int, int)> func2 = std::function<int(int, int)>(func1);
        Function<int(int)> func3 = std::bind(func2, a, std::placeholders::_1);

        qor_pp_assert_that(!!func1);
        qor_pp_assert_that(!!func2);
        qor_pp_assert_that(!!func3);

        int b = rand();
        qor_pp_assert_that(a + b == func1(a, b));
        qor_pp_assert_that(a + b == func2(a, b));
        qor_pp_assert_that(a + b == func3(b));
    }

    // Test convertibility between mf::Function and std::function / std::bind
    // when binding to a MemberFunction and an object pointer.
    {
        int id = rand();
        Object object(id);

        int a = rand();
        auto member_func = qor_pp_make_function(&Object::Sum);
        Function<int(int, int)> func1(member_func, &object);
        Function<int(int, int)> func2 = std::function<int(int, int)>(func1);
        Function<int(int)> func3 = std::bind(func2, a, std::placeholders::_1);

        qor_pp_assert_that(!!member_func);
        qor_pp_assert_that(!!func1);
        qor_pp_assert_that(!!func2);
        qor_pp_assert_that(!!func3);

        int b = rand();
        qor_pp_assert_that(id + a + b == member_func(object, a, b));
        qor_pp_assert_that(id + a + b == func1(a, b));
        qor_pp_assert_that(id + a + b == func2(a, b));
        qor_pp_assert_that(id + a + b == func3(b));
    }

    // Test convertibility between mf::Function and std::function / std::bind
    // when binding to a MemberFunction and a shared pointer.
    {
        int id = rand();
        auto object = std::make_shared<Object>(id);

        int a = rand();
        auto member_func = qor_pp_make_function(&Object::Sum);
        Function<int(int, int)> func1(member_func, object);
        Function<int(int, int)> func2 = std::function<int(int, int)>(func1);
        Function<int(int)> func3 = std::bind(func2, a, std::placeholders::_1);

        qor_pp_assert_that(!!member_func);
        qor_pp_assert_that(!!func1);
        qor_pp_assert_that(!!func2);
        qor_pp_assert_that(!!func3);

        int b = rand();
        qor_pp_assert_that(id + a + b == member_func(*object, a, b));
        qor_pp_assert_that(id + a + b == func1(a, b));
        qor_pp_assert_that(id + a + b == func2(a, b));
        qor_pp_assert_that(id + a + b == func3(b));
    }

    // Test convertibility between mf::Function and std::function / std::bind
    // when binding to a member function address and an object pointer.
    {
        int id = rand();
        Object obj(id);

        int a = rand();
        Function<int(int, int)> func1 = qor_pp_make_function(&Object::Sum, &obj);
        Function<int(int, int)> func2 = std::function<int(int, int)>(func1);
        Function<int(int)> func3 = std::bind(func2, a, std::placeholders::_1);

        qor_pp_assert_that(!!func1);
        qor_pp_assert_that(!!func2);
        qor_pp_assert_that(!!func3);

        int b = rand();
        qor_pp_assert_that(id + a + b == func1(a, b));
        qor_pp_assert_that(id + a + b == func2(a, b));
        qor_pp_assert_that(id + a + b == func3(b));
    }

    // Test convertibility between mf::Function and std::function / std::bind
    // when binding to a member function address and a shared pointer.
    {
        int id = rand();
        auto obj = std::make_shared<Object>(id);

        int a = rand();
        Function<int(int, int)> func1 = qor_pp_make_function(&Object::Sum, obj);
        Function<int(int, int)> func2 = std::function<int(int, int)>(func1);
        Function<int(int)> func3 = std::bind(func2, a, std::placeholders::_1);

        qor_pp_assert_that(!!func1);
        qor_pp_assert_that(!!func2);
        qor_pp_assert_that(!!func3);

        int b = rand();
        qor_pp_assert_that(id + a + b == func1(a, b));
        qor_pp_assert_that(id + a + b == func2(a, b));
        qor_pp_assert_that(id + a + b == func3(b));
    }
}

qor_pp_test_suite_case(TypeErasedFunctionTestSuite, CopyConstructFunction) 
{
    Function<int(int)> func1 = [](int x) { return x + 1; };
    Function<int(int)> func2(func1);
    qor_pp_assert_that(!!func1);
    qor_pp_assert_that(!!func2);
    qor_pp_assert_that(func2(3) == 4);

    Function<int(int)> func3;
    Function<int(int)> func4 = func3;
    qor_pp_assert_that(!func3);
    qor_pp_assert_that(!func4);
}

qor_pp_test_suite_case(TypeErasedFunctionTestSuite, MoveConstructFunction) 
{
    Function<int(int)> func1 = [](int x) { return x + 1; };
    Function<int(int)> func2(std::move(func1));
    qor_pp_assert_that(!func1);
    qor_pp_assert_that(!!func2);
    qor_pp_assert_that(func2(3) == 4);

    Function<int(int)> func3;
    Function<int(int)> func4 = std::move(func3);
    qor_pp_assert_that(!func3);
    qor_pp_assert_that(!func4);
}

qor_pp_test_suite_case(TypeErasedFunctionTestSuite, CopyAssignFunction) 
{
    Function<int(int)> func1 = [](int x) { return x + 1; };
    Function<int(int)> func2;
    func2 = func1;
    qor_pp_assert_that(!!func1);
    qor_pp_assert_that(!!func2);
    qor_pp_assert_that(func2(3) == 4);

    Function<int(int)> func3;
    Function<int(int)> func4;
    func4 = func3;
    qor_pp_assert_that(!func3);
    qor_pp_assert_that(!func4);
}

qor_pp_test_suite_case(TypeErasedFunctionTestSuite, MoveAssignFunction) 
{
    Function<int(int)> func1 = [](int x) { return x + 1; };
    Function<int(int)> func2;
    func2 = std::move(func1);
    qor_pp_assert_that(!func1);
    qor_pp_assert_that(!!func2);
    qor_pp_assert_that(func2(3) == 4);

    Function<int(int)> func3;
    Function<int(int)> func4;
    func4 = std::move(func3);
    qor_pp_assert_that(!func3);
    qor_pp_assert_that(!func4);
}
