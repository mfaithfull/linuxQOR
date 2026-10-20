// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_DATA_JSON_GENERATOR_VALUE
#define QOR_PP_H_DATA_JSON_GENERATOR_VALUE

#include <cstddef>
#include "src/components/data/formats/json/model/value.h"
#include "src/components/io/generator/generator.h"
#include "src/components/io/generator/state.h"

namespace qor { namespace data { namespace generator { namespace json {

    //value = false / null / true / object / array / number / string
    //
    //The entry point for generating a JSON value. It looks at the type held in
    //the model value and pushes the state that writes that value.
    class qor_pp_module_interface(QOR_JSON) _value final : public qor::components::generator::GeneratorState
    {
    public:

        _value(qor::components::generator::Generator* generator, qor::data::model::json::Value* value);
        virtual ~_value() noexcept;

    private:

        qor::data::model::json::Value* m_value;
        size_t m_position;
    };

}}}}//qor::data::generator::json

#endif//QOR_PP_H_DATA_JSON_GENERATOR_VALUE
