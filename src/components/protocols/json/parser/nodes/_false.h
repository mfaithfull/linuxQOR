// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_COMPONENTS_JSON_PARSER_NODES_FALSE
#define QOR_PP_H_COMPONENTS_JSON_PARSER_NODES_FALSE

#include "src/components/data/parser/node.h"
#include "../tokens.h"
#include "../../model/_false.h"

namespace qor { namespace components{ namespace parser{ namespace json {

    class _FalseNode : public NodeAdapter< model::json::_False >
    {
    public:

        _FalseNode() : NodeAdapter< model::json::_False >(static_cast<uint64_t>(jsonToken::_false)){};
        virtual ~_FalseNode() = default;
        virtual std::string ToString() const override { return "false"; };
    };

}}}}//qor::components::parser::json

#endif//QOR_PP_H_COMPONENTS_JSON_PARSER_NODES_FALSE