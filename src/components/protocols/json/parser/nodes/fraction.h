// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_COMPONENTS_JSON_PARSER_NODES_FRACTION
#define QOR_PP_H_COMPONENTS_JSON_PARSER_NODES_FRACTION

#include "src/components/data/parser/node.h"
#include "../tokens.h"
#include "../../model/fraction.h"

namespace qor { namespace components{ namespace parser{ namespace json {

    class FractionNode : public NodeAdapter< model::json::Fraction >
    {
    public:

        FractionNode() : NodeAdapter< model::json::Fraction >(static_cast<uint64_t>(jsonToken::fraction)){ }
        virtual ~FractionNode() = default;
    };

}}}}//qor::components::parser::json

#endif//QOR_PP_H_COMPONENTS_JSON_FRACTION
