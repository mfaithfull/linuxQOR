// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

//JSON generation test

#include "src/configuration/configuration.h"
#include "src/qor/tdd/tdd.h"
#include <cstdio>
#include <limits>
#include <fstream>
#include <string>

#include "src/qor/memory/reference/newref.h"
#include "src/components/io/generator/generator.h"
#include "src/components/io/generator/state.h"
#include "src/components/io/generator/context.h"
#include "src/components/data/formats/json/generator/states/_null.h"
#include "src/components/data/formats/json/generator/states/_true.h"
#include "src/components/data/formats/json/generator/states/_false.h"
#include "src/components/data/formats/json/generator/states/_string.h"
#include "src/components/data/formats/json/generator/states/_number.h"
#include "src/components/data/formats/json/generator/states/_array.h"
#include "src/components/data/formats/json/generator/states/_object.h"
#include "src/components/data/formats/json/generator/states/_value.h"
#include "src/components/data/formats/json/generator/states/_comma.h"
#include "src/components/data/formats/json/generator/states/_colon.h"
#include "src/components/data/formats/json/generator/states/_member.h"
#include "src/components/data/formats/json/model/value.h"
#include "src/components/data/formats/json/model/string.h"
#include "src/components/data/formats/json/model/member.h"
#include "src/components/data/formats/json/model/array.h"
#include "src/components/data/formats/json/model/object.h"
#include "src/components/data/formats/json/model/number.h"
#include "test/components/formats/json/jsonwriter.h"

namespace qor {

    qor_pp_test_case(json_generator_writes_null)
    {
        byte buffer[16] = {};

        auto context = new_ref<qor::components::generator::Context>(buffer, static_cast<size_t>(sizeof(buffer)));

        qor::components::generator::Generator generator(context);

        generator.SetInitialState(new_ref<qor::data::generator::json::_null>(&generator));

        generator.Generate();

        qor_pp_assert_that(context->GetPosition() == 4).isTrue();
        qor_pp_assert_that(buffer[0] == 'n' && buffer[1] == 'u' && buffer[2] == 'l' && buffer[3] == 'l').isTrue();
    }

    qor_pp_test_case(json_generator_writes_true)
    {
        byte buffer[16] = {};

        auto context = new_ref<qor::components::generator::Context>(buffer, static_cast<size_t>(sizeof(buffer)));

        qor::components::generator::Generator generator(context);

        generator.SetInitialState(new_ref<qor::data::generator::json::_true>(&generator));

        generator.Generate();

        qor_pp_assert_that(context->GetPosition() == 4).isTrue();
        qor_pp_assert_that(buffer[0] == 't' && buffer[1] == 'r' && buffer[2] == 'u' && buffer[3] == 'e').isTrue();
    }

    qor_pp_test_case(json_generator_writes_false)
    {
        byte buffer[16] = {};

        auto context = new_ref<qor::components::generator::Context>(buffer, static_cast<size_t>(sizeof(buffer)));

        qor::components::generator::Generator generator(context);

        generator.SetInitialState(new_ref<qor::data::generator::json::_false>(&generator));

        generator.Generate();

        qor_pp_assert_that(context->GetPosition() == 5).isTrue();
        qor_pp_assert_that(buffer[0] == 'f' && buffer[1] == 'a' && buffer[2] == 'l' && buffer[3] == 's' && buffer[4] == 'e').isTrue();
    }

    qor_pp_test_case(json_generator_writes_string)
    {
        byte buffer[16] = {};

        auto context = new_ref<qor::components::generator::Context>(buffer, static_cast<size_t>(sizeof(buffer)));

        qor::components::generator::Generator generator(context);

        generator.SetInitialState(new_ref<qor::data::generator::json::_string>(&generator, "hi", 2));

        generator.Generate();

        qor_pp_assert_that(context->GetPosition() == 4).isTrue();
        qor_pp_assert_that(buffer[0] == '"' && buffer[1] == 'h' && buffer[2] == 'i' && buffer[3] == '"').isTrue();
    }

