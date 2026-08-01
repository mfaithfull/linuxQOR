// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

// This test needs C++ exceptions thrown by MagicFunc exceptions to work.
// These exceptions are turned off in release builds that define NDEBUG.
#undef NDEBUG

#include "src/configuration/configuration.h"
#include "src/qor/tdd/tdd.h"
#include "src/qor/function/error.h"
#include "src/qor/function/make_function.h"
#include "src/qor/function/member_function.h"
#include "src/qor/function/type_id.h"

#include "test_common.h"

using namespace qor;
using namespace qor::tef;

struct TypeErasedMemberFunctionTestSuite{};

qor_pp_test_suite_case(TypeErasedMemberFunctionTestSuite, _Empty) 
{
    MemberFunction<decltype(&Object::Function)> member_function;
    qor_pp_assert_that(!member_function);
    qor_pp_assert_that(nullptr == member_function.GetObject());
    qor_pp_assert_that(get_type_id<decltype(&Object::Function)>() == member_function.type_id());

    // Test calling an empty function.
    try 
    {
        Object object(0);
        bool called;

        member_function(object, called, true);        

    }
    catch(const Serious& error) 
    {
        qor_pp_assert_that(error.what().Content() == tefError.at(Error::kInvalidFunction)).isTrue();
    }
}

qor_pp_test_suite_case(TypeErasedMemberFunctionTestSuite, CallMemberFunction) 
{
    MemberFunction<decltype(&Object::Function)> member_function = MF_MakeFunction(&Object::Function);

    qor_pp_assert_that(!!member_function);
    qor_pp_assert_that(nullptr == member_function.GetObject());
    qor_pp_assert_that(get_type_id<decltype(&Object::Function)>() == member_function.type_id());

    {
        // Test calling regular member functions with regular objects.
        int id = rand();
        Object object(id);

        bool called = false;
        qor_pp_assert_that(id == member_function(object, called, true));
        qor_pp_assert_that(called);
    }

    {
        // Test calling after copying and moving the member function.
        int id = rand();
        Object object(id);

        auto member_function_copy = member_function;
        qor_pp_assert_that(!!member_function_copy);
        qor_pp_assert_that(nullptr == member_function_copy.GetObject());
        qor_pp_assert_that(get_type_id<decltype(&Object::Function)>() == member_function_copy.type_id());

        bool called = false;
        qor_pp_assert_that(id == member_function_copy(object, called, true));
        qor_pp_assert_that(called);

        auto member_function_move = std::move(member_function_copy);
        qor_pp_assert_that(!!member_function_move);
        qor_pp_assert_that(nullptr == member_function_move.GetObject());
        qor_pp_assert_that(get_type_id<decltype(&Object::Function)>() == member_function_move.type_id());

        called = false;
        qor_pp_assert_that(id == member_function_move(object, called, true));
        qor_pp_assert_that(called);
    }
}

qor_pp_test_suite_case(TypeErasedMemberFunctionTestSuite, CallConstMemberFunction) 
{
    MemberFunction<decltype(&Object::ConstFunction)> member_function = MF_MakeFunction(&Object::ConstFunction);

    qor_pp_assert_that(!!member_function);
    qor_pp_assert_that(nullptr == member_function.GetObject());
    qor_pp_assert_that(get_type_id<decltype(&Object::ConstFunction)>() == member_function.type_id());

    {
        // Test calling const member functions with regular objects.
        int id = rand();
        Object object(id);

        bool called = false;
        qor_pp_assert_that(id == member_function(object, called, true));
        qor_pp_assert_that(called);
    }

    {
        // Test calling const member functions with const objects.
        int id = rand();
        const Object object(id);

        bool called = false;
        qor_pp_assert_that(id == member_function(object, called, true));
        qor_pp_assert_that(called);
    }

    {
        // Test calling after copying and moving the member function.
        int id = rand();
        const Object object(id);

        auto member_function_copy = member_function;
        qor_pp_assert_that(!!member_function_copy);
        qor_pp_assert_that(nullptr == member_function_copy.GetObject());
        qor_pp_assert_that(get_type_id<decltype(&Object::ConstFunction)>() == member_function_copy.type_id());

        bool called = false;
        qor_pp_assert_that(id == member_function_copy(object, called, true));
        qor_pp_assert_that(called);

        auto member_function_move = std::move(member_function_copy);
        qor_pp_assert_that(!!member_function_move);
        qor_pp_assert_that(nullptr == member_function_move.GetObject());
        qor_pp_assert_that(get_type_id<decltype(&Object::ConstFunction)>() == member_function_move.type_id());

        called = false;
        qor_pp_assert_that(id == member_function_move(object, called, true));
        qor_pp_assert_that(called);
    }
}

