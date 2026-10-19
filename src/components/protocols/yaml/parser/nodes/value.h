// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_PROTOCOLS_YAML_PARSER_NODES_VALUE
#define QOR_PP_H_PROTOCOLS_YAML_PARSER_NODES_VALUE

#include "src/qor/essentials/current/currentthread.h"
#include "src/qor/memory/reference/newref.h"
#include "src/components/data/parser/node.h"
#include "../tokens.h"
#include "../../model/value.h"

namespace qor { namespace protocols { namespace yaml {

    class ValueNode final : public NodeAdapter< model::yaml::Value >
    {
    public:

        ValueNode() : NodeAdapter< model::yaml::Value >(static_cast<uint64_t>(yamlToken::value)){ }
        virtual ~ValueNode() noexcept = default;
    };

}}}//qor::protocols::yaml

#endif//QOR_PP_H_PROTOCOLS_YAML_PARSER_NODES_VALUE