    qor_pp_test_case(json_generator_writes_zero)
    {
        byte buffer[16] = {};

        auto context = new_ref<qor::components::generator::Context>(buffer, static_cast<size_t>(sizeof(buffer)));

        qor::components::generator::Generator generator(context);

        generator.SetInitialState(new_ref<qor::data::generator::json::_number>(&generator, 0LL));

        generator.Generate();

        qor_pp_assert_that(context->GetPosition() == 1).isTrue();
        qor_pp_assert_that(buffer[0] == '0').isTrue();
    }

    qor_pp_test_case(json_generator_writes_negative_integer)
    {
        byte buffer[16] = {};

        auto context = new_ref<qor::components::generator::Context>(buffer, static_cast<size_t>(sizeof(buffer)));

        qor::components::generator::Generator generator(context);

        generator.SetInitialState(new_ref<qor::data::generator::json::_number>(&generator, -123LL));

        generator.Generate();

        qor_pp_assert_that(context->GetPosition() == 4).isTrue();
        qor_pp_assert_that(buffer[0] == '-' && buffer[1] == '1' && buffer[2] == '2' && buffer[3] == '3').isTrue();
    }

    qor_pp_test_case(json_generator_writes_empty_array)
    {
        byte buffer[16] = {};

        auto context = new_ref<qor::components::generator::Context>(buffer, static_cast<size_t>(sizeof(buffer)));

        qor::components::generator::Generator generator(context);

        generator.SetInitialState(new_ref<qor::data::generator::json::_array>(&generator));

        generator.Generate();

        qor_pp_assert_that(context->GetPosition() == 2).isTrue();
        qor_pp_assert_that(buffer[0] == '[' && buffer[1] == ']').isTrue();
    }

    qor_pp_test_case(json_generator_writes_empty_object)
    {
        byte buffer[16] = {};

        auto context = new_ref<qor::components::generator::Context>(buffer, static_cast<size_t>(sizeof(buffer)));

        qor::components::generator::Generator generator(context);

        generator.SetInitialState(new_ref<qor::data::generator::json::_object>(&generator));

        generator.Generate();

        qor_pp_assert_that(context->GetPosition() == 2).isTrue();
        qor_pp_assert_that(buffer[0] == '{' && buffer[1] == '}').isTrue();
    }

    qor_pp_test_case(json_generator_escapes_quotation_mark)
    {
        byte buffer[16] = {};

        auto context = new_ref<qor::components::generator::Context>(buffer, static_cast<size_t>(sizeof(buffer)));

        qor::components::generator::Generator generator(context);

        generator.SetInitialState(new_ref<qor::data::generator::json::_string>(&generator, "a\"b", 3));

        generator.Generate();

        qor_pp_assert_that(context->GetPosition() == 6).isTrue();
        qor_pp_assert_that(buffer[0] == '"' && buffer[1] == 'a' && buffer[2] == '\\' && buffer[3] == '"' && buffer[4] == 'b' && buffer[5] == '"').isTrue();
    }

    qor_pp_test_case(json_generator_escapes_control_character)
    {
        byte buffer[16] = {};

        auto context = new_ref<qor::components::generator::Context>(buffer, static_cast<size_t>(sizeof(buffer)));

        qor::components::generator::Generator generator(context);

        generator.SetInitialState(new_ref<qor::data::generator::json::_string>(&generator, "x\ny", 3));

        generator.Generate();

        qor_pp_assert_that(context->GetPosition() == 6).isTrue();
        qor_pp_assert_that(buffer[0] == '"' && buffer[1] == 'x' && buffer[2] == '\\' && buffer[3] == 'n' && buffer[4] == 'y' && buffer[5] == '"').isTrue();
    }

    // A model value with no value model set has the _null type, so the value
    // state has to ask the model what to write rather than being told directly.
    qor_pp_test_case(json_generator_writes_model_value)
    {
        byte buffer[16] = {};

        auto context = new_ref<qor::components::generator::Context>(buffer, static_cast<size_t>(sizeof(buffer)));

        qor::components::generator::Generator generator(context);

        qor::data::model::json::Value value;

        generator.SetInitialState(new_ref<qor::data::generator::json::_value>(&generator, &value));

        generator.Generate();

        qor_pp_assert_that(context->GetPosition() == 4).isTrue();
        qor_pp_assert_that(buffer[0] == 'n' && buffer[1] == 'u' && buffer[2] == 'l' && buffer[3] == 'l').isTrue();
    }

