// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_COMPONENTS_JSON_PARSER_NODES_DECIMAL_POINT
#define QOR_PP_H_COMPONENTS_JSON_PARSER_NODES_DECIMAL_POINT

#include "src/components/data/parser/node.h"
#include "../tokens.h"
#include "../../model/decimal_point.h"

namespace qor { namespace components{ namespace parser{ namespace json {

    class DecimalPointNode : public NodeAdapter< model::json::DecimalPoint >
    {
    public:

        DecimalPointNode() : NodeAdapter< model::json::DecimalPoint >(static_cast<uint64_t>(jsonToken::decimal_point)){ };
        virtual ~DecimalPointNode() = default;
    };

}}}}//qor::components::parser::json

#endif//QOR_PP_H_COMPONENTS_JSON_PARSER_NODES_DECIMAL_POINT