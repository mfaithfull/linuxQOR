// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"

#include "src/qor/tdd/tdd.h"
#include "src/qor/reflection/reflection.h"

using namespace qor;
using namespace qor::test;

struct some_person 
{
    std::string name;
    unsigned birth_year;
};

struct foo 
{            // defining structure
    int some_integer;
    char c;
};

qor_pp_test_case(canDoReflection101)
{
    some_person val{"Edgar Allan Poe", 1809};

    std::cout << qor_reflection::get<0>(val)                // No macro!
        << " was born in " << qor_reflection::get<1>(val);  // Works with any aggregate initializables!

    std::cout << qor_reflection::io(val);                   // Outputs: {"Edgar Allan Poe", 1809}    
    qor_pp_assert_that(true).isTrue();
}

constexpr std::string_view n1 = qor_reflection::get_name<0, some_person>();
constexpr std::string_view n2 = qor_reflection::get_name<1, some_person>();

qor_pp_test_case(canGetNamesbyReflection)
{
    qor_pp_assert_that(n1 == "name");
    qor_pp_assert_that(n2 == "birth_year");
}

qor_pp_test_case(canGetFieldsByIndex)
{
    foo f {777, '!'};
    auto& r1 = qor_reflection::get<0>(f); // accessing field with index 0, returns reference to `foo::some_integer`
    auto& r2 = qor_reflection::get<1>(f); // accessing field with index 1, returns reference to `foo::c`

    qor_pp_assert_that(r1).isEqualTo(777);
    qor_pp_assert_that(r2).isEqualTo('!');

    r1 = 42;
    r2 = 'A';

    qor_pp_assert_that(r1).isEqualTo(42);
    qor_pp_assert_that(r2).isEqualTo('A');

    qor_pp_assert_that(f.some_integer).isEqualTo(42);
    qor_pp_assert_that(f.c).isEqualTo('A');
}