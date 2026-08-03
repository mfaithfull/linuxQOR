// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

// Copyright 2026 Ilya Korolev
// Licensed under the Apache License, Version 2.0
// SPDX-License-Identifier: Apache-2.0

#include "src/configuration/configuration.h"
#include "src/qor/tdd/tdd.h"
#include "src/framework/data/contract/contract.h"
#include "src/framework/data/contract/interface.h"
#include "src/qor/reflection/core_name.h"

#include <cassert>
#include <cstdint>
#include <string_view>
#include <tuple>
#include <type_traits>

namespace attribute_tests {

    struct test_vocabulary {};
    struct other_vocabulary {};

    struct type_code 
    {
        int value;
    };

    struct field_code 
    {
        int value;
    };

    struct label 
    {
        const char* value;
    };

    struct other 
    {
        int value;
    };

} // namespace attribute_tests

namespace qor { namespace contract {

    template<>
    struct attr_traits<attribute_tests::type_code> {
        using vocabulary = attribute_tests::test_vocabulary;
        static constexpr attr_targets targets{true, false, false, false, false};
        static constexpr bool repeatable = false;
    };

    template<>
    struct attr_traits<attribute_tests::field_code> {
        using vocabulary = attribute_tests::test_vocabulary;
        static constexpr attr_targets targets{false, true, false, false, false};
        static constexpr bool repeatable = false;
    };

    template<>
    struct attr_traits<attribute_tests::label> {
        using vocabulary = attribute_tests::test_vocabulary;
        static constexpr attr_targets targets{true, true, false, false, false};
        static constexpr bool repeatable = true;
    };

    template<>
    struct attr_traits<attribute_tests::other> {
        using vocabulary = attribute_tests::other_vocabulary;
        static constexpr attr_targets targets{true, true, false, false, false};
        static constexpr bool repeatable = false;
    };

}}//qor::contract

namespace attribute_tests {

    struct Base 
    {
        std::uint32_t id = 0;

        struct contract_fields 
        { 
            using id = decltype(qor::contract::make_member_field<Base, 1, &Base::id>(
                qor_reflection::get_name<0, attribute_tests::Base>(), 
                qor::contract::make_field_attributes(
                    CONTRACT_DESCRIBE_ATTRIBUTE(field_code{20}),
                    CONTRACT_DESCRIBE_ATTRIBUTE(label{"base-id"})
                )
            ));
        };
    
        friend constexpr auto contract_definition(qor::contract::tag<Base>);
        friend struct qor::contract::interface_contract_of<Base>;
        /*CONTRACT(Base,
            ATTRS(type_code{10}),
            (id, 1, field_code{20}, label{"base-id"}))*/
    };
}//attribute_tests

    template<>
    struct qor::contract::interface_contract_of<attribute_tests::Base>
    {
        struct fields
        {
            using id = decltype(qor::contract::make_member_field<attribute_tests::Base, 1, &attribute_tests::Base::id>(
                qor_reflection::get_name<0, attribute_tests::Base>(), 
                qor::contract::make_field_attributes(
                    qor::contract::describe_attribute((attribute_tests::field_code{20}), CONTRACT_STRINGIZE(attribute_tests::field_code{20})),
                    qor::contract::describe_attribute((attribute_tests::label{"base-id"}), CONTRACT_STRINGIZE(attribute_tests::label{"base-id"}))
                )
            ));
        };

        constexpr auto contract_definition(qor::contract::tag<attribute_tests::Base>) 
        { 
            return qor::contract::make_contract_with_attributes<attribute_tests::Base>(
                qor_reflection::nameof<attribute_tests::Base>(),
                qor::contract::make_contract_attributes(
                    qor::contract::describe_attribute((attribute_tests::type_code{10}), qor_pp_stringize(attribute_tests::type_code{10}))
                ), 
                qor::contract::make_member_field<attribute_tests::Base, 1, &attribute_tests::Base::id>(
                    qor_reflection::get_name<0, attribute_tests::Base>(),
                    qor::contract::make_field_attributes(
                        qor::contract::describe_attribute((attribute_tests::field_code{20}), qor_pp_stringize(attribute_tests::field_code{20})),                        
                        qor::contract::describe_attribute((attribute_tests::label{"base-id"}), qor_pp_stringize(attribute_tests::label{"base-id"}))
                    )
                )
            ); 
        }        
    };

