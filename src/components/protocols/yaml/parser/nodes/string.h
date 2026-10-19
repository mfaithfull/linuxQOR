// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_PROTOCOLS_YAML_PARSER_NODES_STRING
#define QOR_PP_H_PROTOCOLS_YAML_PARSER_NODES_STRING

#include "src/components/data/parser/node.h"
#include "../tokens.h"
#include "../../model/string.h"

namespace qor { namespace protocols { namespace yaml {

    class StringNode final : public NodeAdapter< model::yaml::String >
    {
    public:

        StringNode() : NodeAdapter< model::yaml::String >(static_cast<uint64_t>(yamlToken::string)){ }
        virtual ~StringNode() noexcept = default;
    };

}}}//qor::protocols::yaml

#endif//QOR_PP_H_PROTOCOLS_YAML_PARSER_NODES_STRING