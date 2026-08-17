// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_DATA_PARSER_RFC5234
#define QOR_PP_H_DATA_PARSER_RFC5234

#include "../state.h"
#include "specific.h"
#include "oneofarange.h"
#include "anyoneof.h"
#include "sequence.h"
#include "zeroormore.h"
#include "../nodes/digit.h"
#include "../nodes/hexdigit.h"
#include "../nodes/char.h"

namespace qor { namespace data { namespace parser {

    template<typename item_t>
    class CR : public Specific<item_t>
    { 
    public: 
        CR(Parser* parser) : Specific<item_t>(parser, 0x0D, static_cast<uint64_t>(eToken::CarriageReturn)){} 
        virtual ~CR() = default;
        virtual void Emit(){}
    };

    template<typename item_t>
    class LF : public Specific<item_t>
    { public: LF(Parser* parser) : Specific<item_t>(parser, 0x0A, static_cast<uint64_t>(eToken::LineFeed)){} 
        virtual ~LF() = default;
        virtual void Emit(){}
    };

    template<typename item_t>
    class HTAB : public Specific<item_t>
    { public: HTAB(Parser* parser) : Specific<item_t>(parser, 0x09, static_cast<uint64_t>(eToken::HorizontalTab)){} 
        virtual ~HTAB() = default;
        virtual void Emit(){}
    };

    template<typename item_t>
    class DQUOTE : public Specific<item_t>
    { public: DQUOTE(Parser* parser) : Specific<item_t>(parser, 0x22, static_cast<uint64_t>(eToken::DoubleQuote)){} 
        virtual ~DQUOTE() = default;
        virtual void Emit(){}
    };

    template<typename item_t>
    class SP : public Specific<item_t>
    { 
    public: 
        SP(Parser* parser) : Specific<item_t>(parser, 0x20, static_cast<uint64_t>(eToken::Space)){} 
        virtual ~SP() = default;
        virtual void Emit(){}
    };

    template<typename item_t>
    class DIGIT : public OneOfARange_t<item_t>
    { 
    public: 
        DIGIT(Parser* parser) : OneOfARange_t<item_t>(parser, 0x30, 0x39, static_cast<uint64_t>(eToken::Digit)) { }
        
        virtual ~DIGIT() = default;

        virtual void Emit()
        {
            int digitVal = OneOfARange_t<item_t>::m_result.first - '0';            
            OneOfARange_t<item_t>::GetParser()->PushNode(new_ref<DigitNode>(digitVal).AsRef<Node>());
        }
    };

    template<typename item_t>
    class VCHAR : public OneOfARange_t<item_t>
    { 
    public: 
        VCHAR(Parser* parser) : OneOfARange_t<item_t>(parser, 0x21, 0x7e, static_cast<uint64_t>(eToken::VisibleChar)) { }
        virtual ~VCHAR() = default;
        virtual void Emit()
        {
            char charVal = (char)OneOfARange_t<item_t>::m_result.first;
            OneOfARange_t<item_t>::GetParser()->PushNode(new_ref<CharNode>(
                charVal, static_cast<uint64_t>(OneOfARange_t<item_t>::m_token)).AsRef<Node>());
        }
    };

    template<typename item_t>
    class CTL : public AnyOneOf_t<OneOfARange_t<item_t>, Specific<item_t>>
    { 
    public: 
        CTL(Parser* parser) : AnyOneOf_t<OneOfARange_t<item_t>, Specific<item_t>>(parser, &a, &b, static_cast<uint64_t>(eToken::Control)),
            a(parser, (item_t)0x00, (item_t)0x1f, static_cast<uint64_t>(eToken::Lexical)),
            b(parser, (item_t)0x7f, static_cast<uint64_t>(eToken::Lexical)){ }
        virtual ~CTL() = default;
        virtual void Emit()
        {
            char ctlVal = (char)OneOfARange_t<item_t>::m_result.first;
            OneOfARange_t<item_t>::GetParser()->PushNode(new_ref<CharNode>(
                ctlVal, static_cast<uint64_t>(OneOfARange_t<item_t>::m_token)).AsRef<Node>());
        }

