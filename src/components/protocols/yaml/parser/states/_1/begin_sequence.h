// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_PROTOCOLS_YAML_PARSER_BEGIN_SEQUENCE
#define QOR_PP_H_PROTOCOLS_YAML_PARSER_BEGIN_SEQUENCE

#include "../../tokens.h"
#include "../_4/ws.h"

namespace qor { namespace protocols { namespace yaml {

    // begin-sequence = ws %x5B ws  ; [ left square bracket
    class begin_sequence final : public Sequence_t< ws, Sequence_t< Specific<uint32_t>, ws > >
    {
    public:
        begin_sequence(Parser* parser) :
            Sequence_t< ws, Sequence_t< Specific<uint32_t>, ws > >(parser, &m_leadingws, &m_tail, static_cast<uint64_t>(yamlToken::begin_sequence)),
            m_leadingws(parser),
            m_trailingws(parser),
            m_bracket(parser, 0x5B),
            m_tail(parser, &m_bracket, &m_trailingws)
        { }
        virtual ~begin_sequence() noexcept = default;
        virtual void Emit(){ };//Emit nothing

    private:
        ws m_leadingws;
        ws m_trailingws;
        Specific<uint32_t> m_bracket;
        Sequence_t< Specific<uint32_t>, ws > m_tail;
    };

}}}//qor::protocols::yaml

#endif//QOR_PP_H_PROTOCOLS_YAML_PARSER_BEGIN_SEQUENCE