namespace attribute_tests {

    constexpr auto contract_definition(qor::contract::tag<Base>) 
    { 
        return qor::contract::make_contract_with_attributes<Base>(
            qor_reflection::nameof<attribute_tests::Base>(), 
            qor::contract::make_contract_attributes( 
                CONTRACT_DESCRIBE_ATTRIBUTE(type_code{10})
            ), 
            qor::contract::make_member_field<Base, 1, &Base::id>(
                qor_reflection::get_name<0, attribute_tests::Base>(), 
                qor::contract::make_field_attributes(
                    CONTRACT_DESCRIBE_ATTRIBUTE(field_code{20}),
                    CONTRACT_DESCRIBE_ATTRIBUTE(label{"base-id"})
                )
            )
        ); 
    }        

    struct Middle : Base 
    {
        struct contract_fields {  }; 
        
        friend constexpr auto contract_definition(qor::contract::tag<Middle>) 
        { 
            return qor::contract::make_contract<Middle>(qor_reflection::nameof<attribute_tests::Middle>(), qor::contract::base<Base, 100>{}); 
        }
        friend struct qor::contract::interface_contract_of<Base>;
        //CONTRACT(Middle, BASE(Base, 100))
    };

}//attribute_tests

    template<>
    struct qor::contract::interface_contract_of<attribute_tests::Middle>
    {
        struct fields{};

        friend constexpr auto contract_definition(qor::contract::tag<attribute_tests::Middle>) 
        { 
            return qor::contract::make_contract<attribute_tests::Middle>(qor_reflection::nameof<attribute_tests::Middle>(), qor::contract::base<attribute_tests::Base, 100>{}); 
        }
    };

namespace attribute_tests {

    struct Event : Middle {
        std::uint32_t raw_value = 0;

        struct contract_fields 
        {
            using value = decltype(qor::contract::make_property_field<Event, 2, std::uint32_t>(
                "value", 
                qor::contract::make_field_attributes(
                    CONTRACT_DESCRIBE_ATTRIBUTE(field_code{40}),
                    CONTRACT_DESCRIBE_ATTRIBUTE(other{50}),
                    CONTRACT_DESCRIBE_ATTRIBUTE(label{"property"}),
                    CONTRACT_DESCRIBE_ATTRIBUTE(label{"display"})
                )
            ));
        }; 
        
        friend constexpr auto contract_definition(qor::contract::tag<Event>) 
        { 
            return qor::contract::make_contract_with_attributes<Event>(
                qor_reflection::nameof<Event>(),
                qor::contract::make_contract_attributes( 
                    CONTRACT_DESCRIBE_ATTRIBUTE(type_code{30}),
                    CONTRACT_DESCRIBE_ATTRIBUTE(label{"event"})
                ), 
                qor::contract::base<Middle, 1000>{},
                qor::contract::make_property_field<Event, 2, std::uint32_t>(
                    "value", 
                    qor::contract::make_field_attributes(
                        CONTRACT_DESCRIBE_ATTRIBUTE(field_code{40}),
                        CONTRACT_DESCRIBE_ATTRIBUTE(other{50}),
                        CONTRACT_DESCRIBE_ATTRIBUTE(label{"property"}),
                        CONTRACT_DESCRIBE_ATTRIBUTE(label{"display"})
                    )
                )                
            ); 

        }

        /*~, 1(value, 2, std::uint32_t,
                field_code{40}, other{50}, label{"property"}, label{"display"})(value, 2, std::uint32_t,
                field_code{40}, other{50}, label{"property"}, label{"display"})*/

        /*CONTRACT(Event,
            ATTRS(type_code{30}, label{"event"}),
            BASE(Middle, 1000),
            PROPERTY(value, 2, std::uint32_t,
                field_code{40}, other{50}, label{"property"}, label{"display"}))*/
        
        std::uint32_t contract_get(const contract_fields::value&) const 
        {
            return raw_value;
        }

