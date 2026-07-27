// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_DATA_JSON_PARSER_NODES_NUMBER
#define QOR_PP_H_DATA_JSON_PARSER_NODES_NUMBER

#include "src/components/data/parser/node.h"
#include "../tokens.h"
#include "../../model/number.h"

namespace qor { namespace data{ namespace parser{ namespace json {

    class NumberNode : public NodeAdapter< model::json::Number >
    {
    public:

        NumberNode() : NodeAdapter< model::json::Number >(static_cast<uint64_t>(jsonToken::number)){ }
        virtual ~NumberNode() = default;        
    };

}}}}//qor::data::parser::json

#endif//QOR_PP_H_DATA_JSON_PARSER_NODES_NUMBER