// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_DATA_JSON_GENERATOR_NULL
#define QOR_PP_H_DATA_JSON_GENERATOR_NULL

#include <cstddef>
#include "src/components/io/generator/generator.h"
#include "src/components/io/generator/state.h"

namespace qor { namespace data { namespace generator { namespace json {

    //null = %x6E.75.6C.6C ; null
    class qor_pp_module_interface(QOR_JSON) _null final : public qor::components::generator::GeneratorState
    {
    public:

        _null(qor::components::generator::Generator* generator);
        virtual ~_null() noexcept;

    private:

        byte m_text[4];
        size_t m_position;
    };

}}}}//qor::data::generator::json

#endif//QOR_PP_H_DATA_JSON_GENERATOR_NULL