    qor_pp_test_case(json_generator_writes_comma)
    {
        byte buffer[4] = {};

        auto context = new_ref<qor::components::generator::Context>(buffer, static_cast<size_t>(sizeof(buffer)));

        qor::components::generator::Generator generator(context);

        generator.SetInitialState(new_ref<qor::data::generator::json::_comma>(&generator));

        generator.Generate();

        qor_pp_assert_that(context->GetPosition() == 1).isTrue();
        qor_pp_assert_that(buffer[0] == ',').isTrue();
    }

    qor_pp_test_case(json_generator_writes_colon)
    {
        byte buffer[4] = {};

        auto context = new_ref<qor::components::generator::Context>(buffer, static_cast<size_t>(sizeof(buffer)));

        qor::components::generator::Generator generator(context);

        generator.SetInitialState(new_ref<qor::data::generator::json::_colon>(&generator));

        generator.Generate();

        qor_pp_assert_that(context->GetPosition() == 1).isTrue();
        qor_pp_assert_that(buffer[0] == ':').isTrue();
    }

    qor_pp_test_case(json_generator_writes_member_null)
    {
        byte buffer[16] = {};

        auto context = new_ref<qor::components::generator::Context>(buffer, static_cast<size_t>(sizeof(buffer)));

        qor::components::generator::Generator generator(context);

        qor::data::model::json::Member member;
        member.SetName("k");

        generator.SetInitialState(new_ref<qor::data::generator::json::_member>(&generator, &member));

        generator.Generate();

        qor_pp_assert_that(context->GetPosition() == 8).isTrue();
        qor_pp_assert_that(buffer[0] == '"' && buffer[1] == 'k' && buffer[2] == '"' && buffer[3] == ':' && buffer[4] == 'n' && buffer[5] == 'u' && buffer[6] == 'l' && buffer[7] == 'l').isTrue();
    }

    qor_pp_test_case(json_generator_writes_array_one_null)
    {
        byte buffer[16] = {};

        auto context = new_ref<qor::components::generator::Context>(buffer, static_cast<size_t>(sizeof(buffer)));

        qor::components::generator::Generator generator(context);

        qor::data::model::json::Array array;
        array.m_values.push_back(new_ref<qor::data::model::json::Value>());

        generator.SetInitialState(new_ref<qor::data::generator::json::_array>(&generator, &array));

        generator.Generate();

        qor_pp_assert_that(context->GetPosition() == 6).isTrue();
    }

    qor_pp_test_case(json_generator_writes_array_two_nulls)
    {
        byte buffer[16] = {};

        auto context = new_ref<qor::components::generator::Context>(buffer, static_cast<size_t>(sizeof(buffer)));

        qor::components::generator::Generator generator(context);

        qor::data::model::json::Array array;
        array.m_values.push_back(new_ref<qor::data::model::json::Value>());
        array.m_values.push_back(new_ref<qor::data::model::json::Value>());

        generator.SetInitialState(new_ref<qor::data::generator::json::_array>(&generator, &array));

        generator.Generate();

        qor_pp_assert_that(context->GetPosition() == 11).isTrue();
        qor_pp_assert_that(buffer[0] == '[' && buffer[1] == 'n' && buffer[2] == 'u' && buffer[3] == 'l' && buffer[4] == 'l' && buffer[5] == ',' && buffer[6] == 'n' && buffer[7] == 'u' && buffer[8] == 'l' && buffer[9] == 'l' && buffer[10] == ']').isTrue();
    }

