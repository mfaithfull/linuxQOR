// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_COMPONENTS_JSON_PARSER_NODES_ESCAPE
#define QOR_PP_H_COMPONENTS_JSON_PARSER_NODES_ESCAPE

#include "src/components/data/parser/node.h"
#include "../tokens.h"
#include "../../model/escape.h"

namespace qor { namespace components{ namespace parser{ namespace json {

    class EscapeNode : public NodeAdapter< model::json::Escape >
    {
    public:

        EscapeNode(): NodeAdapter< model::json::Escape >(static_cast<uint64_t>(jsonToken::escape)){ }
        virtual ~EscapeNode() = default;
        
        std::string Value(){ return "\\"; }

    private:
        
    };

}}}}//qor::components::parser::json

#endif//QOR_PP_H_COMPONENTS_JSON_PARSER_NODES_ESCAPE