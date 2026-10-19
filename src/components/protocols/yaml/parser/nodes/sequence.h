// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_PROTOCOLS_YAML_PARSER_NODES_SEQUENCE
#define QOR_PP_H_PROTOCOLS_YAML_PARSER_NODES_SEQUENCE

#include "src/components/data/parser/node.h"
#include "../tokens.h"
#include "../../model/sequence.h"

namespace qor { namespace protocols { namespace yaml {

    class SequenceNode final : public NodeAdapter< model::yaml::Sequence >
    {
    public:
        SequenceNode() : NodeAdapter< model::yaml::Sequence >(static_cast<uint64_t>(yamlToken::sequence)){};
        virtual ~SequenceNode() noexcept = default;
    };

}}}//qor::protocols::yaml

#endif//QOR_PP_H_PROTOCOLS_YAML_PARSER_NODES_SEQUENCE