    private:

        OneOfARange_t<item_t> a;
        Specific<item_t> b;
    };

    template<typename item_t>
    class HEXDIGIT : public AnyOneOf_t<DIGIT<item_t>, AnyOneOf_t<OneOfARange_t<item_t>, OneOfARange_t<item_t>>>
    { 
    public: 

        using base = AnyOneOf_t<DIGIT<item_t>, AnyOneOf_t<OneOfARange_t<item_t>, OneOfARange_t<item_t>>>;

        HEXDIGIT(Parser* parser) : base(
                parser, &m_d, &m_tail,static_cast<uint64_t>(eToken::HexDigit)),
            m_tail(parser, &m_upper, &m_lower, static_cast<uint64_t>(eToken::Lexical)),
            m_d(parser),
            m_upper(parser, 'A', 'F', static_cast<uint64_t>(eToken::Lexical)),
            m_lower(parser, 'a', 'f', static_cast<uint64_t>(eToken::Lexical))
        { }

        virtual void Prepare()
        {
            base::GetParser()->PushNode(new_ref<HexDigitNode>(0));
        }

        virtual void Emit()
        {
            unsigned int hexVal = 0;
            auto node = base::GetParser()->PopNode();
            if(node.IsNotNull() && node->GetToken() == static_cast<uint64_t>(eToken::Digit))
            {
                hexVal = node.template AsRef<DigitNode>()->GetValue();
                node = base::GetParser()->PopNode();
            }
            else
            {
                hexVal = (base::m_result.first >= '0' && base::m_result.first <= '9') ? base::m_result.first - '0' :
                            ((base::m_result.first >= 'a' && base::m_result.first <= 'f') ? base::m_result.first - 'a' + 10 :
                                ((base::m_result.first >= 'A' && base::m_result.first <= 'F') ? base::m_result.first - 'A' + 10 : 0));
            }

            if(node.IsNotNull() && node->GetToken() != static_cast<uint64_t>(eToken::HexDigit))
            {
                continuable("Unexpected token: Expected HexDigit token {0} got {1}.", static_cast<uint64_t>(eToken::HexDigit), node->GetToken());
            }

            base::GetParser()->PushNode(new_ref<HexDigitNode>(hexVal));
        }

        virtual void Fail()
        {
            auto node = base::GetParser()->PopNode();
            if(node.IsNotNull() && node->GetToken() != base::m_token)
            {
                base::GetParser()->PushNode(node);
            }
        }

        virtual ~HEXDIGIT() = default;

    private:

        DIGIT<item_t> m_d;
        AnyOneOf_t<OneOfARange_t<item_t>, OneOfARange_t<item_t>> m_tail;
        OneOfARange_t<item_t> m_upper;
        OneOfARange_t<item_t> m_lower;

    };

    template<typename item_t>
    class WSP : public AnyOneOf_t< SP<item_t>, HTAB<item_t> >
    { 
    public: 
        WSP(Parser* parser) : AnyOneOf_t< SP<item_t>, HTAB<item_t> >(parser, &m_a, &m_b, static_cast<uint64_t>(eToken::WhiteSpace)),
            m_a(parser), 
            m_b(parser)
        {}
        virtual ~WSP() = default;
        virtual void Emit(){}
    private:

        SP<item_t> m_a;
        HTAB<item_t> m_b;
    };

    template<typename item_t>
    class CRLF : public Sequence_t< CR<item_t>, LF<item_t> >
    { 
    public: 
        CRLF(Parser* parser) : Sequence_t< CR<item_t>, LF<item_t> >(parser, &m_head, &m_tail, static_cast<uint64_t>(eToken::CarriageReturnLineFeed)),
            m_head(parser), m_tail(parser){}
        virtual ~CRLF() = default;
        virtual void Emit(){}

