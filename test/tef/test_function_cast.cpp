// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

// This test needs C++ exceptions thrown by MagicFunc exceptions to work.
// These exceptions are turned off in release builds that define NDEBUG.
#undef NDEBUG

#include "src/configuration/configuration.h"
#include "src/qor/tdd/tdd.h"

#include "src/qor/function/error.h"
#include "src/qor/function/function.h"
#include "src/qor/function/function_cast.h"
#include "src/qor/function/make_function.h"
#include "src/qor/function/type_erased_function.h"
#include "src/qor/function/type_id.h"

#include "test_common.h"

using namespace qor;
using namespace qor::tef;

struct TypeErasedFunctionCastTestSuite{};

qor_pp_test_suite_case(TypeErasedFunctionCastTestSuite, FunctionAddressCast) 
{
    // Test casting back from a type-erased Function pointing to a function
    // address.
    auto function = MF_MakeFunction(&FreeFunction);
    TypeErasedFunction type_erased = function;
    qor_pp_assert_that(!!type_erased);

    auto casted_function = function_cast<int(bool&, bool&&)>(type_erased);
    qor_pp_assert_that((std::is_same<decltype(casted_function), Function<int(bool&, bool&&)>>::value));

    bool called = false;
    qor_pp_assert_that(42 == casted_function(called, true));
    qor_pp_assert_that(called);

    // Try catching a bad cast.
    try 
    {
        function_cast<int(bool, bool)>(type_erased);        
    } 
    catch (const Serious& error) 
    {
        qor_pp_assert_that(error.what().Content() == tefError.at(Error::kInvalidCast)).isTrue();
    }
}

qor_pp_test_suite_case(TypeErasedFunctionCastTestSuite, FunctionCallable) 
{
    // Test casting back from a type-erased Function that calls a lambda.
    int id = rand();
    Function<int(bool&, bool&&)> function = [=](bool& called, bool&& value) 
    {
        called = std::move(value);
        return id;
    };

    TypeErasedFunction type_erased = function;
    qor_pp_assert_that(!!type_erased);

    auto casted_function = function_cast<int(bool&, bool&&)>(type_erased);
    qor_pp_assert_that((std::is_same<decltype(casted_function), Function<int(bool&, bool&&)>>::value));

    bool called = false;
    qor_pp_assert_that(id == casted_function(called, true));
    qor_pp_assert_that(called);

    // Try catching a bad cast.
    try 
    {
        function_cast<int(bool, bool)>(type_erased);        
    } 
    catch(const Serious& error) 
    {
        qor_pp_assert_that(error.what().Content() == tefError.at(Error::kInvalidCast)).isTrue();        
    }
}

qor_pp_test_suite_case(TypeErasedFunctionCastTestSuite, MemberFunctionAndObjectCast) 
{
    auto member_function = MF_MakeFunction(&Object::Function);

    {
        // Test casting back from a type-erased Function built by binding a
        // MemberFunction and an object pointer.
        int id = rand();
        Object object(id);
        Function<int(bool&, bool&&)> function(member_function, &object);

        TypeErasedFunction type_erased = function;
        qor_pp_assert_that(!!type_erased);

        auto casted_function = function_cast<int(bool&, bool&&)>(type_erased);
        qor_pp_assert_that((std::is_same<decltype(casted_function), Function<int(bool&, bool&&)>>::value));

        bool called = false;
        qor_pp_assert_that(id == casted_function(called, true));
        qor_pp_assert_that(called);

        // Try catching a bad cast.
        try 
        {
            function_cast<decltype(&Object::Function)>(type_erased);            
        } 
        catch(const Serious& error) 
        {
            qor_pp_assert_that(error.what().Content() == tefError.at(Error::kInvalidCast)).isTrue();
        }
    }

    {
        // Test casting back from a type-erased Function built by binding a
        // MemberFunction and an object shared pointer.
        int id = rand();
        auto object = std::make_shared<Object>(id);
        Function<int(bool&, bool&&)> function(member_function, object);

        TypeErasedFunction type_erased = function;
        qor_pp_assert_that(!!type_erased);

        auto casted_function = function_cast<int(bool&, bool&&)>(type_erased);
        qor_pp_assert_that((std::is_same<decltype(casted_function), Function<int(bool&, bool&&)>>::value));

        bool called = false;
        qor_pp_assert_that(id == casted_function(called, true));
        qor_pp_assert_that(called);

        // Try catching a bad cast.
        try 
        {
            function_cast<decltype(&Object::Function)>(type_erased);            
        } 
        catch(const Serious& error) 
        {
            qor_pp_assert_that(error.what().Content() == tefError.at(Error::kInvalidCast)).isTrue();
        }
    }
}

qor_pp_test_suite_case(TypeErasedFunctionCastTestSuite, MemberFunctionAddressAndObjectCast) 
{
    {
        // Test casting back from a type-erased Function built by binding a member
        // function address and an object pointer.
        int id = rand();
        Object object(id);
        auto function = MF_MakeFunction(&Object::Function, &object);

        TypeErasedFunction type_erased = function;
        qor_pp_assert_that(!!type_erased);

        auto casted_function = function_cast<int(bool&, bool&&)>(type_erased);
        qor_pp_assert_that((std::is_same<decltype(casted_function), Function<int(bool&, bool&&)>>::value));

        bool called = false;
        qor_pp_assert_that(id == casted_function(called, true));
        qor_pp_assert_that(called);

        // Try catching a bad cast.
        try 
        {
            function_cast<decltype(&Object::Function)>(type_erased);            
        } 
        catch(const Serious& error) 
        {
            qor_pp_assert_that(error.what().Content() == tefError.at(Error::kInvalidCast)).isTrue();                    
        }
    }

    {
        // Test casting back from a type-erased Function built by binding a member
        // function address and an object shared pointer.
        int id = rand();
        auto object = std::make_shared<Object>(id);
        auto function = MF_MakeFunction(&Object::Function, object);

        TypeErasedFunction type_erased = function;
        qor_pp_assert_that(!!type_erased);

        auto casted_function = function_cast<int(bool&, bool&&)>(type_erased);
        qor_pp_assert_that((std::is_same<decltype(casted_function), Function<int(bool&, bool&&)>>::value));

        bool called = false;
        qor_pp_assert_that(id == casted_function(called, true));
        qor_pp_assert_that(called);

        // Try catching a bad cast.
        try 
        {
            function_cast<decltype(&Object::Function)>(type_erased);            
        } 
        catch(const Serious& error) 
        {
            qor_pp_assert_that(error.what().Content() == tefError.at(Error::kInvalidCast)).isTrue();            
        }
    }
}

qor_pp_test_suite_case(TypeErasedFunctionCastTestSuite, MemberFunctionAddressCast) 
{
    // Test casting back from a type-erased MemberFunction built from a member
    // function address.
    auto member_function = MF_MakeFunction(&Object::Function);
    TypeErasedFunction type_erased = member_function;
    qor_pp_assert_that(!!type_erased);

    auto casted_function = function_cast<decltype(&Object::Function)>(type_erased);
    qor_pp_assert_that((std::is_same<decltype(casted_function), MemberFunction<decltype(&Object::Function)>>::value));

    int id = rand();
    Object object(id);

    bool called = false;
    qor_pp_assert_that(id == casted_function(object, called, true));
    qor_pp_assert_that(called);

    // Try catching a bad cast.
    try 
    {
        function_cast<decltype(&Object::ConstFunction)>(type_erased);
    } 
    catch(const Serious& error) 
    {
        qor_pp_assert_that(error.what().Content() == tefError.at(Error::kInvalidCast)).isTrue();
    }
}
