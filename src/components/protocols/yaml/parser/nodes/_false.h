// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_PROTOCOLS_YAML_PARSER_NODES_FALSE
#define QOR_PP_H_PROTOCOLS_YAML_PARSER_NODES_FALSE

#include "src/components/data/parser/node.h"
#include "../tokens.h"
#include "../../model/_false.h"

namespace qor { namespace protocols { namespace yaml {

    class _FalseNode final : public NodeAdapter< model::yaml::_False >
    {
    public:
        _FalseNode() : NodeAdapter< model::yaml::_False >(static_cast<uint64_t>(yamlToken::_false)){};
        virtual ~_FalseNode() noexcept = default;
        virtual std::string ToString() const override { return "false"; };
    };

}}}//qor::protocols::yaml

#endif//QOR_PP_H_PROTOCOLS_YAML_PARSER_NODES_FALSE