    qor_pp_test_case(json_generator_writes_object_one_member)
    {
        byte buffer[16] = {};

        auto context = new_ref<qor::components::generator::Context>(buffer, static_cast<size_t>(sizeof(buffer)));

        qor::components::generator::Generator generator(context);

        qor::data::model::json::Value value;

        qor::data::model::json::Object object;
        auto member = new_ref<qor::data::model::json::Member>();
        member->SetName("a");
        object.m_members.push_back(member);
        value.SetValue(polymorphic<qor::data::model::json::ValueModel>(object));

        generator.SetInitialState(new_ref<qor::data::generator::json::_value>(&generator, &value));

        generator.Generate();

        qor_pp_assert_that(context->GetPosition() == 10).isTrue();
        qor_pp_assert_that(buffer[0] == '{' && buffer[9] == '}').isTrue();
    }

    qor_pp_test_case(json_generator_value_routes_to_array)
    {
        byte buffer[16] = {};

        auto context = new_ref<qor::components::generator::Context>(buffer, static_cast<size_t>(sizeof(buffer)));

        qor::components::generator::Generator generator(context);

        qor::data::model::json::Value value;

        qor::data::model::json::Array array;
        array.m_values.push_back(new_ref<qor::data::model::json::Value>());
        value.SetValue(polymorphic<qor::data::model::json::ValueModel>(array));

        generator.SetInitialState(new_ref<qor::data::generator::json::_value>(&generator, &value));

        generator.Generate();

        qor_pp_assert_that(context->GetPosition() == 6).isTrue();
        qor_pp_assert_that(buffer[0] == '[' && buffer[1] == 'n' && buffer[2] == 'u' && buffer[3] == 'l' && buffer[4] == 'l' && buffer[5] == ']').isTrue();
    }

    qor_pp_test_case(json_generator_value_routes_to_object)
    {
        byte buffer[16] = {};

        auto context = new_ref<qor::components::generator::Context>(buffer, static_cast<size_t>(sizeof(buffer)));

        qor::components::generator::Generator generator(context);

        qor::data::model::json::Value value;

        qor::data::model::json::Object object;
        auto member = new_ref<qor::data::model::json::Member>();
        member->SetName("a");
        object.m_members.push_back(member);
        value.SetValue(polymorphic<qor::data::model::json::ValueModel>(object));

        generator.SetInitialState(new_ref<qor::data::generator::json::_value>(&generator, &value));

        generator.Generate();

        qor_pp_assert_that(context->GetPosition() == 10).isTrue();
        qor_pp_assert_that(buffer[0] == '{' && buffer[1] == '"' && buffer[2] == 'a' && buffer[3] == '"' && buffer[4] == ':' && buffer[5] == 'n' && buffer[6] == 'u' && buffer[7] == 'l' && buffer[8] == 'l' && buffer[9] == '}').isTrue();
    }

    qor_pp_test_case(json_generator_value_routes_to_number_integer)
    {
        byte buffer[16] = {};

        auto context = new_ref<qor::components::generator::Context>(buffer, static_cast<size_t>(sizeof(buffer)));

        qor::components::generator::Generator generator(context);

        qor::data::model::json::Value value;
        qor::data::model::json::Number number;
        number.SetValue(123.0);
        value.SetValue(polymorphic<qor::data::model::json::ValueModel>(number));

        generator.SetInitialState(new_ref<qor::data::generator::json::_value>(&generator, &value));

        generator.Generate();

        qor_pp_assert_that(context->GetPosition() == 3).isTrue();
        qor_pp_assert_that(buffer[0] == '1' && buffer[1] == '2' && buffer[2] == '3').isTrue();
    }

    qor_pp_test_case(json_generator_value_routes_to_number_negative)
    {
        byte buffer[16] = {};

        auto context = new_ref<qor::components::generator::Context>(buffer, static_cast<size_t>(sizeof(buffer)));

        qor::components::generator::Generator generator(context);

        qor::data::model::json::Value value;
        qor::data::model::json::Number number;
        number.SetValue(-7.0);
        value.SetValue(polymorphic<qor::data::model::json::ValueModel>(number));

        generator.SetInitialState(new_ref<qor::data::generator::json::_value>(&generator, &value));

        generator.Generate();

        qor_pp_assert_that(context->GetPosition() == 2).isTrue();
        qor_pp_assert_that(buffer[0] == '-' && buffer[1] == '7').isTrue();
    }

