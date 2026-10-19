// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_PROTOCOLS_YAML_PARSER_QUOTATION_MARK
#define QOR_PP_H_PROTOCOLS_YAML_PARSER_QUOTATION_MARK

#include "../../tokens.h"

namespace qor { namespace protocols { namespace yaml {

    class quotation_mark final : public Specific<uint32_t>
    {
    public:
        quotation_mark(Parser* parser) : Specific<uint32_t>(parser, 0x22, static_cast<uint64_t>(yamlToken::quotation_mark)){ }
        virtual ~quotation_mark() noexcept = default;
        void Emit(){ }
    };

}}}//qor::protocols::yaml

#endif//QOR_PP_H_PROTOCOLS_YAML_PARSER_QUOTATION_MARK