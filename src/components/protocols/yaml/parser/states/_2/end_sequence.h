// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_PROTOCOLS_YAML_PARSER_END_SEQUENCE
#define QOR_PP_H_PROTOCOLS_YAML_PARSER_END_SEQUENCE

#include "../../tokens.h"

namespace qor { namespace protocols { namespace yaml {

    //end-sequence = ws %x5D ws  ; ] right square bracket
    class end_sequence final : public Sequence_t< ws, Sequence_t< Specific<uint32_t>, ws > >
    {
    public:
        end_sequence(Parser* parser) :
            Sequence_t< ws, Sequence_t< Specific<uint32_t>, ws > >(parser, &m_leadingws, &m_tail, static_cast<uint64_t>(yamlToken::end_sequence)),
            m_leadingws(parser),
            m_trailingws(parser),
            m_bracket(parser, 0x5D),
            m_tail(parser, &m_bracket, &m_trailingws)
        { }
        virtual ~end_sequence() noexcept = default;
        virtual void Emit(){ };//Emit nothing

    private:
        ws m_leadingws;
        ws m_trailingws;
        Specific<uint32_t> m_bracket;
        Sequence_t< Specific<uint32_t>, ws > m_tail;
    };

}}}//qor::protocols::yaml

#endif//QOR_PP_H_PROTOCOLS_YAML_PARSER_END_SEQUENCE