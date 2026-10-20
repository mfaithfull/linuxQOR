// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_DATA_JSON_GENERATOR_FALSE
#define QOR_PP_H_DATA_JSON_GENERATOR_FALSE

#include <cstddef>
#include "src/components/io/generator/generator.h"
#include "src/components/io/generator/state.h"

namespace qor { namespace data { namespace generator { namespace json {

    //false = %x66.61.6c.73.65 ; false
    class qor_pp_module_interface(QOR_JSON) _false final : public qor::components::generator::GeneratorState
    {
    public:

        _false(qor::components::generator::Generator* generator);
        virtual ~_false() noexcept;

    private:

        byte m_text[5];
        size_t m_position;
    };

}}}}//qor::data::generator::json

#endif//QOR_PP_H_DATA_JSON_GENERATOR_FALSE
