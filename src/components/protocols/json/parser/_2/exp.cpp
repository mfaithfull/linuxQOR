// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"

#include <cassert>
#include "src/qor/essentials/current/currentthread.h"
#include "src/qor/memory/reference/newref.h"
#include "src/qor/flyers/error/error.h"
#include "src/qor/flyers/log/debug.h"
#include "src/components/data/parser/nodes/digit.h"
#include "exp.h"
#include "../nodes/exp.h"

namespace qor { namespace components { namespace parser { namespace json {

    exp::exp(Parser* parser) : Sequence(parser, new_ref<_e>(parser),
        new_ref<Sequence>(parser, new_ref<Optional>(parser,
            new_ref<AnyOneOf>(parser, new_ref<minus>(parser),
                new_ref<plus>(parser)
            )),
            new_ref<Sequence>(parser, new_ref<DIGIT>(parser),
                new_ref<ZeroOrMore>(parser, new_ref<DIGIT>(parser))
            )
        ),
        static_cast<uint64_t>(jsonToken::exp))
    { }

    exp::~exp() = default;

    void exp::Prepare()
    {
        log::debug("Looking for an Exp...");
        GetParser()->PushNode(new_ref<ExpNode>());
    }

    void exp::Emit()
    {
        log::debug("Emitting an Exp: ");
        int value = 0;
        int magnitude = 1;
        int sign = 1;
        auto node = GetParser()->PopNode();
        while(node.IsNotNull() && node->GetToken() != m_token)
        {
            uint64_t token = node->GetToken();
            if(token == static_cast<uint64_t>(eToken::Digit))
            {
                auto digitNode = node.AsRef<DigitNode>();
                unsigned int digitVal = digitNode->GetValue();
                value += digitVal * magnitude;
                magnitude *= 10;
            }
            else if( node->GetToken() == static_cast<uint64_t>(jsonToken::plus))
            {
                sign = 1;                
            }
            else if(node->GetToken() == static_cast<uint64_t>(jsonToken::minus))
            {
                sign = -1;                
            }
            else if(token == static_cast<uint64_t>(jsonToken::_e))
            {
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

                continuable("Unexpected: {0}", tokenName);
            }
            node = GetParser()->PopNode();
        }

        if(node.IsNotNull())
        {
            auto expNode = node.AsRef<ExpNode>();
            expNode->GetObject()->SetValue(value * sign);
            GetParser()->PushNode(node);
        }
    }

    void exp::Fail()
    {
        log::debug("...Didn't find an Exp.");
        ref_of<Node>::type node = GetParser()->PopNode();
        if(node.IsNotNull() && node->GetToken() != m_token)
        {
            GetParser()->PushNode(node);
        }
    }

}}}}//qor::components::parser::json
