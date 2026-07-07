// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_COMPONENTS_PARSER_RFC5234
#define QOR_PP_H_COMPONENTS_PARSER_RFC5234

#include "../state.h"
#include "specific.h"
#include "oneofarange.h"
#include "anyoneof.h"
#include "sequence.h"
#include "zeroormore.h"

namespace qor { namespace components { namespace parser {

    class CR : public Specific
    { public: CR(Parser* parser) : Specific(parser, 0x0D, static_cast<uint64_t>(eToken::CarriageReturn)){} 
        virtual ~CR() = default;
        virtual void Emit(){}
    };

    class LF : public Specific
    { public: LF(Parser* parser) : Specific(parser, 0x0A, static_cast<uint64_t>(eToken::LineFeed)){} 
        virtual ~LF() = default;
        virtual void Emit(){}
    };

    class HTAB : public Specific
    { public: HTAB(Parser* parser) : Specific(parser, 0x09, static_cast<uint64_t>(eToken::HorizontalTab)){} 
        virtual ~HTAB() = default;
        virtual void Emit(){}
    };

    class DQUOTE : public Specific
    { public: DQUOTE(Parser* parser) : Specific(parser, 0x22, static_cast<uint64_t>(eToken::DoubleQuote)){} 
        virtual ~DQUOTE() = default;
        virtual void Emit(){}
    };

    class SP : public Specific
    { public: SP(Parser* parser) : Specific(parser, 0x20, static_cast<uint64_t>(eToken::Space)){} 
        virtual ~SP() = default;
        virtual void Emit(){}
    };

    class qor_pp_module_interface(QOR_PARSER) DIGIT : public OneOfARange
    { 
    public: 
        DIGIT(Parser* parser);
        virtual ~DIGIT();
        virtual void Emit();
    };

    class qor_pp_module_interface(QOR_PARSER) VCHAR : public OneOfARange
    { 
    public: 
        VCHAR(Parser* parser);
        virtual ~VCHAR();
        virtual void Emit();
    };

    class qor_pp_module_interface(QOR_PARSER) CTL : public AnyOneOf
    { 
    public: 
        CTL(Parser* parser);
        virtual ~CTL();
        virtual void Emit();
    };

    class qor_pp_module_interface(QOR_PARSER) HEXDIGIT : public AnyOneOf
    { 
    public: 
        HEXDIGIT(Parser* parser);
        virtual ~HEXDIGIT();
        virtual void Prepare();
        virtual void Emit();
        virtual void Fail();
    };

    class WSP : public AnyOneOf
    { public: WSP(Parser* parser) :
        AnyOneOf(parser,
            new_ref<SP>(parser),
            new_ref<HTAB>(parser),
            static_cast<uint64_t>(eToken::WhiteSpace)
        ){}
        virtual ~WSP() = default;
        virtual void Emit(){}
    };

    class CRLF : public Sequence
    { public : CRLF(Parser* parser) :
        Sequence(parser, new_ref<CR>(parser), new_ref<LF>(parser), static_cast<uint64_t>(eToken::CarriageReturnLineFeed)){}
        virtual ~CRLF() = default;
        virtual void Emit(){}
    };

    class LWSP : public ZeroOrMore
    { 
    public: 
        LWSP(Parser* parser) : ZeroOrMore(parser,
            new_ref<AnyOneOf>(parser,
                new_ref<WSP>(parser),
                new_ref<CRLF>(parser),
                static_cast<uint64_t>(eToken::Lexical)
            ),
            static_cast<uint64_t>(eToken::LinearWhiteSpace)
        ){}
        virtual ~LWSP() = default;
        virtual void Emit(){}
    };

    class qor_pp_module_interface(QOR_PARSER) BIT : public AnyOneOf
    { 
    public: 
        BIT(Parser* parser);
        virtual ~BIT();
        virtual void Emit();
    };

    class qor_pp_module_interface(QOR_PARSER) CHAR : public OneOfARange
    { 
    public:
        CHAR(Parser* parser);
        virtual ~CHAR();
        virtual void Emit();
    };

    class qor_pp_module_interface(QOR_PARSER) ALPHA : public AnyOneOf
    { 
    public: 
        ALPHA(Parser* parser);
        virtual ~ALPHA();
        virtual void Emit();
    };

}}}//qor::components::parser

#endif//QOR_PP_H_COMPONENTS_PARSER_RFC5234
