// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_DATA_JSON_PARSER_END_OBJECT
#define QOR_PP_H_DATA_JSON_PARSER_END_OBJECT

#include "../../tokens.h"
#include "src/qor/flyers/log/debug.h"

namespace qor { namespace data { namespace parser { namespace json {

    //end-object      = ws %x7D ws  ; } right curly bracket
    class end_object : public Sequence_t< ws, Sequence_t< Specific<uint32_t>, ws > >
    {
        public: end_object(Parser* parser) :
            Sequence_t< ws, Sequence_t< Specific<uint32_t>, ws > >(parser, &m_leadingws, &m_tail, static_cast<uint64_t>(jsonToken::end_object)),
            m_leadingws(parser),
            m_trailingws(parser),
            m_bracket(parser, 0x7D),
            m_tail(parser, &m_bracket, &m_trailingws)
        { }
        virtual ~end_object() = default;
        virtual void Prepare()
        {
            //log::debug("Looking for an end_object...");
        }

        void Emit()
        {
            //log::debug("Consuming an end_object: ");
        }

        void Fail()
        {
            //log::debug("...Didn't find an end_object.");
        }
        
    private:
        ws m_leadingws;
        ws m_trailingws;
        Specific<uint32_t> m_bracket;
        Sequence_t< Specific<uint32_t>, ws > m_tail;
    };

}}}}//qor::data::parser::json

#endif//QOR_PP_H_DATA_JSON_PARSER_END_OBJECT
