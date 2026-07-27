// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_DATA_JSON_PARSER_NODES_MINUS
#define QOR_PP_H_DATA_JSON_PARSER_NODES_MINUS

#include "src/components/data/parser/node.h"
#include "../tokens.h"
#include "../../model/minus.h"

namespace qor { namespace data{ namespace parser{ namespace json {

    class MinusNode : public NodeAdapter< model::json::Minus >
    {
    public:

        MinusNode() : NodeAdapter< model::json::Minus >(static_cast<uint64_t>(jsonToken::minus)){ }
        virtual ~Minus() = default;
    };

}}}}//qor::data::parser::json

#endif//QOR_PP_H_DATA_JSON_PARSER_NODES_MINUS
