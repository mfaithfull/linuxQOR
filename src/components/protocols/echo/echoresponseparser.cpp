// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"
#include "echoresponseparser.h"
#include "responsenode.h"
#include "src/qor/flyers/error/error.h"

using namespace qor;
using namespace qor::components::parser;

//Client side parser for interpretting response

namespace qor { namespace components { namespace protocols { namespace echo {

    response::response(qor::components::parser::Parser* parser) : qor::components::parser::OneOrMore(
        parser, 
        qor::new_ref<responseChar>(parser).template AsRef<qor::components::parser::ParserState>(), 
        static_cast<uint64_t>(echoResponseToken::response))
    { }

    response::~response() = default;    

    void response::Prepare()
    {
        GetParser()->PushNode(new_ref<ResponseNode>().template AsRef<Node>());
    }

    void response::Emit()
    {
        std::vector<char> chars;
        auto node = GetParser()->PopNode();
        while(node.IsNotNull() && node->GetToken() != m_token)
        {
            uint64_t token = node->GetToken();
            
            if(token == static_cast<uint64_t>(echoResponseToken::responseChar))
            {
                auto characterNode = node.AsRef<CharNode>();
                char c = characterNode->GetValue();
                chars.push_back(c);
            }
            else
            {
                auto f = echoResponseTokenNames.find(token);
                std::string tokenName;
                if(f != echoResponseTokenNames.end())
                {
                    tokenName = f->second;
                }

                std::cout << "Emitting a response expected responseChar found: " << tokenName << std::endl;
                serious("Response parsing failed.");
            }

            node = GetParser()->PopNode();
        }

        if(node.IsNotNull())
        {
            std::string responseValue(chars.rbegin(), chars.rend());
            auto responseNode = node.AsRef<ResponseNode>();
            responseNode->GetObject()->SetValue(responseValue);
            GetParser()->PushNode(node);
        }
    }

    void response::Fail()
    {
        auto node = GetParser()->PopNode();
        if(node.IsNotNull() && node->GetToken() != m_token)
        {
            GetParser()->PushNode(node);
        }
    }

}}}}//qor::components::protocols::echo
