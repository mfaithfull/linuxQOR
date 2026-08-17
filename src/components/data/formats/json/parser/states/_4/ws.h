// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_DATA_JSON_PARSER_WS
#define QOR_PP_H_DATA_JSON_PARSER_WS

#include "../../tokens.h"

namespace qor { namespace data { namespace parser { namespace json {

    /*ws = *(
              %x20 /              ; Space
              %x09 /              ; Horizontal tab
              %x0A /              ; Line feed or New line
              %x0D )              ; Carriage return*/
    class ws : public ZeroOrMore_t< AnyOneOf_t< WSP<uint32_t>, AnyOneOf_t< CR<uint32_t>, LF<uint32_t> > > >
    {
    public: 
        ws(Parser* parser) : ZeroOrMore_t< AnyOneOf_t< WSP<uint32_t>, AnyOneOf_t< CR<uint32_t>, LF<uint32_t> > > >(parser, &m_anyWSP, static_cast<uint64_t>(jsonToken::ws)),
            m_cr(parser),
            m_lf(parser),
            m_wsp(parser),
            m_eol(parser, &m_cr, &m_lf),
            m_anyWSP(parser, &m_wsp, &m_eol)
        { }
        virtual ~ws() = default;
        virtual void Emit(){ };//Emit nothing

    private:
        CR<uint32_t> m_cr;
        LF<uint32_t> m_lf;
        WSP<uint32_t> m_wsp;
        AnyOneOf_t< CR<uint32_t>, LF<uint32_t> > m_eol;
        AnyOneOf_t< WSP<uint32_t>, AnyOneOf_t< CR<uint32_t>, LF<uint32_t> > > m_anyWSP;
        
    };

}}}}//qor::data::parser::json

#endif//QOR_PP_H_DATA_JSON_PARSER_WS
