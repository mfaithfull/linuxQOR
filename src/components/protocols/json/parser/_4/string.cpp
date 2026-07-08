// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"

#include "src/qor/flyers/error/error.h"
#include "src/qor/flyers/log/log.h"
#include "src/qor/flyers/log/debug.h"
#include "string.h"
#include "../nodes/string.h"
#include "../nodes/character.h"

namespace qor { namespace components { namespace parser { namespace json {

    string::string(Parser* parser) :
                Sequence(parser, new_ref<quotation_mark>(parser), 
                    new_ref<Sequence>(parser, new_ref<ZeroOrMore>(parser, new_ref<character>(parser)),
                        new_ref<quotation_mark>(parser)
                    ),
                    static_cast<uint64_t>(jsonToken::string)
                )
    { }

    string::~string() = default;

    void string::Prepare()
    {
        log::debug("Looking for a String...");
        GetParser()->PushNode(new_ref<StringNode>().AsRef<Node>());
    }

    void string::Emit()
    {        
        log::debug("Emitting a String");
        std::string stringValue;
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
            
            if(token == static_cast<uint64_t>(jsonToken::character))
            {
                auto characterNode = node.AsRef<CharacterNode>();
                char c = static_cast<char>(characterNode->GetObject()->GetValue());
                stringValue = std::string(&c,1) + stringValue;
            }
            else if(token == static_cast<uint64_t>(eToken::Digit))
            {                
                stringValue = node->ToString() + stringValue;
            }
            else
            {
                continuable("Unexpected {0}", tokenName);
            }

            node = GetParser()->PopNode();
        }

        if(node.IsNotNull())
        {
            auto stringNode = node.AsRef<StringNode>();
            log::debug("String:{0}", stringValue);
            stringNode->GetObject()->SetValue(stringValue);
            GetParser()->PushNode(node);
        }
    }

    void string::Fail()
    {
        log::debug("...Didn't find a String.");
        auto node = GetParser()->PopNode();
        if(node.IsNotNull() && node->GetToken() != m_token)
        {
            GetParser()->PushNode(node);
        }
    }

}}}}//qor::components::parser::json
