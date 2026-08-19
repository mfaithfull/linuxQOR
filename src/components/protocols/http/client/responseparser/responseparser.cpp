// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"
#include "responseparser.h"
#include "responsenode.h"
#include "src/qor/flyers/error/error.h"

using namespace qor;
using namespace qor::data::parser;

namespace qor { namespace components { namespace protocols { namespace http { namespace response {

    void Initial::Prepare()
    {
        GetParser()->PushNode(new_ref<ResponseNode>().template AsRef<Node>());
    }

    void Initial::Emit()
    {
        std::vector<char> chars;
        auto node = GetParser()->PopNode();
        while(node.IsNotNull() && node->GetToken() != m_token)
        {
            uint64_t token = node->GetToken();
            
            if(token == static_cast<uint64_t>(httpResponseToken::responseChar))
            {
                auto characterNode = node.AsRef<CharNode>();
                char c = characterNode->GetValue();
                chars.push_back(c);
            }
            else
            {
                auto f = httpResponseTokenNames.find((httpResponseToken)token);
                std::string tokenName;
                if(f != httpResponseTokenNames.end())
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

}}}}}//qor::components::protocols::http::response
