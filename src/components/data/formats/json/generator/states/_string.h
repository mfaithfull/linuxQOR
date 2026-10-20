// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_DATA_JSON_GENERATOR_STRING
#define QOR_PP_H_DATA_JSON_GENERATOR_STRING

#include <cstddef>
#include <vector>
#include "src/components/io/generator/generator.h"
#include "src/components/io/generator/state.h"

namespace qor { namespace data { namespace generator { namespace json {

    //string = quotation-mark *char quotation-mark
    //
    //Writes the opening quotation mark, the payload and the closing quotation
    //mark. The payload is the unescaped text: characters that JSON requires to
    //be escaped get the appropriate escape sequence, using the short form where
    //one exists and \u00xx for the other control characters.
    class qor_pp_module_interface(QOR_JSON) _string final : public qor::components::generator::GeneratorState
    {
    public:

        _string(qor::components::generator::Generator* generator, const char* text, size_t length);
        virtual ~_string() noexcept;

    private:

        // The escaped form of the payload, including the quotation marks, which
        // can be longer than the payload itself.
        std::vector<byte> m_text;
        size_t m_length;
        size_t m_position;
    };

}}}}//qor::data::generator::json

#endif//QOR_PP_H_DATA_JSON_GENERATOR_STRING