        void contract_set(const contract_fields::value&, std::uint32_t value) 
        {
            raw_value = value;
        }
    };

}//attribute_tests

qor_pp_test_case(test_contract_attributes) {
    using namespace attribute_tests;

    constexpr auto base_attributes = qor::contract::contract_attributes_of<Base>();
    static_assert(std::tuple_size_v<typename decltype(base_attributes)::tuple_type> == 1);
    static_assert(std::get<0>(base_attributes.entries).attribute.value == 10);
    static_assert(std::get<0>(base_attributes.entries).source == std::string_view("type_code{10}"));

    constexpr auto event_attributes = qor::contract::contract_attributes_of<Event>();
    static_assert(std::tuple_size_v<typename decltype(event_attributes)::tuple_type> == 2);
    static_assert(std::get<0>(event_attributes.entries).attribute.value == 30);
    static_assert(std::get<1>(event_attributes.entries).attribute.value[0] == 'e');

    constexpr auto base_field = qor::contract::field_at<0, Event>();
    using base_field_type = decltype(base_field);
    static_assert(base_field.id == 1101);
    static_assert(base_field.is_base_import);
    static_assert(qor::contract::has_attribute_v<base_field_type, field_code>);
    static_assert(qor::contract::has_attribute_v<base_field_type, label>);
    static_assert(!qor::contract::has_attribute_v<base_field_type, type_code>);
    static_assert(qor::contract::attribute_count_v<base_field_type, label> == 1);
    static_assert(std::get<0>(base_field.attributes.entries).attribute.value == 20);
    static_assert(std::get<1>(base_field.attributes.entries).attribute.value[0] == 'b');

    constexpr auto property = qor::contract::field_at<1, Event>();
    using property_type = decltype(property);
    static_assert(property.id == 2);
    static_assert(property.kind == qor::contract::field_kind::property);
    static_assert(std::is_same_v<qor::contract::attributes_of_t<property_type>, qor::contract::attributes<field_code, other, label, label>>);
    static_assert(qor::contract::attribute_count_v<property_type, label> == 2);
    static_assert(std::is_same_v<qor::contract::attributes_in_vocabulary_t<property_type, test_vocabulary>,qor::contract::attributes<field_code, label, label>>);
    static_assert(std::get<0>(property.attributes.entries).attribute.value == 40);
    static_assert(std::get<1>(property.attributes.entries).attribute.value == 50);
    static_assert(std::get<2>(property.attributes.entries).attribute.value[0] == 'p');
    static_assert(std::get<0>(property.attributes.entries).source == std::string_view("field_code{40}"));
    static_assert(property.attributes.find<field_code>()->value == 40);
    static_assert(property.attributes.get<field_code>().value == 40);
    static_assert(property.attributes.find<type_code>() == nullptr);

    constexpr auto filtered = qor::contract::get_attributes_in_vocabulary<test_vocabulary>(property);
    static_assert(std::get<0>(filtered.entries).attribute.value == 40);
    static_assert(std::get<1>(filtered.entries).attribute.value[0] == 'p');
    static_assert(std::get<2>(filtered.entries).attribute.value[0] == 'd');

    constexpr auto composed = qor::contract::compose_attributes(
        qor::contract::make_attributes(
            qor::contract::describe_attribute(field_code{60}, "field_code{60}"),
            qor::contract::describe_attribute(label{"left"}, "label{\"left\"}")),
        qor::contract::make_attributes(
            qor::contract::describe_attribute(other{70}, "other{70}"),
            qor::contract::describe_attribute(label{"right"}, "label{\"right\"}")));
    static_assert(std::get<0>(composed.entries).attribute.value == 60);
    static_assert(std::get<1>(composed.entries).attribute.value[0] == 'l');
    static_assert(std::get<2>(composed.entries).attribute.value == 70);
    static_assert(std::get<3>(composed.entries).attribute.value[0] == 'r');

    const auto& declared = qor::contract::get_attributes(property);
    static_assert(std::is_same_v<std::remove_cv_t<std::remove_reference_t<decltype(declared)>>, qor::contract::attributes_of_t<property_type>>);
    assert(&declared == &property.attributes);

    Event event;
    property.set(event, 55);
    assert(property.get(event) == 55);

}
