// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"

#include <cassert>
#include <cmath>
#include "src/qor/essentials/current/currentthread.h"
#include "src/qor/memory/reference/newref.h"
#include "src/qor/flyers/error/error.h"
#include "src/qor/flyers/log/debug.h"
#include "number.h"
#include "../nodes/number.h"
#include "../nodes/integer.h"
#include "../nodes/fraction.h"
#include "../nodes/exp.h"

namespace qor { namespace components { namespace parser { namespace json {

    number::number(Parser* parser) : Sequence(parser, new_ref<Optional>(parser,new_ref<minus>(parser)),
            new_ref<Sequence>(parser, new_ref<integer>(parser), 
                new_ref<Sequence>(parser, new_ref<Optional>(parser, new_ref<fraction>(parser)),
                    new_ref<Optional>(parser, new_ref<exp>(parser))
                )
            ),
            static_cast<uint64_t>(jsonToken::number))
    { }

    number::~number() = default;

    void number::Prepare()
    {
        log::debug("Looking for a Number...");
        GetParser()->PushNode(new_ref<NumberNode>());
    }

    //number = [ minus ] int [ frac ] [ exp ]
    void number::Emit()
    {
        log::debug("Emitting a Number");
        double value = 0.0;        
        int sign = 1;
        double frac = 0.0;
        int exp = 0;

        auto node = GetParser()->PopNode();
        do{
            if(node.IsNotNull() && node->GetToken() != m_token)
            {
                uint64_t token = node->GetToken();

                if(token == static_cast<uint64_t>(jsonToken::minus))
                {
                    sign = -1;
                    node = GetParser()->PopNode();
                    token = node->GetToken();
                    if(node.IsNull() || node->GetToken() == m_token)
                    {
                        break;
                    }
                }                
                else if(token == static_cast<uint64_t>(jsonToken::integer))
                {
                    auto integerNode = node.AsRef<IntegerNode>();
                    value = integerNode->GetObject()->GetValue();
                    node = GetParser()->PopNode();
                    token = node->GetToken();
                    if(node.IsNull() || node->GetToken() == m_token)
                    {
                        break;
                    }
                }
                else if(token == static_cast<uint64_t>(jsonToken::zero))
                {
                    value = 0;
                    node = GetParser()->PopNode();
                    token = node->GetToken();
                    if(node.IsNull() || node->GetToken() == m_token)
                    {
                        break;
                    }
                }
                else if(token == static_cast<uint64_t>(jsonToken::exp))
                {
                    auto expNode = node.AsRef<ExpNode>();
                    if(expNode.IsNotNull())
                    {
                        exp = expNode->GetObject()->GetValue();
                    }                    
                    node = GetParser()->PopNode();
                    token = node->GetToken();
                }
                else if(token == static_cast<uint64_t>(jsonToken::fraction))
                {
                    auto fractionNode = node.AsRef<FractionNode>();
                    if(fractionNode.IsNotNull())
                    {
                        frac = fractionNode->GetObject()->GetValue();
                    }
                    node = GetParser()->PopNode();
                    token = node->GetToken();
                    if(node.IsNull() || node->GetToken() == m_token)
                    {
                        break;
                    }
                }
                else
                {
                    auto f = jsonTokenNames.find((jsonToken)token);
                    std::string tokenName;
                    if(f != jsonTokenNames.end())
                    {
                        tokenName = f->second;
                    }

                    continuable("Unexpected: {0}", tokenName);
                }
            }
        }while(node.IsNotNull() && node->GetToken() != m_token);

        if(node.IsNotNull())
        {
            auto numberNode = node.AsRef<NumberNode>();
            value = ((value + frac) * sign) * pow(10, exp);
            numberNode->GetObject()->SetValue(value);
            log::debug("Number: {0}", value);
            GetParser()->PushNode(node);
        }
    }

    void number::Fail()
    {
        log::debug("...Didn't find a Number.");
        ref_of<Node>::type node = GetParser()->PopNode();
        if(node.IsNotNull() && node->GetToken() != m_token)
        {
            GetParser()->PushNode(node);
        }
    }

}}}}//qor::components::json::parser
