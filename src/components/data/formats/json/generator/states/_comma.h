// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_DATA_JSON_GENERATOR_COMMA
#define QOR_PP_H_DATA_JSON_GENERATOR_COMMA

#include <cstddef>
#include "src/components/io/generator/generator.h"
#include "src/components/io/generator/state.h"

namespace qor { namespace data { namespace generator { namespace json {

    //value-separator = ( %x2C ) ; comma
    //
    //Writes the single comma byte used to separate the members of a non-empty
    //object and the elements of a non-empty array.
    class qor_pp_module_interface(QOR_JSON) _comma final : public qor::components::generator::GeneratorState
    {
    public:

        _comma(qor::components::generator::Generator* generator);
        virtual ~_comma() noexcept;

    private:

        byte m_text[1];
        size_t m_position;
    };

}}}}//qor::data::generator::json

#endif//QOR_PP_H_DATA_JSON_GENERATOR_COMMA