    qor_pp_test_case(json_generator_value_routes_to_number_fraction)
    {
        byte buffer[16] = {};

        auto context = new_ref<qor::components::generator::Context>(buffer, static_cast<size_t>(sizeof(buffer)));

        qor::components::generator::Generator generator(context);

        qor::data::model::json::Value value;
        qor::data::model::json::Number number;
        number.SetValue(12.5);
        value.SetValue(polymorphic<qor::data::model::json::ValueModel>(number));

        generator.SetInitialState(new_ref<qor::data::generator::json::_value>(&generator, &value));

        generator.Generate();

        qor_pp_assert_that(context->GetPosition() == 4).isTrue();
        qor_pp_assert_that(buffer[0] == '1' && buffer[1] == '2' && buffer[2] == '.' && buffer[3] == '5').isTrue();
    }

    //Scientific notation is valid JSON and is necessary for values that cannot
    //be written economically as a long run of decimal digits.
    qor_pp_test_case(json_generator_writes_exponent_number)
    {
        byte buffer[16] = {};

        auto context = new_ref<qor::components::generator::Context>(buffer, static_cast<size_t>(sizeof(buffer)));

        qor::components::generator::Generator generator(context);

        qor::data::model::json::Value value;
        qor::data::model::json::Number number;
        number.SetValue(1e10);
        value.SetValue(polymorphic<qor::data::model::json::ValueModel>(number));

        generator.SetInitialState(new_ref<qor::data::generator::json::_value>(&generator, &value));

        generator.Generate();

        qor_pp_assert_that(context->GetPosition() == 5).isTrue();
        qor_pp_assert_that(buffer[0] == '1' && buffer[1] == 'e' && buffer[2] == '+' && buffer[3] == '1' && buffer[4] == '0').isTrue();
    }

    qor_pp_test_case(json_generator_writes_negative_exponent_number)
    {
        byte buffer[16] = {};

        auto context = new_ref<qor::components::generator::Context>(buffer, static_cast<size_t>(sizeof(buffer)));

        qor::components::generator::Generator generator(context);

        qor::data::model::json::Value value;
        qor::data::model::json::Number number;
        number.SetValue(-1e-10);
        value.SetValue(polymorphic<qor::data::model::json::ValueModel>(number));

        generator.SetInitialState(new_ref<qor::data::generator::json::_value>(&generator, &value));

        generator.Generate();

        qor_pp_assert_that(context->GetPosition() == 6).isTrue();
        qor_pp_assert_that(buffer[0] == '-' && buffer[1] == '1' && buffer[2] == 'e' && buffer[3] == '-' && buffer[4] == '1' && buffer[5] == '0').isTrue();
    }

    //The JSON number grammar has no token for non finite values, so the value
    //state must fail without emitting any output.
    qor_pp_test_case(json_generator_value_rejects_nonfinite_number)
    {
        byte buffer[4] = {};

        auto context = new_ref<qor::components::generator::Context>(buffer, static_cast<size_t>(sizeof(buffer)));

        qor::components::generator::Generator generator(context);

        qor::data::model::json::Value value;
        qor::data::model::json::Number number;
        number.SetValue(std::numeric_limits<double>::infinity());
        value.SetValue(polymorphic<qor::data::model::json::ValueModel>(number));

        generator.SetInitialState(new_ref<qor::data::generator::json::_value>(&generator, &value));

        generator.Generate();

        qor_pp_assert_that(context->GetPosition() == 0).isTrue();
        qor_pp_assert_that(buffer[0] == 0).isTrue();
    }

    qor_pp_test_case(json_generator_number_model_rejects_nonfinite)
    {
        byte buffer[4] = {};

        auto context = new_ref<qor::components::generator::Context>(buffer, static_cast<size_t>(sizeof(buffer)));

        qor::components::generator::Generator generator(context);

        qor::data::model::json::Number number;
        number.SetValue(std::numeric_limits<double>::infinity());

        generator.SetInitialState(new_ref<qor::data::generator::json::_number>(&generator, &number));

        generator.Generate();

        qor_pp_assert_that(context->GetPosition() == 0).isTrue();
        qor_pp_assert_that(buffer[0] == 0).isTrue();
    }

