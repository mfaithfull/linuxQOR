// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"
#include "src/qor/tdd/tdd.h"
#include "src/qor/function/make_function.h"
#include "src/qor/function/type_id.h"

#include "test_common.h"

using namespace qor;
using namespace qor::tef;

struct MakeTypeErasedFunctionTestSuite{};

qor_pp_test_suite_case(MakeTypeErasedFunctionTestSuite, FunctionAddress) 
{
    auto function = MF_MakeFunction(&FreeFunction);
    qor_pp_assert_that((std::is_same<Function<int(bool&, bool&&)>, decltype(function)>::value));

    qor_pp_assert_that(!!function);
    qor_pp_assert_that(nullptr == function.GetObject());
    qor_pp_assert_that((get_type_id<int(bool&, bool&&)>()) == function.type_id());

    bool called = false;
    qor_pp_assert_that(42 == function(called, true));
    qor_pp_assert_that(called);
}

qor_pp_test_suite_case(MakeTypeErasedFunctionTestSuite, Lambda) 
{
    {
        // Test creating a function from a lambda lvalue reference.
        int id = rand();
        auto lambda = [=](bool& called, bool&& value) 
        {
            called = std::move(value);
            return id;
        };

        auto function = make_function(lambda);
        qor_pp_assert_that((std::is_same<Function<int(bool&, bool&&)>,decltype(function)>::value));

        qor_pp_assert_that(!!function);
        qor_pp_assert_that(nullptr != function.GetObject());
        qor_pp_assert_that((get_type_id<int(bool&, bool&&)>()) == function.type_id());

        bool called = false;
        qor_pp_assert_that(id == function(called, true));
        qor_pp_assert_that(called);
    }

    {
        // Test creating a function from a lambda rvalue reference.
        int id = rand();
        auto function = make_function([=](bool& called, bool&& value) 
        {
            called = std::move(value);
            return id;
        });

        qor_pp_assert_that((std::is_same<Function<int(bool&, bool&&)>, decltype(function)>::value));

        qor_pp_assert_that(!!function);
        qor_pp_assert_that(nullptr != function.GetObject());
        qor_pp_assert_that((get_type_id<int(bool&, bool&&)>()) == function.type_id());

        bool called = false;
        qor_pp_assert_that(id == function(called, true));
        qor_pp_assert_that(called);
    }
}

qor_pp_test_suite_case(MakeTypeErasedFunctionTestSuite, MemberFunctionAndObject) 
{
    auto member_function = MF_MakeFunction(&Object::Function);
    auto member_function_const = MF_MakeFunction(&Object::ConstFunction);

    {
        // Test non-const functions with object pointers.
        int id = rand();
        Object object(id);
        auto function = make_function(member_function, &object);
        qor_pp_assert_that((std::is_same<Function<int(bool&, bool&&)>, decltype(function)>::value));
        qor_pp_assert_that(&object == function.GetObject());

        bool called = false;
        qor_pp_assert_that(id == function(called, true));
        qor_pp_assert_that(called);
    }

    {
        // Test const functions with object pointers.
        int id = rand();
        Object object(id);
        auto function = make_function(member_function_const, &object);
        qor_pp_assert_that((std::is_same<Function<int(bool&, bool&&)>, decltype(function)>::value));
        qor_pp_assert_that(&object == function.GetObject());

        bool called = false;
        qor_pp_assert_that(id == function(called, true));
        qor_pp_assert_that(called);
    }

    {
        // Test const functions with const object pointers.
        int id = rand();
        const Object const_object(id);
        auto function = make_function(member_function_const, &const_object);
        qor_pp_assert_that((std::is_same<Function<int(bool&, bool&&)>, decltype(function)>::value));
        qor_pp_assert_that(&const_object == function.GetObject());
        bool called = false;
        qor_pp_assert_that(id == function(called, true));
        qor_pp_assert_that(called);
    }

    {
        // Test non-const functions with shared object pointers.
        int id = rand();
        auto shared_object = std::make_shared<Object>(id);
        auto function = make_function(member_function, shared_object);
        qor_pp_assert_that((std::is_same<Function<int(bool&, bool&&)>, decltype(function)>::value));
        qor_pp_assert_that(shared_object.get() == function.GetObject());

        bool called = false;
        qor_pp_assert_that(id == function(called, true));
        qor_pp_assert_that(called);
    }

    {
        // Test const functions with shared object pointers.
        int id = rand();
        auto shared_object = std::make_shared<Object>(id);
        auto function = make_function(member_function_const, shared_object);
        qor_pp_assert_that((std::is_same<Function<int(bool&, bool&&)>, decltype(function)>::value));
        qor_pp_assert_that(shared_object.get() == function.GetObject());
        bool called = false;
        qor_pp_assert_that(id == function(called, true));
        qor_pp_assert_that(called);
    }

    {
        // Test const functions with shared const object pointers.
        int id = rand();
        auto shared_const_object = std::make_shared<const Object>(id);
        auto function = make_function(member_function_const, shared_const_object);
        qor_pp_assert_that((std::is_same<Function<int(bool&, bool&&)>, decltype(function)>::value));
        qor_pp_assert_that(shared_const_object.get() == function.GetObject());

        bool called = false;
        qor_pp_assert_that(id == function(called, true));
        qor_pp_assert_that(called);
    }
}

