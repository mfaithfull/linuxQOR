// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"

#include <cassert>
#include "src/qor/essentials/current/currentthread.h"
#include "src/qor/memory/reference/newref.h"
#include "src/qor/flyers/error/error.h"
#include "src/qor/flyers/log/debug.h"
#include "member.h"
#include "../../nodes/member.h"
#include "../../nodes/value.h"
#include "../../nodes/string.h"

namespace qor { namespace data { namespace parser { namespace json {

    //member = string name-separator value
    member::member(Parser* parser) : 
        Sequence_t< string, Sequence_t<name_separator, value> >(parser, &m_str, &m_tail, static_cast<uint64_t>(jsonToken::member)),
            m_v(parser),
            m_ns(parser),
            m_str(parser),
            m_tail(parser, &m_ns, &m_v)
    { }

    member::~member() = default;

    void member::Prepare()
    {
        //log::debug("Looking for a Member...");        
        GetParser()->PushNode(new_ref<MemberNode>());
    }

    void member::Emit()
    {
        //log::debug("Emitting a Member.");
        std::string name;
        ref_of<ValueNode>::type value;
        auto node = GetParser()->PopNode();
        while(node.IsNotNull() && node->GetToken() != m_token)
        {
            uint64_t token = node->GetToken();
            auto f = jsonTokenNames.find((jsonToken)token);
            std::string tokenName;
            if(f != jsonTokenNames.end())
            {
                tokenName = f->second;
            }

            if(token == static_cast<uint64_t>(jsonToken::value))
            {
                auto valueNode = node.AsRef<ValueNode>();
                value = valueNode;

                node = GetParser()->PopNode();

                uint64_t token2 = node->GetToken();
                auto f2 = jsonTokenNames.find((jsonToken)token2);
                std::string tokenName2;
                if(f2 != jsonTokenNames.end())
                {
                    tokenName2 = f2->second;
                }

                if(token2 == static_cast<uint64_t>(jsonToken::string))
                {
                    auto stringNode = node.AsRef<StringNode>();
                    name = stringNode->GetObject()->GetValue();
                }
                else
                {
                    //continuable("Unexpected: {0}", tokenName2);
                }
            }
            else
            {
                continuable("Unexpected: {0}", tokenName);
            }
            node = GetParser()->PopNode();
        }

        if(node.IsNotNull())
        {
            auto memberNode = node.As<MemberNode>();
            memberNode->GetObject()->SetName(name);
            memberNode->GetObject()->GetValue() = value->GetObject();
            GetParser()->PushNode(node);
        }
    }

    void member::Fail()
    {
        //log::debug("...Didn't find a Member.");
        uint64_t topToken = m_parser->TopNode()->GetToken();        
        if(topToken == m_token)
        {
            m_parser->PopNode();
        }
    }

}}}}//qor::data::parser::json