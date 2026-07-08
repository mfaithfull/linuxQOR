// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_COMPONENTS_JSON_PARSER_NODES_PLUS
#define QOR_PP_H_COMPONENTS_JSON_PARSER_NODES_PLUS

#include "src/components/data/parser/node.h"
#include "../tokens.h"
#include "../../model/plus.h"

namespace qor { namespace components{ namespace json{ namespace parser {

    class PlusNode : public NodeAdapter< model::json::Plus >
    {
    public:
        PlusNode() : NodeAdapter< model::json::Plus >(static_cast<uint64_t>(jsonToken::plus)){}
        virtual ~PlusNode() = default;
    };

}}}}//qor::components::parser::json

#endif//QOR_PP_H_COMPONENTS_JSON_PARSER_NODES_PLUS