    //A nested array value is modelled as an element Value whose polymorphic
    //value model is another Array.
    qor_pp_test_case(json_generator_writes_nested_array_number)
    {
        byte buffer[16] = {};

        auto context = new_ref<qor::components::generator::Context>(buffer, static_cast<size_t>(sizeof(buffer)));

        qor::components::generator::Generator generator(context);

        qor::data::model::json::Number number;
        number.SetValue(7.0);

        qor::data::model::json::Array inner;
        auto innerElement = new_ref<qor::data::model::json::Value>();
        innerElement->SetValue(polymorphic<qor::data::model::json::ValueModel>(number));
        inner.m_values.push_back(innerElement);

        qor::data::model::json::Array outer;
        auto outerElement = new_ref<qor::data::model::json::Value>();
        outerElement->SetValue(polymorphic<qor::data::model::json::ValueModel>(inner));
        outer.m_values.push_back(outerElement);

        generator.SetInitialState(new_ref<qor::data::generator::json::_array>(&generator, &outer));

        generator.Generate();

        qor_pp_assert_that(context->GetPosition() == 5).isTrue();
        qor_pp_assert_that(buffer[0] == '[' && buffer[1] == '[' && buffer[2] == '7' && buffer[3] == ']' && buffer[4] == ']').isTrue();
    }

    qor_pp_test_case(json_generator_writes_array_string_element)
    {
        byte buffer[16] = {};

        auto context = new_ref<qor::components::generator::Context>(buffer, static_cast<size_t>(sizeof(buffer)));

        qor::components::generator::Generator generator(context);

        qor::data::model::json::String text;
        text.SetValue("hi");

        qor::data::model::json::Array array;
        auto element = new_ref<qor::data::model::json::Value>();
        element->SetValue(polymorphic<qor::data::model::json::ValueModel>(text));
        array.m_values.push_back(element);

        generator.SetInitialState(new_ref<qor::data::generator::json::_array>(&generator, &array));

        generator.Generate();

        qor_pp_assert_that(context->GetPosition() == 6).isTrue();
        qor_pp_assert_that(buffer[0] == '[' && buffer[1] == '"' && buffer[2] == 'h' && buffer[3] == 'i' && buffer[4] == '"' && buffer[5] == ']').isTrue();
    }

    //A member value is modelled directly in the Member, so an object member can
    //hold a number, a string, or another container.
    qor_pp_test_case(json_generator_writes_object_number_member)
    {
        byte buffer[16] = {};

        auto context = new_ref<qor::components::generator::Context>(buffer, static_cast<size_t>(sizeof(buffer)));

        qor::components::generator::Generator generator(context);

        qor::data::model::json::Number number;
        number.SetValue(7.0);

        qor::data::model::json::Object object;
        auto member = new_ref<qor::data::model::json::Member>();
        member->SetName("a");
        member->GetValue().SetValue(polymorphic<qor::data::model::json::ValueModel>(number));
        object.m_members.push_back(member);

        generator.SetInitialState(new_ref<qor::data::generator::json::_object>(&generator, &object));

        generator.Generate();

        qor_pp_assert_that(context->GetPosition() == 7).isTrue();
        qor_pp_assert_that(buffer[0] == '{' && buffer[1] == '"' && buffer[2] == 'a' && buffer[3] == '"' && buffer[4] == ':' && buffer[5] == '7' && buffer[6] == '}').isTrue();
    }

    qor_pp_test_case(json_generator_writes_object_string_member)
    {
        byte buffer[16] = {};

        auto context = new_ref<qor::components::generator::Context>(buffer, static_cast<size_t>(sizeof(buffer)));

        qor::components::generator::Generator generator(context);

        qor::data::model::json::String text;
        text.SetValue("hi");

        qor::data::model::json::Object object;
        auto member = new_ref<qor::data::model::json::Member>();
        member->SetName("a");
        member->GetValue().SetValue(polymorphic<qor::data::model::json::ValueModel>(text));
        object.m_members.push_back(member);

        generator.SetInitialState(new_ref<qor::data::generator::json::_object>(&generator, &object));

        generator.Generate();

        qor_pp_assert_that(context->GetPosition() == 10).isTrue();
        qor_pp_assert_that(buffer[0] == '{' && buffer[1] == '"' && buffer[2] == 'a' && buffer[3] == '"' && buffer[4] == ':' && buffer[5] == '"' && buffer[6] == 'h' && buffer[7] == 'i' && buffer[8] == '"' && buffer[9] == '}').isTrue();
    }

