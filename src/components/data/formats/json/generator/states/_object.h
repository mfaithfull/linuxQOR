// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_DATA_JSON_GENERATOR_OBJECT
#define QOR_PP_H_DATA_JSON_GENERATOR_OBJECT

#include <cstddef>
#include "src/components/data/formats/json/model/object.h"
#include "src/components/io/generator/generator.h"
#include "src/components/io/generator/state.h"

namespace qor { namespace data { namespace generator { namespace json {

    //object = left-curly-bracket [ member *( value-separator member ) ] right-curly-bracket
    //
    //Writes the braces that surround an object. The default constructor writes
    //an empty object. The overload that takes an Object model reads the members
    //out of the model and writes each one through the member state, separated by
    //commas.
    class qor_pp_module_interface(QOR_JSON) _object final : public qor::components::generator::GeneratorState
    {
    public:

        _object(qor::components::generator::Generator* generator);
        _object(qor::components::generator::Generator* generator, qor::data::model::json::Object* object);
        virtual ~_object() noexcept;

    private:

        qor::data::model::json::Object* m_object;
        size_t m_count;
        size_t m_position;
    };

}}}}//qor::data::generator::json

#endif//QOR_PP_H_DATA_JSON_GENERATOR_OBJECT
