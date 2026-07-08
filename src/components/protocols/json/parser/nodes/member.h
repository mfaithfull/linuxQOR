// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_COMPONENTS_JSON_PARSER_NODES_MEMBER
#define QOR_PP_H_COMPONENTS_JSON_PARSER_NODES_MEMBER

#include "src/components/data/parser/node.h"
#include "../tokens.h"
#include "../../model/member.h"

namespace qor { namespace components{ namespace parser{ namespace json {

    class MemberNode : public NodeAdapter< model::json::Member >
    {
    public:        

        MemberNode() : NodeAdapter< model::json::Member >(static_cast<uint64_t>(jsonToken::member)){ }
        virtual ~MemberNode() = default;        
    };

}}}}//qor::components::parser::json

#endif//QOR_PP_H_COMPONENTS_JSON_MEMBER
