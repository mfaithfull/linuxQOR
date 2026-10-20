// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_DATA_JSON_GENERATOR_TRUE
#define QOR_PP_H_DATA_JSON_GENERATOR_TRUE

#include <cstddef>
#include "src/components/io/generator/generator.h"
#include "src/components/io/generator/state.h"

namespace qor { namespace data { namespace generator { namespace json {

    //true = %x74.72.75.65 ; true
    class qor_pp_module_interface(QOR_JSON) _true final : public qor::components::generator::GeneratorState
    {
    public:

        _true(qor::components::generator::Generator* generator);
        virtual ~_true() noexcept;

    private:

        byte m_text[4];
        size_t m_position;
    };

}}}}//qor::data::generator::json

#endif//QOR_PP_H_DATA_JSON_GENERATOR_TRUE
