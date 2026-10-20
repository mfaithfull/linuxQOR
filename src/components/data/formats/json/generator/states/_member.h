// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_DATA_JSON_GENERATOR_MEMBER
#define QOR_PP_H_DATA_JSON_GENERATOR_MEMBER

#include <cstddef>
#include <string>
#include "src/components/data/formats/json/model/member.h"
#include "src/components/io/generator/generator.h"
#include "src/components/io/generator/state.h"

namespace qor { namespace data { namespace generator { namespace json {

    //member = string name-separator value
    //
    //Writes one object member: the member name as a JSON string, the colon that
    //separates the name from the value, and then the value itself. The value is
    //written by the _value state so that every value type is handled the same way
    //it is handled at the top level.
    class qor_pp_module_interface(QOR_JSON) _member final : public qor::components::generator::GeneratorState
    {
    public:

        _member(qor::components::generator::Generator* generator, qor::data::model::json::Member* member);
        virtual ~_member() noexcept;

    private:

        qor::data::model::json::Member* m_member;
        std::string m_name;
        size_t m_position;
    };

}}}}//qor::data::generator::json

#endif//QOR_PP_H_DATA_JSON_GENERATOR_MEMBER
