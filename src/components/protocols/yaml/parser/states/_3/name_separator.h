// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_PROTOCOLS_YAML_PARSER_NAME_SEPARATOR
#define QOR_PP_H_PROTOCOLS_YAML_PARSER_NAME_SEPARATOR

#include "../../tokens.h"
#include "../_4/ws.h"

namespace qor { namespace protocols { namespace yaml {

    //name-separator = ws %x3A ws  ; : colon
    class name_separator final : public Sequence_t< ws, Sequence_t< Specific<uint32_t>, ws > >
    {
    public:
        name_separator(Parser* parser) :
            Sequence_t< ws, Sequence_t< Specific<uint32_t>, ws > >(parser, &m_wsBefore, &m_s, static_cast<uint64_t>(yamlToken::name_separator)),
            m_wsBefore(parser),
            m_wsAfter(parser),
            m_ns(parser, 0x3A),
            m_s(parser, &m_ns, &m_wsAfter)
        { }
        virtual ~name_separator() noexcept = default;
        void Emit(){ }

    private:
        ws m_wsBefore;
        Sequence_t< Specific<uint32_t>, ws > m_s;
        Specific<uint32_t> m_ns;
        ws m_wsAfter;
    };

}}}//qor::protocols::yaml

#endif//QOR_PP_H_PROTOCOLS_YAML_PARSER_NAME_SEPARATOR