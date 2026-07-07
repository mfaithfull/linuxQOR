// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"
#include "src/platform/compiler/compiler.h"
#include "src/qor/flyers/error/error.h"
#include "../parser.h"
#include "rfc5234.h"
#include "../nodes/digit.h"
#include "../nodes/char.h"
#include "../nodes/hexdigit.h"

namespace qor { namespace components { namespace parser {

    DIGIT::DIGIT(Parser* parser) : OneOfARange(parser, 0x30, 0x39, static_cast<uint64_t>(eToken::Digit)) { } 

    void DIGIT::Emit()
    {
        int digitVal = m_result.first - '0';
        GetParser()->PushNode(new_ref<DigitNode>(digitVal).AsRef<Node>());
    }

    DIGIT::~DIGIT() = default;

    VCHAR::VCHAR(Parser* parser) : OneOfARange(parser, 0x21, 0x7e, static_cast<uint64_t>(eToken::VisibleChar)) { } 

    void VCHAR::Emit()
    {
        char charVal = (char)m_result.first;
        GetParser()->PushNode(new_ref<CharNode>(charVal, static_cast<uint64_t>(m_token)).AsRef<Node>());
    }

    VCHAR::~VCHAR() = default;

    CTL::CTL(Parser* parser) :  AnyOneOf(parser,  new_ref<OneOfARange>(parser, (byte)0x00, (byte)0x1f, static_cast<uint64_t>(eToken::Lexical)), 
        new_ref<Specific>(parser, (byte)0x7f, static_cast<uint64_t>(eToken::Lexical)), static_cast<uint64_t>(eToken::Control)){ } 

    void CTL::Emit()
    {
        char ctlVal = (char)m_result.first;
        GetParser()->PushNode(new_ref<CharNode>(ctlVal, static_cast<uint64_t>(m_token)).AsRef<Node>());
    }
    
    CTL::~CTL() = default;

    HEXDIGIT::HEXDIGIT(Parser* parser) : AnyOneOf(parser, new_ref<DIGIT>(parser), new_ref<AnyOneOf>(parser,
            new_ref<OneOfARange>(parser, 'A', 'F', static_cast<uint64_t>(eToken::Lexical)),
            new_ref<OneOfARange>(parser, 'a', 'f', static_cast<uint64_t>(eToken::Lexical)),
            static_cast<uint64_t>(eToken::Lexical)
        ),
        static_cast<uint64_t>(eToken::HexDigit)){ }

    void HEXDIGIT::Prepare()
    {
        //log::debug("Looking for a HEX DIGIT...");
        GetParser()->PushNode(new_ref<HexDigitNode>(0));
    }

    void HEXDIGIT::Emit()
    {
        //log::debug("Emitting a HEX DIGIT");

        unsigned int hexVal = 0;
        auto node = GetParser()->PopNode();
        if(node.IsNotNull() && node->GetToken() == static_cast<uint64_t>(eToken::Digit))
        {
            hexVal = node.AsRef<DigitNode>()->GetValue();
            node = GetParser()->PopNode();
        }
        else
        {
            hexVal = (m_result.first >= '0' && m_result.first <= '9') ? m_result.first - '0' :
                        ((m_result.first >= 'a' && m_result.first <= 'f') ? m_result.first - 'a' + 10 :
                            ((m_result.first >= 'A' && m_result.first <= 'F') ? m_result.first - 'A' + 10 : 0));
        }

        if(node.IsNotNull() && node->GetToken() != static_cast<uint64_t>(eToken::HexDigit))
        {
            continuable("Unexpected token: Expected HexDigit token {0} got {1}.", static_cast<uint64_t>(eToken::HexDigit), node->GetToken());
        }
        
        GetParser()->PushNode(new_ref<HexDigitNode>(hexVal));
    }

    void HEXDIGIT::Fail()
    {
        //log::debug("...Didn't find a HEX DIGIT.");
        auto node = GetParser()->PopNode();
        if(node.IsNotNull() && node->GetToken() != m_token)
        {
            GetParser()->PushNode(node);
        }
    }

    HEXDIGIT::~HEXDIGIT() = default;

    BIT::BIT(Parser* parser) : AnyOneOf(parser, new_ref<Specific>(parser, '0', static_cast<uint64_t>(eToken::Lexical)),
        new_ref<Specific>(parser, '1', static_cast<uint64_t>(eToken::Lexical)), static_cast<uint64_t>(eToken::Bit)) { }

    void BIT::Emit()
    {
        unsigned int bitVal = m_result.first == '0' ? 0 : 
        (m_result.first == '1' ? 1 : 0);
        GetParser()->PushNode(new_ref<DigitNode>(bitVal).AsRef<Node>());
    }

    BIT::~BIT() = default;

    void CHAR::Emit()
    {
        char charVal = (char)m_result.first;
        GetParser()->PushNode(new_ref<CharNode>(charVal, static_cast<uint64_t>(m_token)).AsRef<Node>());
    }

    CHAR::~CHAR() = default;

    ALPHA::ALPHA(Parser* parser) : AnyOneOf(parser, new_ref<OneOfARange>(parser, (byte)0x41, (byte)0x5A, static_cast<uint64_t>(eToken::Lexical)),
        new_ref<OneOfARange>(parser, (byte)0x61, (byte)0x7A, static_cast<uint64_t>(eToken::Lexical)), static_cast<uint64_t>(eToken::Alpha)) { }

    void ALPHA::Emit()
    {
        char charVal = (char)m_result.first;
        GetParser()->PushNode(new_ref<CharNode>(charVal, static_cast<uint64_t>(m_token)).AsRef<Node>());
    }
    
    ALPHA::~ALPHA() = default;

}}}//qor::components::parser