qor_pp_test_suite_case(TypeErasedMemberFunctionTestSuite, CallVolatileMemberFunction) 
{
    MemberFunction<decltype(&Object::VolatileFunction)>
    member_function = MF_MakeFunction(&Object::VolatileFunction);

    qor_pp_assert_that(!!member_function);
    qor_pp_assert_that(nullptr == member_function.GetObject());
    qor_pp_assert_that(get_type_id<decltype(&Object::VolatileFunction)>() == member_function.type_id());

    {
        // Test calling volatile member functions with regular objects.
        int id = rand();
        Object object(id);

        bool called = false;
        qor_pp_assert_that(id == member_function(object, called, true));
        qor_pp_assert_that(called);
    }

    {
        // Test calling volatile member functions with volatile objects.
        int id = rand();
        volatile Object object(id);

        bool called = false;
        qor_pp_assert_that(id == member_function(object, called, true));
        qor_pp_assert_that(called);
    }

    {
        // Test calling after copying and moving the member function.
        int id = rand();
        volatile Object object(id);

        auto member_function_copy = member_function;
        qor_pp_assert_that(!!member_function_copy);
        qor_pp_assert_that(nullptr == member_function_copy.GetObject());
        qor_pp_assert_that(get_type_id<decltype(&Object::VolatileFunction)>() == member_function_copy.type_id());

        bool called = false;
        qor_pp_assert_that(id == member_function_copy(object, called, true));
        qor_pp_assert_that(called);

        auto member_function_move = std::move(member_function_copy);
        qor_pp_assert_that(!!member_function_move);
        qor_pp_assert_that(nullptr == member_function_move.GetObject());
        qor_pp_assert_that(get_type_id<decltype(&Object::VolatileFunction)>() == member_function_move.type_id());

        called = false;
        qor_pp_assert_that(id == member_function_move(object, called, true));
        qor_pp_assert_that(called);
    }
}

qor_pp_test_suite_case(TypeErasedMemberFunctionTestSuite, CallConstVolatileMemberFunction) 
{
    MemberFunction<decltype(&Object::ConstVolatileFunction)>
    member_function = MF_MakeFunction(&Object::ConstVolatileFunction);

    qor_pp_assert_that(!!member_function);
    qor_pp_assert_that(nullptr == member_function.GetObject());
    qor_pp_assert_that(get_type_id<decltype(&Object::ConstVolatileFunction)>() == member_function.type_id());

    {
        // Test calling const volatile member functions with regular objects.
        int id = rand();
        Object object(id);

        bool called = false;
        qor_pp_assert_that(id == member_function(object, called, true));
        qor_pp_assert_that(called);
    }

    {
        // Test calling const volatile member functions with volatile objects.
        int id = rand();
        volatile Object object(id);

        bool called = false;
        qor_pp_assert_that(id == member_function(object, called, true));
        qor_pp_assert_that(called);
    }

    {
        // Test calling const volatile member functions with const objects.
        int id = rand();
        const Object object(id);

        bool called = false;
        qor_pp_assert_that(id == member_function(object, called, true));
        qor_pp_assert_that(called);
    }

    {
        // Test calling const volatile member functions with const volatile objects.
        int id = rand();
        const volatile Object object(id);

        bool called = false;
        qor_pp_assert_that(id == member_function(object, called, true));
        qor_pp_assert_that(called);
    }

    {
        // Test calling after copying and moving the member function.
        int id = rand();
        volatile Object object(id);

        auto member_function_copy = member_function;
        qor_pp_assert_that(!!member_function_copy);
        qor_pp_assert_that(nullptr == member_function_copy.GetObject());
        qor_pp_assert_that(get_type_id<decltype(&Object::ConstVolatileFunction)>() == member_function_copy.type_id());

        bool called = false;
        qor_pp_assert_that(id == member_function_copy(object, called, true));
        qor_pp_assert_that(called);

        auto member_function_move = std::move(member_function_copy);
        qor_pp_assert_that(!!member_function_move);
        qor_pp_assert_that(nullptr == member_function_move.GetObject());
        qor_pp_assert_that(get_type_id<decltype(&Object::ConstVolatileFunction)>() == member_function_move.type_id());

        called = false;
        qor_pp_assert_that(id == member_function_move(object, called, true));
        qor_pp_assert_that(called);
    }
}

qor_pp_test_suite_case(TypeErasedMemberFunctionTestSuite, CallVirtualMemberFunction) 
{
    MemberFunction<decltype(&Object::VirtualFunction)>
        member_function = MF_MakeFunction(&Object::VirtualFunction);

    qor_pp_assert_that(!!member_function);
    qor_pp_assert_that(nullptr == member_function.GetObject());
    qor_pp_assert_that(get_type_id<decltype(&Object::VirtualFunction)>() == member_function.type_id());

    {
        // Test calling a virtual function with a base object.
        int id = rand();
        Object object(id);

        bool called = false;
        bool derived = false;
        qor_pp_assert_that(id == member_function(object, called, true, &derived));
        qor_pp_assert_that(called);
        qor_pp_assert_that(!derived);
    }

    {
        // Test calling const member functions with const objects.
        int id = rand();
        DerivedObject object(id);

        bool called = false;
        bool derived = false;
        qor_pp_assert_that(id == member_function(object, called, true, &derived));
        qor_pp_assert_that(called);
        qor_pp_assert_that(derived);
    }
}
