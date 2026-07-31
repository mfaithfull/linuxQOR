// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"

#include <iostream>
#include <string>
#include <vector>

#include "src/qor/tdd/tdd.h"
#include "src/qor/function/tef.h"
#include "test_common.h"

using namespace qor;
using namespace qor::tef;

class Class;

struct TypeErasedFunctionTypeTraitsTestSuite{};

qor_pp_test_suite_case(TypeErasedFunctionTypeTraitsTestSuite, _IsFunctionPointer) 
{
  qor_pp_assert_that(IsFunctionPointer<decltype(&FreeFunction)>::value);
  qor_pp_assert_that(IsFunctionPointer<decltype(&FreeFunction)&>::value);
  qor_pp_assert_that(IsFunctionPointer<decltype(&FreeFunction)&&>::value);
  qor_pp_assert_that(IsFunctionPointer<void (*)()>::value);

  qor_pp_assert_that(!IsFunctionPointer<decltype(&Object::Function)>::value);
  qor_pp_assert_that(!IsFunctionPointer<void (Object::*)()>::value);
  qor_pp_assert_that(!IsFunctionPointer<void()>::value);
  qor_pp_assert_that(!IsFunctionPointer<void*>::value);
}

qor_pp_test_suite_case(TypeErasedFunctionTypeTraitsTestSuite, _IsFunction) 
{
  qor_pp_assert_that(IsFunction<Function<void()>>::value);
  qor_pp_assert_that(IsFunction<Function<void()>&>::value);
  qor_pp_assert_that(IsFunction<Function<void()>&&>::value);
  qor_pp_assert_that(IsFunction<Function<void()> const>::value);
  qor_pp_assert_that(IsFunction<Function<void (*)()>>::value);
  qor_pp_assert_that(IsFunction<Function<Function<int>>>::value);

  qor_pp_assert_that(!IsFunction<int>::value);
  qor_pp_assert_that(!IsFunction<MemberFunction<void (Class::*)()>>::value);
}

qor_pp_test_suite_case(TypeErasedFunctionTypeTraitsTestSuite, _IsMemberFunction) 
{
  qor_pp_assert_that(IsMemberFunction<MemberFunction<void (Class::*)()>>::value);
  qor_pp_assert_that(IsMemberFunction<MemberFunction<void (Class::*)()&>>::value);
  qor_pp_assert_that(IsMemberFunction<MemberFunction<void (Class::*)()&&>>::value);
  qor_pp_assert_that(IsMemberFunction<MemberFunction<void (Class::*)() const>>::value);
  qor_pp_assert_that(IsMemberFunction<MemberFunction<void (*)()>>::value);
  qor_pp_assert_that(IsMemberFunction<MemberFunction<MemberFunction<int>>>::value);

  qor_pp_assert_that(!IsMemberFunction<int>::value);
  qor_pp_assert_that(!IsMemberFunction<Function<void(int)>>::value);
}

qor_pp_test_suite_case(TypeErasedFunctionTypeTraitsTestSuite, _IsSharedPtr) 
{
  qor_pp_assert_that(IsSharedPtr<std::shared_ptr<int>>::value);
  qor_pp_assert_that(IsSharedPtr<std::shared_ptr<int>&>::value);
  qor_pp_assert_that(IsSharedPtr<std::shared_ptr<int>&&>::value);
  qor_pp_assert_that(IsSharedPtr<std::shared_ptr<int> const>::value);
  qor_pp_assert_that(IsSharedPtr<std::shared_ptr<const int>>::value);
  qor_pp_assert_that(IsSharedPtr<std::shared_ptr<const int*>>::value);
  qor_pp_assert_that(IsSharedPtr<std::shared_ptr<std::shared_ptr<int>>>::value);

  qor_pp_assert_that(!IsSharedPtr<int>::value);
}