qor_pp_test_suite_case(MakeTypeErasedFunctionTestSuite, MemberFunctionAddressAndObject) 
{
    {
        // Test non-const functions with object pointers.
        int id = rand();
        Object object(id);
        auto function = MF_MakeFunction(&Object::Function, &object);
        qor_pp_assert_that((std::is_same<Function<int(bool&, bool&&)>, decltype(function)>::value));
        qor_pp_assert_that(&object == function.GetObject());

        bool called = false;
        qor_pp_assert_that(id == function(called, true));
        qor_pp_assert_that(called);
    }

    {
        // Test const functions with object pointers.
        int id = rand();
        Object object(id);
        auto function = MF_MakeFunction(&Object::ConstFunction, &object);
        qor_pp_assert_that((std::is_same<Function<int(bool&, bool&&)>, decltype(function)>::value));
        qor_pp_assert_that(&object == function.GetObject());
        bool called = false;
        qor_pp_assert_that(id == function(called, true));
        qor_pp_assert_that(called);
    }

    {
        // Test const functions with const object pointers.
        int id = rand();
        const Object const_object(id);
        auto function = MF_MakeFunction(&Object::ConstFunction, &const_object);
        qor_pp_assert_that((std::is_same<Function<int(bool&, bool&&)>, decltype(function)>::value));
        qor_pp_assert_that(&const_object == function.GetObject());
        bool called = false;
        qor_pp_assert_that(id == function(called, true));
        qor_pp_assert_that(called);
    }

    {
        // Test non-const functions with shared object pointers.
        int id = rand();
        auto shared_object = std::make_shared<Object>(id);
        auto function = MF_MakeFunction(&Object::Function, shared_object);
        qor_pp_assert_that((std::is_same<Function<int(bool&, bool&&)>, decltype(function)>::value));
        qor_pp_assert_that(shared_object.get() == function.GetObject());
        bool called = false;
        qor_pp_assert_that(id == function(called, true));
        qor_pp_assert_that(called);
    }

    {
        // Test const functions with shared object pointers.
        int id = rand();
        auto shared_object = std::make_shared<Object>(id);
        auto function = MF_MakeFunction(&Object::ConstFunction, shared_object);
        qor_pp_assert_that((std::is_same<Function<int(bool&, bool&&)>, decltype(function)>::value));
        qor_pp_assert_that(shared_object.get() == function.GetObject());
        bool called = false;
        qor_pp_assert_that(id == function(called, true));
        qor_pp_assert_that(called);
    }

    {
        // Test const functions with shared const object pointers.
        int id = rand();
        auto shared_const_object = std::make_shared<const Object>(id);
        auto function = MF_MakeFunction(&Object::ConstFunction, shared_const_object);
        qor_pp_assert_that((std::is_same<Function<int(bool&, bool&&)>, decltype(function)>::value));
        qor_pp_assert_that(shared_const_object.get() == function.GetObject());
        bool called = false;
        qor_pp_assert_that(id == function(called, true));
        qor_pp_assert_that(called);
    }
}

