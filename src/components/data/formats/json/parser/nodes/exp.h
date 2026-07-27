// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_DATA_JSON_PARSER_NODES_EXP
#define QOR_PP_H_DATA_JSON_PARSER_NODES_EXP

#include "src/components/data/parser/node.h"
#include "../tokens.h"
#include "../../model/exp.h"

namespace qor { namespace data{ namespace parser{ namespace json {

    class ExpNode : public NodeAdapter< model::json::Exp >
    {
    public:

        ExpNode(): NodeAdapter< model::json::Exp >(static_cast<uint64_t>(jsonToken::exp)){ }
        virtual ~ExpNode() = default;

    private:
    };

}}}}//qor::data::parser::json

#endif//QOR_PP_H_DATA_JSON_PARSER_NODES_EXP