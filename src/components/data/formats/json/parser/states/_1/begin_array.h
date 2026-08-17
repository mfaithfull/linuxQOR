// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_DATA_JSON_PARSER_BEGIN_ARRAY
#define QOR_PP_H_DATA_JSON_PARSER_BEGIN_ARRAY

#include "../../tokens.h"
#include "../_4/ws.h"

namespace qor { namespace data { namespace parser { namespace json {

    // begin-array     = ws %x5B ws  ; [ left square bracket
    class begin_array : public Sequence_t< ws, Sequence_t< Specific<uint32_t>, ws > >
    {
        public: begin_array(Parser* parser) :
            Sequence_t< ws, Sequence_t< Specific<uint32_t>, ws > >(parser, &m_leadingws, &m_tail, static_cast<uint64_t>(jsonToken::begin_array)),
            m_leadingws(parser),
            m_trailingws(parser),
            m_bracket(parser, 0x5B),
            m_tail(parser, &m_bracket, &m_trailingws)
        { }
        virtual ~begin_array() = default;
        virtual void Emit(){ };//Emit nothing

    private:
        ws m_leadingws;
        ws m_trailingws;
        Specific<uint32_t> m_bracket;
        Sequence_t< Specific<uint32_t>, ws > m_tail;
    };

}}}}//qor::data::parser::json

#endif//QOR_PP_H_DATA_JSON_PARSER_BEGIN_ARRAY
