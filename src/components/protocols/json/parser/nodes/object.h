// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_COMPONENTS_JSON_PARSER_NODES_OBJECT
#define QOR_PP_H_COMPONENTS_JSON_PARSER_NODES_OBJECT

#include "src/components/data/parser/node.h"
#include "../tokens.h"
#include "../../model/object.h"

namespace qor { namespace components{ namespace parser{ namespace json {

    class ObjectNode : public NodeAdapter< model::json::Object >
    {
    public:

        ObjectNode(): NodeAdapter< model::json::Object >(static_cast<uint64_t>(jsonToken::object)){ }
        virtual ~ObjectNode() = default;        
    };

}}}}//qor::components::json::parser

#endif//QOR_PP_H_COMPONENTS_JSON_PARSER_NODES_OBJECT
