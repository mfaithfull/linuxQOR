// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_PROTOCOLS_YAML_PARSER_VALUE_SEPARATOR
#define QOR_PP_H_PROTOCOLS_YAML_PARSER_VALUE_SEPARATOR

#include "../../tokens.h"

namespace qor { namespace protocols { namespace yaml {

    //value-separator = ws %x2C ws  ; , comma
    class value_separator final : public Sequence
    {
    public:
        value_separator(Parser* parser) :
            Sequence(parser,
                new_ref<ws>(parser),
                new_ref<Sequence>(parser,
                    new_ref<Specific<uint32_t>>(parser, 0x2c),
                    new_ref<ws>(parser)
                ),
            static_cast<uint64_t>(yamlToken::value_separator)){ }
        virtual ~value_separator() noexcept = default;
        virtual void Emit(){ };//Emit nothing
    };

}}}//qor::protocols::yaml

#endif//QOR_PP_H_PROTOCOLS_YAML_PARSER_VALUE_SEPARATOR