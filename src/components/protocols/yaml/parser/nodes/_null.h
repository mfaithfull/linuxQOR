// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_PROTOCOLS_YAML_PARSER_NODES_NULL
#define QOR_PP_H_PROTOCOLS_YAML_PARSER_NODES_NULL

#include "src/components/data/parser/node.h"
#include "../tokens.h"
#include "../../model/_null.h"

namespace qor { namespace protocols { namespace yaml {

    class _NullNode final : public NodeAdapter< model::yaml::_Null >
    {
    public:
        _NullNode() : NodeAdapter< model::yaml::_Null >(static_cast<uint64_t>(yamlToken::_null)){};
        virtual ~_NullNode() noexcept = default;
        virtual std::string ToString() const override { return "null"; };
    };

}}}//qor::protocols::yaml

#endif//QOR_PP_H_PROTOCOLS_YAML_PARSER_NODES_NULL