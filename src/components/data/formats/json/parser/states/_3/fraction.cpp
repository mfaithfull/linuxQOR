// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"

#include <cassert>
#include "src/qor/essentials/current/currentthread.h"
#include "src/qor/memory/reference/newref.h"
#include "src/qor/flyers/error/error.h"
#include "src/qor/flyers/log/debug.h"
#include "fraction.h"
#include "../../nodes/fraction.h"
#include "src/components/data/parser/nodes/digit.h"

namespace qor { namespace data { namespace parser { namespace json {

    fraction::fraction(Parser* parser) : 
        Sequence_t< decimal_point, Sequence_t< DIGIT<uint32_t>, ZeroOrMore_t< DIGIT<uint32_t> > > >(
                parser, &dp, &f, static_cast<uint64_t>(jsonToken::fraction)),
        dp(parser),
        pd(parser),
        sd(parser),
        continuation(parser, &sd),
        f(parser, &pd, &continuation)
    { }

    fraction::~fraction() = default;

    void fraction::Prepare()
    {
        log::debug("Looking for a Fraction...");
        GetParser()->PushNode(new_ref<FractionNode>());
    }

    void fraction::Emit()
    {
        log::debug("Emitting a Fraction.");
        double value = 0.0;        
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

            if(token == static_cast<uint64_t>(eToken::Digit))
            {
                auto digitNode = node.AsRef<DigitNode>();
                double digitVal = 0.0 + digitNode->GetValue();
                if(value > 0)
                {
                    value /= 10;
                }
                if( digitVal > 0 )
                {
                    value += digitVal / 10;
                }
            }
            else if(token == static_cast<uint64_t>(jsonToken::decimal_point))
            {
                //TODO: prevent further non fraction tokens
            }
            else
            {
                continuable("Unexpected: {0}", tokenName);
            }
            
            node = GetParser()->PopNode();
        }

        if(node.IsNotNull())
        {
            auto fractionNode = node.AsRef<FractionNode>();
            fractionNode->GetObject()->SetValue(value);
            GetParser()->PushNode(node);
        }
    }

    void fraction::Fail()
    {
        //log::debug("...Didn't find a Fraction.");
        uint64_t topToken = m_parser->TopNode()->GetToken();        
        if(topToken == m_token)
        {
            m_parser->PopNode();
        }
    }

}}}}//qor::data::parser::json
