// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_COMPONENTS_JSON_PARSER_NODES_NULL
#define QOR_PP_H_COMPONENTS_JSON_PARSER_NODES_NULL

#include "src/components/data/parser/node.h"
#include "../tokens.h"
#include "../../model/_null.h"

namespace qor { namespace components{ namespace parser{ namespace json {

    class _NullNode : public NodeAdapter< model::json::_Null >
    {
    public:
        _NullNode() : NodeAdapter< model::json::_Null >(static_cast<uint64_t>(jsonToken::_null)){ };
        virtual ~_NullNode() = default;
        virtual std::string ToString() const override { return "null"; };
    };

}}}}//qor::components::parser::json

#endif//QOR_PP_H_COMPONENTS_JSON_PARSER_NODES_NULL
