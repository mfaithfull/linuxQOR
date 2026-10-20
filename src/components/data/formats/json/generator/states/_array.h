// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_DATA_JSON_GENERATOR_ARRAY
#define QOR_PP_H_DATA_JSON_GENERATOR_ARRAY

#include <cstddef>
#include "src/components/data/formats/json/model/array.h"
#include "src/components/io/generator/generator.h"
#include "src/components/io/generator/state.h"

namespace qor { namespace data { namespace generator { namespace json {

    //array = left-square-bracket [ value *( value-separator value ) ] right-square-bracket
    //
    //Writes the brackets that surround an array. The default constructor writes
    //an empty array. The overload that takes an Array model reads the element
    //values out of the model and writes each one through the shared value state,
    //separated by commas.
    class qor_pp_module_interface(QOR_JSON) _array final : public qor::components::generator::GeneratorState
    {
    public:

        _array(qor::components::generator::Generator* generator);
        _array(qor::components::generator::Generator* generator, qor::data::model::json::Array* array);
        virtual ~_array() noexcept;

    private:

        qor::data::model::json::Array* m_array;
        size_t m_count;
        size_t m_position;
    };

}}}}//qor::data::generator::json

#endif//QOR_PP_H_DATA_JSON_GENERATOR_ARRAY
