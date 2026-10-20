// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_DATA_JSON_GENERATOR_COLON
#define QOR_PP_H_DATA_JSON_GENERATOR_COLON

#include <cstddef>
#include "src/components/io/generator/generator.h"
#include "src/components/io/generator/state.h"

namespace qor { namespace data { namespace generator { namespace json {

    //name-separator = ( %x3A ) ; colon
    //
    //Writes the single colon byte that separates a member name from its value.
    class qor_pp_module_interface(QOR_JSON) _colon final : public qor::components::generator::GeneratorState
    {
    public:

        _colon(qor::components::generator::Generator* generator);
        virtual ~_colon() noexcept;

    private:

        byte m_text[1];
        size_t m_position;
    };

}}}}//qor::data::generator::json

#endif//QOR_PP_H_DATA_JSON_GENERATOR_COLON
