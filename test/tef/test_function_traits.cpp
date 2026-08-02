// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0


#include "src/configuration/configuration.h"
#include "src/qor/tdd/tdd.h"
#include "src/qor/function/function_traits.h"

#include "test_common.h"

using namespace qor;
using namespace qor::tef;

namespace {

    class Callable 
    {
    public:
        const char* operator()(const int&, int&&) { return nullptr; }
    };

    int SampleFunction(const char*, float&, Callable&&);

    struct TestObject 
    {
        int Function(const char*, float&, Callable&&);
        int ConstFunction(const char*, float&, Callable&&) const;
        int VolatileFunction(const char*, float&, Callable&&) volatile;
        int ConstVolatileFunction(const char*, float&, Callable&&) const volatile;
    };

}

struct TypeErasedFunctionTraitsTestSuite{};

qor_pp_test_suite_case(TypeErasedFunctionTraitsTestSuite, FreeFunctionType) 
{
    using T = FunctionTraits<int(const char*, float&, Callable&&)>;
    qor_pp_assert_that((std::is_same<T::Return, int>::value));
    qor_pp_assert_that((std::is_same<
        T::Args, std::tuple<const char*, float&, Callable&&>>::value));
    qor_pp_assert_that((std::is_same<
        T::FunctionType, int(const char*, float&, Callable&&)>::value));
    qor_pp_assert_that((std::is_same<
        T::FunctionPointerType,
        int (*) (const char*, float&, Callable&&)>::value));
    qor_pp_assert_that(3 == T::kNumArgs);
}

qor_pp_test_suite_case(TypeErasedFunctionTraitsTestSuite, FreeFunctionPointer) 
{
    using T = FunctionTraits<decltype(&SampleFunction)>;
    qor_pp_assert_that((std::is_same<T::Return, int>::value));
    qor_pp_assert_that((std::is_same<
        T::Args, std::tuple<const char*, float&, Callable&&>>::value));
    qor_pp_assert_that((std::is_same<
        T::FunctionType, int(const char*, float&, Callable&&)>::value));
    qor_pp_assert_that((std::is_same<
        T::FunctionPointerType,
        decltype(&SampleFunction)>::value));
    qor_pp_assert_that(3 == T::kNumArgs);
}

qor_pp_test_suite_case(TypeErasedFunctionTraitsTestSuite, MemberFunctionPointer) 
{
    using T = FunctionTraits<decltype(&TestObject::Function)>;
    qor_pp_assert_that((std::is_same<T::Class, TestObject>::value));
    qor_pp_assert_that((std::is_same<T::Return, int>::value));
    qor_pp_assert_that((std::is_same<
        T::Args, std::tuple<const char*, float&, Callable&&>>::value));
    qor_pp_assert_that((std::is_same<
        T::FunctionType, int(const char*, float&, Callable&&)>::value));
    qor_pp_assert_that((std::is_same<
        T::FunctionPointerType,
        decltype(&TestObject::Function)>::value));
    qor_pp_assert_that(!T::kIsConst);
    qor_pp_assert_that(!T::kIsVolatile);
    qor_pp_assert_that(3 == T::kNumArgs);
}

qor_pp_test_suite_case(TypeErasedFunctionTraitsTestSuite, MemberConstFunctionPointer) 
{
    using T = FunctionTraits<decltype(&TestObject::ConstFunction)>;
    qor_pp_assert_that((std::is_same<T::Class, const TestObject>::value));
    qor_pp_assert_that((std::is_same<T::Return, int>::value));
    qor_pp_assert_that((std::is_same<
        T::Args, std::tuple<const char*, float&, Callable&&>>::value));
    qor_pp_assert_that((std::is_same<
        T::FunctionType, int(const char*, float&, Callable&&)>::value));
    qor_pp_assert_that((std::is_same<
        T::FunctionPointerType,
        decltype(&TestObject::ConstFunction)>::value));
    qor_pp_assert_that(!!T::kIsConst);
    qor_pp_assert_that(!T::kIsVolatile);
    qor_pp_assert_that(3 == T::kNumArgs);
}

qor_pp_test_suite_case(TypeErasedFunctionTraitsTestSuite, MemberVolatileFunctionPointer) 
{
    using T = FunctionTraits<decltype(&TestObject::VolatileFunction)>;
    qor_pp_assert_that((std::is_same<T::Class, volatile TestObject>::value));
    qor_pp_assert_that((std::is_same<T::Return, int>::value));
    qor_pp_assert_that((std::is_same<
        T::Args, std::tuple<const char*, float&, Callable&&>>::value));
    qor_pp_assert_that((std::is_same<
        T::FunctionType, int(const char*, float&, Callable&&)>::value));
    qor_pp_assert_that((std::is_same<
        T::FunctionPointerType,
        decltype(&TestObject::VolatileFunction)>::value));
    qor_pp_assert_that(!T::kIsConst);
    qor_pp_assert_that(!!T::kIsVolatile);
    qor_pp_assert_that(3 == T::kNumArgs);
}

qor_pp_test_suite_case(TypeErasedFunctionTraitsTestSuite, MemberConstVolatileFunctionPointer) 
{
    using T = FunctionTraits<decltype(&TestObject::ConstVolatileFunction)>;
    qor_pp_assert_that((std::is_same<T::Class, const volatile TestObject>::value));
    qor_pp_assert_that((std::is_same<T::Return, int>::value));
    qor_pp_assert_that((std::is_same<
        T::Args, std::tuple<const char*, float&, Callable&&>>::value));
    qor_pp_assert_that((std::is_same<
        T::FunctionType, int(const char*, float&, Callable&&)>::value));
    qor_pp_assert_that((std::is_same<
        T::FunctionPointerType,
        decltype(&TestObject::ConstVolatileFunction)>::value));
    qor_pp_assert_that(!!T::kIsConst);
    qor_pp_assert_that(!!T::kIsVolatile);
    qor_pp_assert_that(3 == T::kNumArgs);
}

qor_pp_test_suite_case(TypeErasedFunctionTraitsTestSuite, _CallableType) 
{
    auto lambda = [](int x, int y) { return x + y; };

    qor_pp_assert_that((std::is_same<typename FunctionTraits<CallableType<decltype(lambda)>>::FunctionType,int(int, int)>::value));
    qor_pp_assert_that((std::is_same<typename FunctionTraits<CallableType<Callable>>::FunctionType,const char*(const int&, int&&)>::value));
}