    qor_pp_test_case(json_generator_writes_nested_object_object)
    {
        byte buffer[32] = {};

        auto context = new_ref<qor::components::generator::Context>(buffer, static_cast<size_t>(sizeof(buffer)));

        qor::components::generator::Generator generator(context);

        qor::data::model::json::Object inner;
        auto innerMember = new_ref<qor::data::model::json::Member>();
        innerMember->SetName("b");
        inner.m_members.push_back(innerMember);

        qor::data::model::json::Object outer;
        auto outerMember = new_ref<qor::data::model::json::Member>();
        outerMember->SetName("a");
        outerMember->GetValue().SetValue(polymorphic<qor::data::model::json::ValueModel>(inner));
        outer.m_members.push_back(outerMember);

        generator.SetInitialState(new_ref<qor::data::generator::json::_object>(&generator, &outer));

        generator.Generate();

        qor_pp_assert_that(context->GetPosition() == 16).isTrue();
        qor_pp_assert_that(buffer[0] == '{' && buffer[1] == '"' && buffer[2] == 'a' && buffer[3] == '"' && buffer[4] == ':' && buffer[5] == '{' && buffer[6] == '"' && buffer[7] == 'b' && buffer[8] == '"' && buffer[9] == ':' && buffer[10] == 'n').isTrue();
        qor_pp_assert_that(buffer[11] == 'u' && buffer[12] == 'l' && buffer[13] == 'l' && buffer[14] == '}' && buffer[15] == '}').isTrue();
    }

    //--- The JSON pipeline ---------------------------------------------------------
    //
    //Every state above writes into a generator Context that is backed by a plain
    //array. A GeneratorSource reserves that context from a pipeline buffer on
    //each pull, so wiring one to a FileConnector moves the generated text from
    //the source end to the sink end through the pipeline. That is the path the
    //library uses to emit JSON, and these tests check it end to end: a text
    //model goes through the pipeline into a file, and the file holds the text
    //the model was expected to produce. The source and the sink are given the
    //same pipeline buffer, which is what lets the two ends pass data along.

    //The generated files live with the other test fixtures. The path is relative
    //to the directory the test executable runs from, which is the convention the
    //JSON read test uses to find its fixtures.
    constexpr const char* outputFolder = "../../../../../../../test/data";

    //Read a generated file back as text so the produced text can be checked.
    static inline std::string ReadTextFile(const std::string& folder, const std::string& fileName)
    {
        std::ifstream stream(folder + "/" + fileName);
        return std::string(std::istreambuf_iterator<char>(stream), std::istreambuf_iterator<char>());
    }

    qor_pp_test_case(json_generator_object_model_written_to_file_by_pipeline)
    {
        qor::data::model::json::Number number;
        number.SetValue(1.0);

        qor::data::model::json::Object object;
        auto member = new_ref<qor::data::model::json::Member>();
        member->SetName("a");
        member->GetValue().SetValue(polymorphic<qor::data::model::json::ValueModel>(number));
        object.m_members.push_back(member);

        const qor::io::filesystem::Path outputPath(outputFolder);
        const qor::io::filesystem::Index outputIndex(outputPath, "json_generator_object.json");

        //The buffer is larger than the text, so one pull is enough.
        JSONObjectWriter writer(64);
        const size_t written = writer(outputIndex, &object);

        qor_pp_assert_that(written == 7).isTrue();
        qor_pp_assert_that(ReadTextFile(outputFolder, "json_generator_object.json") == "{\"a\":1}").isTrue();
    }

