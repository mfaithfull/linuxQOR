// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

// This test needs C++ exceptions thrown by MagicFunc exceptions to work.
// These exceptions are turned off in release builds that define NDEBUG.
#undef NDEBUG

#include "src/configuration/configuration.h"
#include "src/qor/tdd/tdd.h"
#include "src/qor/function/error.h"
#include "src/qor/function/make_function.h"
#include "src/qor/function/type_erased_function.h"

#include "test_common.h"

using namespace qor;
using namespace qor::tef;

struct TypeErasedFunctionTestSuite{};

qor_pp_test_suite_case(TypeErasedFunctionTestSuite, Empty) 
{
  TypeErasedFunction function;
  qor_pp_assert_that(!function);
  qor_pp_assert_that(function == nullptr);
  qor_pp_assert_that(function.type_id()).isEqualTo(0) ;
  qor_pp_assert_that(function.GetObject()).isEqualTo(nullptr);
}

qor_pp_test_suite_case(TypeErasedFunctionTestSuite, Assign) 
{
    auto function = qor_pp_make_function(&FreeFunction);
    auto member_function = qor_pp_make_function(&Object::Function);
    try 
    {
        TypeErasedFunction type_erased_function;

        // Assigning to another empty type-erased function should work because there is no type set yet.
        type_erased_function = TypeErasedFunction();
        qor_pp_assert_that(!type_erased_function);        
        qor_pp_assert_that(type_erased_function == nullptr);
        qor_pp_assert_that(0 == type_erased_function.type_id());

        // Assigning a new type should work.
        type_erased_function = function;
        qor_pp_assert_that(!!type_erased_function);
        qor_pp_assert_that(type_erased_function != nullptr);
        qor_pp_assert_that(0 != type_erased_function.type_id());

        // Assigning a new Function of the same type should work.
        type_erased_function = qor_pp_make_function(&FreeFunction);        

        // This should throw an invalid cast excepton.
        type_erased_function = member_function;
        

    } 
    catch (const Serious& error) 
    {
        qor_pp_assert_that(error.what().Content() == "Actual types behind type erasure are not compatible.").isTrue();        
    }

    try 
    {
        TypeErasedFunction type_erased_function;
        type_erased_function = function;        

        // Trying to assign a typeless type-erased function when it already as a
        // type should throw an exception.
        type_erased_function = TypeErasedFunction();        

    } 
    catch (const Serious& error) 
    {
        qor_pp_assert_that(error.what().Content() == "Actual types behind type erasure are not compatible.").isTrue();
    }

    try 
    {
        TypeErasedFunction type_erased_function;
        type_erased_function = member_function;        

        // Member functions with same argument and return types but different
        // qualifications are considered different types. This should fail.
        type_erased_function = qor_pp_make_function(&Object::ConstFunction);        

    } 
    catch (const Serious& error) 
    {
        qor_pp_assert_that(error.what().Content() == "Actual types behind type erasure are not compatible.").isTrue();
    }
}
