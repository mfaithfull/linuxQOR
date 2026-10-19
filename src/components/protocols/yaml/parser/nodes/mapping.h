// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_PROTOCOLS_YAML_PARSER_NODES_MAPPING
#define QOR_PP_H_PROTOCOLS_YAML_PARSER_NODES_MAPPING

#include "src/components/data/parser/node.h"
#include "../tokens.h"
#include "../../model/mapping.h"

namespace qor { namespace protocols { namespace yaml {

    class MappingNode final : public NodeAdapter< model::yaml::Mapping >
    {
    public:
        MappingNode() : NodeAdapter< model::yaml::Mapping >(static_cast<uint64_t>(yamlToken::mapping)){};
        virtual ~MappingNode() noexcept = default;
    };

}}}//qor::protocols::yaml

#endif//QOR_PP_H_PROTOCOLS_YAML_PARSER_NODES_MAPPING