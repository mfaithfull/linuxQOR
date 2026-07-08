// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_COMPONENTS_JSON_PARSER_NODES_VALUE
#define QOR_PP_H_COMPONENTS_JSON_PARSER_NODES_VALUE

#include "src/qor/essentials/current/currentthread.h"
#include "src/qor/memory/reference/newref.h"
#include "src/components/data/parser/node.h"
#include "../tokens.h"
#include "../../model/value.h"

namespace qor { namespace components{ namespace parser{ namespace json {

    class ValueNode : public NodeAdapter< model::json::Value >
    {
    public:        

        ValueNode() : NodeAdapter< model::json::Value >(static_cast<uint64_t>(jsonToken::value)){ }
        virtual ~ValueNode() = default;
    };

}}}}//qor::components::parser::json

#endif//QOR_PP_H_COMPONENTS_JSON_PARSER_NODES_VALUE
