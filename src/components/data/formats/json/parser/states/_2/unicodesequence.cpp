// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"

#include "src/qor/flyers/error/error.h"
#include "src/qor/flyers/log/log.h"
#include "src/qor/flyers/log/debug.h"
#include "unicodesequence.h"
#include "../../nodes/unicodesequence.h"
#include "../../nodes/character.h"
#include "src/components/data/parser/nodes/hexdigit.h"

namespace qor { namespace data { namespace parser { namespace json {

    unicodeSequence::unicodeSequence(Parser* parser) : 
        Sequence_t<Specific<uint32_t>, Sequence_t<HEXDIGIT<uint32_t>, Sequence_t<HEXDIGIT<uint32_t>, Sequence_t<HEXDIGIT<uint32_t>, HEXDIGIT<uint32_t>>>>>(
        parser,  &s1, &s2, static_cast<uint64_t>(jsonToken::unicode_sequence)),
        h1(parser), 
        h2(parser),
        h3(parser),
        h4(parser), 
        s1(parser, 0x75),
        s2(parser, &h1, &s3), 
        s3(parser, &h2, &s4), 
        s4(parser, &h3, &h4)        
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
        //log::debug("...Didn't find a Unicode Sequence.");
        uint64_t topToken = m_parser->TopNode()->GetToken();        
        if(topToken == m_token)
        {
            m_parser->PopNode();
        }
    }

}}}}//qor::data::parser::json
