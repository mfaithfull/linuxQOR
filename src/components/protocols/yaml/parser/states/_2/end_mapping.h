// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_PROTOCOLS_YAML_PARSER_END_MAPPING
#define QOR_PP_H_PROTOCOLS_YAML_PARSER_END_MAPPING

#include "../../tokens.h"

namespace qor { namespace protocols { namespace yaml {

    //end-mapping = ws %x7D ws  ; } right curly brace
    class end_mapping final : public Sequence_t< ws, Sequence_t< Specific<uint32_t>, ws > >
    {
    public:
        end_mapping(Parser* parser) :
            Sequence_t< ws, Sequence_t< Specific<uint32_t>, ws > >(parser, &m_leadingws, &m_tail, static_cast<uint64_t>(yamlToken::end_mapping)),
            m_leadingws(parser),
            m_trailingws(parser),
            m_brace(parser, 0x7D),
            m_tail(parser, &m_brace, &m_trailingws)
        { }
        virtual ~end_mapping() noexcept = default;
        virtual void Emit(){ };//Emit nothing

    private:
        ws m_leadingws;
        ws m_trailingws;
        Specific<uint32_t> m_brace;
        Sequence_t< Specific<uint32_t>, ws > m_tail;
    };

}}}//qor::protocols::yaml

#endif//QOR_PP_H_PROTOCOLS_YAML_PARSER_END_MAPPING