// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_COMPONENTS_JSON_ARRAY_PARSER_NODE
#define QOR_PP_H_COMPONENTS_JSON_ARRAY_PARSER_NODE

#include "src/components/data/parser/node.h"
#include "../tokens.h"
#include "../../model/array.h"

namespace qor { namespace components{ namespace parser{ namespace json {

    class ArrayNode : public NodeAdapter< model::json::Array >
    {
    public:

        ArrayNode(): NodeAdapter< model::json::Array >(static_cast<uint64_t>(jsonToken::array)){};
        virtual ~ArrayNode() = default;

        //virtual std::string ToString() const override;

    };

}}}}//qor::components::parser::json

#endif//QOR_PP_H_COMPONENTS_JSON_ARRAY_PARSER_NODE
