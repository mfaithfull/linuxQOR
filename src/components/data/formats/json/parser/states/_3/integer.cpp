// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"

#include <cassert>
#include "src/qor/essentials/current/currentthread.h"
#include "src/qor/memory/reference/newref.h"
#include "src/components/data/parser/nodes/digit.h"
#include "src/qor/flyers/log/debug.h"
#include "src/qor/flyers/error/error.h"
#include "integer.h"
#include "../../nodes/integer.h"
#include "../../nodes/digit1_9.h"

namespace qor { namespace data { namespace parser { namespace json {

    integer::integer(Parser* parser) : AnyOneOf(parser,  new_ref<zero>(parser),
        new_ref<Sequence>(parser, new_ref<digit1_9>(parser),
            new_ref<ZeroOrMore>(parser, new_ref<DIGIT<uint32_t>>(parser))),
        static_cast<uint64_t>(jsonToken::integer))
    { }
    
    integer::~integer() = default;

    void integer::Prepare()
    {
        log::debug("Looking for a Integer...");
        GetParser()->PushNode(new_ref<IntegerNode>());
    }

    void integer::Emit()
    {
        log::debug("Emitting an Integer.");
        int value = 0;
        int magnitude = 1;
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
            
            if(token == static_cast<uint64_t>(jsonToken::zero))
            {
                if(value != 0)
                {
                    continuable("Invalid number format");
                }
            }
            else if(token == static_cast<uint64_t>(eToken::Digit))
            {
                auto digitNode = node.AsRef<DigitNode>();
                unsigned int digitVal = digitNode->GetValue();
                value += digitVal * magnitude;
                magnitude *= 10;
            }
            else if(token == static_cast<uint64_t>(jsonToken::digit1_9))
            {
                auto digitNode = node.AsRef<Digit1_9Node>();
                unsigned int digitVal = digitNode->GetObject()->GetValue();
                value += digitVal * magnitude;
                magnitude *= 10;
            }
            else
            {
                continuable("Unexpected: {0}", tokenName);
            }
            node = GetParser()->PopNode();
        }

        if(node.IsNotNull())
        {
            auto integerNode = node.AsRef<IntegerNode>();
            integerNode->GetObject()->SetValue(value);
            GetParser()->PushNode(node);
        }
    }

    void integer::Fail()
    {
        log::debug("...Didn't find an Integer.");
        ref_of<Node>::type node = GetParser()->PopNode();
        if(node.IsNotNull() && node->GetToken() != m_token)
        {
            GetParser()->PushNode(node);
        }
    }

}}}}//qor::data::parser::json
