// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_DATA_JSON_PARSER_NODES_INTEGER
#define QOR_PP_H_DATA_JSON_PARSER_NODES_INTEGER

#include "src/components/data/parser/node.h"
#include "../tokens.h"
#include "../../model/integer.h"

namespace qor { namespace data{ namespace parser{ namespace json {

    class IntegerNode : public NodeAdapter< model::json::Integer >
    {
    public:

        IntegerNode() : NodeAdapter< model::json::Integer >(static_cast<uint64_t>(jsonToken::integer)){ }
        virtual ~IntegerNode() = default;
        
    };

}}}}//qor::data::parser::json

#endif//QOR_PP_H_DATA_JSON_PARSER_NODES_INTEGER
