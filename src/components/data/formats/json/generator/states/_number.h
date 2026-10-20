// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_DATA_JSON_GENERATOR_NUMBER
#define QOR_PP_H_DATA_JSON_GENERATOR_NUMBER

#include <cstddef>
#include "src/components/io/generator/generator.h"
#include "src/components/io/generator/state.h"
#include "src/components/data/formats/json/model/number.h"

namespace qor { namespace data { namespace generator { namespace json {

    //number = [ minus ] int [ frac ] [ exp ]
    //
    //Writes a JSON number in its shortest form: a leading minus for negative
    //values, no leading zeroes and no leading plus. The fractional and exponent
    //parts may be written when the standard conversion selects them.
    class qor_pp_module_interface(QOR_JSON) _number final : public qor::components::generator::GeneratorState
    {
    public:

        _number(qor::components::generator::Generator* generator, long long value);
        _number(qor::components::generator::Generator* generator, qor::data::model::json::Number* number);
        virtual ~_number() noexcept;

    private:

        // A signed 64 bit value needs at most 19 digits plus a possible minus
        // sign. A double rendered as decimal digits, a fractional point and a
        // possible minus fits comfortably in this wider buffer.
        byte m_text[64];
        size_t m_length;
        size_t m_position;
        bool m_valid;
    };

}}}}//qor::data::generator::json

#endif//QOR_PP_H_DATA_JSON_GENERATOR_NUMBER
