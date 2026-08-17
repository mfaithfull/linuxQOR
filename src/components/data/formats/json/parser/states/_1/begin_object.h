// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_DATA_JSON_PARSER_BEGIN_OBJECT
#define QOR_PP_H_DATA_JSON_PARSER_BEGIN_OBJECT

#include "../../tokens.h"
#include "../_4/ws.h"

namespace qor { namespace data { namespace parser { namespace json {

    //begin-object    = ws %x7B ws  ; { left curly bracket
    class begin_object : public Sequence_t< ws, Sequence_t< Specific<uint32_t>, ws > >
    {
        public: begin_object(Parser* parser) : Sequence_t< ws, Sequence_t< Specific<uint32_t>, ws > >(parser, &m_leadingws, &m_tail, static_cast<uint64_t>(jsonToken::begin_object)),
            m_trailingws(parser),
            m_leadingws(parser),
            m_bracket(parser, 0x7B),
            m_tail(parser, &m_bracket, &m_trailingws)
        { }
        virtual ~begin_object() = default;
        virtual void Emit(){ };//Emit nothing

    private:

        ws m_trailingws;
        ws m_leadingws;
        Specific<uint32_t> m_bracket;
        Sequence_t< Specific<uint32_t>, ws > m_tail;
    };

}}}}//qor::data::parser::json

#endif//QOR_PP_H_DATA_JSON_PARSER_BEGIN_OBJECT
