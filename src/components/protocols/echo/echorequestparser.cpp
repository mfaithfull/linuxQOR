// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"
#include "requestnode.h"
#include "src/qor/flyers/error/error.h"

using namespace qor;
using namespace qor::components::parser;

//Server side parser for interpretting request

namespace qor { namespace components { namespace protocols { namespace echo {
    
    void request::Prepare()
    {
        GetParser()->PushNode(new_ref<RequestNode>().template AsRef<Node>());
    }

    void request::Emit()
    {
        std::vector<char> chars;
        auto node = GetParser()->PopNode();
        while(node.IsNotNull() && node->GetToken() != m_token)
        {
            uint64_t token = node->GetToken();
            
            if(token == static_cast<uint64_t>(echoRequestToken::requestChar))
            {
                auto characterNode = node.AsRef<CharNode>();
                char c = characterNode->GetValue();
                chars.push_back(c);
            }
            else
            {
                auto f = echoRequestTokenNames.find(token);
                std::string tokenName;
                if(f != echoRequestTokenNames.end())
                {
                    tokenName = f->second;
                }

                std::cout << "Emitting a request expected requestChar found: " << tokenName << std::endl;
                serious("Request parsing failed.");
            }

            node = GetParser()->PopNode();
        }

        if(node.IsNotNull())
        {
            std::string requestValue(chars.rbegin(), chars.rend());
            auto requestNode = node.AsRef<RequestNode>();
            requestNode->GetObject()->SetValue(requestValue);
            GetParser()->PushNode(node);
        }
    }
    
}}}}//qor::components::protocols::echo