    private:
        CR<item_t> m_head;
        LF<item_t> m_tail;
    };

    template<typename item_t>
    class LWSP : public ZeroOrMore_t< AnyOneOf_t< WSP<item_t>, CRLF<item_t> > >
    { 
    public: 
        LWSP(Parser* parser) : ZeroOrMore_t< AnyOneOf_t< WSP<item_t>, CRLF<item_t> > >(
                parser, &m_anyOneOf, static_cast<uint64_t>(eToken::LinearWhiteSpace)), 
            m_anyOneOf(parser, &m_WSP, &m_CRLF, static_cast<uint64_t>(eToken::Lexical)),
            m_WSP(parser),
            m_CRLF(parser)
        {}
        virtual ~LWSP() = default;
        virtual void Emit(){}
    
    private:

        AnyOneOf_t< WSP<item_t>, CRLF<item_t> > m_anyOneOf;
        WSP<item_t> m_WSP;
        CRLF<item_t> m_CRLF;
    };

    template<typename item_t>
    class BIT : public AnyOneOf_t< Specific<item_t>, Specific<item_t> >
    { 
    public: 

        using base = AnyOneOf_t< Specific<item_t>, Specific<item_t> >;

        BIT(Parser* parser) : base(
                parser, &m_zero, &m_one, static_cast<uint64_t>(eToken::Bit)),
            m_zero(parser, '0', static_cast<uint64_t>(eToken::Lexical)),
            m_one(parser, '1', static_cast<uint64_t>(eToken::Lexical) )
        { }

        virtual ~BIT() = default;
        virtual void Emit()
        {
            unsigned int bitVal = base::m_result.first == '0' ? 0 :
            (base::m_result.first == '1' ? 1 : 0);
            base::GetParser()->PushNode(new_ref<DigitNode>(bitVal).AsRef<Node>());
        }

    private:

        Specific<item_t> m_zero;
        Specific<item_t> m_one;

    };

    template<typename item_t>
    class CHAR : public OneOfARange_t<item_t>
    { 
    public:

        CHAR(Parser* parser) : OneOfARange_t<item_t>(parser, 0x01, 0x7F, static_cast<uint64_t>(eToken::Char))
        { }

        virtual ~CHAR() = default;

        virtual void Emit()
        {
            char charVal = (char)OneOfARange_t<item_t>::m_result.first;
            OneOfARange_t<item_t>::GetParser()->PushNode(new_ref<CharNode>(charVal, static_cast<uint64_t>(OneOfARange_t<item_t>::m_token)).AsRef<Node>());
        }

    };

    template<typename item_t>
    class ALPHA : public AnyOneOf_t< OneOfARange_t<item_t>, OneOfARange_t<item_t> >
    { 
    public: 
        using base = AnyOneOf_t< OneOfARange_t<item_t>, OneOfARange_t<item_t> >;

        ALPHA(Parser* parser) : base(
                parser, &m_a, &m_b, static_cast<uint64_t>(eToken::Alpha)),
            m_a(parser, (item_t)0x41, (item_t)0x5A, static_cast<uint64_t>(eToken::Lexical)),
            m_b(parser, (item_t)0x61, (item_t)0x7A, static_cast<uint64_t>(eToken::Lexical))
        { }

        virtual ~ALPHA() = default;

        virtual void Emit()
        {
            item_t charVal = (item_t)base::m_result.first;
            base::GetParser()->PushNode(new_ref<CharNode>(charVal, static_cast<uint64_t>(base::m_token)).template AsRef<Node>());
        }

    private:

        OneOfARange_t<item_t> m_a;
        OneOfARange_t<item_t> m_b;

    };

}}}//qor::data::parser

#endif//QOR_PP_H_DATA_PARSER_RFC5234
