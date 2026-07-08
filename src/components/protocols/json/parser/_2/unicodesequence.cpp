// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"

#include "src/qor/flyers/error/error.h"
#include "src/qor/flyers/log/log.h"
#include "src/qor/flyers/log/debug.h"
#include "unicodesequence.h"
#include "../nodes/unicodesequence.h"
#include "../nodes/character.h"
#include "src/components/data/parser/nodes/hexdigit.h"

namespace qor { namespace components { namespace parser { namespace json {

    unicodeSequence::unicodeSequence(Parser* parser) : Sequence(parser,  new_ref<Specific>(parser, (byte)0x75),// 'u'
        new_ref<Sequence>(parser, new_ref<HEXDIGIT>(parser),
            new_ref<Sequence>(parser, new_ref<HEXDIGIT>(parser),
                new_ref<Sequence>(parser, new_ref<HEXDIGIT>(parser),
                    new_ref<HEXDIGIT>(parser)
                )
            )
        ),
        static_cast<uint64_t>(jsonToken::unicode_sequence))
    { }

    unicodeSequence::~unicodeSequence() = default;

    void unicodeSequence::Prepare()
    {
        log::debug("Looking for a Unicode Sequence...");
        GetParser()->PushNode(new_ref<UnicodeSequenceNode>());
    }

    void unicodeSequence::Emit()
    {
        log::debug("Emitting a Unicode Sequence");
        int magnitude = 1;
        char32_t cValue = 0;
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

            if(token == static_cast<uint64_t>(eToken::HexDigit))
            {
                auto digitNode = node.AsRef<HexDigitNode>();
                unsigned int digitVal = digitNode->GetValue();
                cValue += digitVal * magnitude;
                magnitude *= 16;
            }
            else
            {
                continuable("Unexpected: {0}", tokenName);
            }

            node = GetParser()->PopNode();                
        };

        if(node.IsNotNull())
        {
            log::debug("Unicode Sequence: {0}", (unsigned int)cValue);
            GetParser()->PushNode(new_ref<CharacterNode>(cValue));
        }
    }

    void unicodeSequence::Fail()
    {
        log::debug("...Didn't find a Unicode Sequence.");
        auto node = GetParser()->PopNode();
        if(node.IsNotNull() && node->GetToken() != m_token)
        {
            GetParser()->PushNode(node);
        }
    }

}}}}//qor::components::parser::json