    qor_pp_test_case(json_generator_array_model_written_to_file_by_pipeline)
    {
        qor::data::model::json::Number one;
        one.SetValue(1.0);

        qor::data::model::json::Number two;
        two.SetValue(2.0);

        qor::data::model::json::Array array;
        auto firstElement = new_ref<qor::data::model::json::Value>();
        firstElement->SetValue(polymorphic<qor::data::model::json::ValueModel>(one));
        array.m_values.push_back(firstElement);

        auto secondElement = new_ref<qor::data::model::json::Value>();
        secondElement->SetValue(polymorphic<qor::data::model::json::ValueModel>(two));
        array.m_values.push_back(secondElement);

        const qor::io::filesystem::Path outputPath(outputFolder);
        const qor::io::filesystem::Index outputIndex(outputPath, "json_generator_array.json");

        JSONArrayWriter writer(64);
        const size_t written = writer(outputIndex, &array);

        qor_pp_assert_that(written == 5).isTrue();
        qor_pp_assert_that(ReadTextFile(outputFolder, "json_generator_array.json") == "[1,2]").isTrue();
    }

    qor_pp_test_case(json_generator_top_level_value_written_to_file_by_pipeline)
    {
        qor::data::model::json::Number number;
        number.SetValue(7.0);

        qor::data::model::json::Value value;
        value.SetValue(polymorphic<qor::data::model::json::ValueModel>(number));

        const qor::io::filesystem::Path outputPath(outputFolder);
        const qor::io::filesystem::Index outputIndex(outputPath, "json_generator_value.json");

        //A top level value is routed by the value state to the state that writes
        //the value it holds, exactly as it is at any nested position.
        JSONValueWriter writer(64);
        const size_t written = writer(outputIndex, &value);

        qor_pp_assert_that(written == 1).isTrue();
        qor_pp_assert_that(ReadTextFile(outputFolder, "json_generator_value.json") == "7").isTrue();
    }

    //The document is longer than the buffer, so the generator runs out of space
    //mid state. The state stack is left intact, so the next pull resumes the
    //suspended state at its saved position and the file still holds the whole
    //document.
    qor_pp_test_case(json_generator_nested_object_written_to_file_in_parts)
    {
        qor::data::model::json::Object inner;
        auto innerMember = new_ref<qor::data::model::json::Member>();
        innerMember->SetName("b");
        inner.m_members.push_back(innerMember);

        qor::data::model::json::Object outer;
        auto outerMember = new_ref<qor::data::model::json::Member>();
        outerMember->SetName("a");
        outerMember->GetValue().SetValue(polymorphic<qor::data::model::json::ValueModel>(inner));
        outer.m_members.push_back(outerMember);

        const qor::io::filesystem::Path outputPath(outputFolder);
        const qor::io::filesystem::Index outputIndex(outputPath, "json_generator_nested.json");

        //Four bytes of buffer for sixteen bytes of text.
        JSONObjectWriter writer(4);
        const size_t written = writer(outputIndex, &outer);

        qor_pp_assert_that(written == 16).isTrue();
        qor_pp_assert_that(ReadTextFile(outputFolder, "json_generator_nested.json") == "{\"a\":{\"b\":null}}").isTrue();
    }

    //The top level writer, used the way the JSONReader in jsonreader.h is used.
    qor_pp_test_case(json_generator_writer_writes_object_to_file)
    {
        qor::data::model::json::Number number;
        number.SetValue(7.0);

        qor::data::model::json::Object object;
        auto member = new_ref<qor::data::model::json::Member>();
        member->SetName("a");
        member->GetValue().SetValue(polymorphic<qor::data::model::json::ValueModel>(number));
        object.m_members.push_back(member);

        const qor::io::filesystem::Path outputPath(outputFolder);
        const qor::io::filesystem::Index outputIndex(outputPath, "json_generator_writer.json");

        JSONWriter writer;
        const size_t written = writer(outputIndex, &object);

        qor_pp_assert_that(written == 7).isTrue();
        qor_pp_assert_that(ReadTextFile(outputFolder, "json_generator_writer.json") == "{\"a\":7}").isTrue();
    }







}//qor


