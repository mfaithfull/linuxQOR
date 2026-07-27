// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_DATA_JSON_PARSER_NODES_STRING
#define QOR_PP_H_DATA_JSON_PARSER_NODES_STRING

#include "src/components/data/parser/node.h"
#include "../tokens.h"
#include "../../model/string.h"

namespace qor { namespace data{ namespace parser{ namespace json {

    class StringNode : public NodeAdapter< model::json::String >
    {
    public:

        StringNode() : NodeAdapter< model::json::String >(static_cast<uint64_t>(jsonToken::string)){ }
        virtual ~StringNode() = default;        
    };

}}}}//qor::data::parser::json

#endif//QOR_PP_H_DATA_JSON_PARSER_NODES_STRING
