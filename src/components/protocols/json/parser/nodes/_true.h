// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_COMPONENTS_JSON_PARSER_NODES_TRUE
#define QOR_PP_H_COMPONENTS_JSON_PARSER_NODES_TRUE

#include "src/components/data/parser/node.h"
#include "../tokens.h"
#include "../../model/_true.h"

namespace qor { namespace components{ namespace parser{ namespace json {

    class _TrueNode : public NodeAdapter< model::json::_True >
    {
    public:

        _TrueNode() : NodeAdapter< model::json::_True >(static_cast<uint64_t>(jsonToken::_true)){};
        virtual ~_TrueNode() = default;        
        virtual std::string ToString() const override { return "true"; };
    };

}}}}//qor::components::parser::json

#endif//QOR_PP_H_COMPONENTS_JSON_PARSER_NODES_TRUE