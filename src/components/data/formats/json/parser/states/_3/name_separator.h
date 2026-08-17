// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_DATA_JSON_PARSER_NAME_SEPARATOR
#define QOR_PP_H_DATA_JSON_PARSER_NAME_SEPARATOR

#include "../../tokens.h"
#include "../_4/ws.h"

namespace qor { namespace data { namespace parser { namespace json {

    //name-separator  = ws %x3A ws  ; : colon
    class name_separator : public Sequence_t< ws, Sequence_t< Specific<uint32_t>, ws > >
    {
        public: name_separator(Parser* parser) :
            Sequence_t< ws, Sequence_t< Specific<uint32_t>, ws > >(parser, &m_wsBefore, &m_s, static_cast<uint64_t>(jsonToken::name_separator)),
            m_wsBefore(parser),
            m_wsAfter(parser),
            m_ns(parser, 0x3A),
            m_s(parser, &m_ns, &m_wsAfter)
        { }
        virtual ~name_separator() = default;
        virtual void Prepare()
        {
            //log::debug("Looking for a name separator...");
        }

        void Emit()
        {
            //log::debug("Consuming a name separator: ");
        }

        void Fail()
        {
            //log::debug("...Didn't find a name separator.");
        }

    private:

        ws m_wsBefore;
        Sequence_t< Specific<uint32_t>, ws > m_s;
        Specific<uint32_t> m_ns;
        ws m_wsAfter;
    };

}}}}//qor::data::parser::json

#endif//QOR_PP_H_DATA_JSON_PARSER_NAME_SEPARATOR