qor_pp_test_suite_case(MakeTypeErasedFunctionTestSuite, MemberFunctionAddress) 
{
    {
        // Test creating a MemberFunction for a member function address.
        auto member_function = MF_MakeFunction(&Object::Function);
        qor_pp_assert_that((std::is_same<MemberFunction<decltype(&Object::Function)>,decltype(member_function)>::value));
        qor_pp_assert_that(!decltype(member_function)::kIsConst);
        qor_pp_assert_that(!decltype(member_function)::kIsVolatile);

        qor_pp_assert_that(!member_function);
        qor_pp_assert_that(nullptr == member_function.GetObject());
        qor_pp_assert_that((get_type_id<decltype(&Object::Function)>()) == member_function.type_id());

        int id = rand();
        Object object(id);

        bool called = false;
        qor_pp_assert_that(id == member_function(object, called, true));
        qor_pp_assert_that(called);
    }

    {
        // Test creating a MemberFunction to a const member function address.
        auto member_function = MF_MakeFunction(&Object::ConstFunction);
        qor_pp_assert_that((std::is_same<MemberFunction<decltype(&Object::ConstFunction)>, decltype(member_function)>::value));
        qor_pp_assert_that(!!decltype(member_function)::kIsConst);
        qor_pp_assert_that(!decltype(member_function)::kIsVolatile);

        qor_pp_assert_that(!member_function);
        qor_pp_assert_that(nullptr == member_function.GetObject());
        qor_pp_assert_that((get_type_id<decltype(&Object::ConstFunction)>()) == member_function.type_id());

        int id = rand();
        Object object(id);

        bool called = false;
        qor_pp_assert_that(id == member_function(object, called, true));
        qor_pp_assert_that(called);

        int const_id = rand();
        const Object const_object(const_id);

        called = false;
        qor_pp_assert_that(const_id == member_function(const_object, called, true));
        qor_pp_assert_that(called);
    }

    {
        // Test creating a MemberFunction to a volatile member function address.
        auto member_function = MF_MakeFunction(&Object::VolatileFunction);
        qor_pp_assert_that((std::is_same<MemberFunction<decltype(&Object::VolatileFunction)>,decltype(member_function)>::value));
        qor_pp_assert_that(!decltype(member_function)::kIsConst);
        qor_pp_assert_that(!!decltype(member_function)::kIsVolatile);

        qor_pp_assert_that(!!member_function);
        qor_pp_assert_that(nullptr == member_function.GetObject());
        qor_pp_assert_that((get_type_id<decltype(&Object::VolatileFunction)>()) == member_function.type_id());

        int id = rand();
        Object object(id);

        bool called = false;
        qor_pp_assert_that(id == member_function(object, called, true));
        qor_pp_assert_that(called);

        int volatile_id = rand();
        volatile Object volatile_object(volatile_id);

        called = false;
        qor_pp_assert_that(volatile_id == member_function(volatile_object, called, true));
        qor_pp_assert_that(called);
    }

    {
        // Test creating a MemberFunction to a const volatile member function
        // address.
        auto member_function = MF_MakeFunction(&Object::ConstVolatileFunction);
        qor_pp_assert_that((std::is_same<MemberFunction<decltype(&Object::ConstVolatileFunction)>,decltype(member_function)>::value));
        qor_pp_assert_that(!!decltype(member_function)::kIsConst);
        qor_pp_assert_that(!!decltype(member_function)::kIsVolatile);

        qor_pp_assert_that(!!member_function);
        qor_pp_assert_that(nullptr == member_function.GetObject());
        qor_pp_assert_that((get_type_id<decltype(&Object::ConstVolatileFunction)>()) == member_function.type_id());

        int id = rand();
        Object object(id);

        bool called = false;
        qor_pp_assert_that(id == member_function(object, called, true));
        qor_pp_assert_that(called);

        int const_id = rand();
        const Object const_object(const_id);

        called = false;
        qor_pp_assert_that(const_id == member_function(const_object, called, true));
        qor_pp_assert_that(called);

        int volatile_id = rand();
        volatile Object volatile_object(volatile_id);

        called = false;
        qor_pp_assert_that(volatile_id == member_function(volatile_object, called, true));
        qor_pp_assert_that(called);

        int const_volatile_id = rand();
        const volatile Object const_volatile_object(const_volatile_id);

        called = false;
        qor_pp_assert_that(const_volatile_id == member_function(const_volatile_object, called, true));
        qor_pp_assert_that(called);
    }
}
