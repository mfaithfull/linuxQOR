// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"

#include "src/qor/flyers/error/error.h"
#include "src/qor/flyers/log/log.h"
#include "src/qor/flyers/log/debug.h"
#include "string.h"
#include "../../nodes/string.h"
#include "../../nodes/character.h"

namespace qor { namespace data { namespace parser { namespace json {

    UTF8CodePage string::s_codePage;

    string::string(Parser* parser) :
        Sequence_t< quotation_mark, Sequence_t< ZeroOrMore_t< character >, quotation_mark > >(parser, &m_qopen, &m_tail, static_cast<uint64_t>(jsonToken::string)),
            m_qopen(parser),
            m_qclose(parser),
            m_char(parser),
            m_body(parser, &m_char),
            m_tail(parser, &m_body, &m_qclose)
    { }

    string::~string() = default;

    void string::Prepare()
    {
        //log::debug("Looking for a String...");
        GetParser()->PushNode(new_ref<StringNode>().AsRef<Node>());
    }

    void string::Emit()
    {        
        //log::debug("Emitting a String");
        std::string stringValue;        
        auto node = GetParser()->PopNode();
        while(node.IsNotNull() && node->GetToken() != m_token)
        {
            uint64_t token = node->GetToken();
            
            if(token == static_cast<uint64_t>(jsonToken::character))
            {
                auto characterNode = node.AsRef<CharacterNode>();
                char32_t c = static_cast<char32_t>(characterNode->GetObject()->GetValue());
                char8_t space[6];
                char8_t* p = &space[0];
                size_t available = 1;
                if(s_codePage.Encode(c, p, available))
                {
                    stringValue = std::string((const char*)space, (p - &space[0])) + stringValue;
                }
            }
            else if(token == static_cast<uint64_t>(eToken::Digit))
            {                
                stringValue = node->ToString() + stringValue;
            }
            else
            {
                auto f = jsonTokenNames.find((jsonToken)token);
                std::string tokenName;
                if(f != jsonTokenNames.end())
                {
                    tokenName = f->second;
                }
                else
                {
                    auto g = tokenNames.find((eToken)token);
                    if( g != tokenNames.end())
                    {
                        tokenName = g->second;
                    }
                    else
                    {
                        continuable("Unrecognized token {0}", token);
                    }
                }
                continuable("Unexpected {0}", tokenName);
            }

            node = GetParser()->PopNode();
        }

        if(node.IsNotNull())
        {
            auto stringNode = node.AsRef<StringNode>();
            //log::debug("String:{0}", stringValue);
            stringNode->GetObject()->SetValue(stringValue);
            GetParser()->PushNode(node);
        }
    }

    void string::Fail()
    {
        //log::debug("...Didn't find a String.");
        uint64_t topToken = m_parser->TopNode()->GetToken();        
        if(topToken == m_token)
        {
            m_parser->PopNode();
        }
    }

}}}}//qor::data::parser::json
