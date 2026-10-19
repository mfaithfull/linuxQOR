// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_PROTOCOLS_YAML_PARSER_NODES_PAIR
#define QOR_PP_H_PROTOCOLS_YAML_PARSER_NODES_PAIR

#include "src/components/data/parser/node.h"
#include "../tokens.h"
#include "../../model/pair.h"

namespace qor { namespace protocols { namespace yaml {

    class PairNode final : public NodeAdapter< model::yaml::Pair >
    {
    public:
        PairNode() : NodeAdapter< model::yaml::Pair >(static_cast<uint64_t>(yamlToken::pair)){};
        virtual ~PairNode() noexcept = default;
    };

}}}//qor::protocols::yaml

#endif//QOR_PP_H_PROTOCOLS_YAML_PARSER_NODES_PAIR