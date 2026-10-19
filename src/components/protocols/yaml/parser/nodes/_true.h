// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_PROTOCOLS_YAML_PARSER_NODES_TRUE
#define QOR_PP_H_PROTOCOLS_YAML_PARSER_NODES_TRUE

#include "src/components/data/parser/node.h"
#include "../tokens.h"
#include "../../model/_true.h"

namespace qor { namespace protocols { namespace yaml {

    class _TrueNode final : public NodeAdapter< model::yaml::_True >
    {
    public:
        _TrueNode() : NodeAdapter< model::yaml::_True >(static_cast<uint64_t>(yamlToken::_true)){};
        virtual ~_TrueNode() noexcept = default;
        virtual std::string ToString() const override { return "true"; };
    };

}}}//qor::protocols::yaml

#endif//QOR_PP_H_PROTOCOLS_YAML_PARSER_NODES_TRUE