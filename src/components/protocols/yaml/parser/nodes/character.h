// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_PROTOCOLS_YAML_PARSER_NODES_CHARACTER
#define QOR_PP_H_PROTOCOLS_YAML_PARSER_NODES_CHARACTER

#include "src/components/data/parser/node.h"
#include "../tokens.h"
#include "../../model/character.h"

namespace qor { namespace protocols { namespace yaml {

    class CharacterNode final : public NodeAdapter< model::yaml::Character >
    {
    public:

        CharacterNode(unsigned int c = 'c') : NodeAdapter< model::yaml::Character >(static_cast<uint64_t>(yamlToken::character))
        {
            GetObject()->SetValue(c);
        }

        virtual ~CharacterNode() noexcept = default;
    };

}}}//qor::protocols::yaml

#endif//QOR_PP_H_PROTOCOLS_YAML_PARSER_NODES_CHARACTER